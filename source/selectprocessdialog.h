#ifndef SELECTPOCESSDIALOG_H
#define SELECTPOCESSDIALOG_H

#include "ui_selectprocessdialog.h"
#include <MkWidgets/MProcessesModel>

class SelectProcessDialog : public QDialog
{
  Q_OBJECT

  public:
             SelectProcessDialog(QWidget *parent);
    virtual ~SelectProcessDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    QString process() const;

  private:
    MProcessesModel _processesModel;
    Ui::SelectProcessDialog _ui;

  private Q_SLOTS:
    void on_browse_clicked              (bool checked /* false */);
    void on_process_textChanged         (const QString &text) const;
    void on_processesView_currentChanged(const QModelIndex &current, const QModelIndex &previous) const;
};

#endif