/****************************************************************************
** Meta object code from reading C++ file 'gui_action_command.h'
**
** Created: Thu Mar 28 15:11:56 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/gui_action_command.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui_action_command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___GuiActionCommand[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x0a,
      53,   47,   29,   29, 0x0a,
     107,   47,   29,   29, 0x0a,
     147,   47,   29,   29, 0x0a,
     203,   47,   29,   29, 0x0a,
     261,  258,  249,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___GuiActionCommand[] = {
    "SigViewer_::GuiActionCommand\0\0"
    "trigger(QString)\0state\0"
    "updateEnablednessToApplicationState(ApplicationState)\0"
    "updateEnablednessToFileState(FileState)\0"
    "updateEnablednessToTabSelectionState(TabSelectionState)\0"
    "updateEnablednessToTabEditState(TabEditState)\0"
    "QAction*\0id\0getQAction(QString)\0"
};

const QMetaObject SigViewer_::GuiActionCommand::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___GuiActionCommand,
      qt_meta_data_SigViewer___GuiActionCommand, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::GuiActionCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::GuiActionCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::GuiActionCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___GuiActionCommand))
        return static_cast<void*>(const_cast< GuiActionCommand*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::GuiActionCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: trigger((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: updateEnablednessToApplicationState((*reinterpret_cast< ApplicationState(*)>(_a[1]))); break;
        case 2: updateEnablednessToFileState((*reinterpret_cast< FileState(*)>(_a[1]))); break;
        case 3: updateEnablednessToTabSelectionState((*reinterpret_cast< TabSelectionState(*)>(_a[1]))); break;
        case 4: updateEnablednessToTabEditState((*reinterpret_cast< TabEditState(*)>(_a[1]))); break;
        case 5: { QAction* _r = getQAction((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
static const uint qt_meta_data_SigViewer___ActionConnector[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   29,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   28,   28,   28, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___ActionConnector[] = {
    "SigViewer_::ActionConnector\0\0name\0"
    "triggered(QString)\0trigger()\0"
};

const QMetaObject SigViewer_::ActionConnector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SigViewer___ActionConnector,
      qt_meta_data_SigViewer___ActionConnector, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::ActionConnector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::ActionConnector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::ActionConnector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___ActionConnector))
        return static_cast<void*>(const_cast< ActionConnector*>(this));
    return QObject::qt_metacast(_clname);
}

int SigViewer_::ActionConnector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: triggered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: trigger(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::ActionConnector::triggered(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
