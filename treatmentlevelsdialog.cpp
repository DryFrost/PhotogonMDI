#include "treatmentlevelsdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

treatmentLevelsDialog::treatmentLevelsDialog(QWidget *parent,int numb,QString name) : QDialog(parent)
{

    auto *lytMain = new QFormLayout(this);

    for (int i = 0; i < numb; ++i)
    {
        QLabel *tLabel = new QLabel(name + QString(" Level %1:").arg(i), this);
        auto *tLine = new QLineEdit(this);
        lytMain->addRow(tLabel, tLine);

        fields << tLine;
    }

    auto *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                   this, &treatmentLevelsDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &treatmentLevelsDialog::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
    //show();
}

QStringList treatmentLevelsDialog::getStrings(QWidget *parent,int numb,QString name ,bool *ok)
{
    auto *dialog = new treatmentLevelsDialog(parent,numb,name);

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


