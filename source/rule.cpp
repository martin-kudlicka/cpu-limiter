#include "rule.h"

#include <MkCore/MProcesses>
#include <QtCore/QDir>
#include <MkProcessGovernor/MGovernor>

Rule::Rule(const MUuidPtr &id) : _options(id), _active(false), _opId(MGovernor::OPERATION_ID_INVALID)
{
}

void Rule::activate(MGovernor *governor)
{
  restrictSelectedProcesses(governor);

  _active = true;
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

RuleOptions &Rule::options()
{
  return _options;
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

  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Target))
  {
    auto pattern = selectedProcess;
    if (!pattern.contains(QDir::separator()))
    {
      pattern.prepend('*' + QDir::separator());
    }

    for (const auto &processInfo : processesInfo)
    {
      if (processInfo.filePath().isEmpty())
      {
        continue;
      }

      QRegExp regExp(QDir::fromNativeSeparators(pattern), Qt::CaseInsensitive, QRegExp::Wildcard);
      if (regExp.exactMatch(QDir::fromNativeSeparators(processInfo.filePath())))
      {
        if (_opId == MGovernor::OPERATION_ID_INVALID)
        {
          _opId = governor->setCpuRate(processInfo.id(), _options.cpuLimit());
        }
        else
        {
          governor->addCpuRate(_opId, processInfo.id());
        }
      }
    }
  }
}