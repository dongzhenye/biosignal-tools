/********************************************************************************
** Form generated from reading UI file 'adapt_browser_view_widget.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADAPT_BROWSER_VIEW_WIDGET_H
#define UI_ADAPT_BROWSER_VIEW_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdaptBrowserViewWidget
{
public:
    QGridLayout *gridLayout;
    QCheckBox *labels_checkbox_;
    QCheckBox *yGridCheckbox;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSlider *yGridSlider;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *xGridCheckbox;
    QCheckBox *x_axis_checkbox_;
    QCheckBox *y_axis_checkbox_;
    QToolButton *zero_fitted_;
    QToolButton *zero_centered_;
    QLabel *xGridFragmentationLabel;
    QSlider *xGridSlider;
    QSpinBox *channelsPerPageSpinbox;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QLabel *xUnitsPerPageLabel;
    QDoubleSpinBox *secsPerPageSpinbox;
    QSlider *channelOverlappingSlider;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *AdaptBrowserViewWidget)
    {
        if (AdaptBrowserViewWidget->objectName().isEmpty())
            AdaptBrowserViewWidget->setObjectName(QString::fromUtf8("AdaptBrowserViewWidget"));
        AdaptBrowserViewWidget->resize(916, 73);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AdaptBrowserViewWidget->sizePolicy().hasHeightForWidth());
        AdaptBrowserViewWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(AdaptBrowserViewWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labels_checkbox_ = new QCheckBox(AdaptBrowserViewWidget);
        labels_checkbox_->setObjectName(QString::fromUtf8("labels_checkbox_"));
        labels_checkbox_->setChecked(true);

        gridLayout->addWidget(labels_checkbox_, 0, 0, 1, 1);

        yGridCheckbox = new QCheckBox(AdaptBrowserViewWidget);
        yGridCheckbox->setObjectName(QString::fromUtf8("yGridCheckbox"));
        yGridCheckbox->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(yGridCheckbox->sizePolicy().hasHeightForWidth());
        yGridCheckbox->setSizePolicy(sizePolicy1);
        yGridCheckbox->setChecked(true);

        gridLayout->addWidget(yGridCheckbox, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 7, 1, 1);

        label = new QLabel(AdaptBrowserViewWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 8, 1, 1);

        yGridSlider = new QSlider(AdaptBrowserViewWidget);
        yGridSlider->setObjectName(QString::fromUtf8("yGridSlider"));
        yGridSlider->setMaximum(30);
        yGridSlider->setSingleStep(1);
        yGridSlider->setOrientation(Qt::Horizontal);
        yGridSlider->setInvertedControls(false);
        yGridSlider->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(yGridSlider, 0, 9, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 10, 1, 1);

        xGridCheckbox = new QCheckBox(AdaptBrowserViewWidget);
        xGridCheckbox->setObjectName(QString::fromUtf8("xGridCheckbox"));
        xGridCheckbox->setEnabled(false);
        xGridCheckbox->setChecked(true);

        gridLayout->addWidget(xGridCheckbox, 1, 3, 1, 1);

        x_axis_checkbox_ = new QCheckBox(AdaptBrowserViewWidget);
        x_axis_checkbox_->setObjectName(QString::fromUtf8("x_axis_checkbox_"));
        x_axis_checkbox_->setChecked(true);

        gridLayout->addWidget(x_axis_checkbox_, 1, 1, 1, 1);

        y_axis_checkbox_ = new QCheckBox(AdaptBrowserViewWidget);
        y_axis_checkbox_->setObjectName(QString::fromUtf8("y_axis_checkbox_"));
        y_axis_checkbox_->setChecked(true);

        gridLayout->addWidget(y_axis_checkbox_, 0, 1, 1, 1);

        zero_fitted_ = new QToolButton(AdaptBrowserViewWidget);
        zero_fitted_->setObjectName(QString::fromUtf8("zero_fitted_"));

        gridLayout->addWidget(zero_fitted_, 0, 13, 1, 1);

        zero_centered_ = new QToolButton(AdaptBrowserViewWidget);
        zero_centered_->setObjectName(QString::fromUtf8("zero_centered_"));

        gridLayout->addWidget(zero_centered_, 1, 13, 1, 1);

        xGridFragmentationLabel = new QLabel(AdaptBrowserViewWidget);
        xGridFragmentationLabel->setObjectName(QString::fromUtf8("xGridFragmentationLabel"));
        xGridFragmentationLabel->setEnabled(false);
        xGridFragmentationLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(xGridFragmentationLabel, 1, 8, 1, 1);

        xGridSlider = new QSlider(AdaptBrowserViewWidget);
        xGridSlider->setObjectName(QString::fromUtf8("xGridSlider"));
        xGridSlider->setEnabled(false);
        xGridSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(xGridSlider, 1, 9, 1, 1);

        channelsPerPageSpinbox = new QSpinBox(AdaptBrowserViewWidget);
        channelsPerPageSpinbox->setObjectName(QString::fromUtf8("channelsPerPageSpinbox"));
        channelsPerPageSpinbox->setMinimum(1);

        gridLayout->addWidget(channelsPerPageSpinbox, 0, 6, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 4, 1, 1);

        label_2 = new QLabel(AdaptBrowserViewWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 0, 5, 1, 1);

        xUnitsPerPageLabel = new QLabel(AdaptBrowserViewWidget);
        xUnitsPerPageLabel->setObjectName(QString::fromUtf8("xUnitsPerPageLabel"));
        xUnitsPerPageLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(xUnitsPerPageLabel, 1, 5, 1, 1);

        secsPerPageSpinbox = new QDoubleSpinBox(AdaptBrowserViewWidget);
        secsPerPageSpinbox->setObjectName(QString::fromUtf8("secsPerPageSpinbox"));
        secsPerPageSpinbox->setDecimals(1);
        secsPerPageSpinbox->setMinimum(0.1);

        gridLayout->addWidget(secsPerPageSpinbox, 1, 6, 1, 1);

        channelOverlappingSlider = new QSlider(AdaptBrowserViewWidget);
        channelOverlappingSlider->setObjectName(QString::fromUtf8("channelOverlappingSlider"));
        channelOverlappingSlider->setMaximum(100);
        channelOverlappingSlider->setOrientation(Qt::Vertical);

        gridLayout->addWidget(channelOverlappingSlider, 0, 11, 2, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 12, 1, 1);


        retranslateUi(AdaptBrowserViewWidget);

        QMetaObject::connectSlotsByName(AdaptBrowserViewWidget);
    } // setupUi

    void retranslateUi(QWidget *AdaptBrowserViewWidget)
    {
        AdaptBrowserViewWidget->setWindowTitle(QApplication::translate("AdaptBrowserViewWidget", "Form", 0, QApplication::UnicodeUTF8));
        labels_checkbox_->setText(QApplication::translate("AdaptBrowserViewWidget", "Labels", 0, QApplication::UnicodeUTF8));
        yGridCheckbox->setText(QApplication::translate("AdaptBrowserViewWidget", "Y-Grid", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AdaptBrowserViewWidget", "Y-Grid Fragmentation", 0, QApplication::UnicodeUTF8));
        xGridCheckbox->setText(QApplication::translate("AdaptBrowserViewWidget", "X-Grid", 0, QApplication::UnicodeUTF8));
        x_axis_checkbox_->setText(QApplication::translate("AdaptBrowserViewWidget", "X-Axis", 0, QApplication::UnicodeUTF8));
        y_axis_checkbox_->setText(QApplication::translate("AdaptBrowserViewWidget", "Y-Axis", 0, QApplication::UnicodeUTF8));
        zero_fitted_->setText(QApplication::translate("AdaptBrowserViewWidget", "0-Line Fitted", 0, QApplication::UnicodeUTF8));
        zero_centered_->setText(QApplication::translate("AdaptBrowserViewWidget", "0-Line Centered", 0, QApplication::UnicodeUTF8));
        xGridFragmentationLabel->setText(QApplication::translate("AdaptBrowserViewWidget", "X-Grid Fragmentation:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AdaptBrowserViewWidget", "Channels / Page:", 0, QApplication::UnicodeUTF8));
        xUnitsPerPageLabel->setText(QApplication::translate("AdaptBrowserViewWidget", " / Page:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AdaptBrowserViewWidget: public Ui_AdaptBrowserViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADAPT_BROWSER_VIEW_WIDGET_H
