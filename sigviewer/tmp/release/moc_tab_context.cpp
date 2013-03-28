/****************************************************************************
** Meta object code from reading C++ file 'tab_context.h'
**
** Created: Thu Mar 28 15:12:17 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/tab_context.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tab_context.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___TabContext[] = {

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
      30,   24,   23,   23, 0x05,
      71,   24,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     102,   24,   23,   23, 0x0a,
     139,   24,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___TabContext[] = {
    "SigViewer_::TabContext\0\0state\0"
    "selectionStateChanged(TabSelectionState)\0"
    "editStateChanged(TabEditState)\0"
    "setSelectionState(TabSelectionState)\0"
    "setEditState(TabEditState)\0"
};

const QMetaObject SigViewer_::TabContext::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___TabContext,
      qt_meta_data_SigViewer___TabContext, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::TabContext::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::TabContext::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::TabContext::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___TabContext))
        return static_cast<void*>(const_cast< TabContext*>(this));
    if (!strcmp(_clname, "CommandExecuter"))
        return static_cast< CommandExecuter*>(const_cast< TabContext*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::TabContext::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectionStateChanged((*reinterpret_cast< TabSelectionState(*)>(_a[1]))); break;
        case 1: editStateChanged((*reinterpret_cast< TabEditState(*)>(_a[1]))); break;
        case 2: setSelectionState((*reinterpret_cast< TabSelectionState(*)>(_a[1]))); break;
        case 3: setEditState((*reinterpret_cast< TabEditState(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::TabContext::selectionStateChanged(TabSelectionState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::TabContext::editStateChanged(TabEditState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
