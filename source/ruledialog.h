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

    const RuleOptions &options() const;

  private:
    MWidgetSettings            _widgetSettings;
    RuleOptions                _options;
    Ui::RuleDialog             _ui;
    SelectedProcessesListModel _conditionProcessesModel;
    SelectedProcessesListModel _targetProcessesModel;

    void setupSettings ();
    void setupWidgets  ();
    void updateOkButton(bool preEnabled = true) const;

    virtual void accept() Q_DECL_OVERRIDE;

  private slots:
    void on_conditionProcessAdd_clicked                    (bool checked = false);
    void on_conditionProcessRemove_clicked                 (bool checked = false);
    void on_conditionSelectedProcessesList_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
    void on_name_textChanged                               (const QString &text)                                              const;
    void on_targetProcessAdd_clicked                       (bool checked = false);
    void on_targetProcessRemove_clicked                    (bool checked = false);
    void on_targetSelectedProcessesList_selectionChanged   (const QItemSelection &selected, const QItemSelection &deselected) const;
};

#endif