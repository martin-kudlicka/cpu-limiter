#include "rulesmodel.h"

#include <QtGui/QIcon>

RulesModel::RulesModel() : _rules(this)
{
}

MUuidPtr RulesModel::id(const QModelIndex &index) const
{
  return index.internalId();
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

RuleSPtr RulesModel::rule(const MUuidPtr &id)
{
  return _rules.get(id);
}

RuleSPtr RulesModel::rule(const QModelIndex &index)
{
  return _rules.get(index.internalId());
}

void RulesModel::setDataChanged(const MUuidPtr &id, Column column)
{
  auto row = _rules.index(id);
  setDataChanged(row, column);
}

void RulesModel::setDataChanged(int row, Column column)
{
  auto index2 = index(row, static_cast<int>(column));
  emit dataChanged(index2, index2);
}

int RulesModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant RulesModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  if (role != Qt::DisplayRole && role != Qt::DecorationRole && role != Qt::CheckStateRole)
  {
    return QVariant();
  }

  auto rule = const_cast<Rules *>(&_rules)->get(index.internalId());

  switch (role)
  {
    case Qt::DisplayRole:
      switch (index.column())
      {
        case Column::Name:
          return rule->options().name();
      }
      break;
    case Qt::DecorationRole:
      switch (index.column())
      {
        case Column::Status:
          switch (rule->status())
          {
            case Rule::Status::Inactive:
              return QIcon(":/resources/images/status/inactive.png");
            case Rule::Status::Active:
              return rule->isRestricting() ? QIcon(":/resources/images/status/activerestricting.png") : QIcon(":/resources/images/status/activenotrestricting.png");
              break;
            case Rule::Status::Delayed:
              return QIcon(":/resources/images/status/delayed.png");
            default:
              Q_ASSERT_X(false, "RulesModel::data", "switch (rule->status())");
          }
      }
      break;
    case Qt::CheckStateRole:
      switch (index.column())
      {
        case Column::Enabled:
          return rule->options().enabled() ? Qt::Checked : Qt::Unchecked;
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
      return QVariant();
    case Column::Status:
      return QVariant();
    case Column::Name:
      return tr("Name");
    default:
      Q_UNREACHABLE();
  }

  return QVariant();
}

QModelIndex RulesModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  if (_rules.isEmpty())
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
  return _rules.count();
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
        if (rule->conditionsMet())
        {
          rule->activate();
        }
      }
      else
      {
        if (rule->status() != Rule::Status::Inactive)
        {
          rule->deactivate();
        }
      }

      emit dataChanged(index, index);
      setDataChanged(index.row(), Column::Status);

      return true;
    default:
      Q_UNREACHABLE();
  }

  return false;
}