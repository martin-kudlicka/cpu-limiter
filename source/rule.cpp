#include "rule.h"

#include <MkCore/MProcesses>
#include <QtCore/QDir>
#include <MkProcessGovernor/MProcessGovernor>
#include "log.h"

Rule::Rule(const MUuidPtr &id) : _options(id), _active(false), _opId(MProcessGovernor::OPERATION_ID_INVALID)
{
}

void Rule::activate(MProcessGovernor *processGovernor)
{
  restrictSelectedProcesses(processGovernor);

  _active = true;

  mCInfo(CPULimiter) << "rule \"" << _options.name() << "\" activated";
}

bool Rule::active() const
{
  return _active;
}

bool Rule::conditionsMet(const MProcessInfo &foregroundProcess)
{
  auto processesInfo = MProcesses::enumerate();

  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Condition))
  {
    auto conditionFound = false;

    for (const auto &processInfo : processesInfo)
    {
      if (conditionsMet(selectedProcess, processInfo, foregroundProcess))
      {
        conditionFound = true;
        break;
      }
    }

    if (!conditionFound)
    {
      return false;
    }
  }

  return true;
}

void Rule::deactivate(MProcessGovernor *processGovernor)
{
  processGovernor->revert(_opId);
  _opId = MProcessGovernor::OPERATION_ID_INVALID;
  _restrictedProcesses.clear();

  _active = false;

  mCInfo(CPULimiter) << "rule \"" << _options.name() << "\" deactivated";
}

bool Rule::isRestricting() const
{
  return !_restrictedProcesses.isEmpty();
}

bool Rule::isTargetProcess(const MProcessInfo &runningProcess)
{
  if (runningProcess.filePath().isEmpty())
  {
    return false;
  }

  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Target))
  {
    auto pattern = selectedProcess;
    if (!pattern.contains(QDir::separator()))
    {
      pattern.prepend('*' + QDir::separator());
    }

    QRegExp regExp(QDir::fromNativeSeparators(pattern), Qt::CaseInsensitive, QRegExp::Wildcard);
    if (regExp.exactMatch(QDir::fromNativeSeparators(runningProcess.filePath())))
    {
      return true;
    }
  }

  return false;
}

RuleOptions &Rule::options()
{
  return _options;
}

void Rule::processEnded(DWORD processId)
{
  _restrictedProcesses.remove(processId);
}

void Rule::restrictProcess(MProcessGovernor *processGovernor, const MProcessInfo &runningProcess)
{
  if (_opId == MProcessGovernor::OPERATION_ID_INVALID)
  {
    _opId = processGovernor->setCpuRate(runningProcess.id(), _options.cpuLimit());
  }
  else
  {
    processGovernor->addCpuRate(_opId, runningProcess.id(), _options.cpuLimit());
  }

  _restrictedProcesses.insert(runningProcess.id());
}

bool Rule::conditionsMet(const QString &selectedProcess, const MProcessInfo &runningProcess, const MProcessInfo &foregroundProcess)
{
  if (runningProcess.filePath().isEmpty())
  {
    return false;
  }

  auto pattern = selectedProcess;
  if (!pattern.contains(QDir::separator()))
  {
    pattern.prepend('*' + QDir::separator());
  }
  QRegExp regExp(QDir::fromNativeSeparators(pattern), Qt::CaseInsensitive, QRegExp::Wildcard);
  if (regExp.exactMatch(QDir::fromNativeSeparators(runningProcess.filePath())))
  {
    if (_options.status() == RuleOptions::Status::Running)
    {
      switch (_options.state())
      {
        case RuleOptions::State::Anyhow:
          return true;
        case RuleOptions::State::Foreground:
          if (runningProcess == foregroundProcess)
          {
            return true;
          }
          break;
        case RuleOptions::State::Background:
          if ((runningProcess != foregroundProcess) && (runningProcess.filePath() != foregroundProcess.filePath()))
          {
            return true;
          }
          break;
        default:
          Q_ASSERT_X(false, "Rule::conditionProcessRunning", "switch (_options.state())");
      }
    }
  }
  else
  {
    if (_options.status() == RuleOptions::Status::NotRunning)
    {
      return true;
    }
  }

  return false;
}

void Rule::restrictSelectedProcesses(MProcessGovernor *processGovernor)
{
  auto processesInfo = MProcesses::enumerate();

  for (const auto &processInfo : processesInfo)
  {
    if (isTargetProcess(processInfo))
    {
      restrictProcess(processGovernor, processInfo);
    }
  }
}