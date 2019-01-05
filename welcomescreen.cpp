#include "welcomescreen.h"
#include "ui_welcomescreen.h"

welcomeScreen::welcomeScreen(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::welcomeScreen)
{
  ui->setupUi(this);
  mSettings = new QSettings("internal.ini",QSettings::IniFormat);
  ui->checkBox->setChecked(mSettings->value("startupDialog",true).toBool());
  if(mSettings->value("recentFiles").toString().isEmpty()){
      ui->toolButton->setEnabled(false);
      ui->toolButton->setText("There is no recent projects");
    }

  show();
}

welcomeScreen::~welcomeScreen()
{
  delete ui;
}

void welcomeScreen::on_commandLinkButton_clicked()
{

  close();

}

void welcomeScreen::on_commandLinkButton_2_clicked()
{


  close();
}


