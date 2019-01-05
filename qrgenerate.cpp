#include "qrgenerate.h"
#include "ui_qrgenerate.h"
#include <QDebug>
QrGenerate::QrGenerate(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QrGenerate)
{
  ui->setupUi(this);
  QSettings internal("internal.ini",QSettings::IniFormat);
  ProjectDir = internal.value("ProjectDir").value<QString>();
  ProjectName = internal.value("ProjectName").value<QString>();
  show();
}

QrGenerate::~QrGenerate()
{
  delete ui;
}

void QrGenerate::on_view_clicked()
{
  int brd = 4;
  QString msgfromtextbox = ui->labelinput->text();
  std::string stringtext = msgfromtextbox.toStdString();
  const char *text = stringtext.c_str();
  qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(msgfromtextbox.toUtf8().constData(),qrcodegen::QrCode::Ecc::HIGH);
  std::ofstream myfile;
  myfile.open("temp.svg");
  myfile << qr.toSvgString(brd)<<std::endl;
  myfile.close();

  QSvgRenderer renderer(QString("temp.svg"));
  QImage image(200,200, QImage::Format_ARGB32);
  QPainter painter(&image);
  renderer.render(&painter);

  ui->label_2->setPixmap(QPixmap::fromImage(image));

}

void QrGenerate::on_save_clicked()
{

  fileDir = ProjectDir+"/QRs/";
  if(!QDir(fileDir).exists()){
      QDir().mkdir(fileDir);
  }


  const int brd =4;
  QString msgfromtextbox = ui->labelinput->text();
  std::string stringtext = msgfromtextbox.toStdString();
  const char *text = stringtext.c_str();
  qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(msgfromtextbox.toUtf8().constData(),qrcodegen::QrCode::Ecc::HIGH);
  std::ofstream outfile;
  QString out;
  out = fileDir+msgfromtextbox+".svg";
  outfile.open(out.toStdString());
  outfile << qr.toSvgString(brd)<<std::endl;
  outfile.close();
  ui->labelinput->clear();

}

void QrGenerate::on_accept_clicked()
{
  close();
}

void QrGenerate::on_labelinput_textChanged(const QString &arg1)
{
  if(ui->labelinput->text().isEmpty()){
      ui->view->setEnabled(false);
      ui->save->setEnabled(false);
    }else{
      ui->view->setEnabled(true);
      ui->save->setEnabled(true);
    }
}
