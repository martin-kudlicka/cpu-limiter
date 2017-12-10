#include "ruleoptions.h"

#include "rules.h"

Q_DECL_CONSTEXPR QString RuleOptions::Property::Enabled = "enabled";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Name    = "name";

Q_DECL_CONSTEXPR QString RuleOptions::Property::Condition_SelectedProcesses = "condition/selectedProcesses";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Condition_Status            = "condition/status";

Q_DECL_CONSTEXPR QString RuleOptions::Property::Target_Action            = "target/action";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Target_CPULimit          = "target/cpuLimit";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Target_SelectedProcesses = "target/selectedProcesses";

RuleOptions::RuleOptions(MUuidPtr &&id) : _id(qMove(id))
{
  beginGroup(Rules::Property::Group);
  beginGroup(id.toString());
}

RuleOptions::RuleOptions(const MUuidPtr &id) : _id(id)
{
  beginGroup(Rules::Property::Group);
  beginGroup(id.toString());
}

const MUuidPtr &RuleOptions::id() const
{
  return _id;
}

QString RuleOptions::name() const
{
  return value(Property::Name).toString();
}

QString RuleOptions::selectedProcess(SelectedProcesses type, quintptr index)
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
      Q_ASSERT_X(false, "RuleOptions::selectedProcess", "switch (type)");
      return QString();
  }

  setArrayIndex(index);

  auto name = value("name").toString();

  endArray();

  return name;
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