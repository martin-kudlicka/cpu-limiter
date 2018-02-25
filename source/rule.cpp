#include "rule.h"

#include <QtCore/QDir>
#include <MkProcessGovernor/MProcessGovernor>
#include <MkCore/MWinEventInfo>
#include "rulesmodel.h"
#include "log.h"
#include <MkNetwork/MNetwork>

Rule::Rule(const MUuidPtr &id, MProcessGovernor *processGovernor, RulesModel *rulesModel) : _delayTimer(0), _options(id), _processGovernor(processGovernor), _rulesModel(rulesModel), _foregroundProcess(GetForegroundWindow()), _connectivity(MNetwork().connectivity()), _opId(MProcessGovernor::OPERATION_ID_INVALID), _status(Status::Inactive)
{
}

Rule::~Rule()
{
  if (_status == Status::Active)
  {
    deactivate();
  }
}

void Rule::activate(bool checkDelay /* true */)
{
  if (checkDelay && _options.applyDelay())
  {
    mCInfo(CPULimiter) << "rule \"" << _options.name() << "\" delayed for " << _options.applyDelayValue() << 's';

    _delayTimer = startTimer(_options.applyDelayValue() * 1000, Qt::VeryCoarseTimer);

    _status = Status::Delayed;
  }
  else
  {
    mCInfo(CPULimiter) << "rule \"" << _options.name() << "\" activated";

    restrictSelectedProcesses();

    _status = Status::Active;
  }
}

bool Rule::conditionsMet()
{
  if (_options.internetConnectionCheck())
  {
    switch (_options.internetConnectionStatus())
    {
      case RuleOptions::InternetConnection::Disconnected:
        if (_connectivity & (NLM_CONNECTIVITY_IPV4_INTERNET | NLM_CONNECTIVITY_IPV6_INTERNET))
        {
          return false;
        }
        break;
      case RuleOptions::InternetConnection::Connected:
        if (!(_connectivity & (NLM_CONNECTIVITY_IPV4_INTERNET | NLM_CONNECTIVITY_IPV6_INTERNET)))
        {
          return false;
        }
        break;
      default:
        Q_ASSERT_X(false, "Rule::conditionsMet", "switch (_options.internetConnectionStatus())");
    }
  }

  auto processesInfo = _processGovernor->runningProcesses();
  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Condition))
  {
    auto conditionFound = false;

    for (const auto &processInfo : processesInfo)
    {
      if (conditionsMet(selectedProcess, *processInfo))
      {
        conditionFound = true;
        break;
      }
    }

    if (!conditionFound)
    {
      return false;
    }
  }

  return true;
}

void Rule::deactivate()
{
  if (_delayTimer)
  {
    killTimer(_delayTimer);
    _delayTimer = 0;
  }
  else
  {
    _processGovernor->revert(_opId);
    _opId = MProcessGovernor::OPERATION_ID_INVALID;
    _restrictedProcesses.clear();
  }

  _status = Status::Inactive;

  mCInfo(CPULimiter) << "rule \"" << _options.name() << "\" deactivated";
}

bool Rule::isRestricting() const
{
  return !_restrictedProcesses.isEmpty();
}

RuleOptions &Rule::options()
{
  return _options;
}

Rule::Status Rule::status() const
{
  return _status;
}

bool Rule::conditionsMet(const QString &selectedProcess, const MProcessInfo &runningProcess)
{
  if (runningProcess.fileInfo().filePath().isEmpty())
  {
    return false;
  }

  auto pattern = selectedProcess;
  if (!pattern.contains(QDir::separator()))
  {
    pattern.prepend('*' + QDir::separator());
  }
  QRegExp regExp(QDir::fromNativeSeparators(pattern), Qt::CaseInsensitive, QRegExp::Wildcard);
  if (regExp.exactMatch(runningProcess.fileInfo().filePath()))
  {
    if (_options.status() == RuleOptions::Status::Running)
    {
      switch (_options.state())
      {
        case RuleOptions::State::Anyhow:
          return true;
        case RuleOptions::State::Foreground:
          if (runningProcess == _foregroundProcess)
          {
            return true;
          }
          break;
        case RuleOptions::State::Background:
          if ((runningProcess != _foregroundProcess) && (runningProcess.fileInfo() != _foregroundProcess.fileInfo()))
          {
            return true;
          }
          break;
        default:
          Q_ASSERT_X(false, "Rule::conditionProcessRunning", "switch (_options.state())");
      }
    }
  }
  else
  {
    if (_options.status() == RuleOptions::Status::NotRunning)
    {
      return true;
    }
  }

  return false;
}

bool Rule::isTargetProcess(const MProcessInfo &runningProcess)
{
  if (runningProcess.fileInfo().filePath().isEmpty())
  {
    return false;
  }

  for (const auto &selectedProcess : _options.selectedProcesses(RuleOptions::Section::Target))
  {
    auto pattern = selectedProcess;
    if (!pattern.contains(QDir::separator()))
    {
      pattern.prepend('*' + QDir::separator());
    }

    QRegExp regExp(QDir::fromNativeSeparators(pattern), Qt::CaseInsensitive, QRegExp::Wildcard);
    if (regExp.exactMatch(runningProcess.fileInfo().filePath()))
    {
      return true;
    }
  }

  return false;
}

