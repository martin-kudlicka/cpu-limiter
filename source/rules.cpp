#include "rules.h"

Q_DECL_CONSTEXPR QString Rules::Property::Group = "rules";

Rules::Rules()
{
  _settings.beginGroup(Property::Group);
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

quintptr Rules::size() const
{
  return _settings.childGroups().size();
}