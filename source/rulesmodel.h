#ifndef RULESMODEL_H
#define RULESMODEL_H

#include "rules.h"

class RulesModel : public QAbstractItemModel
{
  public:
    enum class Column
    {
      Enabled,
      Status,
      Name,
      Count
    };

             RulesModel();
    virtual ~RulesModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    MUuidPtr id            (const QModelIndex &index) const;
    void     insert        (const MUuidPtr &id);
    void     remove        (const QModelIndex &index);
    RuleSPtr rule          (const MUuidPtr &id);
    RuleSPtr rule          (const QModelIndex &index);
    void     setDataChanged(const MUuidPtr &id, Column column);

  private:
    Rules _rules;

    void setDataChanged(int row, Column column);

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