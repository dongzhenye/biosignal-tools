/****************************************************************************
** Meta object code from reading C++ file 'file_context.h'
**
** Created: Thu Mar 28 15:12:17 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/file_context.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'file_context.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___FileContext[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   25,   24,   24, 0x05,
      74,   55,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   25,   24,   24, 0x0a,
     119,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___FileContext[] = {
    "SigViewer_::FileContext\0\0state\0"
    "stateChanged(FileState)\0file_path_and_name\0"
    "fileNameChanged(QString)\0setState(FileState)\0"
    "setAsChanged()\0"
};

const QMetaObject SigViewer_::FileContext::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___FileContext,
      qt_meta_data_SigViewer___FileContext, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::FileContext::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::FileContext::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::FileContext::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___FileContext))
        return static_cast<void*>(const_cast< FileContext*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::FileContext::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< FileState(*)>(_a[1]))); break;
        case 1: fileNameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: setState((*reinterpret_cast< FileState(*)>(_a[1]))); break;
        case 3: setAsChanged(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::FileContext::stateChanged(FileState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::FileContext::fileNameChanged(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
