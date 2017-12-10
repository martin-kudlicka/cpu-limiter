#include "mainwindow.h"

#include "ruledialog.h"

MainWindow::MainWindow()
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
}

void MainWindow::on_ruleRemove_clicked(bool checked /* false */)
{
  auto index = _ui.rules->currentIndex();
  _rulesModel.remove(index);
}

void MainWindow::on_rules_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  auto isSelected = !_ui.rules->selectionModel()->selectedRows().isEmpty();
  _ui.ruleRemove->setEnabled(isSelected);
}