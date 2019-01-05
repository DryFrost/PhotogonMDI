#ifndef TREATMENTNUMDIALOG_H
#define TREATMENTNUMDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;


class treatmentNumDialog :public QDialog
{
  Q_OBJECT
public:
  explicit treatmentNumDialog(QWidget *parent = nullptr, int =3, QStringList = QStringList());
  static QStringList getStrings(QWidget *parent,int,QStringList,bool *ok = nullptr);

private:
  QList<QLineEdit*> fields;
};

#endif // TREATMENTNUMDIALOG_H
