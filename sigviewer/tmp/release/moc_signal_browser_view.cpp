/****************************************************************************
** Meta object code from reading C++ file 'signal_browser_view.h'
**
** Created: Thu Mar 28 15:12:02 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/signal_browser_view.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_browser_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalBrowserView[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   31,   30,   30, 0x05,
      66,   60,   30,   30, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   89,   30,   30, 0x0a,
     127,   30,   30,   30, 0x08,
     154,   30,   30,   30, 0x08,
     191,  183,   30,   30, 0x08,
     231,  183,   30,   30, 0x08,
     275,  269,   30,   30, 0x08,
     298,  269,   30,   30, 0x08,
     331,  269,   30,   30, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalBrowserView[] = {
    "SigViewer_::SignalBrowserView\0\0new_x\0"
    "visibleXChanged(int32)\0new_y\0"
    "visibleYChanged(int32)\0mode\0"
    "setMode(SignalVisualisationMode)\0"
    "verticalSrollbarMoved(int)\0"
    "horizontalSrollbarMoved(int)\0min,max\0"
    "horizontalScrollBarRangeChaned(int,int)\0"
    "verticalScrollBarRangeChaned(int,int)\0"
    "event\0dropEvent(QDropEvent*)\0"
    "dragEnterEvent(QDragEnterEvent*)\0"
    "graphicsViewResized(QResizeEvent*)\0"
};

const QMetaObject SigViewer_::SignalBrowserView::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SigViewer___SignalBrowserView,
      qt_meta_data_SigViewer___SignalBrowserView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalBrowserView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalBrowserView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalBrowserView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalBrowserView))
        return static_cast<void*>(const_cast< SignalBrowserView*>(this));
    if (!strcmp(_clname, "SignalVisualisationView"))
        return static_cast< SignalVisualisationView*>(const_cast< SignalBrowserView*>(this));
    return QFrame::qt_metacast(_clname);
}

int SigViewer_::SignalBrowserView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: visibleXChanged((*reinterpret_cast< int32(*)>(_a[1]))); break;
        case 1: visibleYChanged((*reinterpret_cast< int32(*)>(_a[1]))); break;
        case 2: setMode((*reinterpret_cast< SignalVisualisationMode(*)>(_a[1]))); break;
        case 3: verticalSrollbarMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: horizontalSrollbarMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: horizontalScrollBarRangeChaned((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: verticalScrollBarRangeChaned((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: dropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 8: dragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 9: graphicsViewResized((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::SignalBrowserView::visibleXChanged(int32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::SignalBrowserView::visibleYChanged(int32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
