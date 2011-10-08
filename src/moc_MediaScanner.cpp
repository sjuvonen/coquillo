/****************************************************************************
** Meta object code from reading C++ file 'MediaScanner.h'
**
** Created: Sat Oct 8 12:37:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MediaScanner.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaScanner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MediaScanner[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      25,   13,   13,   13, 0x05,
      40,   13,   13,   13, 0x05,
      64,   60,   13,   13, 0x05,
      85,   13,   13,   13, 0x05,
     103,   95,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     125,   13,   13,   13, 0x0a,
     137,  132,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MediaScanner[] = {
    "MediaScanner\0\0finished()\0itemsDone(int)\0"
    "itemFound(MetaData)\0max\0maxItemsChanged(int)\0"
    "started()\0min,max\0rangeChanged(int,int)\0"
    "scan()\0path\0scanPath(QString)\0"
};

const QMetaObject MediaScanner::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MediaScanner,
      qt_meta_data_MediaScanner, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MediaScanner::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MediaScanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MediaScanner::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MediaScanner))
        return static_cast<void*>(const_cast< MediaScanner*>(this));
    return QObject::qt_metacast(_clname);
}

int MediaScanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        case 1: itemsDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: itemFound((*reinterpret_cast< const MetaData(*)>(_a[1]))); break;
        case 3: maxItemsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: started(); break;
        case 5: rangeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: scan(); break;
        case 7: scanPath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MediaScanner::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MediaScanner::itemsDone(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MediaScanner::itemFound(const MetaData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MediaScanner::maxItemsChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MediaScanner::started()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MediaScanner::rangeChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
