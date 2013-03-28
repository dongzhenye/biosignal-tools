/****************************************************************************
** Meta object code from reading C++ file 'event_creation_widget.h'
**
** Created: Thu Mar 28 15:12:07 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/event_creation_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_creation_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___EventCreationWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      51,   33,   32,   32, 0x0a,
     116,  100,   32,   32, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___EventCreationWidget[] = {
    "SigViewer_::EventCreationWidget\0\0"
    "shown_event_types\0"
    "updateShownEventTypes(std::set<EventType>const&)\0"
    "combo_box_index\0"
    "on_type_combobox__currentIndexChanged(int)\0"
};

const QMetaObject SigViewer_::EventCreationWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SigViewer___EventCreationWidget,
      qt_meta_data_SigViewer___EventCreationWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::EventCreationWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::EventCreationWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::EventCreationWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___EventCreationWidget))
        return static_cast<void*>(const_cast< EventCreationWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SigViewer_::EventCreationWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateShownEventTypes((*reinterpret_cast< std::set<EventType>const(*)>(_a[1]))); break;
        case 1: on_type_combobox__currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
