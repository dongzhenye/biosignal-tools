/********************************************************************************
** Form generated from reading UI file 'event_creation_widget.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENT_CREATION_WIDGET_H
#define UI_EVENT_CREATION_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventCreationWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QComboBox *type_combobox_;

    void setupUi(QWidget *EventCreationWidget)
    {
        if (EventCreationWidget->objectName().isEmpty())
            EventCreationWidget->setObjectName(QString::fromUtf8("EventCreationWidget"));
        horizontalLayout = new QHBoxLayout(EventCreationWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        type_combobox_ = new QComboBox(EventCreationWidget);
        type_combobox_->setObjectName(QString::fromUtf8("type_combobox_"));

        horizontalLayout->addWidget(type_combobox_);


        retranslateUi(EventCreationWidget);

        QMetaObject::connectSlotsByName(EventCreationWidget);
    } // setupUi

    void retranslateUi(QWidget *EventCreationWidget)
    {
        EventCreationWidget->setWindowTitle(QApplication::translate("EventCreationWidget", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventCreationWidget: public Ui_EventCreationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENT_CREATION_WIDGET_H
