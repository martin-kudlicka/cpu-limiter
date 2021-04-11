#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include "ui_ruledialog.h"
#include "processlistmodel.h"
#include "ruleoptions.h"

class RuleDialog : public MOptionsDialog<Ui::RuleDialog, RuleOptions>
{
  Q_OBJECT

  public:
             RuleDialog(QWidget *parent);
             RuleDialog(const MUuidPtr &id, QWidget *parent);
    virtual ~RuleDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    ProcessListModel _conditionProcessesModel;
    ProcessListModel _targetProcessesModel;

    void addProcess     (QStringListModel *model, const QString &process) const;
    void removeProcesses(QListView *view)                                 const;
    void updateOkButton (bool preEnabled = true)                          const;

    virtual void setupSettings() Q_DECL_OVERRIDE;
    virtual void setupWidgets () Q_DECL_OVERRIDE;

  private Q_SLOTS:
    void on_conditionInternetConnection_stateChanged       (int state)                                                        const;
    void on_conditionProcessAdd_clicked                    (bool checked = false);
    void on_conditionProcessRemove_clicked                 (bool checked = false);
    void on_conditionSelectedProcessesList_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
    void on_conditionStatusNotRunning_clicked              (bool checked = false)                                             const;
    void on_conditionStatusRunning_clicked                 (bool checked = false)                                             const;
    void on_name_textChanged                               (const QString &text)                                              const;
    void on_targetApplyDelay_stateChanged                  (int state)                                                        const;
    void on_targetProcessAdd_clicked                       (bool checked = false);
    void on_targetProcessRemove_clicked                    (bool checked = false);
    void on_targetSelectedProcessesList_selectionChanged   (const QItemSelection &selected, const QItemSelection &deselected) const;
};

#endif