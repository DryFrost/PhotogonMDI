#include "treatmentnumdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

treatmentNumDialog::treatmentNumDialog(QWidget *parent,int numb,QStringList treats) : QDialog(parent)
{

    auto *lytMain = new QFormLayout(this);

    for (int i = 0; i < numb; ++i)
    {
        QLabel *tLabel = new QLabel(QString("Number of levels for ")+treats[i], this);
        auto *tLine = new QLineEdit(this);
        lytMain->addRow(tLabel, tLine);

        fields << tLine;
    }

    auto *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                   this, &treatmentNumDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &treatmentNumDialog::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
    //show();
}

QStringList treatmentNumDialog::getStrings(QWidget *parent,int numb,QStringList lis, bool *ok)
{
    auto *dialog = new treatmentNumDialog(parent,numb,lis);

    QStringList list;

    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;

    if (ret) {
        foreach (auto field, dialog->fields) {
            list << field->text();
        }
    }

    dialog->deleteLater();

    return list;
}
