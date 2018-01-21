#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include <MkNotifier/MWinEventNotifier>
#include <MkNotifier/MNetworkNotifier>
#include <MkCore/MProcessInfo>

class MProcessGovernor;
class RulesModel;

class RuleMonitor : public QObject
{
  Q_OBJECT

  public:
             RuleMonitor(RulesModel *rulesModel, MProcessGovernor *processGovernor);
    virtual ~RuleMonitor() Q_DECL_OVERRIDE;

  private:
    MNetworkNotifier  _networkNotifier;
    MProcessGovernor *_processGovernor;
    MProcessInfo      _foregroundProcess;
    MWinEventNotifier _winEventNotifier;
    NLM_CONNECTIVITY  _connectivity;
    RulesModel       *_rulesModel;

  private slots:
    void on_networkNotifier_connectivityChanged(NLM_CONNECTIVITY newConnectivity) const;
    void on_processNotifier_ended              (DWORD id);
    void on_processNotifier_started            (const MProcessInfo &processInfo);
    void on_winEventNotifier_notify            (const MWinEventInfo &winEventInfo);
};

#endif