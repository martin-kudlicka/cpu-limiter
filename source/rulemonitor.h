#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include <MkProcessGovernor/MGovernor>
#include <MkNotifier/MWinEventNotifier>

class Rules;

class RuleMonitor : public QObject
{
  Q_OBJECT

  public:
             RuleMonitor(Rules *rules);
    virtual ~RuleMonitor() Q_DECL_OVERRIDE;

    MGovernor *governor();

  private:
    MGovernor         _governor;
    MWinEventNotifier _winEventNotifier;
    Rules            *_rules;

  private slots:
    void on_processNotifier_ended  (DWORD id);
    void on_processNotifier_started(const MProcessInfo &processInfo);
    void on_winEventNotifier_notify(const MWinEventInfo &winEventInfo);
};

#endif