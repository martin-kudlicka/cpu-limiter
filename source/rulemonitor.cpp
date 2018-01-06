#include "rulemonitor.h"

#include "rules.h"
#include <MkNotifier/MProcessNotifier>
#include <MkCore/MWinEventInfo>

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

  _winEventNotifier.addEvent(EVENT_SYSTEM_FOREGROUND);

  connect(&*mProcessNotifier, &MProcessNotifier::started, this, &RuleMonitor::on_processNotifier_started);
  connect(&*mProcessNotifier, &MProcessNotifier::ended,   this, &RuleMonitor::on_processNotifier_ended);

  connect(&_winEventNotifier, &MWinEventNotifier::notify, this, &RuleMonitor::on_winEventNotifier_notify);
}

void RuleMonitor::on_processNotifier_ended(DWORD id)
{
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &rule : _rules->get())
  {
    switch (rule->options().status())
    {
      case RuleOptions::Status::Running:
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
      case RuleOptions::Status::NotRunning:
        if (rule->active())
        {
          continue;
        }
        else
        {
          auto conditionsMet = rule->conditionsMet(foregroundProcess);
          if (conditionsMet)
          {
            rule->activate(&_governor);
          }
        }
        break;
      default:
        Q_ASSERT_X(false, "RuleMonitor::on_processNotifier_ended", "switch (rule->options().status())");
        continue;
    }
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
        Q_ASSERT_X(false, "RuleMonitor::on_processNotifier_started", "switch (rule->options().status())");
        continue;
    }
  }
}

void RuleMonitor::on_winEventNotifier_notify(const MWinEventInfo &winEventInfo)
{
  auto foregroundProcess = MProcessInfo(winEventInfo.window());

  for (const auto &rule : _rules->get())
  {
    if (rule->options().status() == RuleOptions::Status::NotRunning)
    {
      continue;
    }
    if (rule->options().state() == RuleOptions::State::Anyhow)
    {
      continue;
    }

    auto conditionsMet = rule->conditionsMet(foregroundProcess);
    if (rule->active() && !conditionsMet)
    {
      rule->deactivate(&_governor);
    }
    else if (!rule->active() && conditionsMet)
    {
      rule->activate(&_governor);
    }
  }
}