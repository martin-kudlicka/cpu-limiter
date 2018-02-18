#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"
#include <Netlistmgr.h>
#include <QtCore/QSet>
#include <MkCore/MProcessInfo>

class MProcessGovernor;
class MWinEventInfo;
class RulesModel;

class Rule : public QObject
{
  Q_OBJECT

  public:
    enum class Status
    {
      Inactive,
      Active,
      Delayed
    };

             Rule(const MUuidPtr &id, MProcessGovernor *processGovernor, RulesModel *rulesModel);
    virtual ~Rule() Q_DECL_OVERRIDE;

    void         activate     ();
    bool         conditionsMet();
    void         deactivate   ();
    bool         isRestricting() const;
    RuleOptions &options      ();
    Status       status       () const;

  private:
    int               _delayTimer;
    MProcessGovernor *_processGovernor;
    MProcessInfo      _foregroundProcess;
    NLM_CONNECTIVITY  _connectivity;
    QSet<DWORD>       _restrictedProcesses;
    quintptr          _opId;
    RuleOptions       _options;
    RulesModel       *_rulesModel;
    Status            _status;

    bool conditionsMet            (const QString &selectedProcess, const MProcessInfo &runningProcess);
    bool isTargetProcess          (const MProcessInfo &runningProcess);
    void restrictProcess          (const MProcessInfo &runningProcess);
    void restrictSelectedProcesses();

    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

  public Q_SLOTS:
    void on_networkNotifier_connectivityChanged(NLM_CONNECTIVITY newConnectivity);
    void on_processNotifier_ended              (DWORD id);
    void on_processNotifier_started            (const MProcessInfo &processInfo);
    void on_winEventNotifier_notify            (const MWinEventInfo &winEventInfo);
};

using RuleSPtr     = QSharedPointer<Rule>;
using RuleSPtrList = QList<RuleSPtr>;

#endif