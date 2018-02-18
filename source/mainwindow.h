#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "rulesmodel.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
  	         MainWindow();
    virtual ~MainWindow() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    Ui::MainWindow _ui;
    RulesModel     _rulesModel;

    void setupWidgets();

  private Q_SLOTS:
    void on_actionAbout_triggered       (bool checked = false);
    void on_actionSendFeedback_triggered(bool checked = false) const;
    void on_ruleAdd_clicked             (bool checked = false);
    void on_ruleEdit_clicked            (bool checked = false);
    void on_ruleRemove_clicked          (bool checked = false);
    void on_rules_selectionChanged      (const QItemSelection &selected, const QItemSelection &deselected) const;
};

#endif