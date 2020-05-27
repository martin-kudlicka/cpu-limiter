#include "selectprocessdialog.h"

#include <QtWidgets/QFileDialog>

SelectProcessDialog::SelectProcessDialog(QWidget *parent) : QDialog(parent), _processesModel(MProcesses::Filter::ValidName | MProcesses::Filter::ValidPath)
{
  _ui.setupUi(this);

  _ui.processesView->setModel(&_processesModel);

  _ui.processesView->header()->setSectionResizeMode(static_cast<int>(MProcessesModel::Column::Name), QHeaderView::ResizeToContents);
  _ui.processesView->header()->setSectionResizeMode(static_cast<int>(MProcessesModel::Column::Id),   QHeaderView::ResizeToContents);

  connect(_ui.processesView->selectionModel(), &QItemSelectionModel::currentChanged, this, &SelectProcessDialog::on_processesView_currentChanged);
}

QString SelectProcessDialog::process() const
{
  return _ui.process->text();
}

void SelectProcessDialog::on_browse_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  auto filePath = QFileDialog::getOpenFileName(this, tr("Select file"), QString(), "*.exe");
  if (filePath.isNull())
  {
    return;
  }

  _ui.process->setText(QDir::toNativeSeparators(filePath));
}

void SelectProcessDialog::on_process_textChanged(const QString &text) const
{
  _ui.okButton->setEnabled(!text.isEmpty());
}

void SelectProcessDialog::on_processesView_currentChanged(const QModelIndex &current, const QModelIndex &previous) const
{
  Q_UNUSED(previous);

  auto index    = _processesModel.index(current.row(), static_cast<int>(MProcessesModel::Column::FilePath), current.parent());
  auto filePath = _processesModel.data(index).toString();

  _ui.process->setText(filePath);
}