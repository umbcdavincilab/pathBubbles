/****************************************************************************
** Meta object code from reading C++ file 'qtbox.h'
**
** Created: Mon Jun 30 18:28:39 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ItemBase[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   10,    9,    9, 0x05,
      41,   10,    9,    9, 0x05,
      72,   60,    9,    9, 0x05,
     118,  104,    9,    9, 0x05,
     171,  148,    9,    9, 0x05,
     205,   10,    9,    9, 0x05,
     228,   10,    9,    9, 0x05,
     251,   10,    9,    9, 0x05,
     285,  281,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ItemBase[] = {
    "ItemBase\0\0init\0itemPosUpdated(ItemBase*)\0"
    "unGroup(ItemBase*)\0init,offset\0"
    "itemPosMoved(ItemBase*,QPointF)\0"
    "init,initRect\0itemResized(ItemBase*,QRectF)\0"
    "init,initRect,cornerID\0"
    "itemResized(ItemBase*,QRectF,int)\0"
    "itemResized(ItemBase*)\0itemRemoved(ItemBase*)\0"
    "bubbleDeleteItself(ItemBase*)\0str\0"
    "logInfor(QString)\0"
};

void ItemBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ItemBase *_t = static_cast<ItemBase *>(_o);
        switch (_id) {
        case 0: _t->itemPosUpdated((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->unGroup((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 2: _t->itemPosMoved((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 3: _t->itemResized((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2]))); break;
        case 4: _t->itemResized((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->itemResized((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 6: _t->itemRemoved((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 7: _t->bubbleDeleteItself((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 8: _t->logInfor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ItemBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ItemBase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ItemBase,
      qt_meta_data_ItemBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ItemBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ItemBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ItemBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ItemBase))
        return static_cast<void*>(const_cast< ItemBase*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< ItemBase*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< ItemBase*>(this));
    return QObject::qt_metacast(_clname);
}

int ItemBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ItemBase::itemPosUpdated(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ItemBase::unGroup(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ItemBase::itemPosMoved(ItemBase * _t1, QPointF _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ItemBase::itemResized(ItemBase * _t1, QRectF _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ItemBase::itemResized(ItemBase * _t1, QRectF _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ItemBase::itemResized(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ItemBase::itemRemoved(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ItemBase::bubbleDeleteItself(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ItemBase::logInfor(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
