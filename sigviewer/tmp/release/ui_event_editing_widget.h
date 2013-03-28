/********************************************************************************
** Form generated from reading UI file 'event_editing_widget.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENT_EDITING_WIDGET_H
#define UI_EVENT_EDITING_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventEditingWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *selection_frame_;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *type_combobox_;
    QLabel *label;
    QDoubleSpinBox *begin_spinbox_;
    QLabel *label_2;
    QDoubleSpinBox *duration_spinbox_;
    QToolButton *fit_button_;
    QToolButton *previous_button_;
    QToolButton *next_button_;

    void setupUi(QWidget *EventEditingWidget)
    {
        if (EventEditingWidget->objectName().isEmpty())
            EventEditingWidget->setObjectName(QString::fromUtf8("EventEditingWidget"));
        EventEditingWidget->setEnabled(true);
        EventEditingWidget->resize(423, 29);
        horizontalLayout = new QHBoxLayout(EventEditingWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        selection_frame_ = new QFrame(EventEditingWidget);
        selection_frame_->setObjectName(QString::fromUtf8("selection_frame_"));
        selection_frame_->setEnabled(false);
        selection_frame_->setFrameShape(QFrame::NoFrame);
        selection_frame_->setLineWidth(0);
        horizontalLayout_2 = new QHBoxLayout(selection_frame_);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        type_combobox_ = new QComboBox(selection_frame_);
        type_combobox_->setObjectName(QString::fromUtf8("type_combobox_"));

        horizontalLayout_2->addWidget(type_combobox_);

        label = new QLabel(selection_frame_);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label);

        begin_spinbox_ = new QDoubleSpinBox(selection_frame_);
        begin_spinbox_->setObjectName(QString::fromUtf8("begin_spinbox_"));

        horizontalLayout_2->addWidget(begin_spinbox_);

        label_2 = new QLabel(selection_frame_);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        duration_spinbox_ = new QDoubleSpinBox(selection_frame_);
        duration_spinbox_->setObjectName(QString::fromUtf8("duration_spinbox_"));

        horizontalLayout_2->addWidget(duration_spinbox_);

        fit_button_ = new QToolButton(selection_frame_);
        fit_button_->setObjectName(QString::fromUtf8("fit_button_"));
        fit_button_->setIconSize(QSize(22, 22));

        horizontalLayout_2->addWidget(fit_button_);


        horizontalLayout->addWidget(selection_frame_);

        previous_button_ = new QToolButton(EventEditingWidget);
        previous_button_->setObjectName(QString::fromUtf8("previous_button_"));
        previous_button_->setEnabled(false);
        previous_button_->setIconSize(QSize(22, 22));

        horizontalLayout->addWidget(previous_button_);

        next_button_ = new QToolButton(EventEditingWidget);
        next_button_->setObjectName(QString::fromUtf8("next_button_"));
        next_button_->setEnabled(false);
        next_button_->setIconSize(QSize(22, 22));

        horizontalLayout->addWidget(next_button_);


        retranslateUi(EventEditingWidget);

        QMetaObject::connectSlotsByName(EventEditingWidget);
    } // setupUi

    void retranslateUi(QWidget *EventEditingWidget)
    {
        EventEditingWidget->setWindowTitle(QApplication::translate("EventEditingWidget", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EventEditingWidget", "Begin", 0, QApplication::UnicodeUTF8));
        begin_spinbox_->setSuffix(QApplication::translate("EventEditingWidget", "s", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EventEditingWidget", "Duration", 0, QApplication::UnicodeUTF8));
        duration_spinbox_->setSuffix(QApplication::translate("EventEditingWidget", "s", 0, QApplication::UnicodeUTF8));
        fit_button_->setText(QString());
        previous_button_->setText(QString());
        next_button_->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class EventEditingWidget: public Ui_EventEditingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENT_EDITING_WIDGET_H
