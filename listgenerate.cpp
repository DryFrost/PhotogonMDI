#include "listgenerate.h"
#include "ui_listgenerate.h"

listGenerate::listGenerate(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::listGenerate)
{
  ui->setupUi(this);
  QSettings internal("internal.ini",QSettings::IniFormat);
  ProjectDir = internal.value("ProjectDir").value<QString>();
  ProjectName = internal.value("ProjectName").value<QString>();
  show();
}

QList<int> list2Int(const QStringList& treatList){
  QList<int> intList;
  foreach(const QString& list,treatList)
    for(int i = 0; i < list.size();i++){
        bool isNum = false;
        int n = QString(list.at(i)).toInt(&isNum);

        if(isNum)
          intList << n;
      }
  return intList;
}

listGenerate::~listGenerate()
{
  delete ui;
}

void listGenerate::on_pushButton_clicked()
{
  numTreatments = ui->spinBox->value();
  numRepeats = ui->spinBox_2->value();
  treatmentList = treatmentDialog::getStrings(this,numTreatments);
  treatmentNumList = treatmentNumDialog::getStrings(this,numTreatments,treatmentList);
  QList<int> numList = list2Int(treatmentNumList);
  for(int i = 0; i<numTreatments;i++){
      model << treatmentLevelsDialog::getStrings(this,numList[i],treatmentList[i]);
    }
  ui->pushButton_4->setEnabled(true);


}

void listGenerate::on_pushButton_3_clicked()
{
    close();
}

void listGenerate::on_pushButton_4_clicked()
{
  int a =0;
  int b =0;
  QString FileProjectDir;
  FileProjectDir = ProjectDir+"/"+ProjectName+".ini";
  QSettings setup(FileProjectDir,QSettings::IniFormat);
  setup.setValue("numTreatments",QVariant::fromValue(numTreatments));
  setup.setValue("numRepeats",QVariant::fromValue(numRepeats));
  setup.setValue("treatmentList",QVariant::fromValue(treatmentList));
  setup.setValue("treatmentNumList",QVariant::fromValue(treatmentNumList));
  setup.setValue("model",QVariant::fromValue(model));

  setup.setValue("a",QVariant::fromValue(a));
  setup.setValue("b",QVariant::fromValue(b));
  close();

}
