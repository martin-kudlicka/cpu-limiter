#include "rule.h"

Rule::Rule(const MUuidPtr &id) : _options(id)
{
}

bool Rule::active() const
{
  // TODO
  return false;
}

RuleOptions &Rule::options()
{
  return _options;
}