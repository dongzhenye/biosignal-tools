/****************************************************************************
** Meta object code from reading C++ file 'y_axis_widget_4.h'
**
** Created: Thu Mar 28 15:12:04 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/y_axis_widget_4.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'y_axis_widget_4.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___YAxisWidget[] = {

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
      39,   25,   24,   24, 0x0a,
      72,   64,   24,   24, 0x0a,
     103,   92,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___YAxisWidget[] = {
    "SigViewer_::YAxisWidget\0\0signal_height\0"
    "changeSignalHeight(uint)\0y_start\0"
    "changeYStart(int32)\0channel_id\0"
    "updateChannel(ChannelID)\0"
};

const QMetaObject SigViewer_::YAxisWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SigViewer___YAxisWidget,
      qt_meta_data_SigViewer___YAxisWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::YAxisWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::YAxisWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::YAxisWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___YAxisWidget))
        return static_cast<void*>(const_cast< YAxisWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SigViewer_::YAxisWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeSignalHeight((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: changeYStart((*reinterpret_cast< int32(*)>(_a[1]))); break;
        case 2: updateChannel((*reinterpret_cast< ChannelID(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE