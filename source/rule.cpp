#include "rule.h"

#include <MkCore/MProcesses>
#include <QtCore/QDir>

MLazySingleton<MGovernor> Rule::_governor;

Rule::Rule(const MUuidPtr &id) : _options(id), _active(false), _opId(-1)
{
}

void Rule::activate()
{
  restrictSelectedProcesses();

  _active = true;
}

bool Rule::active() const
{
  return _active;
}

bool Rule::conditionsMet()
{
  auto hasConditionProcess = conditionProcessRunning();

  if (_options.status() == RuleOptions::Status::Running && hasConditionProcess)
  {
    return true;
  }
  else if (_options.status() == RuleOptions::Status::NotRunning && !hasConditionProcess)
  {
    return true;
  }

  return false;
}

RuleOptions &Rule::options()
{
  return _options;
}

bool Rule::conditionProcessRunning()
{
  auto processesInfo     = MProcesses::enumerate();
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Condition))
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
        switch (_options.state())
        {
          case RuleOptions::State::Anyhow:
            return true;
          case RuleOptions::State::Foreground:
            if (processInfo == foregroundProcess)
            {
              return true;
            }
            break;
          case RuleOptions::State::Background:
            if (processInfo != foregroundProcess)
            {
              return true;
            }
            break;
          default:
            Q_ASSERT_X(false, "Rule::conditionProcessRunning", "switch (_options.state())");
            continue;
        }
      }
    }
  }

  return false;
}

void Rule::restrictSelectedProcesses()
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
        if (_opId == -1)
        {
          _opId = _governor->setCpuRate(processInfo.id(), _options.cpuLimit());
        }
        else
        {
          // TODO
        }
      }
    }
  }
}