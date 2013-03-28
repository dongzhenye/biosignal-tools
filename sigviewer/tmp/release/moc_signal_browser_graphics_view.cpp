/****************************************************************************
** Meta object code from reading C++ file 'signal_browser_graphics_view.h'
**
** Created: Thu Mar 28 15:12:06 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/signal_browser_graphics_view.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_browser_graphics_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalBrowserGraphicsView[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      39,   38,   38,   38, 0x05,
      72,   62,   38,   38, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalBrowserGraphicsView[] = {
    "SigViewer_::SignalBrowserGraphicsView\0"
    "\0resized(QResizeEvent*)\0scene_pos\0"
    "sceneMouseMoved(QPointF)\0"
};

const QMetaObject SigViewer_::SignalBrowserGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_SigViewer___SignalBrowserGraphicsView,
      qt_meta_data_SigViewer___SignalBrowserGraphicsView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalBrowserGraphicsView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalBrowserGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalBrowserGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalBrowserGraphicsView))
        return static_cast<void*>(const_cast< SignalBrowserGraphicsView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int SigViewer_::SignalBrowserGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: resized((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 1: sceneMouseMoved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::SignalBrowserGraphicsView::resized(QResizeEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::SignalBrowserGraphicsView::sceneMouseMoved(QPointF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
