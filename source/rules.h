#ifndef RULES_H
#define RULES_H

#include <QtCore/QSettings>
#include "rule.h"
#include <MkProcessGovernor/MProcessGovernor>
#include <MkNotifier/MWinEventNotifier>
#include <MkNotifier/MNetworkNotifier>

class Rules
{
  public:
    struct Property
    {
      static Q_DECL_CONSTEXPR QString Group;
    };

    Rules(RulesModel *rulesModel);

    const RuleSPtr    &get        (const MUuidPtr &id);
          MUuidPtr     id         (quintptr index)     const;
          quintptr     index      (const MUuidPtr &id) const;
          bool         isEmpty    ()                   const;
          void         removeIndex(quintptr index);
          quintptr     size       ()                   const;

  private:
    MNetworkNotifier          _networkNotifier;
    MProcessGovernor          _processGovernor;
    MWinEventNotifier         _winEventNotifier;
    QHash<MUuidPtr, RuleSPtr> _rules;
    QSettings                 _settings;
    RulesModel               *_rulesModel;

    RuleSPtrList get     ();
    void         removeId(const MUuidPtr &id);
};

#endif