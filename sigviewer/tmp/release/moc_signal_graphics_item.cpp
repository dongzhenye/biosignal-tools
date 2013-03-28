/****************************************************************************
** Meta object code from reading C++ file 'signal_graphics_item.h'
**
** Created: Thu Mar 28 15:12:03 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/signal_graphics_item.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_graphics_item.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalGraphicsItem[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   32,   31,   31, 0x05,
      75,   59,   31,   31, 0x05,
     104,   93,   31,   31, 0x05,
     124,   93,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
     148,   31,   31,   31, 0x0a,
     178,  171,   31,   31, 0x0a,
     203,  194,   31,   31, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalGraphicsItem[] = {
    "SigViewer_::SignalGraphicsItem\0\0sec\0"
    "mouseAtSecond(float64)\0mouse_is_moving\0"
    "mouseMoving(bool)\0channel_id\0"
    "shifting(ChannelID)\0updatedYGrid(ChannelID)\0"
    "updateYGridIntervall()\0height\0"
    "setHeight(uint)\0interval\0"
    "setXGridInterval(uint)\0"
};

const QMetaObject SigViewer_::SignalGraphicsItem::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_SigViewer___SignalGraphicsItem,
      qt_meta_data_SigViewer___SignalGraphicsItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalGraphicsItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalGraphicsItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalGraphicsItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalGraphicsItem))
        return static_cast<void*>(const_cast< SignalGraphicsItem*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< SignalGraphicsItem*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int SigViewer_::SignalGraphicsItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouseAtSecond((*reinterpret_cast< float64(*)>(_a[1]))); break;
        case 1: mouseMoving((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: shifting((*reinterpret_cast< ChannelID(*)>(_a[1]))); break;
        case 3: updatedYGrid((*reinterpret_cast< ChannelID(*)>(_a[1]))); break;
        case 4: updateYGridIntervall(); break;
        case 5: setHeight((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 6: setXGridInterval((*reinterpret_cast< uint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::SignalGraphicsItem::mouseAtSecond(float64 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::SignalGraphicsItem::mouseMoving(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SigViewer_::SignalGraphicsItem::shifting(ChannelID _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SigViewer_::SignalGraphicsItem::updatedYGrid(ChannelID _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
