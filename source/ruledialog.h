#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include "ui_ruledialog.h"
#include <MkWidgets/MWidgetSettings>
#include "selectedprocesseslistmodel.h"

class RuleDialog : public QDialog
{
  Q_OBJECT

  public:
             RuleDialog(QWidget *parent);
             RuleDialog(MUuidPtr &&id, QWidget *parent);
    virtual ~RuleDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    MWidgetSettings            _widgetSettings;
    RuleOptions                _options;
    Ui::RuleDialog             _ui;
    SelectedProcessesListModel _conditionProcesses;
    SelectedProcessesListModel _targetProcesses;

    void setupSettings ();
    void setupWidgets  ();
    void updateOkButton(bool preEnabled = true) const;

    virtual void accept() Q_DECL_OVERRIDE;

  private slots:
    void on_conditionProcessAdd_clicked(bool checked = false);
    void on_name_textChanged           (const QString &text) const;
    void on_targetProcessAdd_clicked   (bool checked = false);
};

#endif