#ifndef RULESMODEL_H
#define RULESMODEL_H

#include <QtCore/QAbstractItemModel>
#include "rules.h"

class RulesModel : public QAbstractItemModel
{
  public:
             RulesModel(MGovernor *governor);
    virtual ~RulesModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void   insert(const MUuidPtr &id);
    void   remove(const QModelIndex &index);
    Rules *rules ();

  private:
    enum class Column
    {
      Enabled,
      Name,
      Count
    };

    MGovernor *_governor;
    Rules      _rules;

    virtual int           columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant      data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags      (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual QVariant      headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex   index      (int row, int column, const QModelIndex &parent = QModelIndex())       const Q_DECL_OVERRIDE;
    virtual bool          insertRows (int row, int count, const QModelIndex &parent = QModelIndex())              Q_DECL_OVERRIDE;
    virtual QModelIndex   parent     (const QModelIndex &child)                                             const Q_DECL_OVERRIDE;
    virtual bool          removeRows (int row, int count, const QModelIndex &parent = QModelIndex())              Q_DECL_OVERRIDE;
    virtual int           rowCount   (const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual bool          setData    (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)   Q_DECL_OVERRIDE;
};

#endif