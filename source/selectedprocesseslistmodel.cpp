#include "selectedprocesseslistmodel.h"

SelectedProcessesListModel::SelectedProcessesListModel(RuleOptions *options, RuleOptions::SelectedProcesses type) : _options(options), _type(type)
{
}

int SelectedProcessesListModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return _options->selectedProcessesSize(_type);
}

QVariant SelectedProcessesListModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  return _options->selectedProcesses(_type, index.row());
}