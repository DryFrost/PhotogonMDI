#include "newproject.h"
#include "ui_newproject.h"
NewProject::NewProject(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewProject)
{
  ui->setupUi(this);
  show();
}

NewProject::~NewProject()
{
  delete ui;
}

void NewProject::on_SetDirectory_clicked()
{
  ProjectDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               "/home",
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
}

void NewProject::on_Generate_clicked()
{

  ProjectName = ui->projectName->text();
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("ProjectDir",QVariant::fromValue(ProjectDir));
  internal.setValue("ProjectName",QVariant::fromValue(ProjectName));

  if(ui->qrBox->isChecked()){
      new QrGenerate();


    }
  if(ui->listBox->isChecked()){
      new listGenerate();

    }
  if(ui->manualBox->isChecked()){

    }

}

void NewProject::on_projectName_textChanged(const QString &arg1)
{
  if(ui->projectName->text().isEmpty()){
      ui->Generate->setEnabled(false);
    }else{
      ui->Generate->setEnabled(true);
    }
}

void NewProject::on_pushButton_5_clicked()
{
    close();
}

void NewProject::on_Config_clicked()
{
  new configDialog();
}

void NewProject::on_SetupCameras_clicked()
{

}

void NewProject::on_pushButton_4_clicked()
{
  int tLowM = 40;
  int tHighM = 255;
  int b1LM = 0;
  int b1HM = 120;
  int b2LM = 135;
  int b2HM = 200;
  int blurKM = 6;

  int x1 = 300;
  int y1 = 100;
  int x2 = 1000;
  int y2 = 650;

  int a = 0;

  QString FileProjectDir;
  FileProjectDir = ProjectDir+"/"+ProjectName+".ini";
  QSettings setup(FileProjectDir,QSettings::IniFormat);
  setup.setValue("ProjectName",QVariant::fromValue(ProjectName));
  setup.setValue("ProjectDir",QVariant::fromValue(ProjectDir));
  setup.setValue("FtLowM",QVariant::fromValue(tLowM));
  setup.setValue("FtHighM",QVariant::fromValue(tHighM));
  setup.setValue("Fb1LM",QVariant::fromValue(b1LM));
  setup.setValue("Fb1HM",QVariant::fromValue(b1HM));
  setup.setValue("Fb2LM",QVariant::fromValue(b2LM));
  setup.setValue("Fb2HM",QVariant::fromValue(b2HM));
  setup.setValue("FblurKM",QVariant::fromValue(blurKM));

  setup.setValue("TtLowM",QVariant::fromValue(tLowM));
  setup.setValue("TtHighM",QVariant::fromValue(tHighM));
  setup.setValue("Tb1LM",QVariant::fromValue(b1LM));
  setup.setValue("Tb1HM",QVariant::fromValue(b1HM));
  setup.setValue("Tb2LM",QVariant::fromValue(b2LM));
  setup.setValue("Tb2HM",QVariant::fromValue(b2HM));
  setup.setValue("TblurKM",QVariant::fromValue(blurKM));

  setup.setValue("StLowM",QVariant::fromValue(tLowM));
  setup.setValue("StHighM",QVariant::fromValue(tHighM));
  setup.setValue("Sb1LM",QVariant::fromValue(b1LM));
  setup.setValue("Sb1HM",QVariant::fromValue(b1HM));
  setup.setValue("Sb2LM",QVariant::fromValue(b2LM));
  setup.setValue("Sb2HM",QVariant::fromValue(b2HM));
  setup.setValue("SblurKM",QVariant::fromValue(blurKM));

  setup.setValue("x1Top",QVariant::fromValue(x1));
  setup.setValue("x1Side",QVariant::fromValue(x1));
  setup.setValue("x1Front",QVariant::fromValue(x1));

  setup.setValue("y1Top",QVariant::fromValue(y1));
  setup.setValue("y1Side",QVariant::fromValue(y1));
  setup.setValue("y1Front",QVariant::fromValue(y1));

  setup.setValue("x2Top",QVariant::fromValue(x2));
  setup.setValue("x2Side",QVariant::fromValue(x2));
  setup.setValue("x2Front",QVariant::fromValue(x2));

  setup.setValue("y2Top",QVariant::fromValue(y2));
  setup.setValue("y2Side",QVariant::fromValue(y2));
  setup.setValue("y2Front",QVariant::fromValue(y2));

  setup.setValue("a",QVariant::fromValue(a));
  setup.setValue("b",QVariant::fromValue(a));


  QString Qr;
  QString List;
  QString Manual;
  if(ui->qrBox->isChecked()){
      Qr = "true";
      List = "false";
      Manual = "false";
    }
  if(ui->listBox->isChecked()){
      Qr = "false";
      List = "true";
      Manual = "false";
    }
  if(ui->manualBox->isChecked()){
      Qr = "false";
      List = "false";
      Manual = "true";
    }
  setup.setValue("Qr",QVariant::fromValue(Qr));
  setup.setValue("List",QVariant::fromValue(List));
  setup.setValue("Manual",QVariant::fromValue(Manual));
  close();

}
