#include "pch.h"
#include "mainwindow.h"

#include "ruledialog.h"

MainWindow::MainWindow()
{
  _ui.setupUi(this);

  setupWidgets();
}

void MainWindow::setupWidgets()
{
  _rulesProxyModel.setSourceModel(&_rulesModel);
  _ui.rules->setModel(&_rulesProxyModel);

  _ui.rules->header()->setSectionResizeMode(gsl::narrow<int>(RulesModel::Column::Enabled), QHeaderView::ResizeToContents);
  _ui.rules->header()->setSectionResizeMode(gsl::narrow<int>(RulesModel::Column::Status),  QHeaderView::ResizeToContents);
  _ui.rules->header()->setSectionResizeMode(gsl::narrow<int>(RulesModel::Column::Name),    QHeaderView::Stretch);
  _ui.rules->header()->setSortIndicator(gsl::narrow<int>(RulesModel::Column::Name), Qt::AscendingOrder);

  connect(_ui.rules->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::on_rules_selectionChanged);
}

void MainWindow::editRule(const QModelIndex &index)
{
  auto modelIndex = _rulesProxyModel.mapToSource(index);
  auto id         = _rulesModel.id(modelIndex);

  RuleDialog ruleDialog(id, this);
  if (ruleDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  auto rule = _rulesModel.rule(id);
  if (rule->status() != Rule::Status::Inactive)
  {
    rule->deactivate();

    if (rule->options().enabled())
    {
      if (rule->conditionsMet())
      {
        rule->activate();
      }
    }
  }
}

void MainWindow::on_actionAbout_triggered(bool checked /* false */)
{
  Q_UNUSED(checked);

  MAboutBox(this).exec();
}

void MainWindow::on_actionSendFeedback_triggered(bool checked /* false */) const
{
  Q_UNUSED(checked);

  MFeedback::createEmailForm();
}

void MainWindow::on_ruleAdd_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  RuleDialog ruleDialog(this);
  if (ruleDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  _rulesModel.insert(ruleDialog.options().id());

  auto rule = _rulesModel.rule(ruleDialog.options().id());
  if (rule->options().enabled())
  {
    if (rule->conditionsMet())
    {
      rule->activate();
    }
  }
}

void MainWindow::on_ruleEdit_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  editRule(_ui.rules->currentIndex());
}

void MainWindow::on_ruleRemove_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  auto index = _rulesProxyModel.mapToSource(_ui.rules->currentIndex());
  auto rule  = _rulesModel.rule(index);
  if (rule->status() != Rule::Status::Inactive)
  {
    rule->deactivate();
  }

  _rulesModel.remove(index);
}

void MainWindow::on_rules_doubleClicked(const QModelIndex &index)
{
  editRule(index);
}

void MainWindow::on_rules_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  auto isSelected = !_ui.rules->selectionModel()->selectedRows().isEmpty();

  _ui.ruleEdit->setEnabled(isSelected);
  _ui.ruleRemove->setEnabled(isSelected);
}