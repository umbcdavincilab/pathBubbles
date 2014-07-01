/****************************************************************************
** Meta object code from reading C++ file 'webQuery.h'
**
** Created: Mon Nov 5 12:52:53 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../webQuery.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webQuery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebQuery[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      26,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,    9,    9,    9, 0x0a,
      59,    9,    9,    9, 0x0a,
      69,    9,    9,    9, 0x0a,
      87,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WebQuery[] = {
    "WebQuery\0\0needSetToHide()\0"
    "moveButtonState(bool)\0needHide()\0"
    "openURL()\0moveButtonPress()\0"
    "moveButtonRelease()\0"
};

void WebQuery::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebQuery *_t = static_cast<WebQuery *>(_o);
        switch (_id) {
        case 0: _t->needSetToHide(); break;
        case 1: _t->moveButtonState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->needHide(); break;
        case 3: _t->openURL(); break;
        case 4: _t->moveButtonPress(); break;
        case 5: _t->moveButtonRelease(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WebQuery::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WebQuery::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_WebQuery,
      qt_meta_data_WebQuery, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebQuery::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebQuery::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebQuery::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebQuery))
        return static_cast<void*>(const_cast< WebQuery*>(this));
    return QDialog::qt_metacast(_clname);
}

int WebQuery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void WebQuery::needSetToHide()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void WebQuery::moveButtonState(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
