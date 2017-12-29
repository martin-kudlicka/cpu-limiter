#include "rulemonitor.h"

#include "rules.h"

RuleMonitor::RuleMonitor(Rules *rules) : _rules(rules)
{
  for (const auto &rule : rules->get())
  {
    if (rule->options().enabled())
    {
      auto ok = rule->conditionsMet();
      if (ok)
      {
        rule->activate();
      }
    }
  }
}