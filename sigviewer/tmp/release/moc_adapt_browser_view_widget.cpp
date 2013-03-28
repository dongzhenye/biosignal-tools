/****************************************************************************
** Meta object code from reading C++ file 'adapt_browser_view_widget.h'
**
** Created: Thu Mar 28 15:12:08 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/adapt_browser_view_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'adapt_browser_view_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___AdaptBrowserViewWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   36,   35,   35, 0x05,
      73,   36,   35,   35, 0x05,
     102,   36,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
     138,  132,   35,   35, 0x08,
     184,  132,   35,   35, 0x08,
     217,  132,   35,   35, 0x08,
     261,  132,   35,   35, 0x08,
     304,   35,   35,   35, 0x08,
     319,   35,   35,   35, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___AdaptBrowserViewWidget[] = {
    "SigViewer_::AdaptBrowserViewWidget\0\0"
    "visible\0xAxisVisibilityChanged(bool)\0"
    "yAxisVisibilityChanged(bool)\0"
    "labelsVisibilityChanged(bool)\0value\0"
    "on_channelOverlappingSlider_valueChanged(int)\0"
    "on_yGridSlider_valueChanged(int)\0"
    "on_channelsPerPageSpinbox_valueChanged(int)\0"
    "on_secsPerPageSpinbox_valueChanged(double)\0"
    "updateValues()\0selfUpdatingFinished()\0"
};

const QMetaObject SigViewer_::AdaptBrowserViewWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SigViewer___AdaptBrowserViewWidget,
      qt_meta_data_SigViewer___AdaptBrowserViewWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::AdaptBrowserViewWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::AdaptBrowserViewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::AdaptBrowserViewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___AdaptBrowserViewWidget))
        return static_cast<void*>(const_cast< AdaptBrowserViewWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SigViewer_::AdaptBrowserViewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: xAxisVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: yAxisVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: labelsVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_channelOverlappingSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_yGridSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: on_channelsPerPageSpinbox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_secsPerPageSpinbox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: updateValues(); break;
        case 8: selfUpdatingFinished(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::AdaptBrowserViewWidget::xAxisVisibilityChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::AdaptBrowserViewWidget::yAxisVisibilityChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SigViewer_::AdaptBrowserViewWidget::labelsVisibilityChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
