#include "rulemonitor.h"

#include "rules.h"
#include <MkNotifier/MProcessNotifier>

RuleMonitor::RuleMonitor(Rules *rules) : _rules(rules)
{
  for (const auto &rule : rules->get())
  {
    if (rule->options().enabled())
    {
      auto ok = rule->conditionsMet();
      if (ok)
      {
        rule->activate(&_governor);
      }
    }
  }

  connect(&*mProcessNotifier, &MProcessNotifier::started, this, &RuleMonitor::on_processNotifier_started);
  connect(&*mProcessNotifier, &MProcessNotifier::ended,   this, &RuleMonitor::on_processNotifier_ended);
}

void RuleMonitor::on_processNotifier_ended(DWORD id)
{
  // TODO
}

void RuleMonitor::on_processNotifier_started(const MProcessInfo &processInfo)
{
  for (const auto &rule : _rules->get())
  {
    // TODO
  }
}