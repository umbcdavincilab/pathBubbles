/****************************************************************************
** Meta object code from reading C++ file 'PenBlock.h'
**
** Created: Fri Jan 11 20:49:01 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PenBlock.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PenBlock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PenBlock[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   10,    9,    9, 0x05,
      61,   56,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,   89,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PenBlock[] = {
    "PenBlock\0\0bats\0addBatsToScene(QList<QList<ItemBase*> >)\0"
    "item\0updateLabelState(ItemBase*)\0"
    "bats,rates\0"
    "needSlider(QList<QList<ItemBase*> >,QList<QList<float> >)\0"
};

void PenBlock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PenBlock *_t = static_cast<PenBlock *>(_o);
        switch (_id) {
        case 0: _t->addBatsToScene((*reinterpret_cast< QList<QList<ItemBase*> >(*)>(_a[1]))); break;
        case 1: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 2: _t->needSlider((*reinterpret_cast< QList<QList<ItemBase*> >(*)>(_a[1])),(*reinterpret_cast< QList<QList<float> >(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PenBlock::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PenBlock::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_PenBlock,
      qt_meta_data_PenBlock, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PenBlock::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PenBlock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PenBlock::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PenBlock))
        return static_cast<void*>(const_cast< PenBlock*>(this));
    return ItemBase::qt_metacast(_clname);
}

int PenBlock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ItemBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PenBlock::addBatsToScene(QList<QList<ItemBase*> > _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PenBlock::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE