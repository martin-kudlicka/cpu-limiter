#include <MkWidgets/MApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  MApplication application(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return application.exec();
}