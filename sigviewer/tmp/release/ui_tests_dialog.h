/********************************************************************************
** Form generated from reading UI file 'tests_dialog.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTS_DIALOG_H
#define UI_TESTS_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TestDialog
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *start_button_;
    QPushButton *open_dummy_;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TestDialog)
    {
        if (TestDialog->objectName().isEmpty())
            TestDialog->setObjectName(QString::fromUtf8("TestDialog"));
        TestDialog->resize(398, 298);
        verticalLayout = new QVBoxLayout(TestDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        progressBar = new QProgressBar(TestDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        listWidget = new QListWidget(TestDialog);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        start_button_ = new QPushButton(TestDialog);
        start_button_->setObjectName(QString::fromUtf8("start_button_"));

        horizontalLayout->addWidget(start_button_);

        open_dummy_ = new QPushButton(TestDialog);
        open_dummy_->setObjectName(QString::fromUtf8("open_dummy_"));

        horizontalLayout->addWidget(open_dummy_);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(TestDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(TestDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TestDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TestDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TestDialog);
    } // setupUi

    void retranslateUi(QDialog *TestDialog)
    {
        TestDialog->setWindowTitle(QApplication::translate("TestDialog", "Tests", 0, QApplication::UnicodeUTF8));
        start_button_->setText(QApplication::translate("TestDialog", "Start", 0, QApplication::UnicodeUTF8));
        open_dummy_->setText(QApplication::translate("TestDialog", "Open Dummy Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TestDialog: public Ui_TestDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTS_DIALOG_H
