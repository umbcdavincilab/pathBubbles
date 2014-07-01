/****************************************************************************
** Meta object code from reading C++ file 'SearchFind.h'
**
** Created: Mon Jun 30 18:28:41 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SearchFind.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchFind.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SearchFind[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      28,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   11,   11,   11, 0x0a,
      68,   11,   11,   11, 0x0a,
      86,   11,   11,   11, 0x0a,
      97,   11,   11,   11, 0x0a,
     107,   11,   11,   11, 0x0a,
     128,  116,   11,   11, 0x0a,
     166,   11,   11,   11, 0x0a,
     182,   11,   11,   11, 0x0a,
     200,   11,   11,   11, 0x0a,
     224,  220,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SearchFind[] = {
    "SearchFind\0\0needSetToHide()\0"
    "moveButtonState(bool)\0updatePreview_1()\0"
    "updatePreview_2()\0needHide()\0openURL()\0"
    "search()\0bOK,ip,area\0"
    "onQueryFinished(bool,QString,QString)\0"
    "onQueryButton()\0moveButtonPress()\0"
    "moveButtonRelease()\0web\0setWebItem(WebBlock*)\0"
};

void SearchFind::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SearchFind *_t = static_cast<SearchFind *>(_o);
        switch (_id) {
        case 0: _t->needSetToHide(); break;
        case 1: _t->moveButtonState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updatePreview_1(); break;
        case 3: _t->updatePreview_2(); break;
        case 4: _t->needHide(); break;
        case 5: _t->openURL(); break;
        case 6: _t->search(); break;
        case 7: _t->onQueryFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: _t->onQueryButton(); break;
        case 9: _t->moveButtonPress(); break;
        case 10: _t->moveButtonRelease(); break;
        case 11: _t->setWebItem((*reinterpret_cast< WebBlock*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SearchFind::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SearchFind::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SearchFind,
      qt_meta_data_SearchFind, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SearchFind::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SearchFind::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SearchFind::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchFind))
        return static_cast<void*>(const_cast< SearchFind*>(this));
    return QDialog::qt_metacast(_clname);
}

int SearchFind::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SearchFind::needSetToHide()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SearchFind::moveButtonState(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
