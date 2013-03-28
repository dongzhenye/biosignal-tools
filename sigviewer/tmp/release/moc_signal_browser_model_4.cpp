/****************************************************************************
** Meta object code from reading C++ file 'signal_browser_model_4.h'
**
** Created: Thu Mar 28 15:12:03 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/signal_browser_model_4.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_browser_model_4.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalBrowserModel[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   32,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
      96,   31,   31,   31, 0x0a,
     111,  105,   31,   31, 0x0a,
     162,  159,   31,   31, 0x0a,
     187,  159,   31,   31, 0x0a,
     208,  159,   31,   31, 0x0a,
     229,   31,   31,   31, 0x0a,
     245,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalBrowserModel[] = {
    "SigViewer_::SignalBrowserModel\0\0"
    "selected_event\0"
    "eventSelected(QSharedPointer<const SignalEvent>)\0"
    "update()\0event\0"
    "addEventItem(QSharedPointer<const SignalEvent>)\0"
    "id\0removeEventItem(EventID)\0"
    "updateEvent(EventID)\0selectEvent(EventID)\0"
    "unselectEvent()\0removeEventItemImpl()\0"
};

const QMetaObject SigViewer_::SignalBrowserModel::staticMetaObject = {
    { &SignalVisualisationModel::staticMetaObject, qt_meta_stringdata_SigViewer___SignalBrowserModel,
      qt_meta_data_SigViewer___SignalBrowserModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalBrowserModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalBrowserModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalBrowserModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalBrowserModel))
        return static_cast<void*>(const_cast< SignalBrowserModel*>(this));
    return SignalVisualisationModel::qt_metacast(_clname);
}

int SigViewer_::SignalBrowserModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SignalVisualisationModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: eventSelected((*reinterpret_cast< QSharedPointer<const SignalEvent>(*)>(_a[1]))); break;
        case 1: update(); break;
        case 2: addEventItem((*reinterpret_cast< QSharedPointer<const SignalEvent>(*)>(_a[1]))); break;
        case 3: removeEventItem((*reinterpret_cast< EventID(*)>(_a[1]))); break;
        case 4: updateEvent((*reinterpret_cast< EventID(*)>(_a[1]))); break;
        case 5: selectEvent((*reinterpret_cast< EventID(*)>(_a[1]))); break;
        case 6: unselectEvent(); break;
        case 7: removeEventItemImpl(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::SignalBrowserModel::eventSelected(QSharedPointer<SignalEvent const> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
