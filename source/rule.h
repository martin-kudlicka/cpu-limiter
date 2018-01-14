#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"

class MGovernor;
class MProcessInfo;

class Rule
{
  public:
    Rule(const MUuidPtr &id);

    void         activate       (MGovernor *governor);
    bool         active         () const;
    bool         conditionsMet  (const MProcessInfo &foregroundProcess);
    bool         conditionsMet  (const MProcessInfo &runningProcess, const MProcessInfo &foregroundProcess);
    void         deactivate     (MGovernor *governor);
    bool         isRestricting  () const;
    bool         isTargetProcess(const MProcessInfo &runningProcess);
    RuleOptions &options        ();
    void         restrictProcess(MGovernor *governor, const MProcessInfo &runningProcess);

  private:
    bool        _active;
    quintptr    _opId;
    RuleOptions _options;

    bool conditionsMet            (const QString &selectedProcess, const MProcessInfo &runningProcess, const MProcessInfo &foregroundProcess);
    void restrictSelectedProcesses(MGovernor *governor);
};

using RuleSPtr     = QSharedPointer<Rule>;
using RuleSPtrList = QList<RuleSPtr>;

#endif