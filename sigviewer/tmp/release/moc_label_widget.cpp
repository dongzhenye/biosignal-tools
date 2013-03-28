/****************************************************************************
** Meta object code from reading C++ file 'label_widget.h'
**
** Created: Thu Mar 28 15:12:04 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/signal_browser/label_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'label_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___LabelWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      33,   25,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___LabelWidget[] = {
    "SigViewer_::LabelWidget\0\0y_start\0"
    "changeYStart(int32)\0"
};

const QMetaObject SigViewer_::LabelWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SigViewer___LabelWidget,
      qt_meta_data_SigViewer___LabelWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::LabelWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::LabelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::LabelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___LabelWidget))
        return static_cast<void*>(const_cast< LabelWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SigViewer_::LabelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeYStart((*reinterpret_cast< int32(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
