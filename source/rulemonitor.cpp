#include "rulemonitor.h"

#include "rules.h"
#include <MkCore/MProcesses>
#include <QtCore/QDir>

RuleMonitor::RuleMonitor(Rules *rules) : _rules(rules)
{
  for (auto ruleIndex = 0; ruleIndex < rules->size(); ruleIndex++)
  {
    auto rule = rules->get(rules->id(ruleIndex));

    if (rule->options().enabled())
    {
      auto ok = evaluateConditions(rule);
      // TODO
    }
  }
}

bool RuleMonitor::containsRunningProcess(const QStringList &selectedProcesses, RuleOptions::State state) const
{
  auto processesInfo     = MProcesses::enumerate();
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &selectedProcess : selectedProcesses)
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
        switch (state)
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
            Q_ASSERT_X(false, "RuleMonitor::containsRunningProcess", "switch (state)");
            continue;
        }
      }
    }
  }

  return false;
}

bool RuleMonitor::evaluateConditions(const RuleSPtr &rule) const
{
  auto hasConditionProcess = containsRunningProcess(rule->options().selectedProcesses(RuleOptions::Section::Condition), rule->options().state());

  if (rule->options().status() == RuleOptions::Status::Running && hasConditionProcess)
  {
    // TODO
    return true;
  }
  else if (rule->options().status() == RuleOptions::Status::NotRunning && !hasConditionProcess)
  {
    return true;
  }
  return false;
}