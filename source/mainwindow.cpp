#include "mainwindow.h"

#include "ruledialog.h"

MainWindow::MainWindow() : _ruleMonitor(_rulesModel.rules())
{
	_ui.setupUi(this);

  setupWidgets();
}

void MainWindow::setupWidgets()
{
  _ui.rules->setModel(&_rulesModel);
  connect(_ui.rules->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::on_rules_selectionChanged);
}

void MainWindow::on_ruleAdd_clicked(bool checked /* false */)
{
  RuleDialog ruleDialog(this);
  if (ruleDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  _rulesModel.insert(ruleDialog.options().id());

  auto foregroundProcess = MProcessInfo(GetForegroundWindow());
  auto rule = _rulesModel.rules()->get(ruleDialog.options().id());
  if (rule->conditionsMet(foregroundProcess))
  {
    rule->activate(_ruleMonitor.governor());
  }
}

void MainWindow::on_ruleRemove_clicked(bool checked /* false */)
{
  auto index = _ui.rules->currentIndex();
  auto id    = _rulesModel.rules()->id(index.row());
  auto rule  = _rulesModel.rules()->get(id);
  if (rule->active())
  {
    rule->deactivate(_ruleMonitor.governor());
  }

  _rulesModel.remove(index);
}

void MainWindow::on_rules_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  auto isSelected = !_ui.rules->selectionModel()->selectedRows().isEmpty();
  _ui.ruleRemove->setEnabled(isSelected);
}