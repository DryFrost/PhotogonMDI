#include "cameradiscovery.h"
#include "ui_cameradiscovery.h"

cameraDiscovery::cameraDiscovery(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::cameraDiscovery)
{
  ui->setupUi(this);
  show();
  udp = new MyUDP(this);
  connect(udp,SIGNAL(updateList(QString)),this,SLOT(onUDPReceived(QString)));
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
  udp->deviceDiscover();
}

void cameraDiscovery::on_pushButton_2_clicked()
{
  currentIPs.clear();
  ui->tableWidget->setRowCount(0);
}

void cameraDiscovery::onUDPReceived(const QString address){
  if(!currentIPs.contains(address)){
      currentIPs.append(address);
      QHostInfo HI = QHostInfo::fromName(address);
      int row = ui->tableWidget->rowCount();
      ui->tableWidget->insertRow(row);
      ui->tableWidget->setItem(row,0,new QTableWidgetItem(HI.hostName()));
      ui->tableWidget->setItem(row,1,new QTableWidgetItem(address));
    }
}
