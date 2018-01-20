#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include "ui_ruledialog.h"
#include <MkWidgets/MWidgetSettings>
#include <QtCore/QStringListModel>
#include "ruleoptions.h"

class RuleDialog : public QDialog
{
  Q_OBJECT

  public:
             RuleDialog(QWidget *parent);
             RuleDialog(MUuidPtr &&id, QWidget *parent);
             RuleDialog(const MUuidPtr &id, QWidget *parent);
    virtual ~RuleDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    const RuleOptions &options() const;

  private:
    MWidgetSettings  _widgetSettings;
    QStringListModel _conditionProcessesModel;
    QStringListModel _targetProcessesModel;
    RuleOptions      _options;
    Ui::RuleDialog   _ui;

    void addProcess     (QStringListModel *model, const QString &process) const;
    void removeProcesses(QListView *view) const;
    void setupSettings  ();
    void setupWidgets   ();
    void updateOkButton (bool preEnabled = true) const;

    virtual void accept() Q_DECL_OVERRIDE;

  private slots:
    void on_conditionInternetConnection_stateChanged       (int state)                                                        const;
    void on_conditionProcessAdd_clicked                    (bool checked = false);
    void on_conditionProcessRemove_clicked                 (bool checked = false);
    void on_conditionSelectedProcessesList_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
    void on_conditionStatusNotRunning_clicked              (bool checked = false)                                             const;
    void on_conditionStatusRunning_clicked                 (bool checked = false)                                             const;
    void on_name_textChanged                               (const QString &text)                                              const;
    void on_targetProcessAdd_clicked                       (bool checked = false);
    void on_targetProcessRemove_clicked                    (bool checked = false);
    void on_targetSelectedProcessesList_selectionChanged   (const QItemSelection &selected, const QItemSelection &deselected) const;
};

#endif