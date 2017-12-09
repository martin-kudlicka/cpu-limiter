#include "rule.h"

Rule::Rule(const MUuidPtr &id) : _options(id)
{
}

const RuleOptions &Rule::options() const
{
  return _options;
}