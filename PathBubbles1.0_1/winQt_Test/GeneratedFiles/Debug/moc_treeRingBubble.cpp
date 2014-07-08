/****************************************************************************
** Meta object code from reading C++ file 'treeRingBubble.h'
**
** Created: Mon Jul 7 18:35:31 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../treeRingBubble.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'treeRingBubble.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_treeRingBubble[] = {

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
      22,   16,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_treeRingBubble[] = {
    "treeRingBubble\0\0b0,b1\0"
    "selfDelection(treeRingBubble*,treeRingBubble*)\0"
};

void treeRingBubble::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        treeRingBubble *_t = static_cast<treeRingBubble *>(_o);
        switch (_id) {
        case 0: _t->selfDelection((*reinterpret_cast< treeRingBubble*(*)>(_a[1])),(*reinterpret_cast< treeRingBubble*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData treeRingBubble::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject treeRingBubble::staticMetaObject = {
    { &PlotBase::staticMetaObject, qt_meta_stringdata_treeRingBubble,
      qt_meta_data_treeRingBubble, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &treeRingBubble::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *treeRingBubble::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *treeRingBubble::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_treeRingBubble))
        return static_cast<void*>(const_cast< treeRingBubble*>(this));
    return PlotBase::qt_metacast(_clname);
}

int treeRingBubble::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PlotBase::qt_metacall(_c, _id, _a);
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
