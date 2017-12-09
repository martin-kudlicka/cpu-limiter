#ifndef RULE_H
#define RULE_H

#include <QtCore/QSharedPointer>
#include "ruleoptions.h"

class Rule
{
  public:
    const RuleOptions &options() const;

  private:
    RuleOptions _options;
};

using RuleSPtr = QSharedPointer<Rule>;

#endif