#include "cameradiscovery.h"
#include "ui_cameradiscovery.h"

cameraDiscovery::cameraDiscovery(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::cameraDiscovery)
{
  ui->setupUi(this);
  show();
  udp1 = new MyUDP(this);
  connect(udp1,SIGNAL(updateList(QString)),this,SLOT(onUDPReceived(QString)));
}

cameraDiscovery::~cameraDiscovery()
{
  delete ui;
}

void cameraDiscovery::on_pushButton_4_clicked()
{
    close();
}

void cameraDiscovery::on_pushButton_clicked()
{
  QString command = "Request";
  udp1->deviceDiscover(command);
}

void cameraDiscovery::on_pushButton_2_clicked()
{
  currentIPs.clear();
  ui->tableWidget->setRowCount(0);
}

void cameraDiscovery::onUDPReceived(QString address){
  if(!currentIPs.contains(address)){
      currentIPs.append(address);
      qDebug() << "Updating List";
      QHostInfo HI = QHostInfo::fromName(address);
      int row = ui->tableWidget->rowCount();
      ui->tableWidget->insertRow(row);
      ui->tableWidget->setItem(row,0,new QTableWidgetItem(HI.hostName()));
      ui->tableWidget->setItem(row,1,new QTableWidgetItem(address));
    }
}
