/****************************************************************************
** Meta object code from reading C++ file 'qtLCom.h'
**
** Created: Fri Jan 11 20:49:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtLCom.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtLCom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtLCom[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      26,    7,    7,    7, 0x0a,
      42,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtLCom[] = {
    "QtLCom\0\0needToBeRemoved()\0removeCompare()\0"
    "swithPlot()\0"
};

void QtLCom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtLCom *_t = static_cast<QtLCom *>(_o);
        switch (_id) {
        case 0: _t->needToBeRemoved(); break;
        case 1: _t->removeCompare(); break;
        case 2: _t->swithPlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QtLCom::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtLCom::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_QtLCom,
      qt_meta_data_QtLCom, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtLCom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtLCom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtLCom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtLCom))
        return static_cast<void*>(const_cast< QtLCom*>(this));
    return ItemBase::qt_metacast(_clname);
}

int QtLCom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
