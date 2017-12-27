#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"

class Rule
{
  public:
    Rule(const MUuidPtr &id);

    bool         active       () const;
    bool         conditionsMet();
    RuleOptions &options      ();

  private:
    RuleOptions _options;

    bool conditionProcessRunning();
};

using RuleSPtr     = QSharedPointer<Rule>;
using RulesPtrList = QList<RuleSPtr>;

#endif