void Rule::restrictProcess(const MProcessInfo &runningProcess)
{
  switch (_options.action())
  {
    case RuleOptions::Action::SetCpuRate:
      if (_opId == MProcessGovernor::OPERATION_ID_INVALID)
      {
        _opId = _processGovernor->setCpuRate(runningProcess.id(), _options.cpuLimit());
      }
      else
      {
        _processGovernor->addCpuRate(_opId, runningProcess.id(), _options.cpuLimit());
      }
      break;
    case RuleOptions::Action::Suspend:
      if (_opId == MProcessGovernor::OPERATION_ID_INVALID)
      {
        _opId = _processGovernor->suspend(runningProcess.id());
      }
      else
      {
        _processGovernor->addSuspend(_opId, runningProcess.id());
      }
      break;
    default:
      Q_ASSERT_X(false, "Rule::restrictProcess", "switch (_options.action())");
  }

  _restrictedProcesses.insert(runningProcess.id());
}

void Rule::restrictSelectedProcesses()
{
  auto processesInfo = _processGovernor->runningProcesses();
  for (const auto &processInfo : processesInfo)
  {
    if (isTargetProcess(*processInfo))
    {
      restrictProcess(*processInfo);
    }
  }
}

void Rule::timerEvent(QTimerEvent *event)
{
  killTimer(_delayTimer);
  _delayTimer = 0;

  activate(false);

  _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
}

void Rule::on_networkNotifier_connectivityChanged(NLM_CONNECTIVITY newConnectivity)
{
  _connectivity = newConnectivity;

  if (!_options.enabled())
  {
    return;
  }

  switch (_status)
  {
    case Status::Inactive:
      if (conditionsMet())
      {
        activate();

        _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
      }
      break;
    case Status::Active:
      Q_FALLTHROUGH();
    case Status::Delayed:
      if (!conditionsMet())
      {
        deactivate();

        _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
      }
      break;
    default:
      Q_ASSERT_X(false, "Rule::on_networkNotifier_connectivityChanged", "switch (_status)");
  }
}

void Rule::on_processNotifier_ended(DWORD id)
{
  if (!_options.enabled())
  {
    return;
  }

  switch (_options.status())
  {
    case RuleOptions::Status::Running:
      switch (_status)
      {
        case Status::Inactive:
          break;
        case Status::Active:
          if (conditionsMet())
          {
            _restrictedProcesses.remove(id);
          }
          else
          {
            deactivate();

            _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
          }
          break;
        case Status::Delayed:
          if (!conditionsMet())
          {
            deactivate();

            _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
          }
          break;
        default:
          Q_ASSERT_X(false, "Rule::on_processNotifier_ended", "switch (_status)");
      }
      break;
    case RuleOptions::Status::NotRunning:
      switch (_status)
      {
        case Status::Inactive:
          if (conditionsMet())
          {
            activate();

            _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
          }
          break;
        case Status::Active:
          _restrictedProcesses.remove(id);
          break;
        case Status::Delayed:
          break;
        default:
          Q_ASSERT_X(false, "Rule::on_processNotifier_ended", "switch (_status)");
      }
      break;
    default:
      Q_ASSERT_X(false, "Rule::on_processNotifier_ended", "switch (_options.status())");
  }
}

void Rule::on_processNotifier_started(const MProcessInfo &processInfo)
{
  if (!_options.enabled())
  {
    return;
  }

  switch (_options.status())
  {
    case RuleOptions::Status::Running:
      switch (_status)
      {
        case Status::Inactive:
          if (conditionsMet())
          {
            activate();

            _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
          }
          break;
        case Status::Active:
          if (isTargetProcess(processInfo))
          {
            restrictProcess(processInfo);
          }
          break;
        case Status::Delayed:
          break;
        default:
          Q_ASSERT_X(false, "Rule::on_processNotifier_started", "switch (_status)");
      }
      break;
    case RuleOptions::Status::NotRunning:
      switch (_status)
      {
        case Status::Inactive:
          break;
        case Status::Active:
          Q_FALLTHROUGH();
        case Status::Delayed:
          if (!conditionsMet())
          {
            deactivate();

            _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
          }
          break;
        default:
          Q_ASSERT_X(false, "Rule::on_processNotifier_started", "switch (_status)");
      }
      break;
    default:
      Q_ASSERT_X(false, "Rule::on_processNotifier_started", "switch (_options.status())");
  }
}

void Rule::on_winEventNotifier_notify(const MWinEventInfo &winEventInfo)
{
  _foregroundProcess = MProcessInfo(winEventInfo.window());

  if (!_options.enabled())
  {
    return;
  }
  if (_options.status() == RuleOptions::Status::NotRunning)
  {
    return;
  }
  if (_options.state() == RuleOptions::State::Anyhow)
  {
    return;
  }

  switch (_status)
  {
    case Status::Inactive:
      if (conditionsMet())
      {
        activate();

        _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
      }
      break;
    case Status::Active:
      Q_FALLTHROUGH();
    case Status::Delayed:
      if (!conditionsMet())
      {
        deactivate();

        _rulesModel->setDataChanged(_options.id(), RulesModel::Column::Active);
      }
      break;
    default:
      Q_ASSERT_X(false, "Rule::on_winEventNotifier_notify", "switch (_options.status())");
  }
}