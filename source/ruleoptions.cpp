#include "ruleoptions.h"

Q_DECL_CONSTEXPR QString RuleOptions::Property::Enabled           = "enabled";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Name              = "name";

Q_DECL_CONSTEXPR QString RuleOptions::Property::Condition_SelectedProcesses = "condition/selectedProcesses";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Condition_Status            = "condition/status";

Q_DECL_CONSTEXPR QString RuleOptions::Property::Target_Action            = "target/action";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Target_CPULimit          = "target/cpuLimit";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Target_SelectedProcesses = "target/selectedProcesses";

RuleOptions::RuleOptions(MUuidPtr &&id) : _id(qMove(id))
{
  beginGroup("rules");
  beginGroup(id.toString());
}

QString RuleOptions::selectedProcesses(SelectedProcesses type, quintptr index)
{
  switch (type)
  {
  case SelectedProcesses::Condition:
    beginReadArray(Property::Condition_SelectedProcesses);
    break;
  case SelectedProcesses::Target:
    beginReadArray(Property::Target_SelectedProcesses);
    break;
  default:
    Q_ASSERT_X(false, "RuleOptions::selectedProcesses", "switch (type)");
    return QString();
  }

  setArrayIndex(index);

  auto processName = value("name").toString();

  endArray();

  return processName;
}

quintptr RuleOptions::selectedProcessesSize(SelectedProcesses type)
{
  quintptr size;

  switch (type)
  {
    case SelectedProcesses::Condition:
      size = beginReadArray(Property::Condition_SelectedProcesses);
      break;
    case SelectedProcesses::Target:
      size = beginReadArray(Property::Target_SelectedProcesses);
      break;
    default:
      Q_ASSERT_X(false, "RuleOptions::selectedProcessesSize", "switch (type)");
      return 0;
  }

  endArray();

  return size;
}