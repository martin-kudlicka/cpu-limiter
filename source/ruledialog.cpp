#include "ruledialog.h"

#include <QtWidgets/QInputDialog>

RuleDialog::RuleDialog(QWidget *parent) : RuleDialog(MUuidPtr::createUuid(), parent)
{
}

RuleDialog::RuleDialog(MUuidPtr &&id, QWidget *parent) : QDialog(parent), _options(qMove(id)), _widgetSettings(&_options), _conditionProcessesModel(&_options, RuleOptions::SelectedProcesses::Condition), _targetProcessesModel(&_options, RuleOptions::SelectedProcesses::Target)
{
  _ui.setupUi(this);

  setupWidgets();
  setupSettings();
}

const RuleOptions &RuleDialog::options() const
{
  return _options;
}

void RuleDialog::setupSettings()
{
  _widgetSettings.setWidget(RuleOptions::Property::Name,    _ui.name);
  _widgetSettings.setWidget(RuleOptions::Property::Enabled, _ui.enabled);

  _widgetSettings.setWidget(RuleOptions::Property::Condition_Status, qMove(QRadioButtonPtrList() << _ui.conditionStatusRunning << _ui.conditionStatusForeground << _ui.conditionStatusBackground << _ui.conditionStatusNotRunning));

  _widgetSettings.setWidget(RuleOptions::Property::Target_Action,   qMove(QRadioButtonPtrList() << _ui.targetActionLimitCPU << _ui.targetActionSuspend));
  _widgetSettings.setWidget(RuleOptions::Property::Target_CPULimit, _ui.tagetCPULimit);

  _widgetSettings.load();
}

void RuleDialog::setupWidgets()
{
  _ui.conditionSelectedProcessesList->setModel(&_conditionProcessesModel);
  _ui.targetSelectedProcessesList->setModel(&_targetProcessesModel);
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

void RuleDialog::on_conditionProcessAdd_clicked(bool checked /* false */)
{
  auto process = QInputDialog::getText(this, tr("Add condition process"), tr("Name:"));
  if (process.isEmpty())
  {
    return;
  }

  _conditionProcessesModel.add(process);
}

void RuleDialog::on_name_textChanged(const QString &text) const
{
  updateOkButton(!text.isEmpty());
}

void RuleDialog::on_targetProcessAdd_clicked(bool checked /* false */)
{
  auto process = QInputDialog::getText(this, tr("Add target process"), tr("Name:"));
  if (process.isEmpty())
  {
    return;
  }

  _targetProcessesModel.add(process);
}