/****************************************************************************
** Meta object code from reading C++ file 'moveButton.h'
**
** Created: Fri Jan 11 20:48:53 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../moveButton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'moveButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_moveButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      24,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_moveButton[] = {
    "moveButton\0\0_released()\0_pushed()\0"
};

void moveButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        moveButton *_t = static_cast<moveButton *>(_o);
        switch (_id) {
        case 0: _t->_released(); break;
        case 1: _t->_pushed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData moveButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject moveButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_moveButton,
      qt_meta_data_moveButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &moveButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *moveButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *moveButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_moveButton))
        return static_cast<void*>(const_cast< moveButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int moveButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
