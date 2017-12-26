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

    bool containsRunningProcess(const QStringList &selectedProcesses, RuleOptions::State state) const;
    bool evaluateConditions    (const RuleSPtr &rule)                                           const;
};

#endif