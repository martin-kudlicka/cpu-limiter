#include "ruleoptions.h"

const QString RuleOptions::Property::Enabled = "enabled";
const QString RuleOptions::Property::Name    = "name";

const QString RuleOptions::Property::Condition_Status = "condition/status";

const QString RuleOptions::Property::Target_Action   = "target/action";
const QString RuleOptions::Property::Target_CPULimit = "target/cpuLimit";

RuleOptions::RuleOptions(MUuidPtr &&id) : _id(qMove(id))
{
  beginGroup("rules");
  beginGroup(id.toString());
}