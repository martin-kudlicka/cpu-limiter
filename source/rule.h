#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"
#include <MkCore/MLazySingleton>
#include <MkProcessGovernor/MGovernor>

class Rule
{
  public:
    Rule(const MUuidPtr &id);

    void         activate     ();
    bool         active       () const;
    bool         conditionsMet();
    RuleOptions &options      ();

  private:
    bool        _active;
    quintptr    _opId;
    RuleOptions _options;

    static MLazySingleton<MGovernor> _governor;

    bool conditionProcessRunning  ();
    void restrictSelectedProcesses();
};

using RuleSPtr     = QSharedPointer<Rule>;
using RulesPtrList = QList<RuleSPtr>;

#endif