/****************************************************************************
** Meta object code from reading C++ file 'open_file_gui_command.h'
**
** Created: Thu Mar 28 15:12:14 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/commands/open_file_gui_command.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'open_file_gui_command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___OpenFileGuiCommand[] = {

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
      32,   31,   31,   31, 0x08,
      39,   31,   31,   31, 0x08,
      54,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___OpenFileGuiCommand[] = {
    "SigViewer_::OpenFileGuiCommand\0\0open()\0"
    "importEvents()\0showFileInfo()\0"
};

const QMetaObject SigViewer_::OpenFileGuiCommand::staticMetaObject = {
    { &GuiActionCommand::staticMetaObject, qt_meta_stringdata_SigViewer___OpenFileGuiCommand,
      qt_meta_data_SigViewer___OpenFileGuiCommand, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::OpenFileGuiCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::OpenFileGuiCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::OpenFileGuiCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___OpenFileGuiCommand))
        return static_cast<void*>(const_cast< OpenFileGuiCommand*>(this));
    return GuiActionCommand::qt_metacast(_clname);
}

int SigViewer_::OpenFileGuiCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GuiActionCommand::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: open(); break;
        case 1: importEvents(); break;
        case 2: showFileInfo(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
