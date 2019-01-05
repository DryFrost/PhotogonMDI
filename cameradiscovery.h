#ifndef CAMERADISCOVERY_H
#define CAMERADISCOVERY_H

#include <QDialog>
#include "myudp.h"
#include <QtWidgets>
#include <QHostInfo>

namespace Ui {
  class cameraDiscovery;
}

class cameraDiscovery : public QDialog
{
  Q_OBJECT

public:
  explicit cameraDiscovery(QWidget *parent = nullptr);
  ~cameraDiscovery();
public slots:
  void onUDPReceived(QString);

private slots:
  void on_pushButton_4_clicked();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  Ui::cameraDiscovery *ui;
  MyUDP* udp;
  QStringList currentIPs;
};

#endif // CAMERADISCOVERY_H
