/****************************************************************************
** Meta object code from reading C++ file 'channel_selection_dialog.h'
**
** Created: Thu Mar 28 15:12:00 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/dialogs/channel_selection_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'channel_selection_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___ChannelSelectionDialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      36,   35,   35,   35, 0x08,
      70,   35,   35,   35, 0x08,
     102,   35,   35,   35, 0x08,
     147,  136,   35,   35, 0x08,
     186,  136,   35,   35, 0x08,
     225,   35,   35,   35, 0x08,
     254,  251,   35,   35, 0x08,
     288,   35,   35,   35, 0x08,
     333,  327,   35,   35, 0x08,
     381,   35,   35,   35, 0x08,
     413,   35,   35,   35, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___ChannelSelectionDialog[] = {
    "SigViewer_::ChannelSelectionDialog\0\0"
    "on_unselect_all_button__clicked()\0"
    "on_select_all_button__clicked()\0"
    "on_reset_colors_button__clicked()\0"
    "row,column\0on_channel_table__cellClicked(int,int)\0"
    "on_channel_table__cellChanged(int,int)\0"
    "on_button_box__accepted()\0on\0"
    "on_show_colors_box__toggled(bool)\0"
    "on_set_default_color_button__clicked()\0"
    "value\0on_downsample_factor_spinbox__valueChanged(int)\0"
    "on_add_filter_button__clicked()\0"
    "on_remove_filter_button__clicked()\0"
};

const QMetaObject SigViewer_::ChannelSelectionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SigViewer___ChannelSelectionDialog,
      qt_meta_data_SigViewer___ChannelSelectionDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::ChannelSelectionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::ChannelSelectionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::ChannelSelectionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___ChannelSelectionDialog))
        return static_cast<void*>(const_cast< ChannelSelectionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SigViewer_::ChannelSelectionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_unselect_all_button__clicked(); break;
        case 1: on_select_all_button__clicked(); break;
        case 2: on_reset_colors_button__clicked(); break;
        case 3: on_channel_table__cellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: on_channel_table__cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: on_button_box__accepted(); break;
        case 6: on_show_colors_box__toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: on_set_default_color_button__clicked(); break;
        case 8: on_downsample_factor_spinbox__valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: on_add_filter_button__clicked(); break;
        case 10: on_remove_filter_button__clicked(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
