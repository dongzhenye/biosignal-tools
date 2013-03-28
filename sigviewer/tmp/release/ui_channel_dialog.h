/********************************************************************************
** Form generated from reading UI file 'channel_dialog.ui'
**
** Created: Thu Mar 28 15:10:59 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANNEL_DIALOG_H
#define UI_CHANNEL_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChannelDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabs;
    QWidget *channels_tab;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *channel_table_;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QPushButton *select_all_button_;
    QPushButton *unselect_all_button_;
    QGroupBox *show_colors_box_;
    QVBoxLayout *verticalLayout_2;
    QPushButton *reset_colors_button_;
    QPushButton *set_default_color_button_;
    QWidget *filters_tab;
    QGridLayout *gridLayout_3;
    QListWidget *chosen_filter_list_;
    QPushButton *add_filter_button_;
    QSpacerItem *verticalSpacer_2;
    QPushButton *remove_filter_button_;
    QWidget *downsampling_tab;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLabel *sr_file_label_;
    QSpinBox *downsample_factor_spinbox_;
    QLabel *sr_load_label_;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *button_box_;

    void setupUi(QDialog *ChannelDialog)
    {
        if (ChannelDialog->objectName().isEmpty())
            ChannelDialog->setObjectName(QString::fromUtf8("ChannelDialog"));
        ChannelDialog->resize(343, 431);
        ChannelDialog->setSizeGripEnabled(false);
        ChannelDialog->setModal(true);
        verticalLayout = new QVBoxLayout(ChannelDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabs = new QTabWidget(ChannelDialog);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        tabs->setEnabled(true);
        channels_tab = new QWidget();
        channels_tab->setObjectName(QString::fromUtf8("channels_tab"));
        verticalLayout_4 = new QVBoxLayout(channels_tab);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        channel_table_ = new QTableWidget(channels_tab);
        if (channel_table_->columnCount() < 3)
            channel_table_->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        channel_table_->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        channel_table_->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        channel_table_->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        channel_table_->setObjectName(QString::fromUtf8("channel_table_"));
        channel_table_->setFrameShape(QFrame::StyledPanel);
        channel_table_->setAutoScrollMargin(0);
        channel_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        channel_table_->setSelectionMode(QAbstractItemView::NoSelection);
        channel_table_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        channel_table_->setShowGrid(false);
        channel_table_->setCornerButtonEnabled(false);
        channel_table_->horizontalHeader()->setDefaultSectionSize(110);
        channel_table_->horizontalHeader()->setStretchLastSection(true);
        channel_table_->verticalHeader()->setVisible(false);
        channel_table_->verticalHeader()->setDefaultSectionSize(18);
        channel_table_->verticalHeader()->setMinimumSectionSize(0);

        verticalLayout_4->addWidget(channel_table_);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        select_all_button_ = new QPushButton(channels_tab);
        select_all_button_->setObjectName(QString::fromUtf8("select_all_button_"));

        verticalLayout_3->addWidget(select_all_button_);

        unselect_all_button_ = new QPushButton(channels_tab);
        unselect_all_button_->setObjectName(QString::fromUtf8("unselect_all_button_"));

        verticalLayout_3->addWidget(unselect_all_button_);


        horizontalLayout->addLayout(verticalLayout_3);

        show_colors_box_ = new QGroupBox(channels_tab);
        show_colors_box_->setObjectName(QString::fromUtf8("show_colors_box_"));
        show_colors_box_->setMinimumSize(QSize(162, 99));
        show_colors_box_->setCheckable(true);
        show_colors_box_->setChecked(false);
        verticalLayout_2 = new QVBoxLayout(show_colors_box_);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        reset_colors_button_ = new QPushButton(show_colors_box_);
        reset_colors_button_->setObjectName(QString::fromUtf8("reset_colors_button_"));

        verticalLayout_2->addWidget(reset_colors_button_);

        set_default_color_button_ = new QPushButton(show_colors_box_);
        set_default_color_button_->setObjectName(QString::fromUtf8("set_default_color_button_"));

        verticalLayout_2->addWidget(set_default_color_button_);


        horizontalLayout->addWidget(show_colors_box_);


        verticalLayout_4->addLayout(horizontalLayout);

        tabs->addTab(channels_tab, QString());
        filters_tab = new QWidget();
        filters_tab->setObjectName(QString::fromUtf8("filters_tab"));
        filters_tab->setEnabled(false);
        gridLayout_3 = new QGridLayout(filters_tab);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        chosen_filter_list_ = new QListWidget(filters_tab);
        chosen_filter_list_->setObjectName(QString::fromUtf8("chosen_filter_list_"));
        chosen_filter_list_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        chosen_filter_list_->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_3->addWidget(chosen_filter_list_, 0, 1, 3, 1);

        add_filter_button_ = new QPushButton(filters_tab);
        add_filter_button_->setObjectName(QString::fromUtf8("add_filter_button_"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        add_filter_button_->setIcon(icon);

        gridLayout_3->addWidget(add_filter_button_, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 2, 0, 1, 1);

        remove_filter_button_ = new QPushButton(filters_tab);
        remove_filter_button_->setObjectName(QString::fromUtf8("remove_filter_button_"));
        remove_filter_button_->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/icons/editdelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        remove_filter_button_->setIcon(icon1);

        gridLayout_3->addWidget(remove_filter_button_, 1, 0, 1, 1);

        tabs->addTab(filters_tab, QString());
        downsampling_tab = new QWidget();
        downsampling_tab->setObjectName(QString::fromUtf8("downsampling_tab"));
        downsampling_tab->setEnabled(true);
        gridLayout_2 = new QGridLayout(downsampling_tab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(downsampling_tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        label = new QLabel(downsampling_tab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        label_3 = new QLabel(downsampling_tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        gridLayout_2->addWidget(label_3, 0, 2, 1, 1);

        sr_file_label_ = new QLabel(downsampling_tab);
        sr_file_label_->setObjectName(QString::fromUtf8("sr_file_label_"));
        sr_file_label_->setFrameShape(QFrame::NoFrame);

        gridLayout_2->addWidget(sr_file_label_, 1, 0, 1, 1);

        downsample_factor_spinbox_ = new QSpinBox(downsampling_tab);
        downsample_factor_spinbox_->setObjectName(QString::fromUtf8("downsample_factor_spinbox_"));
        downsample_factor_spinbox_->setMinimum(1);

        gridLayout_2->addWidget(downsample_factor_spinbox_, 1, 1, 1, 1);

        sr_load_label_ = new QLabel(downsampling_tab);
        sr_load_label_->setObjectName(QString::fromUtf8("sr_load_label_"));

        gridLayout_2->addWidget(sr_load_label_, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 1, 1, 1);

        tabs->addTab(downsampling_tab, QString());

        verticalLayout->addWidget(tabs);

        button_box_ = new QDialogButtonBox(ChannelDialog);
        button_box_->setObjectName(QString::fromUtf8("button_box_"));
        button_box_->setOrientation(Qt::Horizontal);
        button_box_->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(button_box_);


        retranslateUi(ChannelDialog);
        QObject::connect(button_box_, SIGNAL(accepted()), ChannelDialog, SLOT(accept()));
        QObject::connect(button_box_, SIGNAL(rejected()), ChannelDialog, SLOT(reject()));

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ChannelDialog);
    } // setupUi

    void retranslateUi(QDialog *ChannelDialog)
    {
        ChannelDialog->setWindowTitle(QApplication::translate("ChannelDialog", "Channels", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = channel_table_->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ChannelDialog", "Channel", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = channel_table_->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ChannelDialog", "Color", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = channel_table_->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ChannelDialog", "ID", 0, QApplication::UnicodeUTF8));
        select_all_button_->setText(QApplication::translate("ChannelDialog", "Select All", 0, QApplication::UnicodeUTF8));
        unselect_all_button_->setText(QApplication::translate("ChannelDialog", "Unselect All", 0, QApplication::UnicodeUTF8));
        show_colors_box_->setTitle(QApplication::translate("ChannelDialog", "Color Settings", 0, QApplication::UnicodeUTF8));
        reset_colors_button_->setText(QApplication::translate("ChannelDialog", "Reset All Colors", 0, QApplication::UnicodeUTF8));
        set_default_color_button_->setText(QApplication::translate("ChannelDialog", "Set Default Color", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(channels_tab), QApplication::translate("ChannelDialog", "Channels", 0, QApplication::UnicodeUTF8));
        add_filter_button_->setText(QApplication::translate("ChannelDialog", "Add Notch", 0, QApplication::UnicodeUTF8));
        remove_filter_button_->setText(QApplication::translate("ChannelDialog", "Remove Filter", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(filters_tab), QApplication::translate("ChannelDialog", "Filters", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ChannelDialog", "Original Samplingrate", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ChannelDialog", "Factor", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ChannelDialog", "Loading Samplingrate", 0, QApplication::UnicodeUTF8));
        sr_file_label_->setText(QApplication::translate("ChannelDialog", "100 Hz", 0, QApplication::UnicodeUTF8));
        sr_load_label_->setText(QApplication::translate("ChannelDialog", "100 Hz", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(downsampling_tab), QApplication::translate("ChannelDialog", "Downsampling", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ChannelDialog: public Ui_ChannelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANNEL_DIALOG_H
