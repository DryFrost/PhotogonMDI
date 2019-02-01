#include "qrgenerate.h"
#include "ui_qrgenerate.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
using namespace cv;
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
cv::Mat qimage_to_mat_ref(QImage &img, int format)
{
    return cv::Mat(img.height(), img.width(),
            format, img.bits(), img.bytesPerLine());
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
  QImage image(200,200, QImage::Format_RGB888);


  //imwrite("/Users/dnguyen/Desktop/qrCO.png",view);

  QPainter painter(&image);
  renderer.render(&painter);
  image.save("QRR.png");
  cv::Mat qrA = imread("QRR.png");
  cv::putText(qrA,
                msgfromtextbox.toUtf8().constData(),
                cv::Point(6,20), // Coordinates
                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                0.8, // Scale. 2.0 = 2x bigger
                cv::Scalar(0,0,0), // BGR Color
                1); // Anti-alias (Optional)
  imwrite("/Users/dnguyen/Desktop/qrCO.png",qrA);


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
