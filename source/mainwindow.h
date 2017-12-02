#pragma once

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
  	MainWindow();

  private:
	  Ui::MainWindow _ui;
};