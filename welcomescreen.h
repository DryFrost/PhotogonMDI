#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QDialog>
#include <QSettings>
#include <QMenu>

class MainWindow;

namespace Ui {
  class welcomeScreen;
}

class welcomeScreen : public QDialog
{
  Q_OBJECT

public:
  explicit welcomeScreen(QWidget *parent = nullptr);
  ~welcomeScreen();

private slots:
  void on_commandLinkButton_clicked();

  void on_commandLinkButton_2_clicked();

private:
  Ui::welcomeScreen *ui;
  MainWindow* mMainWindow;
  QSettings* mSettings;
};

#endif // WELCOMESCREEN_H
