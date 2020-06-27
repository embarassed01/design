/****************************************************************************
** Meta object code from reading C++ file 'runcrack.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt-design/runcrack.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runcrack.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RunCrack_t {
    QByteArrayData data[9];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunCrack_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunCrack_t qt_meta_stringdata_RunCrack = {
    {
QT_MOC_LITERAL(0, 0, 8), // "RunCrack"
QT_MOC_LITERAL(1, 9, 20), // "sig_done_crack_force"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "is_success"
QT_MOC_LITERAL(4, 42, 29), // "sig_realtime_crack_force_info"
QT_MOC_LITERAL(5, 72, 4), // "info"
QT_MOC_LITERAL(6, 77, 16), // "slot_crack_force"
QT_MOC_LITERAL(7, 94, 6), // "minlen"
QT_MOC_LITERAL(8, 101, 6) // "maxlen"

    },
    "RunCrack\0sig_done_crack_force\0\0"
    "is_success\0sig_realtime_crack_force_info\0"
    "info\0slot_crack_force\0minlen\0maxlen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunCrack[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::SChar,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Short, QMetaType::Short,    7,    8,

       0        // eod
};

void RunCrack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RunCrack *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_done_crack_force((*reinterpret_cast< qint8(*)>(_a[1]))); break;
        case 1: _t->sig_realtime_crack_force_info((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slot_crack_force((*reinterpret_cast< qint16(*)>(_a[1])),(*reinterpret_cast< qint16(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RunCrack::*)(qint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunCrack::sig_done_crack_force)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RunCrack::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunCrack::sig_realtime_crack_force_info)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RunCrack::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_RunCrack.data,
    qt_meta_data_RunCrack,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RunCrack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RunCrack::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RunCrack.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RunCrack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void RunCrack::sig_done_crack_force(qint8 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RunCrack::sig_realtime_crack_force_info(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
