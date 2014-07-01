/****************************************************************************
** Meta object code from reading C++ file 'eQuery.h'
**
** Created: Mon Jun 30 18:28:46 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../eQuery.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eQuery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_eQuery[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   51,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_eQuery[] = {
    "eQuery\0\0bOK,ip,area\0finished(bool,QString,QString)\0"
    "reply\0onReplyFinished(QNetworkReply*)\0"
};

void eQuery::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        eQuery *_t = static_cast<eQuery *>(_o);
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->onReplyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData eQuery::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject eQuery::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_eQuery,
      qt_meta_data_eQuery, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &eQuery::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *eQuery::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *eQuery::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_eQuery))
        return static_cast<void*>(const_cast< eQuery*>(this));
    return QObject::qt_metacast(_clname);
}

int eQuery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void eQuery::finished(bool _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
