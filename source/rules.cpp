#include "pch.h"
#include "rules.h"

const QString Rules::Property::Group = "rules";

Rules::Rules(RulesModel *rulesModel) : _rulesModel(rulesModel)
{
  _settings.beginGroup(Property::Group);

  for (const auto &rule : get())
  {
    if (rule->options().enabled())
    {
      auto ok = rule->conditionsMet();
      if (ok)
      {
        rule->activate();
      }
    }
  }

  _winEventNotifier.addEvent(EVENT_SYSTEM_FOREGROUND);
}

const RuleSPtr &Rules::get(const MUuidPtr &id)
{
  Q_ASSERT_X(!id.isNull(), "Rules::get", "!id.isNull()");

  if (_rules.contains(id))
  {
    return _rules[id];
  }

  auto rule = _rules.insert(id, QSharedPointer<Rule>::create(id, &_processGovernor, _rulesModel));

  QObject::connect(&_networkNotifier, &MNetworkNotifier::connectivityChanged, &**rule, &Rule::on_networkNotifier_connectivityChanged);

  QObject::connect(&*mProcessNotifier, &MProcessNotifier::started, &**rule, &Rule::on_processNotifier_started);
  QObject::connect(&*mProcessNotifier, &MProcessNotifier::ended,   &**rule, &Rule::on_processNotifier_ended);

  QObject::connect(&_winEventNotifier, &MWinEventNotifier::notify, &**rule, &Rule::on_winEventNotifier_notify);

  return *rule;
}

void Rules::removeIndex(quintptr index)
{
  auto id2 = id(index);

  _rules.remove(id2);
  _settings.remove(id2.toString());
}

RuleSPtrList Rules::get()
{
  auto rulesCount = count();
  if (gsl::narrow<decltype(rulesCount)>(_rules.count()) != rulesCount)
  {
    for (decltype(rulesCount) index2 = 0; index2 < rulesCount; ++index2)
    {
      get(id(index2));
    }
  }

  return _rules.values();
}