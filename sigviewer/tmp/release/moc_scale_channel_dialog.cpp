/****************************************************************************
** Meta object code from reading C++ file 'scale_channel_dialog.h'
**
** Created: Thu Mar 28 15:12:01 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/dialogs/scale_channel_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scale_channel_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___ScaleChannelDialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   32,   31,   31, 0x08,
      69,   32,   31,   31, 0x08,
      97,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___ScaleChannelDialog[] = {
    "SigViewer_::ScaleChannelDialog\0\0checked\0"
    "on_fixedButton_toggled(bool)\0"
    "on_autoButton_toggled(bool)\0storeAccepted()\0"
};

const QMetaObject SigViewer_::ScaleChannelDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SigViewer___ScaleChannelDialog,
      qt_meta_data_SigViewer___ScaleChannelDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::ScaleChannelDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::ScaleChannelDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::ScaleChannelDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___ScaleChannelDialog))
        return static_cast<void*>(const_cast< ScaleChannelDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SigViewer_::ScaleChannelDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_fixedButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: on_autoButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: storeAccepted(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
