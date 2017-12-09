#pragma once

#include <QtCore/QAbstractListModel>
#include "ruleoptions.h"

class SelectedProcessesListModel : public QAbstractListModel
{
  public:
             SelectedProcessesListModel(RuleOptions *options, RuleOptions::SelectedProcesses type);
    virtual ~SelectedProcessesListModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    RuleOptions                   *_options;
    RuleOptions::SelectedProcesses _type;

    virtual int      rowCount(const QModelIndex &parent = QModelIndex())            const Q_DECL_OVERRIDE;
    virtual QVariant data    (const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};