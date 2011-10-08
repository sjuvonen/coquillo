/****************************************************************************
** Meta object code from reading C++ file 'ModelDataInspector.h'
**
** Created: Sat Oct 8 12:37:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ModelDataInspector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModelDataInspector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModelDataInspector[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      44,   20,   19,   19, 0x0a,
      92,   75,   19,   19, 0x2a,
     131,  111,   19,   19, 0x08,
     190,  173,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ModelDataInspector[] = {
    "ModelDataInspector\0\0firstRow,lastRow,parent\0"
    "checkData(int,int,QModelIndex)\0"
    "firstRow,lastRow\0checkData(int,int)\0"
    "topLeft,bottomRight\0"
    "modelDataChanged(QModelIndex,QModelIndex)\0"
    "parent,start,end\0"
    "modelRowsInserted(QModelIndex,int,int)\0"
};

const QMetaObject ModelDataInspector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ModelDataInspector,
      qt_meta_data_ModelDataInspector, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModelDataInspector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModelDataInspector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModelDataInspector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModelDataInspector))
        return static_cast<void*>(const_cast< ModelDataInspector*>(this));
    return QObject::qt_metacast(_clname);
}

int ModelDataInspector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: checkData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QModelIndex(*)>(_a[3]))); break;
        case 1: checkData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: modelDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 3: modelRowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
