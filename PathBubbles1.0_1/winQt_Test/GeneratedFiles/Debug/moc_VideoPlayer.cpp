/****************************************************************************
** Meta object code from reading C++ file 'VideoPlayer.h'
**
** Created: Mon Jun 30 18:28:40 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VideoPlayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_myVideoPlayer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      43,   14,   14,   14, 0x05,
      61,   14,   14,   14, 0x05,
      72,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,   14,   14,   14, 0x0a,
      92,   14,   14,   14, 0x0a,
     103,   14,   14,   14, 0x0a,
     110,   14,   14,   14, 0x08,
     130,  124,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_myVideoPlayer[] = {
    "myVideoPlayer\0\0needCapture(myVideoPlayer*)\0"
    "addBatWithVideo()\0logPause()\0logPlay()\0"
    "capture()\0openFile()\0play()\0updateFrame()\0"
    "frame\0setPosition(int)\0"
};

void myVideoPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        myVideoPlayer *_t = static_cast<myVideoPlayer *>(_o);
        switch (_id) {
        case 0: _t->needCapture((*reinterpret_cast< myVideoPlayer*(*)>(_a[1]))); break;
        case 1: _t->addBatWithVideo(); break;
        case 2: _t->logPause(); break;
        case 3: _t->logPlay(); break;
        case 4: _t->capture(); break;
        case 5: _t->openFile(); break;
        case 6: _t->play(); break;
        case 7: _t->updateFrame(); break;
        case 8: _t->setPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData myVideoPlayer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject myVideoPlayer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_myVideoPlayer,
      qt_meta_data_myVideoPlayer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &myVideoPlayer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *myVideoPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *myVideoPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myVideoPlayer))
        return static_cast<void*>(const_cast< myVideoPlayer*>(this));
    return QWidget::qt_metacast(_clname);
}

int myVideoPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void myVideoPlayer::needCapture(myVideoPlayer * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void myVideoPlayer::addBatWithVideo()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void myVideoPlayer::logPause()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void myVideoPlayer::logPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
