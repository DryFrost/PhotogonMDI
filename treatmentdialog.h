#ifndef TREATMENTDIALOG_H
#define TREATMENTDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

class treatmentDialog :public QDialog
{
  Q_OBJECT
public:
  explicit treatmentDialog(QWidget *parent = nullptr, int =3);
  static QStringList getStrings(QWidget *parent,int,bool *ok = nullptr);

private:
  QList<QLineEdit*> fields;
};


#endif // TREATMENTDIALOG_H
