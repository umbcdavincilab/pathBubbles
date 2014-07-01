/****************************************************************************
** Meta object code from reading C++ file 'textBubble2.h'
**
** Created: Mon Jun 30 18:28:43 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../textBubble2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textBubble2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TextBubble2[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   13,   12,   12, 0x05,
      71,   46,   12,   12, 0x05,
     139,  122,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     180,   12,   12,   12, 0x0a,
     193,   12,   12,   12, 0x0a,
     213,   12,  205,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TextBubble2[] = {
    "TextBubble2\0\0item\0updateLabelState(ItemBase*)\0"
    "pname,run_num,dataResult\0"
    "addParameterFromMat(QString,QString,QList<double>)\0"
    "pname,dataResult\0"
    "updateDataFromMat(QString,QList<double>)\0"
    "updatePath()\0updatePos()\0QString\0"
    "on_textEdit_clicked()\0"
};

void TextBubble2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TextBubble2 *_t = static_cast<TextBubble2 *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->addParameterFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<double>(*)>(_a[3]))); break;
        case 2: _t->updateDataFromMat((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QList<double>(*)>(_a[2]))); break;
        case 3: _t->updatePath(); break;
        case 4: _t->updatePos(); break;
        case 5: { QString _r = _t->on_textEdit_clicked();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TextBubble2::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TextBubble2::staticMetaObject = {
    { &ItemBase::staticMetaObject, qt_meta_stringdata_TextBubble2,
      qt_meta_data_TextBubble2, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TextBubble2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TextBubble2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TextBubble2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextBubble2))
        return static_cast<void*>(const_cast< TextBubble2*>(this));
    return ItemBase::qt_metacast(_clname);
}

int TextBubble2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ItemBase::qt_metacall(_c, _id, _a);
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
void TextBubble2::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextBubble2::addParameterFromMat(QString _t1, QString _t2, QList<double> _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TextBubble2::updateDataFromMat(QString _t1, QList<double> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
