/****************************************************************************
** Meta object code from reading C++ file 'qtsm.h'
**
** Created: Mon Jun 30 18:28:41 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtsm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtsm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtSM[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    6,    5,    5, 0x05,
      50,   39,    5,    5, 0x05,
      93,   84,    5,    5, 0x05,
     142,  132,    5,    5, 0x05,
     182,  132,    5,    5, 0x05,
     222,    6,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
     258,    5,    5,    5, 0x0a,
     292,  278,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtSM[] = {
    "QtSM\0\0item\0updateLabelState(ItemBase*)\0"
    "item,input\0updatePCColorState(ItemBase*,int)\0"
    "item,sta\0HighlightChangeInGroup(ItemBase*,bool)\0"
    "item,rect\0pressHightlightInGroup(ItemBase*,QRect)\0"
    "moveHightlightInGroup(ItemBase*,QRectF)\0"
    "releaseHightlightInGroup(ItemBase*)\0"
    "switchColorMethod()\0source,target\0"
    "needSwapBars(int,int)\0"
};

void QtSM::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtSM *_t = static_cast<QtSM *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->updatePCColorState((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->HighlightChangeInGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->pressHightlightInGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRect(*)>(_a[2]))); break;
        case 4: _t->moveHightlightInGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2]))); break;
        case 5: _t->releaseHightlightInGroup((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 6: _t->switchColorMethod(); break;
        case 7: _t->needSwapBars((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtSM::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtSM::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_QtSM,
      qt_meta_data_QtSM, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtSM::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtSM::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtSM::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtSM))
        return static_cast<void*>(const_cast< QtSM*>(this));
    return ItemBase::qt_metacast(_clname);
}

int QtSM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ItemBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QtSM::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtSM::updatePCColorState(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtSM::HighlightChangeInGroup(ItemBase * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtSM::pressHightlightInGroup(ItemBase * _t1, QRect _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtSM::moveHightlightInGroup(ItemBase * _t1, QRectF _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtSM::releaseHightlightInGroup(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
