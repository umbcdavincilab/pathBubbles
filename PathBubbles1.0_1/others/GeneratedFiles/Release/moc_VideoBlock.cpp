/****************************************************************************
** Meta object code from reading C++ file 'VideoBlock.h'
**
** Created: Mon Nov 5 12:52:52 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VideoBlock.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoBlock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VideoBlock[] = {

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
      17,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   11,   11,   11, 0x0a,
      63,   11,   11,   11, 0x0a,
      74,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_VideoBlock[] = {
    "VideoBlock\0\0item\0updateLabelState(ItemBase*)\0"
    "addBatWithVideo()\0logPause()\0logPlay()\0"
};

void VideoBlock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VideoBlock *_t = static_cast<VideoBlock *>(_o);
        switch (_id) {
        case 0: _t->updateLabelState((*reinterpret_cast< ItemBase*(*)>(_a[1]))); break;
        case 1: _t->addBatWithVideo(); break;
        case 2: _t->logPause(); break;
        case 3: _t->logPlay(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VideoBlock::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VideoBlock::staticMetaObject = {
    { &ControlBubble::staticMetaObject, qt_meta_stringdata_VideoBlock,
      qt_meta_data_VideoBlock, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VideoBlock::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VideoBlock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VideoBlock::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoBlock))
        return static_cast<void*>(const_cast< VideoBlock*>(this));
    return ControlBubble::qt_metacast(_clname);
}

int VideoBlock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ControlBubble::qt_metacall(_c, _id, _a);
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
void VideoBlock::updateLabelState(ItemBase * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
