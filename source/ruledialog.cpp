#include "ruledialog.h"

RuleDialog::RuleDialog(QWidget *parent) : RuleDialog(MUuidPtr::createUuid(), parent)
{
}

RuleDialog::RuleDialog(const MUuidPtr &id, QWidget *parent) : QDialog(parent), _widgetSettings(&_options)
{
  _ui.setupUi(this);

  setupSettings();
}

void RuleDialog::setupSettings()
{
  _widgetSettings.setWidget(RuleOptions::Property::Name,    _ui.name);
  _widgetSettings.setWidget(RuleOptions::Property::Enabled, _ui.enabled);

  _widgetSettings.setWidget(RuleOptions::Property::Condition_Status, qMove(QRadioButtonPtrList() << _ui.conditionStatusRunning << _ui.conditionStatusForeground << _ui.conditionStatusBackground << _ui.conditionStatusNotRunning));

  _widgetSettings.setWidget(RuleOptions::Property::Target_Action, qMove(QRadioButtonPtrList() << _ui.targetActionLimitCPU << _ui.targetActionSuspend));

  _widgetSettings.load();
}

void RuleDialog::updateOkButton(bool preEnabled /* true */) const
{
  auto enabled = preEnabled && !_ui.name->text().isEmpty();

  _ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

void RuleDialog::accept()
{
  _widgetSettings.save();

  QDialog::accept();
}

void RuleDialog::on_name_textChanged(const QString &text) const
{
  updateOkButton(!text.isEmpty());
}