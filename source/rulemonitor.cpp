#include "rulemonitor.h"

#include "rules.h"
#include <MkNotifier/MProcessNotifier>

RuleMonitor::RuleMonitor(Rules *rules) : _rules(rules)
{
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &rule : rules->get())
  {
    if (rule->options().enabled())
    {
      auto ok = rule->conditionsMet(foregroundProcess);
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
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &rule : _rules->get())
  {
    if ((rule->active() && rule->options().status() == RuleOptions::Status::NotRunning) || (!rule->active() && rule->options().status() == RuleOptions::Status::Running))
    {
      continue;
    }

    // TODO
  }
}

void RuleMonitor::on_processNotifier_started(const MProcessInfo &processInfo)
{
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &rule : _rules->get())
  {
    switch (rule->options().status())
    {
      case RuleOptions::Status::Running:
        if (rule->active())
        {
          if (rule->isTargetProcess(processInfo))
          {
            rule->restrictProcess(&_governor, processInfo);
          }
        }
        else
        {
          auto conditionsMet = rule->conditionsMet(processInfo, foregroundProcess);
          if (conditionsMet)
          {
            rule->activate(&_governor);
          }
        }
        break;
      case RuleOptions::Status::NotRunning:
        if (rule->active())
        {
          auto conditionsMet = rule->conditionsMet(foregroundProcess);
          if (!conditionsMet)
          {
            rule->deactivate(&_governor);
          }
        }
        else
        {
          continue;
        }
        break;
      default:
        Q_ASSERT_X(false, "RuleMonitor::on_processNotifier_started", "RuleMonitor::on_processNotifier_started");
        continue;
    }
  }
}