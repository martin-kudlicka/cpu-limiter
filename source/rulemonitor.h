#ifndef RULEMONITOR_H
#define RULEMONITOR_H

class Rules;

class RuleMonitor
{
  public:
    RuleMonitor(Rules *rules);

  private:
    Rules *_rules;
};

#endif