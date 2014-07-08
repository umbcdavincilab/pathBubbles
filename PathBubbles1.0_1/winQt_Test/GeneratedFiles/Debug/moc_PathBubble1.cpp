/****************************************************************************
** Meta object code from reading C++ file 'PathBubble1.h'
**
** Created: Mon Jul 7 18:35:34 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PathBubble1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PathBubble1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PathBubble1[] = {

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
      19,   13,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PathBubble1[] = {
    "PathBubble1\0\0b0,pb\0"
    "selfDelection(PathBubble1*,PathBubble1*)\0"
};

void PathBubble1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PathBubble1 *_t = static_cast<PathBubble1 *>(_o);
        switch (_id) {
        case 0: _t->selfDelection((*reinterpret_cast< PathBubble1*(*)>(_a[1])),(*reinterpret_cast< PathBubble1*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PathBubble1::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PathBubble1::staticMetaObject = {
    { &PlotBase::staticMetaObject, qt_meta_stringdata_PathBubble1,
      qt_meta_data_PathBubble1, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PathBubble1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PathBubble1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PathBubble1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PathBubble1))
        return static_cast<void*>(const_cast< PathBubble1*>(this));
    return PlotBase::qt_metacast(_clname);
}

int PathBubble1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
