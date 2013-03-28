/****************************************************************************
** Meta object code from reading C++ file 'event_time_selection_dialog.h'
**
** Created: Thu Mar 28 15:12:00 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/dialogs/event_time_selection_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_time_selection_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___EventTimeSelectionDialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      38,   37,   37,   37, 0x08,
      72,   37,   37,   37, 0x08,
     120,  104,   37,   37, 0x08,
     171,  165,   37,   37, 0x08,
     215,   37,   37,   37, 0x08,
     254,  165,   37,   37, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___EventTimeSelectionDialog[] = {
    "SigViewer_::EventTimeSelectionDialog\0"
    "\0on_unselect_all_button__clicked()\0"
    "on_select_all_button__clicked()\0"
    "combo_box_index\0"
    "on_event_combo_box__currentIndexChanged(int)\0"
    "value\0on_add_before_spinbox__valueChanged(double)\0"
    "on_list_widget__itemSelectionChanged()\0"
    "on_length_spinbox__valueChanged(double)\0"
};

const QMetaObject SigViewer_::EventTimeSelectionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SigViewer___EventTimeSelectionDialog,
      qt_meta_data_SigViewer___EventTimeSelectionDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::EventTimeSelectionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::EventTimeSelectionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::EventTimeSelectionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___EventTimeSelectionDialog))
        return static_cast<void*>(const_cast< EventTimeSelectionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SigViewer_::EventTimeSelectionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_unselect_all_button__clicked(); break;
        case 1: on_select_all_button__clicked(); break;
        case 2: on_event_combo_box__currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: on_add_before_spinbox__valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: on_list_widget__itemSelectionChanged(); break;
        case 5: on_length_spinbox__valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
