#include "mainwindow.h"

#include "ruledialog.h"
#include <MkWidgets/MAboutBox>
#include <MkSupport/MFeedback>

MainWindow::MainWindow()
{
	_ui.setupUi(this);

  setupWidgets();
}

void MainWindow::setupWidgets()
{
  _ui.rules->setModel(&_rulesModel);

  _ui.rules->header()->setSectionResizeMode(static_cast<int>(RulesModel::Column::Enabled), QHeaderView::ResizeToContents);
  _ui.rules->header()->setSectionResizeMode(static_cast<int>(RulesModel::Column::Status),  QHeaderView::ResizeToContents);

  connect(_ui.rules->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::on_rules_selectionChanged);
}

void MainWindow::on_actionAbout_triggered(bool checked /* false */)
{
  MAboutBox(this).exec();
}

void MainWindow::on_actionSendFeedback_triggered(bool checked /* false */) const
{
  MFeedback::createEmailForm();
}

void MainWindow::on_ruleAdd_clicked(bool checked /* false */)
{
  RuleDialog ruleDialog(this);
  if (ruleDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  _rulesModel.insert(ruleDialog.options().id());

  auto rule = _rulesModel.rules()->get(ruleDialog.options().id());
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
  auto index = _ui.rules->currentIndex();
  auto id    = _rulesModel.rules()->id(index.row());

  RuleDialog ruleDialog(id, this);
  if (ruleDialog.exec() == QDialog::Rejected)
  {
    return;
  }

  auto rule = _rulesModel.rules()->get(id);
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

void MainWindow::on_ruleRemove_clicked(bool checked /* false */)
{
  auto index = _ui.rules->currentIndex();
  auto id    = _rulesModel.rules()->id(index.row());
  auto rule  = _rulesModel.rules()->get(id);
  if (rule->status() != Rule::Status::Inactive)
  {
    rule->deactivate();
  }

  _rulesModel.remove(index);
}

void MainWindow::on_rules_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  auto isSelected = !_ui.rules->selectionModel()->selectedRows().isEmpty();

  _ui.ruleEdit->setEnabled(isSelected);
  _ui.ruleRemove->setEnabled(isSelected);
}