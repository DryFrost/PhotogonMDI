#ifndef QRGENERATE_H
#define QRGENERATE_H

#include <QDialog>
#include <qrcode.hpp>
#include <QSettings>
#include <QSvgRenderer>
#include <QPainter>
#include <QImage>
#include <iostream>
#include <fstream>
#include <string>
#include <QtCore>

namespace Ui {
  class QrGenerate;
}

class QrGenerate : public QDialog
{
  Q_OBJECT

public:
  explicit QrGenerate(QWidget *parent = nullptr);
  ~QrGenerate();

private slots:
  void on_view_clicked();

  void on_save_clicked();

  void on_accept_clicked();

  void on_labelinput_textChanged(const QString &arg1);

private:
  Ui::QrGenerate *ui;
  QString ProjectDir;
  QString ProjectName;
  QString fileDir;
};

#endif // QRGENERATE_H
