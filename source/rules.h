#ifndef RULES_H
#define RULES_H

#include "rule.h"

class Rules : public MSettingsGroup<Rule>
{
  public:
    struct Property
    {
      static const QString Group;
    };

    explicit Rules(RulesModel *rulesModel);

    const RuleSPtr &get(const MUuidPtr &id);

    virtual void removeIndex(quintptr index) Q_DECL_OVERRIDE;

  private:
    MNetworkNotifier          _networkNotifier;
    MProcessGovernor          _processGovernor;
    MWinEventNotifier         _winEventNotifier;
    QHash<MUuidPtr, RuleSPtr> _rules;
    QSettings                 _settings;
    RulesModel               *_rulesModel;

    RuleSPtrList get();
};

#endif