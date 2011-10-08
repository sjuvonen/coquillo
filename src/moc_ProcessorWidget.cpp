/****************************************************************************
** Meta object code from reading C++ file 'ProcessorWidget.h'
**
** Created: Sat Oct 8 12:37:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ProcessorWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProcessorWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProcessorWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   16,   16,   16, 0x0a,
      82,   70,   16,   16, 0x0a,
     106,   16,   16,   16, 0x08,
     114,   16,   16,   16, 0x08,
     136,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ProcessorWidget[] = {
    "ProcessorWidget\0\0closeButtonPressed()\0"
    "setDataIndexes(QModelIndexList)\0"
    "setFloating\0setFloatingLayout(bool)\0"
    "apply()\0updateRenamePreview()\0"
    "updateResolvePreview()\0"
};

const QMetaObject ProcessorWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ProcessorWidget,
      qt_meta_data_ProcessorWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProcessorWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProcessorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProcessorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProcessorWidget))
        return static_cast<void*>(const_cast< ProcessorWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ProcessorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closeButtonPressed(); break;
        case 1: setDataIndexes((*reinterpret_cast< const QModelIndexList(*)>(_a[1]))); break;
        case 2: setFloatingLayout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: apply(); break;
        case 4: updateRenamePreview(); break;
        case 5: updateResolvePreview(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ProcessorWidget::closeButtonPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
