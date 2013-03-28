/********************************************************************************
** Form generated from reading UI file 'event_time_selection_dialog.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENT_TIME_SELECTION_DIALOG_H
#define UI_EVENT_TIME_SELECTION_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EventTimeSelectionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *list_widget_;
    QHBoxLayout *horizontalLayout;
    QPushButton *select_all_button_;
    QPushButton *unselect_all_button_;
    QComboBox *event_combo_box_;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *number_of_events_label_;
    QLabel *label;
    QDoubleSpinBox *length_spinbox_;
    QLabel *label_2;
    QDoubleSpinBox *add_before_spinbox_;
    QDialogButtonBox *button_box_;

    void setupUi(QDialog *EventTimeSelectionDialog)
    {
        if (EventTimeSelectionDialog->objectName().isEmpty())
            EventTimeSelectionDialog->setObjectName(QString::fromUtf8("EventTimeSelectionDialog"));
        EventTimeSelectionDialog->resize(287, 377);
        verticalLayout = new QVBoxLayout(EventTimeSelectionDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        list_widget_ = new QListWidget(EventTimeSelectionDialog);
        list_widget_->setObjectName(QString::fromUtf8("list_widget_"));
        list_widget_->setSelectionMode(QAbstractItemView::MultiSelection);

        verticalLayout->addWidget(list_widget_);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        select_all_button_ = new QPushButton(EventTimeSelectionDialog);
        select_all_button_->setObjectName(QString::fromUtf8("select_all_button_"));

        horizontalLayout->addWidget(select_all_button_);

        unselect_all_button_ = new QPushButton(EventTimeSelectionDialog);
        unselect_all_button_->setObjectName(QString::fromUtf8("unselect_all_button_"));

        horizontalLayout->addWidget(unselect_all_button_);


        verticalLayout->addLayout(horizontalLayout);

        event_combo_box_ = new QComboBox(EventTimeSelectionDialog);
        event_combo_box_->setObjectName(QString::fromUtf8("event_combo_box_"));

        verticalLayout->addWidget(event_combo_box_);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(EventTimeSelectionDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        number_of_events_label_ = new QLabel(EventTimeSelectionDialog);
        number_of_events_label_->setObjectName(QString::fromUtf8("number_of_events_label_"));

        gridLayout->addWidget(number_of_events_label_, 0, 1, 1, 1);

        label = new QLabel(EventTimeSelectionDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        length_spinbox_ = new QDoubleSpinBox(EventTimeSelectionDialog);
        length_spinbox_->setObjectName(QString::fromUtf8("length_spinbox_"));

        gridLayout->addWidget(length_spinbox_, 1, 1, 1, 1);

        label_2 = new QLabel(EventTimeSelectionDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        add_before_spinbox_ = new QDoubleSpinBox(EventTimeSelectionDialog);
        add_before_spinbox_->setObjectName(QString::fromUtf8("add_before_spinbox_"));

        gridLayout->addWidget(add_before_spinbox_, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        button_box_ = new QDialogButtonBox(EventTimeSelectionDialog);
        button_box_->setObjectName(QString::fromUtf8("button_box_"));
        button_box_->setOrientation(Qt::Horizontal);
        button_box_->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(button_box_);


        retranslateUi(EventTimeSelectionDialog);
        QObject::connect(button_box_, SIGNAL(accepted()), EventTimeSelectionDialog, SLOT(accept()));
        QObject::connect(button_box_, SIGNAL(rejected()), EventTimeSelectionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EventTimeSelectionDialog);
    } // setupUi

    void retranslateUi(QDialog *EventTimeSelectionDialog)
    {
        EventTimeSelectionDialog->setWindowTitle(QApplication::translate("EventTimeSelectionDialog", "Event Time Channel", 0, QApplication::UnicodeUTF8));
        select_all_button_->setText(QApplication::translate("EventTimeSelectionDialog", "Select All", 0, QApplication::UnicodeUTF8));
        unselect_all_button_->setText(QApplication::translate("EventTimeSelectionDialog", "Unselect All", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("EventTimeSelectionDialog", "Number of Events", 0, QApplication::UnicodeUTF8));
        number_of_events_label_->setText(QString());
        label->setText(QApplication::translate("EventTimeSelectionDialog", "Length", 0, QApplication::UnicodeUTF8));
        length_spinbox_->setSuffix(QApplication::translate("EventTimeSelectionDialog", "s", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EventTimeSelectionDialog", "Start before", 0, QApplication::UnicodeUTF8));
        add_before_spinbox_->setSuffix(QApplication::translate("EventTimeSelectionDialog", "s", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventTimeSelectionDialog: public Ui_EventTimeSelectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENT_TIME_SELECTION_DIALOG_H
