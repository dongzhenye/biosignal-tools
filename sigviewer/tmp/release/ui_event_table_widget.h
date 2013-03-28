/********************************************************************************
** Form generated from reading UI file 'event_table_widget.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENT_TABLE_WIDGET_H
#define UI_EVENT_TABLE_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventTableWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *event_table_;

    void setupUi(QWidget *EventTableWidget)
    {
        if (EventTableWidget->objectName().isEmpty())
            EventTableWidget->setObjectName(QString::fromUtf8("EventTableWidget"));
        EventTableWidget->resize(653, 450);
        horizontalLayout = new QHBoxLayout(EventTableWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        event_table_ = new QTableWidget(EventTableWidget);
        if (event_table_->columnCount() < 5)
            event_table_->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        event_table_->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        event_table_->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        event_table_->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        event_table_->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        event_table_->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        event_table_->setObjectName(QString::fromUtf8("event_table_"));
        event_table_->setMinimumSize(QSize(400, 0));
        event_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        event_table_->setSelectionMode(QAbstractItemView::MultiSelection);
        event_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
        event_table_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        event_table_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        event_table_->setSortingEnabled(true);
        event_table_->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(event_table_);


        retranslateUi(EventTableWidget);

        QMetaObject::connectSlotsByName(EventTableWidget);
    } // setupUi

    void retranslateUi(QWidget *EventTableWidget)
    {
        EventTableWidget->setWindowTitle(QApplication::translate("EventTableWidget", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = event_table_->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("EventTableWidget", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = event_table_->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("EventTableWidget", "Position", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = event_table_->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("EventTableWidget", "Duration", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = event_table_->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("EventTableWidget", "Channel", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = event_table_->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("EventTableWidget", "Type", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventTableWidget: public Ui_EventTableWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENT_TABLE_WIDGET_H
