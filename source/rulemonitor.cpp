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

bool RuleMonitor::containsRunningProcess(const RuleSPtr &rule, RuleOptions::Section section) const
{
  auto processesInfo = MProcesses::enumerate();

  auto selectedProcesses = rule->options().selectedProcesses(section);
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
        return true;
      }
    }
  }

  return false;
}

bool RuleMonitor::evaluateConditions(const RuleSPtr &rule) const
{
  auto hasConditionProcess = containsRunningProcess(rule, RuleOptions::Section::Condition);
  // TODO
  return false;
}