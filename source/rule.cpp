#include "rule.h"

#include <MkCore/MProcesses>
#include <QtCore/QDir>

Rule::Rule(const MUuidPtr &id) : _options(id)
{
}

bool Rule::active() const
{
  // TODO
  return false;
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