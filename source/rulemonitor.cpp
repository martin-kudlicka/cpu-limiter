#include "rulemonitor.h"

#include "rulesmodel.h"
#include <MkNotifier/MProcessNotifier>
#include <MkCore/MWinEventInfo>
#include "log.h"
#include <MkNetwork/MNetwork>

RuleMonitor::RuleMonitor(RulesModel *rulesModel, MProcessGovernor *processGovernor) : _rulesModel(rulesModel), _processGovernor(processGovernor), _foregroundProcess(GetForegroundWindow()), _connectivity(MNetwork().connectivity())
{
  for (const auto &rule : rulesModel->rules()->get())
  {
    if (rule->options().enabled())
    {
      auto ok = rule->conditionsMet(_foregroundProcess);
      if (ok)
      {
        rule->activate(processGovernor);
      }
    }
  }

  connect(&_networkNotifier, &MNetworkNotifier::connectivityChanged, this, &RuleMonitor::on_networkNotifier_connectivityChanged);

  connect(&*mProcessNotifier, &MProcessNotifier::started, this, &RuleMonitor::on_processNotifier_started);
  connect(&*mProcessNotifier, &MProcessNotifier::ended,   this, &RuleMonitor::on_processNotifier_ended);

  connect(&_winEventNotifier, &MWinEventNotifier::notify, this, &RuleMonitor::on_winEventNotifier_notify);

  _winEventNotifier.addEvent(EVENT_SYSTEM_FOREGROUND);
}

RuleMonitor::~RuleMonitor()
{
  for (const auto &rule : _rulesModel->rules()->get())
  {
    if (rule->active())
    {
      rule->deactivate(_processGovernor);
    }
  }
}

void RuleMonitor::on_networkNotifier_connectivityChanged(NLM_CONNECTIVITY newConnectivity) const
{
  // TODO
}

void RuleMonitor::on_processNotifier_ended(DWORD id)
{
  for (const auto &rule : _rulesModel->rules()->get())
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
          auto conditionsMet = rule->conditionsMet(_foregroundProcess);
          if (conditionsMet)
          {
            rule->processEnded(id);
          }
          else
          {
            rule->deactivate(_processGovernor);

            _rulesModel->setDataChanged(rule, RulesModel::Column::Active);
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
          auto conditionsMet = rule->conditionsMet(_foregroundProcess);
          if (conditionsMet)
          {
            rule->activate(_processGovernor);

            _rulesModel->setDataChanged(rule, RulesModel::Column::Active);
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
  for (const auto &rule : _rulesModel->rules()->get())
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
          auto conditionsMet = rule->conditionsMet(_foregroundProcess);
          if (conditionsMet)
          {
            rule->activate(_processGovernor);

            _rulesModel->setDataChanged(rule, RulesModel::Column::Active);
          }
        }
        break;
      case RuleOptions::Status::NotRunning:
        if (rule->active())
        {
          auto conditionsMet = rule->conditionsMet(_foregroundProcess);
          if (!conditionsMet)
          {
            rule->deactivate(_processGovernor);

            _rulesModel->setDataChanged(rule, RulesModel::Column::Active);
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
  _foregroundProcess = MProcessInfo(winEventInfo.window());

  mCDebug(CPULimiter) << "process #" << _foregroundProcess.id() << " in foreground" << " (" << _foregroundProcess.filePath() << ')';

  for (const auto &rule : _rulesModel->rules()->get())
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

    auto conditionsMet = rule->conditionsMet(_foregroundProcess);
    if (rule->active() && !conditionsMet)
    {
      rule->deactivate(_processGovernor);

      _rulesModel->setDataChanged(rule, RulesModel::Column::Active);
    }
    else if (!rule->active() && conditionsMet)
    {
      rule->activate(_processGovernor);

      _rulesModel->setDataChanged(rule, RulesModel::Column::Active);
    }
  }
}