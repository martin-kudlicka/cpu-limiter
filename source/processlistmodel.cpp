#include "pch.h"
#include "processlistmodel.h"

QVariant ProcessListModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  if (role == Qt::DecorationRole)
  {
    auto filePath = data(index, Qt::EditRole).toString();
    if (QFile::exists(filePath))
    {
      return QFileIconProvider().icon(filePath);
    }
  }

  return QStringListModel::data(index, role);
}