#include "mainwindow.h"

#include "ruledialog.h"

MainWindow::MainWindow()
{
	_ui.setupUi(this);

  _ui.rules->setModel(&_rulesModel);
}

void MainWindow::on_ruleAdd_clicked(bool checked /* false */)
{
  RuleDialog ruleDialog(this);
  if (ruleDialog.exec() == QDialog::Rejected)
  {
    // TODO remove
    return;
  }

  _rulesModel.insert(ruleDialog.options().id());
}