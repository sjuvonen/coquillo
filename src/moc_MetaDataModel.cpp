/****************************************************************************
** Meta object code from reading C++ file 'MetaDataModel.h'
**
** Created: Sat Oct 8 12:37:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MetaDataModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MetaDataModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MetaDataModel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   15,   14,   14, 0x05,
      57,   53,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      92,   87,   14,   14, 0x0a,
     118,   14,   14,   14, 0x0a,
     140,   14,   14,   14, 0x0a,
     156,   14,   14,   14, 0x0a,
     170,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MetaDataModel[] = {
    "MetaDataModel\0\0isModified\0"
    "metaDataStateChanged(bool)\0idx\0"
    "metaDataModified(QModelIndex)\0path\0"
    "setRootDirectory(QString)\0"
    "addMetaData(MetaData)\0clearContents()\0"
    "saveChanges()\0undoChanges()\0"
};

const QMetaObject MetaDataModel::staticMetaObject = {
    { &QStandardItemModel::staticMetaObject, qt_meta_stringdata_MetaDataModel,
      qt_meta_data_MetaDataModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MetaDataModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MetaDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MetaDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MetaDataModel))
        return static_cast<void*>(const_cast< MetaDataModel*>(this));
    return QStandardItemModel::qt_metacast(_clname);
}

int MetaDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStandardItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: metaDataStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: metaDataModified((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: setRootDirectory((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: addMetaData((*reinterpret_cast< const MetaData(*)>(_a[1]))); break;
        case 4: clearContents(); break;
        case 5: saveChanges(); break;
        case 6: undoChanges(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MetaDataModel::metaDataStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MetaDataModel::metaDataModified(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
