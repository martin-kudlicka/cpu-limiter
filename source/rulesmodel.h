#ifndef RULESMODEL_H
#define RULESMODEL_H

#include <QtCore/QAbstractItemModel>
#include "rules.h"

class RulesModel : public QAbstractItemModel
{
  public:
    virtual ~RulesModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void insert(const MUuidPtr &id);

  private:
    enum class Column
    {
      Name,
      Count
    };

    Rules _rules;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                           const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                const Q_DECL_OVERRIDE;
    virtual QVariant    headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())      const Q_DECL_OVERRIDE;
    virtual bool        insertRows (int row, int count, const QModelIndex &parent = QModelIndex())             Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &child)                                            const Q_DECL_OVERRIDE;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                           const Q_DECL_OVERRIDE;
};

#endif