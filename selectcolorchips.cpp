#include "selectcolorchips.h"
#include "ui_selectcolorchips.h"

selectColorChips::selectColorChips(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::selectColorChips)
{
  ui->setupUi(this);
}

selectColorChips::~selectColorChips()
{
  delete ui;
}
