#include "rule.h"

#include <MkCore/MProcesses>
#include <QtCore/QDir>
#include <MkProcessGovernor/MGovernor>
#include "log.h"

Rule::Rule(const MUuidPtr &id) : _options(id), _active(false), _opId(MGovernor::OPERATION_ID_INVALID)
{
}

void Rule::activate(MGovernor *governor)
{
  restrictSelectedProcesses(governor);

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
    for (const auto &processInfo : processesInfo)
    {
      if (conditionsMet(selectedProcess, processInfo, foregroundProcess))
      {
        return true;
      }
    }
  }

  return false;
}

bool Rule::conditionsMet(const MProcessInfo &runningProcess, const MProcessInfo &foregroundProcess)
{
  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Condition))
  {
    if (conditionsMet(selectedProcess, runningProcess, foregroundProcess))
    {
      return true;
    }
  }

  return false;
}

void Rule::deactivate(MGovernor *governor)
{
  governor->revert(_opId);
  _opId = MGovernor::OPERATION_ID_INVALID;

  _active = false;

  mCInfo(CPULimiter) << "rule \"" << _options.name() << "\" deactivated";
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

void Rule::restrictProcess(MGovernor *governor, const MProcessInfo &runningProcess)
{
  if (_opId == MGovernor::OPERATION_ID_INVALID)
  {
    _opId = governor->setCpuRate(runningProcess.id(), _options.cpuLimit());
  }
  else
  {
    governor->addCpuRate(_opId, runningProcess.id(), _options.cpuLimit());
  }
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
          if (runningProcess != foregroundProcess)
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

void Rule::restrictSelectedProcesses(MGovernor *governor)
{
  auto processesInfo = MProcesses::enumerate();

  for (const auto &processInfo : processesInfo)
  {
    if (isTargetProcess(processInfo))
    {
      restrictProcess(governor, processInfo);
    }
  }
}