#ifndef TREATMENTLEVELSDIALOG_H
#define TREATMENTLEVELSDIALOG_H


#include <QDialog>

class QLineEdit;
class QLabel;

class treatmentLevelsDialog :public QDialog
{
  Q_OBJECT
public:
  explicit treatmentLevelsDialog(QWidget *parent = nullptr, int =3, QString ="");
  static QStringList getStrings(QWidget *parent,int,QString,bool *ok = nullptr);

private:
  QList<QLineEdit*> fields;
};

#endif // TREATMENTLEVELSDIALOG_H
