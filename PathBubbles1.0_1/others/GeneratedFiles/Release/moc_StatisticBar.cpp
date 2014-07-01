/****************************************************************************
** Meta object code from reading C++ file 'StatisticBar.h'
**
** Created: Mon Nov 5 12:52:56 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../StatisticBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StatisticBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StatisticBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   83,   13,   13, 0x0a,
     133,  108,   13,   13, 0x0a,
     201,  184,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StatisticBar[] = {
    "StatisticBar\0\0pos,manager,index\0"
    "needStatisticalData(QPointF,StatisticManager*,int)\0"
    "index\0indexAccepted(int)\0"
    "pName,run_num,dataResult\0"
    "addParameterFromMat(QString,QString,QList<double>)\0"
    "pName,dataResult\0"
    "updateDataFromMat(QString,QList<double>)\0"
};

void StatisticBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StatisticBar *_t = static_cast<StatisticBar *>(_o);
        switch (_id) {
        case 0: _t->needStatisticalData((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< StatisticManager*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->indexAccepted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->addParameterFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<double>(*)>(_a[3]))); break;
        case 3: _t->updateDataFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QList<double>(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StatisticBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StatisticBar::staticMetaObject = {
    { &BarBase::staticMetaObject, qt_meta_stringdata_StatisticBar,
      qt_meta_data_StatisticBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StatisticBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StatisticBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StatisticBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatisticBar))
        return static_cast<void*>(const_cast< StatisticBar*>(this));
    return BarBase::qt_metacast(_clname);
}

int StatisticBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BarBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void StatisticBar::needStatisticalData(QPointF _t1, StatisticManager * _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
