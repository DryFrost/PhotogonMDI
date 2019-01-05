#ifndef SELECTCOLORCHIPS_H
#define SELECTCOLORCHIPS_H

#include <QWidget>

namespace Ui {
  class selectColorChips;
}

class selectColorChips : public QWidget
{
  Q_OBJECT

public:
  explicit selectColorChips(QWidget *parent = nullptr);
  ~selectColorChips();

private:
  Ui::selectColorChips *ui;
};

#endif // SELECTCOLORCHIPS_H
