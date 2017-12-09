#include "rulesmodel.h"

void RulesModel::insert(const MUuidPtr &id)
{
  auto row = _rules.index(id);

  insertRow(row);
}

int RulesModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant RulesModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  // TODO
  return QVariant();
}

QModelIndex RulesModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  if (_rules.empty())
  {
    return createIndex(row, column);
  }

  return createIndex(row, column, _rules.id(row));
}

bool RulesModel::insertRows(int row, int count, const QModelIndex &parent /* QModelIndex() */)
{
  beginInsertRows(parent, row, row + count - 1);
  // already added
  endInsertRows();

  return true;
}

QModelIndex RulesModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int RulesModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return _rules.size();
}