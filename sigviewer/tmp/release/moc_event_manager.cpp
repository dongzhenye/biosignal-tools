/****************************************************************************
** Meta object code from reading C++ file 'event_manager.h'
**
** Created: Thu Mar 28 15:11:56 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/file_handling/event_manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___EventManager[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   26,   25,   25, 0x05,
      57,   51,   25,   25, 0x05,
     105,   26,   25,   25, 0x05,
     127,   25,   25,   25, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___EventManager[] = {
    "SigViewer_::EventManager\0\0id\0"
    "eventChanged(EventID)\0event\0"
    "eventCreated(QSharedPointer<const SignalEvent>)\0"
    "eventRemoved(EventID)\0changed()\0"
};

const QMetaObject SigViewer_::EventManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___EventManager,
      qt_meta_data_SigViewer___EventManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::EventManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::EventManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::EventManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___EventManager))
        return static_cast<void*>(const_cast< EventManager*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::EventManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: eventChanged((*reinterpret_cast< EventID(*)>(_a[1]))); break;
        case 1: eventCreated((*reinterpret_cast< QSharedPointer<const SignalEvent>(*)>(_a[1]))); break;
        case 2: eventRemoved((*reinterpret_cast< EventID(*)>(_a[1]))); break;
        case 3: changed(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::EventManager::eventChanged(EventID _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::EventManager::eventCreated(QSharedPointer<SignalEvent const> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SigViewer_::EventManager::eventRemoved(EventID _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SigViewer_::EventManager::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
