/********************************************************************************
** Form generated from reading UI file 'event_type_selection_dialog.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENT_TYPE_SELECTION_DIALOG_H
#define UI_EVENT_TYPE_SELECTION_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EventTypeSelectionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *tree_widget_;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QPushButton *select_all_button_;
    QPushButton *unselect_all_button_;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *all_events_button_;
    QRadioButton *existing_events_button_;
    QGroupBox *show_colors_box_;
    QVBoxLayout *verticalLayout_4;
    QPushButton *reset_colors_button_;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EventTypeSelectionDialog)
    {
        if (EventTypeSelectionDialog->objectName().isEmpty())
            EventTypeSelectionDialog->setObjectName(QString::fromUtf8("EventTypeSelectionDialog"));
        EventTypeSelectionDialog->resize(470, 494);
        verticalLayout = new QVBoxLayout(EventTypeSelectionDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tree_widget_ = new QTreeWidget(EventTypeSelectionDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(3, QString::fromUtf8("4"));
        __qtreewidgetitem->setText(2, QString::fromUtf8("3"));
        __qtreewidgetitem->setText(1, QString::fromUtf8("2"));
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tree_widget_->setHeaderItem(__qtreewidgetitem);
        tree_widget_->setObjectName(QString::fromUtf8("tree_widget_"));
        tree_widget_->setIndentation(10);
        tree_widget_->setWordWrap(true);
        tree_widget_->setColumnCount(4);
        tree_widget_->header()->setDefaultSectionSize(75);
        tree_widget_->header()->setMinimumSectionSize(0);

        verticalLayout->addWidget(tree_widget_);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        select_all_button_ = new QPushButton(EventTypeSelectionDialog);
        select_all_button_->setObjectName(QString::fromUtf8("select_all_button_"));

        verticalLayout_3->addWidget(select_all_button_);

        unselect_all_button_ = new QPushButton(EventTypeSelectionDialog);
        unselect_all_button_->setObjectName(QString::fromUtf8("unselect_all_button_"));

        verticalLayout_3->addWidget(unselect_all_button_);


        horizontalLayout->addLayout(verticalLayout_3);

        groupBox = new QGroupBox(EventTypeSelectionDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        all_events_button_ = new QRadioButton(groupBox);
        all_events_button_->setObjectName(QString::fromUtf8("all_events_button_"));

        verticalLayout_2->addWidget(all_events_button_);

        existing_events_button_ = new QRadioButton(groupBox);
        existing_events_button_->setObjectName(QString::fromUtf8("existing_events_button_"));
        existing_events_button_->setChecked(true);

        verticalLayout_2->addWidget(existing_events_button_);


        horizontalLayout->addWidget(groupBox);

        show_colors_box_ = new QGroupBox(EventTypeSelectionDialog);
        show_colors_box_->setObjectName(QString::fromUtf8("show_colors_box_"));
        show_colors_box_->setCheckable(true);
        show_colors_box_->setChecked(true);
        verticalLayout_4 = new QVBoxLayout(show_colors_box_);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        reset_colors_button_ = new QPushButton(show_colors_box_);
        reset_colors_button_->setObjectName(QString::fromUtf8("reset_colors_button_"));

        verticalLayout_4->addWidget(reset_colors_button_);


        horizontalLayout->addWidget(show_colors_box_);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(EventTypeSelectionDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EventTypeSelectionDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), EventTypeSelectionDialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), EventTypeSelectionDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(EventTypeSelectionDialog);
    } // setupUi

    void retranslateUi(QDialog *EventTypeSelectionDialog)
    {
        EventTypeSelectionDialog->setWindowTitle(QApplication::translate("EventTypeSelectionDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        select_all_button_->setText(QApplication::translate("EventTypeSelectionDialog", "Select All", 0, QApplication::UnicodeUTF8));
        unselect_all_button_->setText(QApplication::translate("EventTypeSelectionDialog", "Unselect All", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("EventTypeSelectionDialog", "View", 0, QApplication::UnicodeUTF8));
        all_events_button_->setText(QApplication::translate("EventTypeSelectionDialog", "All Events", 0, QApplication::UnicodeUTF8));
        existing_events_button_->setText(QApplication::translate("EventTypeSelectionDialog", "Existing Events", 0, QApplication::UnicodeUTF8));
        show_colors_box_->setTitle(QApplication::translate("EventTypeSelectionDialog", "Color Settings", 0, QApplication::UnicodeUTF8));
        reset_colors_button_->setText(QApplication::translate("EventTypeSelectionDialog", "Reset All Colors", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventTypeSelectionDialog: public Ui_EventTypeSelectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENT_TYPE_SELECTION_DIALOG_H
