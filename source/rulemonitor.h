#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include <MkNotifier/MWinEventNotifier>

class MGovernor;
class MProcessInfo;
class Rules;

class RuleMonitor : public QObject
{
  Q_OBJECT

  public:
             RuleMonitor(Rules *rules, MGovernor *governor);
    virtual ~RuleMonitor() Q_DECL_OVERRIDE;

  private:
    MGovernor        *_governor;
    MWinEventNotifier _winEventNotifier;
    Rules            *_rules;

  private slots:
    void on_processNotifier_ended  (DWORD id);
    void on_processNotifier_started(const MProcessInfo &processInfo);
    void on_winEventNotifier_notify(const MWinEventInfo &winEventInfo);
};

#endif