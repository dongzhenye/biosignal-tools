/****************************************************************************
** Meta object code from reading C++ file 'tests_dialog.h'
**
** Created: Thu Mar 28 15:12:15 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/tests/tests_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tests_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___TestsDialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x08,
      52,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___TestsDialog[] = {
    "SigViewer_::TestsDialog\0\0"
    "on_start_button__clicked()\0"
    "on_open_dummy__clicked()\0"
};

const QMetaObject SigViewer_::TestsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SigViewer___TestsDialog,
      qt_meta_data_SigViewer___TestsDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::TestsDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::TestsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::TestsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___TestsDialog))
        return static_cast<void*>(const_cast< TestsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SigViewer_::TestsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_start_button__clicked(); break;
        case 1: on_open_dummy__clicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
