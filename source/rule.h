#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"

class MProcessGovernor;
class MProcessInfo;

class Rule
{
  public:
    Rule(const MUuidPtr &id);

    void         activate       (MProcessGovernor *processGovernor);
    bool         active         () const;
    bool         conditionsMet  (const MProcessInfo &foregroundProcess);
    bool         conditionsMet  (const MProcessInfo &runningProcess, const MProcessInfo &foregroundProcess);
    void         deactivate     (MProcessGovernor *processGovernor);
    bool         isRestricting  () const;
    bool         isTargetProcess(const MProcessInfo &runningProcess);
    RuleOptions &options        ();
    void         restrictProcess(MProcessGovernor *processGovernor, const MProcessInfo &runningProcess);

  private:
    bool        _active;
    quintptr    _opId;
    RuleOptions _options;

    bool conditionsMet            (const QString &selectedProcess, const MProcessInfo &runningProcess, const MProcessInfo &foregroundProcess);
    void restrictSelectedProcesses(MProcessGovernor *processGovernor);
};

using RuleSPtr     = QSharedPointer<Rule>;
using RuleSPtrList = QList<RuleSPtr>;

#endif