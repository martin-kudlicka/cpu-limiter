#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"

class Rule
{
  public:
    Rule(const MUuidPtr &id);

    const RuleOptions &options() const;

  private:
    RuleOptions _options;
};

using RuleSPtr = QSharedPointer<Rule>;

#endif