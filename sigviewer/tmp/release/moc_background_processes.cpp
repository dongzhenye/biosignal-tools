/****************************************************************************
** Meta object code from reading C++ file 'background_processes.h'
**
** Created: Thu Mar 28 15:11:57 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/background_processes.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'background_processes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___BackgroundProcesses[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   33,   32,   32, 0x05,
      77,   66,   32,   32, 0x05,
     115,  110,   32,   32, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___BackgroundProcesses[] = {
    "SigViewer_::BackgroundProcesses\0\0"
    "name,max\0newProcess(QString,int)\0"
    "name,state\0processChangedState(QString,int)\0"
    "name\0processRemoved(QString)\0"
};

const QMetaObject SigViewer_::BackgroundProcesses::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___BackgroundProcesses,
      qt_meta_data_SigViewer___BackgroundProcesses, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::BackgroundProcesses::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::BackgroundProcesses::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::BackgroundProcesses::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___BackgroundProcesses))
        return static_cast<void*>(const_cast< BackgroundProcesses*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::BackgroundProcesses::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newProcess((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: processChangedState((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: processRemoved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::BackgroundProcesses::newProcess(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::BackgroundProcesses::processChangedState(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SigViewer_::BackgroundProcesses::processRemoved(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
