#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"

class MGovernor;

class Rule
{
  public:
    Rule(const MUuidPtr &id);

    void         activate     (MGovernor *governor);
    bool         active       () const;
    bool         conditionsMet();
    RuleOptions &options      ();

  private:
    bool        _active;
    quintptr    _opId;
    RuleOptions _options;

    bool conditionProcessRunning  ();
    void restrictSelectedProcesses(MGovernor *governor);
};

using RuleSPtr     = QSharedPointer<Rule>;
using RulesPtrList = QList<RuleSPtr>;

#endif