#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include <MkNotifier/MWinEventNotifier>

class MProcessGovernor;
class MProcessInfo;
class RulesModel;

class RuleMonitor : public QObject
{
  Q_OBJECT

  public:
             RuleMonitor(RulesModel *rulesModel, MProcessGovernor *processGovernor);
    virtual ~RuleMonitor() Q_DECL_OVERRIDE;

  private:
    MProcessGovernor *_processGovernor;
    MWinEventNotifier _winEventNotifier;
    RulesModel       *_rulesModel;

  private slots:
    void on_processNotifier_ended  (DWORD id);
    void on_processNotifier_started(const MProcessInfo &processInfo);
    void on_winEventNotifier_notify(const MWinEventInfo &winEventInfo);
};

#endif