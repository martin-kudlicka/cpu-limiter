#include "pch.h"
#include "ruleoptions.h"

#include "rules.h"

const QString RuleOptions::Property::Enabled = "enabled";
const QString RuleOptions::Property::Name    = "name";

const QString RuleOptions::Property::Condition_InternetConnection       = "condition/internetConnection";
const QString RuleOptions::Property::Condition_InternetConnectionStatus = "condition/internetConnectionStatus";
const QString RuleOptions::Property::Condition_SelectedProcesses        = "condition/selectedProcesses";
const QString RuleOptions::Property::Condition_State                    = "condition/state";
const QString RuleOptions::Property::Condition_Status                   = "condition/status";

const QString RuleOptions::Property::Target_Action            = "target/action";
const QString RuleOptions::Property::Target_ApplyDelay        = "target/applyDelay";
const QString RuleOptions::Property::Target_ApplyDelayValue   = "target/applyDelayValue";
const QString RuleOptions::Property::Target_CPULimit          = "target/cpuLimit";
const QString RuleOptions::Property::Target_SelectedProcesses = "target/selectedProcesses";

RuleOptions::RuleOptions(const MUuidPtr &id) : _id(id)
{
  beginGroup(Rules::Property::Group);
  beginGroup(id.toString());
}

RuleOptions::Action RuleOptions::action() const
{
  return gsl::narrow<Action>(value(Property::Target_Action).toUInt());
}

bool RuleOptions::applyDelay() const
{
  return value(Property::Target_ApplyDelay).toBool();
}

quintptr RuleOptions::applyDelayValue() const
{
  return value(Property::Target_ApplyDelayValue).toUInt();
}

quintptr RuleOptions::cpuLimit() const
{
  return value(Property::Target_CPULimit).toUInt();
}

bool RuleOptions::enabled() const
{
  return value(Property::Enabled).toBool();
}

const MUuidPtr &RuleOptions::id() const
{
  return _id;
}

bool RuleOptions::internetConnectionCheck() const
{
  return value(Property::Condition_InternetConnection).toBool();
}

RuleOptions::InternetConnection RuleOptions::internetConnectionStatus() const
{
  return gsl::narrow<InternetConnection>(value(Property::Condition_InternetConnectionStatus).toUInt());
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

  for (decltype(size) index = 0; index < size; ++index)
  {
    setArrayIndex(index);

    auto name = value("value").toString();
    names.append(name);
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

void RuleOptions::setEnabled(bool enabled)
{
  setValue(Property::Enabled, enabled ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

RuleOptions::State RuleOptions::state() const
{
  return gsl::narrow<State>(value(Property::Condition_State).toUInt());
}

RuleOptions::Status RuleOptions::status() const
{
  return gsl::narrow<Status>(value(Property::Condition_Status).toUInt());
}