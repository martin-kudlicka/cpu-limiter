#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include <MkProcessGovernor/MGovernor>

class Rules;

class RuleMonitor : public QObject
{
  Q_OBJECT

  public:
    RuleMonitor(Rules *rules);

  private:
    MGovernor _governor;
    Rules    *_rules;

  private slots:
    void on_processNotifier_ended  (DWORD id);
    void on_processNotifier_started(const MProcessInfo &processInfo);
};

#endif