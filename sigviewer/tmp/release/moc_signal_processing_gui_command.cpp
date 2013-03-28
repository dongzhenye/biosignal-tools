/****************************************************************************
** Meta object code from reading C++ file 'signal_processing_gui_command.h'
**
** Created: Thu Mar 28 15:12:11 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/commands/signal_processing_gui_command.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_processing_gui_command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___SignalProcessingGuiCommand[] = {

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
      40,   39,   39,   39, 0x08,
      76,   39,   39,   39, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___SignalProcessingGuiCommand[] = {
    "SigViewer_::SignalProcessingGuiCommand\0"
    "\0calculateMeanAndStandardDeviation()\0"
    "calculatePowerSpectrum()\0"
};

const QMetaObject SigViewer_::SignalProcessingGuiCommand::staticMetaObject = {
    { &GuiActionCommand::staticMetaObject, qt_meta_stringdata_SigViewer___SignalProcessingGuiCommand,
      qt_meta_data_SigViewer___SignalProcessingGuiCommand, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::SignalProcessingGuiCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::SignalProcessingGuiCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::SignalProcessingGuiCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___SignalProcessingGuiCommand))
        return static_cast<void*>(const_cast< SignalProcessingGuiCommand*>(this));
    return GuiActionCommand::qt_metacast(_clname);
}

int SigViewer_::SignalProcessingGuiCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GuiActionCommand::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: calculateMeanAndStandardDeviation(); break;
        case 1: calculatePowerSpectrum(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
