/****************************************************************************
** Meta object code from reading C++ file 'qtbat.h'
**
** Created: Fri Jan 11 20:49:00 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtbat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtbat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtBat[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    7,    6,    6, 0x05,
      47,   40,    6,    6, 0x05,
      67,    6,    6,    6, 0x05,
      95,   83,    6,    6, 0x05,
     129,   40,    6,    6, 0x05,
     160,   40,    6,    6, 0x05,
     193,  187,    6,    6, 0x05,
     229,  187,    6,    6, 0x05,
     266,  187,    6,    6, 0x05,
     305,  187,    6,    6, 0x05,
     341,  187,    6,    6, 0x05,
     378,  187,    6,    6, 0x05,
     415,  187,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
     459,  452,    6,    6, 0x0a,
     483,    6,    6,    6, 0x0a,
     502,    6,    6,    6, 0x0a,
     522,    6,    6,    6, 0x0a,
     544,    6,    6,    6, 0x0a,
     563,    6,    6,    6, 0x0a,
     583,    6,    6,    6, 0x0a,
     603,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtBat[] = {
    "QtBat\0\0item\0updateLabelState(ItemBase*)\0"
    "object\0needCompare(QtBat*)\0removeCompare()\0"
    "item,offset\0updateRotation(ItemBase*,QPointF)\0"
    "setCurrentBatAttribute(QtBat*)\0"
    "getNewBatAttribute(QtBat*)\0item,\0"
    "sliderChangedinGroup(ItemBase*,int)\0"
    "surfaceCheckedinGroup(ItemBase*,int)\0"
    "wireframeCheckedinGroup(ItemBase*,int)\0"
    "symbolCheckedinGroup(ItemBase*,int)\0"
    "camber1CheckedinGroup(ItemBase*,int)\0"
    "camber2CheckedinGroup(ItemBase*,int)\0"
    "camber3CheckedinGroup(ItemBase*,int)\0"
    "offset\0acceptRotation(QPointF)\0"
    "sliderChanged(int)\0surfaceChecked(int)\0"
    "wireframeChecked(int)\0symbolChecked(int)\0"
    "camber1Checked(int)\0camber2Checked(int)\0"
    "camber3Checked(int)\0"
};

void QtBat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtBat *_t = static_cast<QtBat *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->needCompare((*reinterpret_cast< QtBat*(*)>(_a[1]))); break;
        case 2: _t->removeCompare(); break;
        case 3: _t->updateRotation((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 4: _t->setCurrentBatAttribute((*reinterpret_cast< QtBat*(*)>(_a[1]))); break;
        case 5: _t->getNewBatAttribute((*reinterpret_cast< QtBat*(*)>(_a[1]))); break;
        case 6: _t->sliderChangedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->surfaceCheckedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->wireframeCheckedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->symbolCheckedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->camber1CheckedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->camber2CheckedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->camber3CheckedinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->acceptRotation((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 14: _t->sliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->surfaceChecked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->wireframeChecked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->symbolChecked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->camber1Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->camber2Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->camber3Checked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtBat::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtBat::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_QtBat,
      qt_meta_data_QtBat, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtBat::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtBat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtBat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtBat))
        return static_cast<void*>(const_cast< QtBat*>(this));
    return ItemBase::qt_metacast(_clname);
}

int QtBat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ItemBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void QtBat::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtBat::needCompare(QtBat * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtBat::removeCompare()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QtBat::updateRotation(ItemBase * _t1, QPointF _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtBat::setCurrentBatAttribute(QtBat * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtBat::getNewBatAttribute(QtBat * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtBat::sliderChangedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QtBat::surfaceCheckedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QtBat::wireframeCheckedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QtBat::symbolCheckedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QtBat::camber1CheckedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QtBat::camber2CheckedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QtBat::camber3CheckedinGroup(ItemBase * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
