/****************************************************************************
** Meta object code from reading C++ file 'GroupBubble4.h'
**
** Created: Mon Jul 7 18:35:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GroupBubble4.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GroupBubble4.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GroupBubble4[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x05,
      72,   47,   13,   13, 0x05,
     140,  123,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     181,   13,   13,   13, 0x0a,
     194,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GroupBubble4[] = {
    "GroupBubble4\0\0item\0updateLabelState(ItemBase*)\0"
    "pname,run_num,dataResult\0"
    "addParameterFromMat(QString,QString,QList<double>)\0"
    "pname,dataResult\0"
    "updateDataFromMat(QString,QList<double>)\0"
    "updatePath()\0updatePos()\0"
};

void GroupBubble4::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GroupBubble4 *_t = static_cast<GroupBubble4 *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->addParameterFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<double>(*)>(_a[3]))); break;
        case 2: _t->updateDataFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QList<double>(*)>(_a[2]))); break;
        case 3: _t->updatePath(); break;
        case 4: _t->updatePos(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GroupBubble4::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GroupBubble4::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_GroupBubble4,
      qt_meta_data_GroupBubble4, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GroupBubble4::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GroupBubble4::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GroupBubble4::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GroupBubble4))
        return static_cast<void*>(const_cast< GroupBubble4*>(this));
    return ItemBase::qt_metacast(_clname);
}

int GroupBubble4::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ItemBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GroupBubble4::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GroupBubble4::addParameterFromMat(QString _t1, QString _t2, QList<double> _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GroupBubble4::updateDataFromMat(QString _t1, QList<double> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
