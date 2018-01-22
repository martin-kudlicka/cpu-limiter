#include "rules.h"

#include <MkNotifier/MProcessNotifier>

Q_DECL_CONSTEXPR QString Rules::Property::Group = "rules";

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

bool Rules::empty() const
{
  return size() == 0;
}

const RuleSPtr &Rules::get(const MUuidPtr &id)
{
  if (_rules.contains(id))
  {
    return _rules[id];
  }

  auto rule = _rules.insert(id, RuleSPtr(new Rule(id, &_processGovernor, _rulesModel)));

  QObject::connect(&_networkNotifier, &MNetworkNotifier::connectivityChanged, &**rule, &Rule::on_networkNotifier_connectivityChanged);

  QObject::connect(&*mProcessNotifier, &MProcessNotifier::started, &**rule, &Rule::on_processNotifier_started);
  QObject::connect(&*mProcessNotifier, &MProcessNotifier::ended,   &**rule, &Rule::on_processNotifier_ended);

  QObject::connect(&_winEventNotifier, &MWinEventNotifier::notify, &**rule, &Rule::on_winEventNotifier_notify);

  return *rule;
}

MUuidPtr Rules::id(quintptr index) const
{
  return _settings.childGroups().at(index);
}

quintptr Rules::index(const MUuidPtr &id) const
{
  quintptr index2 = 0;

  for (const auto &id2 : _settings.childGroups())
  {
    if (id2 == id.toString())
    {
      return index2;
    }

    ++index2;
  }

  Q_UNREACHABLE();
  return -1;
}

void Rules::removeIndex(quintptr index)
{
  auto id2 = id(index);
  removeId(id2);
}

quintptr Rules::size() const
{
  return _settings.childGroups().size();
}

RuleSPtrList Rules::get()
{
  if (_rules.size() != size())
  {
    for (auto index2 = 0; index2 < size(); index2++)
    {
      get(id(index2));
    }
  }

  return _rules.values();
}

void Rules::removeId(const MUuidPtr &id)
{
  _rules.remove(id);
  _settings.remove(id.toString());
}