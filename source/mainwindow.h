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

  private slots:
    void on_ruleAdd_clicked(bool checked = false);
};

#endif