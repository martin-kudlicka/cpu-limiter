#include "rulemonitor.h"

#include "rules.h"
#include <MkCore/MProcessInfo>
#include <MkNotifier/MProcessNotifier>
#include <MkCore/MWinEventInfo>
#include "log.h"

RuleMonitor::RuleMonitor(Rules *rules, MProcessGovernor *processGovernor) : _rules(rules), _processGovernor(processGovernor)
{
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &rule : rules->get())
  {
    if (rule->options().enabled())
    {
      auto ok = rule->conditionsMet(foregroundProcess);
      if (ok)
      {
        rule->activate(processGovernor);
      }
    }
  }

  _winEventNotifier.addEvent(EVENT_SYSTEM_FOREGROUND);

  connect(&*mProcessNotifier, &MProcessNotifier::started, this, &RuleMonitor::on_processNotifier_started);
  connect(&*mProcessNotifier, &MProcessNotifier::ended,   this, &RuleMonitor::on_processNotifier_ended);

  connect(&_winEventNotifier, &MWinEventNotifier::notify, this, &RuleMonitor::on_winEventNotifier_notify);
}

RuleMonitor::~RuleMonitor()
{
  for (const auto &rule : _rules->get())
  {
    if (rule->active())
    {
      rule->deactivate(_processGovernor);
    }
  }
}

void RuleMonitor::on_processNotifier_ended(DWORD id)
{
  auto foregroundProcess = MProcessInfo(GetForegroundWindow());

  for (const auto &rule : _rules->get())
  {
    if (!rule->options().enabled())
    {
      continue;
    }

    switch (rule->options().status())
    {
      case RuleOptions::Status::Running:
        if (rule->active())
        {
          auto conditionsMet = rule->conditionsMet(foregroundProcess);
          if (conditionsMet)
          {
            rule->processEnded(id);
          }
          else
          {
            rule->deactivate(_processGovernor);
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
          rule->processEnded(id);
        }
        else
        {
          auto conditionsMet = rule->conditionsMet(foregroundProcess);
          if (conditionsMet)
          {
            rule->activate(_processGovernor);
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
    if (!rule->options().enabled())
    {
      continue;
    }

    switch (rule->options().status())
    {
      case RuleOptions::Status::Running:
        if (rule->active())
        {
          if (rule->isTargetProcess(processInfo))
          {
            rule->restrictProcess(_processGovernor, processInfo);
          }
        }
        else
        {
          auto conditionsMet = rule->conditionsMet(processInfo, foregroundProcess);
          if (conditionsMet)
          {
            rule->activate(_processGovernor);
          }
        }
        break;
      case RuleOptions::Status::NotRunning:
        if (rule->active())
        {
          auto conditionsMet = rule->conditionsMet(foregroundProcess);
          if (!conditionsMet)
          {
            rule->deactivate(_processGovernor);
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

  mCDebug(CPULimiter) << "process #" << foregroundProcess.id() << " in foreground" << " (" << foregroundProcess.filePath() << ')';

  for (const auto &rule : _rules->get())
  {
    if (!rule->options().enabled())
    {
      continue;
    }
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
      rule->deactivate(_processGovernor);
    }
    else if (!rule->active() && conditionsMet)
    {
      rule->activate(_processGovernor);
    }
  }
}