#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setStyle("Material");
  MainWindow w;
  QApplication::setOrganizationName("MST");
  QApplication::setOrganizationDomain("mst.edu");
  QApplication::setApplicationName("Photogon");
  w.show();

  return a.exec();
}
