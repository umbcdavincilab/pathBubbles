/****************************************************************************
** Meta object code from reading C++ file 'matBlock.h'
**
** Created: Mon Nov 5 12:52:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../matBlock.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'matBlock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MatBlock[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   10,    9,    9, 0x05,
      54,   43,    9,    9, 0x05,
     107,   82,    9,    9, 0x05,
     175,  158,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     216,    9,    9,    9, 0x0a,
     236,  231,    9,    9, 0x0a,
     257,    9,    9,    9, 0x0a,
     270,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MatBlock[] = {
    "MatBlock\0\0item\0updateLabelState(ItemBase*)\0"
    "currentMat\0setMatFileBubble(MatBlock*)\0"
    "pname,run_num,dataResult\0"
    "addParameterFromMat(QString,QString,QList<double>)\0"
    "pname,dataResult\0"
    "updateDataFromMat(QString,QList<double>)\0"
    "finishMatlab()\0name\0checkByName(QString)\0"
    "updatePath()\0updatePos()\0"
};

void MatBlock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MatBlock *_t = static_cast<MatBlock *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->setMatFileBubble((*reinterpret_cast< MatBlock*(*)>(_a[1]))); break;
        case 2: _t->addParameterFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<double>(*)>(_a[3]))); break;
        case 3: _t->updateDataFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QList<double>(*)>(_a[2]))); break;
        case 4: _t->finishMatlab(); break;
        case 5: _t->checkByName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->updatePath(); break;
        case 7: _t->updatePos(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MatBlock::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MatBlock::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_MatBlock,
      qt_meta_data_MatBlock, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MatBlock::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MatBlock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MatBlock::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MatBlock))
        return static_cast<void*>(const_cast< MatBlock*>(this));
    return ItemBase::qt_metacast(_clname);
}

int MatBlock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MatBlock::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MatBlock::setMatFileBubble(MatBlock * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MatBlock::addParameterFromMat(QString _t1, QString _t2, QList<double> _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MatBlock::updateDataFromMat(QString _t1, QList<double> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
