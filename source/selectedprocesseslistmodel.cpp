#include "selectedprocesseslistmodel.h"

SelectedProcessesListModel::SelectedProcessesListModel(RuleOptions *options, RuleOptions::SelectedProcesses type) : _options(options), _type(type)
{
}

void SelectedProcessesListModel::add(const QString &name)
{
  auto addPosition = rowCount();
  beginInsertRows(QModelIndex(), addPosition, addPosition);

  _options->setSelectedProcess(_type, addPosition, name);

  endInsertRows();
}

void SelectedProcessesListModel::remove(quintptr index)
{
  beginRemoveRows(QModelIndex(), index, index);

  _options->removeSelectedProcess(_type, index);

  endRemoveRows();
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

  return _options->selectedProcess(_type, index.row());
}