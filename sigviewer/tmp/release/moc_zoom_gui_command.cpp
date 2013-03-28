/****************************************************************************
** Meta object code from reading C++ file 'zoom_gui_command.h'
**
** Created: Thu Mar 28 15:12:12 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/commands/zoom_gui_command.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'zoom_gui_command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___ZoomGuiCommand[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x09,
      50,   27,   27,   27, 0x08,
      57,   27,   27,   27, 0x08,
      76,   27,   27,   27, 0x08,
      96,   27,   27,   27, 0x08,
     113,   27,   27,   27, 0x08,
     131,   27,   27,   27, 0x08,
     150,   27,   27,   27, 0x08,
     163,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___ZoomGuiCommand[] = {
    "SigViewer_::ZoomGuiCommand\0\0"
    "evaluateEnabledness()\0goTo()\0"
    "zoomInHorizontal()\0zoomOutHorizontal()\0"
    "zoomInVertical()\0zoomOutVertical()\0"
    "autoZoomVertical()\0scaleXAxis()\0"
    "setChannelsPerPage()\0"
};

const QMetaObject SigViewer_::ZoomGuiCommand::staticMetaObject = {
    { &GuiActionCommand::staticMetaObject, qt_meta_stringdata_SigViewer___ZoomGuiCommand,
      qt_meta_data_SigViewer___ZoomGuiCommand, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::ZoomGuiCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::ZoomGuiCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::ZoomGuiCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___ZoomGuiCommand))
        return static_cast<void*>(const_cast< ZoomGuiCommand*>(this));
    return GuiActionCommand::qt_metacast(_clname);
}

int SigViewer_::ZoomGuiCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GuiActionCommand::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: evaluateEnabledness(); break;
        case 1: goTo(); break;
        case 2: zoomInHorizontal(); break;
        case 3: zoomOutHorizontal(); break;
        case 4: zoomInVertical(); break;
        case 5: zoomOutVertical(); break;
        case 6: autoZoomVertical(); break;
        case 7: scaleXAxis(); break;
        case 8: setChannelsPerPage(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
