/****************************************************************************
** Meta object code from reading C++ file 'event_types_selection_dialog.h'
**
** Created: Thu Mar 28 15:12:01 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/dialogs/event_types_selection_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_types_selection_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___EventTypesSelectionDialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      51,   39,   38,   38, 0x08,
     101,   39,   38,   38, 0x08,
     159,  151,   38,   38, 0x08,
     195,  151,   38,   38, 0x08,
     236,   38,   38,   38, 0x08,
     268,   38,   38,   38, 0x08,
     305,  302,   38,   38, 0x08,
     339,   38,   38,   38, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___EventTypesSelectionDialog[] = {
    "SigViewer_::EventTypesSelectionDialog\0"
    "\0item,column\0"
    "on_tree_widget__itemClicked(QTreeWidgetItem*,int)\0"
    "on_tree_widget__itemChanged(QTreeWidgetItem*,int)\0"
    "checked\0on_all_events_button__toggled(bool)\0"
    "on_existing_events_button__toggled(bool)\0"
    "on_select_all_button__clicked()\0"
    "on_unselect_all_button__clicked()\0on\0"
    "on_show_colors_box__toggled(bool)\0"
    "on_reset_colors_button__clicked()\0"
};

const QMetaObject SigViewer_::EventTypesSelectionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SigViewer___EventTypesSelectionDialog,
      qt_meta_data_SigViewer___EventTypesSelectionDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::EventTypesSelectionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::EventTypesSelectionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::EventTypesSelectionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___EventTypesSelectionDialog))
        return static_cast<void*>(const_cast< EventTypesSelectionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SigViewer_::EventTypesSelectionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_tree_widget__itemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: on_tree_widget__itemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: on_all_events_button__toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_existing_events_button__toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: on_select_all_button__clicked(); break;
        case 5: on_unselect_all_button__clicked(); break;
        case 6: on_show_colors_box__toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: on_reset_colors_button__clicked(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
