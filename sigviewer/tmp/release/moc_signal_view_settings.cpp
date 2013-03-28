/****************************************************************************
** Meta object code from reading C++ file 'signal_view_settings.h'
**
** Created: Thu Mar 28 15:11:57 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/signal_view_settings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_view_settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalViewSettings[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       3,   59, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   31,   31,   31, 0x05,
      57,   31,   31,   31, 0x05,
      85,   31,   31,   31, 0x05,
     132,  108,   31,   31, 0x05,
     159,   31,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
     203,  186,   31,   31, 0x0a,
     249,  229,   31,   31, 0x0a,
     303,  278,   31,   31, 0x0a,
     351,  325,   31,   31, 0x0a,

 // properties: name, type, flags
     399,  393, 0x87095103,
     415,  393, 0x87095103,
     438,  434, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalViewSettings[] = {
    "SigViewer_::SignalViewSettings\0\0"
    "pixelsPerSampleChanged()\0"
    "channelOverlappingChanged()\0"
    "channelHeightChanged()\0channel_height_in_pixel\0"
    "channelHeightChanged(uint)\0"
    "gridFragmentationChanged()\0pixel_per_sample\0"
    "setPixelsPerSample(float)\0channel_overlapping\0"
    "setChannelOverlapping(float)\0"
    "channel_heigth_in_pixels\0setChannelHeight(int)\0"
    "orientation,fragmentation\0"
    "setGridFragmentation(Qt::Orientation,int)\0"
    "float\0pixelsPerSample\0channelOverlapping\0"
    "int\0channelHeight\0"
};

const QMetaObject SigViewer_::SignalViewSettings::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___SignalViewSettings,
      qt_meta_data_SigViewer___SignalViewSettings, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalViewSettings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalViewSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalViewSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalViewSettings))
        return static_cast<void*>(const_cast< SignalViewSettings*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::SignalViewSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pixelsPerSampleChanged(); break;
        case 1: channelOverlappingChanged(); break;
        case 2: channelHeightChanged(); break;
        case 3: channelHeightChanged((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 4: gridFragmentationChanged(); break;
        case 5: setPixelsPerSample((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: setChannelOverlapping((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: setChannelHeight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setGridFragmentation((*reinterpret_cast< Qt::Orientation(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = getPixelsPerSample(); break;
        case 1: *reinterpret_cast< float*>(_v) = getChannelOverlapping(); break;
        case 2: *reinterpret_cast< int*>(_v) = getChannelHeight(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPixelsPerSample(*reinterpret_cast< float*>(_v)); break;
        case 1: setChannelOverlapping(*reinterpret_cast< float*>(_v)); break;
        case 2: setChannelHeight(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SigViewer_::SignalViewSettings::pixelsPerSampleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SigViewer_::SignalViewSettings::channelOverlappingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SigViewer_::SignalViewSettings::channelHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SigViewer_::SignalViewSettings::channelHeightChanged(unsigned  _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SigViewer_::SignalViewSettings::gridFragmentationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
