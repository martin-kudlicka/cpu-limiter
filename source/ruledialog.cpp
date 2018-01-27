#include "ruledialog.h"

#include "selectprocessdialog.h"

RuleDialog::RuleDialog(QWidget *parent) : RuleDialog(MUuidPtr::createUuid(), parent)
{
}

RuleDialog::RuleDialog(MUuidPtr &&id, QWidget *parent) : QDialog(parent), _options(qMove(id)), _widgetSettings(&_options)
{
  _ui.setupUi(this);

  setupWidgets();
  setupSettings();
}

RuleDialog::RuleDialog(const MUuidPtr &id, QWidget *parent) : QDialog(parent), _options(id), _widgetSettings(&_options)
{
  _ui.setupUi(this);

  setupWidgets();
  setupSettings();
}

const RuleOptions &RuleDialog::options() const
{
  return _options;
}

void RuleDialog::addProcess(QStringListModel *model, const QString &process) const
{
  auto row = model->rowCount();
  model->insertRow(row);

  auto index = model->index(row);
  model->setData(index, process);
}

void RuleDialog::removeProcesses(QListView *view) const
{
  forever
  {
    auto selected = view->selectionModel()->selectedRows();
    if (selected.isEmpty())
    {
      break;
    }

    view->model()->removeRow(selected.first().row());
  }
}

void RuleDialog::setupSettings()
{
  _widgetSettings.setWidget(RuleOptions::Property::Name,    _ui.name);
  _widgetSettings.setWidget(RuleOptions::Property::Enabled, _ui.enabled);

  _widgetSettings.setWidget(RuleOptions::Property::Condition_SelectedProcesses,        _ui.conditionSelectedProcessesList);
  _widgetSettings.setWidget(RuleOptions::Property::Condition_InternetConnection,       _ui.conditionInternetConnection);
  _widgetSettings.setWidget(RuleOptions::Property::Condition_InternetConnectionStatus, _ui.conditionInternetConnectionStatus);
  _widgetSettings.setWidget(RuleOptions::Property::Condition_Status,                   qMove(QRadioButtonPtrList() << _ui.conditionStatusRunning << _ui.conditionStatusNotRunning));
  _widgetSettings.setWidget(RuleOptions::Property::Condition_State,                    _ui.conditionStatusRunningState);

  _widgetSettings.setWidget(RuleOptions::Property::Target_SelectedProcesses, _ui.targetSelectedProcessesList);
  _widgetSettings.setWidget(RuleOptions::Property::Target_Action,            qMove(QRadioButtonPtrList() << _ui.targetActionLimitCPU << _ui.targetActionSuspend));
  _widgetSettings.setWidget(RuleOptions::Property::Target_CPULimit,          _ui.tagetCPULimit);

  _widgetSettings.load();
}

void RuleDialog::setupWidgets()
{
  _ui.targetActionSuspend->hide();

  _ui.conditionSelectedProcessesList->setModel(&_conditionProcessesModel);
  _ui.targetSelectedProcessesList->setModel(&_targetProcessesModel);

  _ui.conditionInternetConnectionStatus->addItem(tr("Connected"),    static_cast<quintptr>(RuleOptions::InternetConnection::Connected));
  _ui.conditionInternetConnectionStatus->addItem(tr("Disconnected"), static_cast<quintptr>(RuleOptions::InternetConnection::Disconnected));

  _ui.conditionStatusRunningState->addItem(tr("Anyhow"),     static_cast<quintptr>(RuleOptions::State::Anyhow));
  _ui.conditionStatusRunningState->addItem(tr("Foreground"), static_cast<quintptr>(RuleOptions::State::Foreground));
  _ui.conditionStatusRunningState->addItem(tr("Background"), static_cast<quintptr>(RuleOptions::State::Background));

  _ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

  connect(_ui.conditionSelectedProcessesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RuleDialog::on_conditionSelectedProcessesList_selectionChanged);
  connect(_ui.targetSelectedProcessesList->selectionModel(),    &QItemSelectionModel::selectionChanged, this, &RuleDialog::on_targetSelectedProcessesList_selectionChanged);
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

void RuleDialog::on_conditionInternetConnection_stateChanged(int state) const
{
  _ui.conditionInternetConnectionStatus->setEnabled(state == Qt::Checked);
}

void RuleDialog::on_conditionProcessAdd_clicked(bool checked /* false */)
{
  SelectProcessDialog selectProcessDialog(this);
  if (selectProcessDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  addProcess(&_conditionProcessesModel, selectProcessDialog.process());
}

void RuleDialog::on_conditionProcessRemove_clicked(bool checked /* false */)
{
  removeProcesses(_ui.conditionSelectedProcessesList);
}

void RuleDialog::on_conditionSelectedProcessesList_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  auto isSelected = !_ui.conditionSelectedProcessesList->selectionModel()->selectedRows().isEmpty();
  _ui.conditionProcessRemove->setEnabled(isSelected);
}

void RuleDialog::on_conditionStatusNotRunning_clicked(bool checked /* false */) const
{
  _ui.conditionStatusRunningState->setEnabled(false);
}

void RuleDialog::on_conditionStatusRunning_clicked(bool checked /* false */) const
{
  _ui.conditionStatusRunningState->setEnabled(true);
}

void RuleDialog::on_name_textChanged(const QString &text) const
{
  updateOkButton(!text.isEmpty());
}

void RuleDialog::on_targetProcessAdd_clicked(bool checked /* false */)
{
  SelectProcessDialog selectProcessDialog(this);
  if (selectProcessDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  addProcess(&_targetProcessesModel, selectProcessDialog.process());
}

void RuleDialog::on_targetProcessRemove_clicked(bool checked /* false */)
{
  removeProcesses(_ui.targetSelectedProcessesList);
}

void RuleDialog::on_targetSelectedProcessesList_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  auto isSelected = !_ui.targetSelectedProcessesList->selectionModel()->selectedRows().isEmpty();
  _ui.targetProcessRemove->setEnabled(isSelected);
}