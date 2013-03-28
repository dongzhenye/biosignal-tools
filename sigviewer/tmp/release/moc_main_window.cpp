/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created: Thu Mar 28 15:11:58 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui_impl/main_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SigViewer___MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   24,   23,   23, 0x05,
      73,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     109,  101,   23,   23, 0x08,
     131,   23,   23,   23, 0x08,
     160,  151,   23,   23, 0x08,
     216,  205,   23,   23, 0x08,
     269,  264,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SigViewer___MainWindow[] = {
    "SigViewer_::MainWindow\0\0recent_file_action\0"
    "recentFileActivated(QAction*)\0"
    "recentFileMenuAboutToShow()\0visible\0"
    "toggleStatusBar(bool)\0toggleAllToolbars()\0"
    "name,max\0addBackgroundProcessToStatusBar(QString,int)\0"
    "name,value\0updateBackgroundProcessonStatusBar(QString,int)\0"
    "name\0removeBackgroundProcessFromStatusBar(QString)\0"
};

const QMetaObject SigViewer_::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SigViewer___MainWindow,
      qt_meta_data_SigViewer___MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SigViewer_::MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SigViewer_::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SigViewer_::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SigViewer___MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SigViewer_::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: recentFileActivated((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: recentFileMenuAboutToShow(); break;
        case 2: toggleStatusBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: toggleAllToolbars(); break;
        case 4: addBackgroundProcessToStatusBar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: updateBackgroundProcessonStatusBar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: removeBackgroundProcessFromStatusBar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SigViewer_::MainWindow::recentFileActivated(QAction * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SigViewer_::MainWindow::recentFileMenuAboutToShow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
