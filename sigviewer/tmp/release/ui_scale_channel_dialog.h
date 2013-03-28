/********************************************************************************
** Form generated from reading UI file 'scale_channel_dialog.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCALE_CHANNEL_DIALOG_H
#define UI_SCALE_CHANNEL_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ScaleChannelDialog
{
public:
    QVBoxLayout *verticalLayout;
    QRadioButton *autoButton;
    QRadioButton *fixedButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *unitLabelUpper;
    QDoubleSpinBox *upper_spinbox_;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *unitLabelLower;
    QDoubleSpinBox *lower_spinbox_;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ScaleChannelDialog)
    {
        if (ScaleChannelDialog->objectName().isEmpty())
            ScaleChannelDialog->setObjectName(QString::fromUtf8("ScaleChannelDialog"));
        ScaleChannelDialog->resize(339, 170);
        verticalLayout = new QVBoxLayout(ScaleChannelDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        autoButton = new QRadioButton(ScaleChannelDialog);
        autoButton->setObjectName(QString::fromUtf8("autoButton"));
        autoButton->setChecked(true);

        verticalLayout->addWidget(autoButton);

        fixedButton = new QRadioButton(ScaleChannelDialog);
        fixedButton->setObjectName(QString::fromUtf8("fixedButton"));

        verticalLayout->addWidget(fixedButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ScaleChannelDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        unitLabelUpper = new QLabel(ScaleChannelDialog);
        unitLabelUpper->setObjectName(QString::fromUtf8("unitLabelUpper"));

        horizontalLayout->addWidget(unitLabelUpper);

        upper_spinbox_ = new QDoubleSpinBox(ScaleChannelDialog);
        upper_spinbox_->setObjectName(QString::fromUtf8("upper_spinbox_"));
        upper_spinbox_->setEnabled(false);

        horizontalLayout->addWidget(upper_spinbox_);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(ScaleChannelDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        unitLabelLower = new QLabel(ScaleChannelDialog);
        unitLabelLower->setObjectName(QString::fromUtf8("unitLabelLower"));

        horizontalLayout_2->addWidget(unitLabelLower);

        lower_spinbox_ = new QDoubleSpinBox(ScaleChannelDialog);
        lower_spinbox_->setObjectName(QString::fromUtf8("lower_spinbox_"));
        lower_spinbox_->setEnabled(false);
        lower_spinbox_->setMinimum(-23);

        horizontalLayout_2->addWidget(lower_spinbox_);


        verticalLayout->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(ScaleChannelDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ScaleChannelDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ScaleChannelDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ScaleChannelDialog, SLOT(reject()));
        QObject::connect(fixedButton, SIGNAL(toggled(bool)), lower_spinbox_, SLOT(setEnabled(bool)));
        QObject::connect(fixedButton, SIGNAL(toggled(bool)), upper_spinbox_, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(ScaleChannelDialog);
    } // setupUi

    void retranslateUi(QDialog *ScaleChannelDialog)
    {
        ScaleChannelDialog->setWindowTitle(QApplication::translate("ScaleChannelDialog", "Scale Channel", 0, QApplication::UnicodeUTF8));
        autoButton->setText(QApplication::translate("ScaleChannelDialog", "Auto Scaling", 0, QApplication::UnicodeUTF8));
        fixedButton->setText(QApplication::translate("ScaleChannelDialog", "Fixed Scaling", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ScaleChannelDialog", "Upper Value", 0, QApplication::UnicodeUTF8));
        unitLabelUpper->setText(QString());
        label_2->setText(QApplication::translate("ScaleChannelDialog", "Lower Value", 0, QApplication::UnicodeUTF8));
        unitLabelLower->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ScaleChannelDialog: public Ui_ScaleChannelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCALE_CHANNEL_DIALOG_H
