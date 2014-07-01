/****************************************************************************
** Meta object code from reading C++ file 'ImagePlayer.h'
**
** Created: Mon Nov 5 12:52:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ImagePlayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImagePlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImagePlayer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      24,   12,   12,   12, 0x0a,
      37,   31,   12,   12, 0x08,
      81,   75,   12,   12, 0x08,
      99,   75,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImagePlayer[] = {
    "ImagePlayer\0\0openFile()\0play()\0state\0"
    "movieStateChanged(QMovie::MovieState)\0"
    "frame\0frameChanged(int)\0setPosition(int)\0"
};

void ImagePlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ImagePlayer *_t = static_cast<ImagePlayer *>(_o);
        switch (_id) {
        case 0: _t->openFile(); break;
        case 1: _t->play(); break;
        case 2: _t->movieStateChanged((*reinterpret_cast< QMovie::MovieState(*)>(_a[1]))); break;
        case 3: _t->frameChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ImagePlayer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ImagePlayer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ImagePlayer,
      qt_meta_data_ImagePlayer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImagePlayer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImagePlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImagePlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImagePlayer))
        return static_cast<void*>(const_cast< ImagePlayer*>(this));
    return QWidget::qt_metacast(_clname);
}

int ImagePlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
