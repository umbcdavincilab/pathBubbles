/****************************************************************************
** Meta object code from reading C++ file 'openglscene.h'
**
** Created: Mon Nov 5 12:52:55 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../openglscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openglscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OpenGLScene[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   36,   12,   12, 0x0a,
      67,   58,   12,   12, 0x0a,
     121,  103,   12,   12, 0x0a,
     172,   12,   12,   12, 0x0a,
     194,   12,   12,   12, 0x0a,
     216,   12,   12,   12, 0x0a,
     230,   12,   12,   12, 0x0a,
     251,   12,   12,   12, 0x0a,
     264,   12,   12,   12, 0x0a,
     293,  287,   12,   12, 0x0a,
     320,  313,   12,   12, 0x0a,
     349,  342,   12,   12, 0x0a,
     368,   12,   12,   12, 0x0a,
     381,   12,   12,   12, 0x0a,
     392,   12,   12,   12, 0x0a,
     403,   12,   12,   12, 0x0a,
     414,   12,   12,   12, 0x0a,
     425,   12,   12,   12, 0x0a,
     436,   12,   12,   12, 0x0a,
     447,   12,   12,   12, 0x0a,
     458,   12,   12,   12, 0x0a,
     469,   12,   12,   12, 0x0a,
     480,  342,   12,   12, 0x0a,
     511,  342,   12,   12, 0x0a,
     549,  538,   12,   12, 0x0a,
     581,   12,  577,   12, 0x0a,
     598,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OpenGLScene[] = {
    "OpenGLScene\0\0flag\0item_clicked(int)\0"
    "str\0logInfor(QString)\0aGen,pos\0"
    "setWingShapePara(ArrowGen*,QPointF)\0"
    "pos,manager,index\0"
    "needStatisticalData(QPointF,StatisticManager*,int)\0"
    "moveButtonState(bool)\0updateCoverage(float)\0"
    "hideWebView()\0setBackgroundColor()\0"
    "orderViews()\0floatingButtonPushed()\0"
    "index\0index_recieved(int)\0offset\0"
    "shiftViewport(QPoint)\0object\0"
    "addCompare(QtBat*)\0openAllPCs()\0"
    "mapViews()\0openCBRY()\0openCBOR()\0"
    "openCBDG()\0openCBMA()\0openCBYR()\0"
    "openPCM1()\0openPCM2()\0openPCM3()\0"
    "setCurrentBatAttribute(QtBat*)\0"
    "getNewBatAttribute(QtBat*)\0currentMat\0"
    "setMatFileBubble(MatBlock*)\0int\0"
    "getBubbleIndex()\0appHasClosed()\0"
};

void OpenGLScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OpenGLScene *_t = static_cast<OpenGLScene *>(_o);
        switch (_id) {
        case 0: _t->item_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->logInfor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setWingShapePara((*reinterpret_cast< ArrowGen*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 3: _t->needStatisticalData((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< StatisticManager*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->moveButtonState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->updateCoverage((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->hideWebView(); break;
        case 7: _t->setBackgroundColor(); break;
        case 8: _t->orderViews(); break;
        case 9: _t->floatingButtonPushed(); break;
        case 10: _t->index_recieved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->shiftViewport((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 12: _t->addCompare((*reinterpret_cast< QtBat*(*)>(_a[1]))); break;
        case 13: _t->openAllPCs(); break;
        case 14: _t->mapViews(); break;
        case 15: _t->openCBRY(); break;
        case 16: _t->openCBOR(); break;
        case 17: _t->openCBDG(); break;
        case 18: _t->openCBMA(); break;
        case 19: _t->openCBYR(); break;
        case 20: _t->openPCM1(); break;
        case 21: _t->openPCM2(); break;
        case 22: _t->openPCM3(); break;
        case 23: _t->setCurrentBatAttribute((*reinterpret_cast< QtBat*(*)>(_a[1]))); break;
        case 24: _t->getNewBatAttribute((*reinterpret_cast< QtBat*(*)>(_a[1]))); break;
        case 25: _t->setMatFileBubble((*reinterpret_cast< MatBlock*(*)>(_a[1]))); break;
        case 26: { int _r = _t->getBubbleIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 27: _t->appHasClosed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OpenGLScene::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OpenGLScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_OpenGLScene,
      qt_meta_data_OpenGLScene, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OpenGLScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OpenGLScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OpenGLScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLScene))
        return static_cast<void*>(const_cast< OpenGLScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int OpenGLScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void OpenGLScene::item_clicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
