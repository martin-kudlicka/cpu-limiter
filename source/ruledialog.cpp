#include "ruledialog.h"

RuleDialog::RuleDialog(QWidget *parent) : QDialog(parent), _widgetSettings(&_options)
{
  _ui.setupUi(this);

  setupSettings();
}

void RuleDialog::setupSettings()
{
  _widgetSettings.setWidget(RuleOptions::Property::Name,    _ui.name);
  _widgetSettings.setWidget(RuleOptions::Property::Enabled, _ui.enabled);

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