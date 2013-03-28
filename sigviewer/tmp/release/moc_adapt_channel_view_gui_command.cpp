/****************************************************************************
** Meta object code from reading C++ file 'adapt_channel_view_gui_command.h'
**
** Created: Thu Mar 28 15:12:08 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/commands/adapt_channel_view_gui_command.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'adapt_channel_view_gui_command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___AdaptChannelViewGuiCommand[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   39,   39,   39, 0x08,
      62,   39,   39,   39, 0x08,
      76,   39,   39,   39, 0x08,
      84,   39,   39,   39, 0x08,
     112,   39,   39,   39, 0x08,
     119,   39,   39,   39, 0x08,
     130,   39,   39,   39, 0x08,
     157,   39,   39,   39, 0x08,
     182,   39,   39,   39, 0x08,
     201,   39,   39,   39, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___AdaptChannelViewGuiCommand[] = {
    "SigViewer_::AdaptChannelViewGuiCommand\0"
    "\0selectShownChannels()\0changeColor()\0"
    "scale()\0applyScaleToOtherChannels()\0"
    "hide()\0scaleAll()\0setScaleModeZeroCentered()\0"
    "setScaleModeZeroFitted()\0toggleAnimations()\0"
    "setAnimationDuration()\0"
};

const QMetaObject SigViewer_::AdaptChannelViewGuiCommand::staticMetaObject = {
    { &GuiActionCommand::staticMetaObject, qt_meta_stringdata_SigViewer___AdaptChannelViewGuiCommand,
      qt_meta_data_SigViewer___AdaptChannelViewGuiCommand, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::AdaptChannelViewGuiCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::AdaptChannelViewGuiCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::AdaptChannelViewGuiCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___AdaptChannelViewGuiCommand))
        return static_cast<void*>(const_cast< AdaptChannelViewGuiCommand*>(this));
    return GuiActionCommand::qt_metacast(_clname);
}

int SigViewer_::AdaptChannelViewGuiCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GuiActionCommand::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectShownChannels(); break;
        case 1: changeColor(); break;
        case 2: scale(); break;
        case 3: applyScaleToOtherChannels(); break;
        case 4: hide(); break;
        case 5: scaleAll(); break;
        case 6: setScaleModeZeroCentered(); break;
        case 7: setScaleModeZeroFitted(); break;
        case 8: toggleAnimations(); break;
        case 9: setAnimationDuration(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
