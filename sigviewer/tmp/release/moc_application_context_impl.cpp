/****************************************************************************
** Meta object code from reading C++ file 'application_context_impl.h'
**
** Created: Thu Mar 28 15:12:16 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/application_context_impl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'application_context_impl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___ApplicationContextImpl[] = {

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
      42,   36,   35,   35, 0x05,
      73,   36,   35,   35, 0x05,
     108,   36,   35,   35, 0x05,
     159,   36,   35,   35, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___ApplicationContextImpl[] = {
    "SigViewer_::ApplicationContextImpl\0\0"
    "state\0stateChanged(ApplicationState)\0"
    "currentFileStateChanged(FileState)\0"
    "currentTabSelectionStateChanged(TabSelectionState)\0"
    "currentTabEditStateChanged(TabEditState)\0"
};

const QMetaObject SigViewer_::ApplicationContextImpl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___ApplicationContextImpl,
      qt_meta_data_SigViewer___ApplicationContextImpl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::ApplicationContextImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::ApplicationContextImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::ApplicationContextImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___ApplicationContextImpl))
        return static_cast<void*>(const_cast< ApplicationContextImpl*>(this));
    if (!strcmp(_clname, "ApplicationContext"))
        return static_cast< ApplicationContext*>(const_cast< ApplicationContextImpl*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::ApplicationContextImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< ApplicationState(*)>(_a[1]))); break;
        case 1: currentFileStateChanged((*reinterpret_cast< FileState(*)>(_a[1]))); break;
        case 2: currentTabSelectionStateChanged((*reinterpret_cast< TabSelectionState(*)>(_a[1]))); break;
        case 3: currentTabEditStateChanged((*reinterpret_cast< TabEditState(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::ApplicationContextImpl::stateChanged(ApplicationState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::ApplicationContextImpl::currentFileStateChanged(FileState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SigViewer_::ApplicationContextImpl::currentTabSelectionStateChanged(TabSelectionState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SigViewer_::ApplicationContextImpl::currentTabEditStateChanged(TabEditState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
