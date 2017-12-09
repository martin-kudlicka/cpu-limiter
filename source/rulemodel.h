#ifndef RULEMODEL_H
#define RULEMODEL_H

#include <QtCore/QAbstractItemModel>

class RuleModel : public QAbstractItemModel
{
  public:
    virtual ~RuleModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)           const Q_DECL_OVERRIDE;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &child)                                       const Q_DECL_OVERRIDE;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;
};

#endif