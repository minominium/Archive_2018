/****************************************************************************
** Meta object code from reading C++ file 'cubicspline.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Drone_GCS_180702/cubicspline.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cubicspline.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CubicSpline_t {
    QByteArrayData data[11];
    char stringdata[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CubicSpline_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CubicSpline_t qt_meta_stringdata_CubicSpline = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CubicSpline"
QT_MOC_LITERAL(1, 12, 13), // "sigDrawSpline"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "GPS_data"
QT_MOC_LITERAL(4, 36, 9), // "runSpline"
QT_MOC_LITERAL(5, 46, 5), // "Point"
QT_MOC_LITERAL(6, 52, 2), // "sp"
QT_MOC_LITERAL(7, 55, 2), // "wp"
QT_MOC_LITERAL(8, 58, 2), // "dp"
QT_MOC_LITERAL(9, 61, 2), // "ep"
QT_MOC_LITERAL(10, 64, 2) // "nf"

    },
    "CubicSpline\0sigDrawSpline\0\0GPS_data\0"
    "runSpline\0Point\0sp\0wp\0dp\0ep\0nf"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CubicSpline[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    5,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 5,    6,    7,    8,    9,   10,

       0        // eod
};

void CubicSpline::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CubicSpline *_t = static_cast<CubicSpline *>(_o);
        switch (_id) {
        case 0: _t->sigDrawSpline((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->runSpline((*reinterpret_cast< Point(*)>(_a[1])),(*reinterpret_cast< Point(*)>(_a[2])),(*reinterpret_cast< Point(*)>(_a[3])),(*reinterpret_cast< Point(*)>(_a[4])),(*reinterpret_cast< Point(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CubicSpline::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CubicSpline::sigDrawSpline)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CubicSpline::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CubicSpline.data,
      qt_meta_data_CubicSpline,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CubicSpline::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CubicSpline::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CubicSpline.stringdata))
        return static_cast<void*>(const_cast< CubicSpline*>(this));
    return QObject::qt_metacast(_clname);
}

int CubicSpline::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CubicSpline::sigDrawSpline(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
