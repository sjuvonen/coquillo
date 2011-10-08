/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Sat Oct 8 12:37:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   49,   11,   11, 0x0a,
      84,   78,   11,   11, 0x0a,
     111,   11,   11,   11, 0x08,
     119,   11,   11,   11, 0x08,
     136,  131,   11,   11, 0x08,
     165,   11,   11,   11, 0x28,
     190,   11,   11,   11, 0x08,
     211,   11,   11,   11, 0x08,
     226,   11,   11,   11, 0x08,
     240,   11,   11,   11, 0x08,
     255,   11,   11,   11, 0x08,
     280,   11,   11,   11, 0x08,
     312,   11,   11,   11, 0x08,
     335,   11,   11,   11, 0x08,
     353,   11,   11,   11, 0x08,
     384,   11,   11,   11, 0x08,
     414,   11,   11,   11, 0x08,
     435,   11,   11,   11, 0x08,
     456,   11,   11,   11, 0x08,
     493,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0selectedRowsChanged(QModelIndexList)\0"
    "size\0setToolBarIconSize(int)\0style\0"
    "setToolBarButtonStyle(int)\0check()\0"
    "abortScan()\0open\0toggleCddbSearchDialog(bool)\0"
    "toggleCddbSearchDialog()\0openSettingsDialog()\0"
    "saveMetaData()\0scanStarted()\0"
    "scanFinished()\0setInterfaceLocked(bool)\0"
    "setInterfaceObjectVisible(bool)\0"
    "setRecursiveScan(bool)\0showAboutDialog()\0"
    "showToolBarContextMenu(QPoint)\0"
    "showHeaderContextMenu(QPoint)\0"
    "setupBookmarksMenu()\0openBookmarkDialog()\0"
    "slotSelectionChanged(QItemSelection)\0"
    "slotSelectInverse()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectedRowsChanged((*reinterpret_cast< const QModelIndexList(*)>(_a[1]))); break;
        case 1: setToolBarIconSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setToolBarButtonStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: check(); break;
        case 4: abortScan(); break;
        case 5: toggleCddbSearchDialog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: toggleCddbSearchDialog(); break;
        case 7: openSettingsDialog(); break;
        case 8: saveMetaData(); break;
        case 9: scanStarted(); break;
        case 10: scanFinished(); break;
        case 11: setInterfaceLocked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: setInterfaceObjectVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: setRecursiveScan((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: showAboutDialog(); break;
        case 15: showToolBarContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 16: showHeaderContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 17: setupBookmarksMenu(); break;
        case 18: openBookmarkDialog(); break;
        case 19: slotSelectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1]))); break;
        case 20: slotSelectInverse(); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::selectedRowsChanged(const QModelIndexList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
