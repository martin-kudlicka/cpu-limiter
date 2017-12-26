#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include "rule.h"

class Rules;

class RuleMonitor
{
  public:
    RuleMonitor(Rules *rules);

  private:
    Rules *_rules;

    bool containsRunningProcess(const RuleSPtr &rule, RuleOptions::Section section) const;
    bool evaluateConditions    (const RuleSPtr &rule)                               const;
};

#endif