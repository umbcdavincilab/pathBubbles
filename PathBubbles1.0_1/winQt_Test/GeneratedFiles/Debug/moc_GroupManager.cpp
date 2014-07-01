/****************************************************************************
** Meta object code from reading C++ file 'GroupManager.h'
**
** Created: Mon Jun 30 18:28:44 2014
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
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,
      67,   61,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     146,  121,   13,   13, 0x0a,
     187,  182,   13,   13, 0x0a,
     226,  215,   13,   13, 0x0a,
     260,  182,   13,   13, 0x0a,
     298,  286,   13,   13, 0x0a,
     330,  182,   13,   13, 0x0a,
     353,  182,   13,   13, 0x0a,
     383,  372,   13,   13, 0x0a,
     436,  419,   13,   13, 0x0a,
     468,  463,   13,   13, 0x0a,
     497,  463,   13,   13, 0x0a,
     527,  463,   13,   13, 0x0a,
     559,  463,   13,   13, 0x0a,
     588,  463,   13,   13, 0x0a,
     618,  463,   13,   13, 0x0a,
     648,  463,   13,   13, 0x0a,
     684,  678,   13,   13, 0x0a,
     733,  723,   13,   13, 0x0a,
     774,  723,   13,   13, 0x0a,
     819,  814,   13,   13, 0x0a,
     872,  182,  855,   13, 0x0a,
     906,  182,  899,   13, 0x0a,
     926,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GroupManager[] = {
    "GroupManager\0\0b0,tmp\0"
    "findEnclosed(PathBubble1*,PathBubble1*)\0"
    "b1,b2\0findEnclosedTreeRing(treeRingBubble*,treeRingBubble*)\0"
    "init,oriRect,cornerIndex\0"
    "bubbleResized(ItemBase*,QRectF,int)\0"
    "init\0updateLabelState(ItemBase*)\0"
    "init,input\0updatePCColorState(ItemBase*,int)\0"
    "itemPosUpdated(ItemBase*)\0init,offset\0"
    "itemPosMoved(ItemBase*,QPointF)\0"
    "itemRemoved(ItemBase*)\0unGroup(ItemBase*)\0"
    "bat,offset\0updateSelectBats(ItemBase*,QPointF)\0"
    "sm,source,target\0swapSelectPCs(SM*,int,int)\0"
    "bat,\0sliderChanged(ItemBase*,int)\0"
    "surfaceChecked(ItemBase*,int)\0"
    "wireframeChecked(ItemBase*,int)\0"
    "symbolChecked(ItemBase*,int)\0"
    "camber1Checked(ItemBase*,int)\0"
    "camber2Checked(ItemBase*,int)\0"
    "camber3Checked(ItemBase*,int)\0item,\0"
    "HighlightChangeinGroup(ItemBase*,bool)\0"
    "item,rect\0pressHightlightinGroup(ItemBase*,QRectF)\0"
    "moveHightlightinGroup(ItemBase*,QRectF)\0"
    "item\0releaseHightlightinGroup(ItemBase*)\0"
    "QList<ItemBase*>\0getGroupMembers(ItemBase*)\0"
    "Group*\0getGroup(ItemBase*)\0"
    "tightenItemsinSameGroup()\0"
};

void GroupManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GroupManager *_t = static_cast<GroupManager *>(_o);
        switch (_id) {
        case 0: _t->findEnclosed((*reinterpret_cast< PathBubble1*(*)>(_a[1])),(*reinterpret_cast< PathBubble1*(*)>(_a[2]))); break;
        case 1: _t->findEnclosedTreeRing((*reinterpret_cast< treeRingBubble*(*)>(_a[1])),(*reinterpret_cast< treeRingBubble*(*)>(_a[2]))); break;
        case 2: _t->bubbleResized((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 4: _t->updatePCColorState((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->itemPosUpdated((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 6: _t->itemPosMoved((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 7: _t->itemRemoved((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 8: _t->unGroup((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 9: _t->updateSelectBats((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 10: _t->swapSelectPCs((*reinterpret_cast< SM*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: _t->sliderChanged((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->surfaceChecked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->wireframeChecked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->symbolChecked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->camber1Checked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->camber2Checked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->camber3Checked((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->HighlightChangeinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 19: _t->pressHightlightinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2]))); break;
        case 20: _t->moveHightlightinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2]))); break;
        case 21: _t->releaseHightlightinGroup((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 22: { QList<ItemBase*> _r = _t->getGroupMembers((*reinterpret_cast< ItemBase*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<ItemBase*>*>(_a[0]) = _r; }  break;
        case 23: { Group* _r = _t->getGroup((*reinterpret_cast< ItemBase*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Group**>(_a[0]) = _r; }  break;
        case 24: _t->tightenItemsinSameGroup(); break;
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
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void GroupManager::findEnclosed(PathBubble1 * _t1, PathBubble1 * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GroupManager::findEnclosedTreeRing(treeRingBubble * _t1, treeRingBubble * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
