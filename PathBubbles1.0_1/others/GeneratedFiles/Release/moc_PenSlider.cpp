/****************************************************************************
** Meta object code from reading C++ file 'PenSlider.h'
**
** Created: Mon Nov 5 12:52:51 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PenSlider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PenSlider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PenSlider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PenSlider[] = {
    "PenSlider\0\0init\0posUpdate(ItemBase*)\0"
};

void PenSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PenSlider *_t = static_cast<PenSlider *>(_o);
        switch (_id) {
        case 0: _t->posUpdate((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PenSlider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PenSlider::staticMetaObject = {
    { &PenItem::staticMetaObject, qt_meta_stringdata_PenSlider,
      qt_meta_data_PenSlider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PenSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PenSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PenSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PenSlider))
        return static_cast<void*>(const_cast< PenSlider*>(this));
    return PenItem::qt_metacast(_clname);
}

int PenSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PenItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
