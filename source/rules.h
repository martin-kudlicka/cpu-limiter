#ifndef RULES_H
#define RULES_H

#include <QtCore/QSettings>
#include "rule.h"

class Rules
{
  public:
    class Property
    {
      public:
        static Q_DECL_CONSTEXPR QString Group;
    };

    Rules();

    bool     empty()                   const;
    MUuidPtr id   (quintptr index)     const;
    quintptr index(const MUuidPtr &id) const;
    quintptr size ()                   const;

  private:
    QMap<MUuidPtr, RuleSPtr> _rules;
    QSettings                _settings;
};

#endif