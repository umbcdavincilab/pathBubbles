/****************************************************************************
** Meta object code from reading C++ file 'GroupManager.h'
**
** Created: Mon Nov 5 12:52:55 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GroupManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GroupManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GroupManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x0a,
      58,   47,   13,   13, 0x0a,
      92,   14,   13,   13, 0x0a,
     130,  118,   13,   13, 0x0a,
     162,   14,   13,   13, 0x0a,
     185,   14,   13,   13, 0x0a,
     215,  204,   13,   13, 0x0a,
     268,  251,   13,   13, 0x0a,
     300,  295,   13,   13, 0x0a,
     346,  341,   13,   13, 0x0a,
     375,  341,   13,   13, 0x0a,
     405,  341,   13,   13, 0x0a,
     437,  341,   13,   13, 0x0a,
     466,  341,   13,   13, 0x0a,
     496,  341,   13,   13, 0x0a,
     526,  341,   13,   13, 0x0a,
     562,  556,   13,   13, 0x0a,
     611,  601,   13,   13, 0x0a,
     651,  601,   13,   13, 0x0a,
     695,  690,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GroupManager[] = {
    "GroupManager\0\0init\0updateLabelState(ItemBase*)\0"
    "init,input\0updatePCColorState(ItemBase*,int)\0"
    "itemPosUpdated(ItemBase*)\0init,offset\0"
    "itemPosMoved(ItemBase*,QPointF)\0"
    "itemRemoved(ItemBase*)\0unGroup(ItemBase*)\0"
    "bat,offset\0updateSelectBats(ItemBase*,QPointF)\0"
    "sm,source,target\0swapSelectPCs(SM*,int,int)\0"
    "bats\0addBatsToScene(QList<QList<ItemBase*> >)\0"
    "bat,\0sliderChanged(ItemBase*,int)\0"
    "surfaceChecked(ItemBase*,int)\0"
    "wireframeChecked(ItemBase*,int)\0"
    "symbolChecked(ItemBase*,int)\0"
    "camber1Checked(ItemBase*,int)\0"
    "camber2Checked(ItemBase*,int)\0"
    "camber3Checked(ItemBase*,int)\0item,\0"
    "HighlightChangeinGroup(ItemBase*,bool)\0"
    "item,rect\0pressHightlightinGroup(ItemBase*,QRect)\0"
    "moveHightlightinGroup(ItemBase*,QRect)\0"
    "item\0releaseHightlightinGroup(ItemBase*)\0"
};

void GroupManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GroupManager *_t = static_cast<GroupManager *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->updatePCColorState((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->itemPosUpdated((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 3: _t->itemPosMoved((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 4: _t->itemRemoved((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 5: _t->unGroup((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 6: _t->updateSelectBats((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 7: _t->swapSelectPCs((*reinterpret_cast< SM*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->addBatsToScene((*reinterpret_cast< QList<QList<ItemBase*> >(*)>(_a[1]))); break;
        case 9: _t->sliderChanged((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->surfaceChecked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->wireframeChecked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->symbolChecked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->camber1Checked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->camber2Checked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->camber3Checked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->HighlightChangeinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 17: _t->pressHightlightinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRect(*)>(_a[2]))); break;
        case 18: _t->moveHightlightinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRect(*)>(_a[2]))); break;
        case 19: _t->releaseHightlightinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GroupManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GroupManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GroupManager,
      qt_meta_data_GroupManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GroupManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GroupManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GroupManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GroupManager))
        return static_cast<void*>(const_cast< GroupManager*>(this));
    return QObject::qt_metacast(_clname);
}

int GroupManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
