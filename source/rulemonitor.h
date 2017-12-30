#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include <MkProcessGovernor/MGovernor>

class Rules;

class RuleMonitor
{
  public:
    RuleMonitor(Rules *rules);

  private:
    MGovernor _governor;
    Rules    *_rules;
};

#endif