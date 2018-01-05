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
    if (!rule->active() && rule->options().status() == RuleOptions::Status::NotRunning)
    {
      continue;
    }

    auto conditionsMet = rule->active() && rule->options().status() == RuleOptions::Status::Running;
    if (!conditionsMet)
    {
      switch (rule->options().status())
      {
        case RuleOptions::Status::Running:
          conditionsMet = rule->conditionsMet(processInfo, foregroundProcess);
          break;
        case RuleOptions::Status::NotRunning:
          conditionsMet = rule->conditionsMet(foregroundProcess);
          break;
        default:
          Q_ASSERT_X(false, "RuleMonitor::on_processNotifier_started", "RuleMonitor::on_processNotifier_started");
          continue;
      }
    }

    if (conditionsMet)
    {
      if (rule->active())
      {
        if (rule->isTargetProcess(processInfo))
        {
          rule->restrictProcess(&_governor, processInfo);
        }
      }
      else
      {
        rule->activate(&_governor);
      }
    }
    else
    {
      if (rule->active())
      {
        rule->deactivate(&_governor);
      }
    }
  }
}