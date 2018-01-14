#include "rulesmodel.h"

#include <MkCore/MProcessInfo>

RulesModel::RulesModel(MGovernor *governor) : _governor(governor)
{
}

void RulesModel::insert(const MUuidPtr &id)
{
  auto row = _rules.index(id);

  insertRow(row);
}

void RulesModel::remove(const QModelIndex &index)
{
  removeRow(index.row());
}

Rules *RulesModel::rules()
{
  return &_rules;
}

int RulesModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant RulesModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  if (role != Qt::DisplayRole && role != Qt::CheckStateRole)
  {
    return QVariant();
  }

  auto rule = const_cast<Rules *>(&_rules)->get(index.internalId());

  switch (role)
  {
    case Qt::CheckStateRole:
      switch (index.column())
      {
        case Column::Enabled:
          return rule->options().enabled() ? Qt::Checked : Qt::Unchecked;
      }
      break;
    case Qt::DisplayRole:
      switch (index.column())
      {
        case Column::Name:
          return rule->options().name();
      }
  }

  return QVariant();
}

Qt::ItemFlags RulesModel::flags(const QModelIndex &index) const
{
  auto itemFlags = QAbstractItemModel::flags(index);

  switch (index.column())
  {
    case Column::Enabled:
      itemFlags |= Qt::ItemIsUserCheckable;
  }

  return itemFlags;
}

QVariant RulesModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  if (orientation == Qt::Orientation::Vertical || role != Qt::DisplayRole)
  {
    return QVariant();
  }

  switch (section)
  {
    case Column::Enabled:
      return tr("*");
    case Column::Name:
      return tr("Name");
    default:
      Q_UNREACHABLE();
  }

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

bool RulesModel::removeRows(int row, int count, const QModelIndex &parent /* QModelIndex() */)
{
  beginRemoveRows(parent, row, row + count - 1);

  _rules.removeIndex(row);

  endRemoveRows();

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

bool RulesModel::setData(const QModelIndex &index, const QVariant &value, int role /* Qt::EditRole */)
{
  if (role != Qt::CheckStateRole)
  {
    return QAbstractItemModel::setData(index, value, role);
  }

  auto rule = const_cast<Rules *>(&_rules)->get(index.internalId());

  switch (index.column())
  {
    case Column::Enabled:
      rule->options().setEnabled(value.toBool());

      if (value.toBool())
      {
        if (rule->conditionsMet(MProcessInfo(GetForegroundWindow())))
        {
          rule->activate(_governor);
        }
      }
      else
      {
        if (rule->active())
        {
          rule->deactivate(_governor);
        }
      }

      emit dataChanged(index, index);
      return true;
    default:
      Q_UNREACHABLE();
  }

  return false;
}