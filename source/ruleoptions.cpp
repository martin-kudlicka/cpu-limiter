#include "ruleoptions.h"

#include "rules.h"

Q_DECL_CONSTEXPR QString RuleOptions::Property::Enabled = "enabled";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Name    = "name";

Q_DECL_CONSTEXPR QString RuleOptions::Property::Condition_SelectedProcesses = "condition/selectedProcesses";
Q_DECL_CONSTEXPR QString RuleOptions::Property::Condition_State             = "condition/state";
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

bool RuleOptions::enabled() const
{
  return value(Property::Enabled).toBool();
}

const MUuidPtr &RuleOptions::id() const
{
  return _id;
}

QString RuleOptions::name() const
{
  return value(Property::Name).toString();
}

QString RuleOptions::selectedProcess(Section section, quintptr index)
{
  switch (section)
  {
    case Section::Condition:
      beginReadArray(Property::Condition_SelectedProcesses);
      break;
    case Section::Target:
      beginReadArray(Property::Target_SelectedProcesses);
      break;
    default:
      Q_ASSERT_X(false, "RuleOptions::selectedProcess", "switch (section)");
      return QString();
  }

  setArrayIndex(index);

  auto name = value("value").toString();

  endArray();

  return name;
}

QStringList RuleOptions::selectedProcesses(Section section)
{
  QStringList names;

  quintptr size;
  switch (section)
  {
    case Section::Condition:
      size = beginReadArray(Property::Condition_SelectedProcesses);
      break;
    case Section::Target:
      size = beginReadArray(Property::Target_SelectedProcesses);
      break;
    default:
      Q_ASSERT_X(false, "RuleOptions::selectedProcesses", "switch (section)");
      return names;
  }

  for (auto index = 0; index < size; index++)
  {
    setArrayIndex(index);

    auto name = value("value").toString();
    names.append(qMove(name));
  }

  endArray();

  return names;
}

quintptr RuleOptions::selectedProcessesSize(Section section)
{
  quintptr size;

  switch (section)
  {
    case Section::Condition:
      size = beginReadArray(Property::Condition_SelectedProcesses);
      break;
    case Section::Target:
      size = beginReadArray(Property::Target_SelectedProcesses);
      break;
    default:
      Q_ASSERT_X(false, "RuleOptions::selectedProcessesSize", "switch (section)");
      return 0;
  }

  endArray();

  return size;
}