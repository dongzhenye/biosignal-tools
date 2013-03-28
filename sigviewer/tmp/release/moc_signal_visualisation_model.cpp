/****************************************************************************
** Meta object code from reading C++ file 'signal_visualisation_model.h'
**
** Created: Thu Mar 28 15:11:57 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/signal_visualisation_model.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_visualisation_model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalVisualisationModel[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       1,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      56,   38,   37,   37, 0x05,
     120,  106,   37,   37, 0x05,
     153,  148,   37,   37, 0x05,

 // slots: signature, parameters, type, tag, flags
     190,   37,   37,   37, 0x0a,
     204,  199,   37,   37, 0x0a,

 // properties: name, type, flags
     246,  242, 0x02095003,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalVisualisationModel[] = {
    "SigViewer_::SignalVisualisationModel\0"
    "\0shown_event_types\0"
    "shownEventTypesChanged(std::set<EventType>const&)\0"
    "signal_height\0signalHeightChanged(uint32)\0"
    "mode\0modeChanged(SignalVisualisationMode)\0"
    "update()\0type\0setActualEventCreationType(EventType)\0"
    "int\0sample_position_\0"
};

const QMetaObject SigViewer_::SignalVisualisationModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___SignalVisualisationModel,
      qt_meta_data_SigViewer___SignalVisualisationModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalVisualisationModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalVisualisationModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalVisualisationModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalVisualisationModel))
        return static_cast<void*>(const_cast< SignalVisualisationModel*>(this));
    if (!strcmp(_clname, "EventView"))
        return static_cast< EventView*>(const_cast< SignalVisualisationModel*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::SignalVisualisationModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: shownEventTypesChanged((*reinterpret_cast< std::set<EventType>const(*)>(_a[1]))); break;
        case 1: signalHeightChanged((*reinterpret_cast< uint32(*)>(_a[1]))); break;
        case 2: modeChanged((*reinterpret_cast< SignalVisualisationMode(*)>(_a[1]))); break;
        case 3: update(); break;
        case 4: setActualEventCreationType((*reinterpret_cast< EventType(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = getShownPosition(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: goToSample(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SigViewer_::SignalVisualisationModel::shownEventTypesChanged(std::set<EventType> const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::SignalVisualisationModel::signalHeightChanged(uint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SigViewer_::SignalVisualisationModel::modeChanged(SignalVisualisationMode _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
