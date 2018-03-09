#ifndef PROCESSLISTMODEL_H
#define PROCESSLISTMODEL_H

#include <QtCore/QStringListModel>

class ProcessListModel : public QStringListModel
{
  public:
    virtual ~ProcessListModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

};

#endif