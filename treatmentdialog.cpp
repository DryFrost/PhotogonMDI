#include "treatmentdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

treatmentDialog::treatmentDialog(QWidget *parent,int numb) : QDialog(parent)
{

    auto *lytMain = new QFormLayout(this);

    for (int i = 0; i < numb; ++i)
    {
        QLabel *tLabel = new QLabel(QString("Treatment %1:").arg(i), this);
        auto *tLine = new QLineEdit(this);
        lytMain->addRow(tLabel, tLine);

        fields << tLine;
    }

    auto *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                   this, &treatmentDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &treatmentDialog::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
    //show();
}

QStringList treatmentDialog::getStrings(QWidget *parent,int numb, bool *ok)
{
    auto *dialog = new treatmentDialog(parent,numb);

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

