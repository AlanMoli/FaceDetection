/****************************************************************************
** Meta object code from reading C++ file 'FaceDetection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FaceDetection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FaceDetection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FaceDetection_t {
    QByteArrayData data[19];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FaceDetection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FaceDetection_t qt_meta_stringdata_FaceDetection = {
    {
QT_MOC_LITERAL(0, 0, 13), // "FaceDetection"
QT_MOC_LITERAL(1, 14, 9), // "File_open"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "FrontFace_open"
QT_MOC_LITERAL(4, 40, 16), // "ProfileFace_open"
QT_MOC_LITERAL(5, 57, 11), // "Camera_open"
QT_MOC_LITERAL(6, 69, 11), // "Camera_save"
QT_MOC_LITERAL(7, 81, 12), // "Camera_close"
QT_MOC_LITERAL(8, 94, 10), // "File_close"
QT_MOC_LITERAL(9, 105, 11), // "Full_screen"
QT_MOC_LITERAL(10, 117, 11), // "Exit_screen"
QT_MOC_LITERAL(11, 129, 11), // "Select_skin"
QT_MOC_LITERAL(12, 141, 7), // "showWin"
QT_MOC_LITERAL(13, 149, 9), // "showImage"
QT_MOC_LITERAL(14, 159, 5), // "skin1"
QT_MOC_LITERAL(15, 165, 5), // "skin2"
QT_MOC_LITERAL(16, 171, 10), // "closeEvent"
QT_MOC_LITERAL(17, 182, 12), // "QCloseEvent*"
QT_MOC_LITERAL(18, 195, 5) // "event"

    },
    "FaceDetection\0File_open\0\0FrontFace_open\0"
    "ProfileFace_open\0Camera_open\0Camera_save\0"
    "Camera_close\0File_close\0Full_screen\0"
    "Exit_screen\0Select_skin\0showWin\0"
    "showImage\0skin1\0skin2\0closeEvent\0"
    "QCloseEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FaceDetection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    0,   95,    2, 0x08 /* Private */,
       9,    0,   96,    2, 0x08 /* Private */,
      10,    0,   97,    2, 0x08 /* Private */,
      11,    0,   98,    2, 0x08 /* Private */,
      12,    0,   99,    2, 0x08 /* Private */,
      13,    0,  100,    2, 0x08 /* Private */,
      14,    0,  101,    2, 0x08 /* Private */,
      15,    0,  102,    2, 0x08 /* Private */,
      16,    1,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void FaceDetection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FaceDetection *_t = static_cast<FaceDetection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->File_open(); break;
        case 1: _t->FrontFace_open(); break;
        case 2: _t->ProfileFace_open(); break;
        case 3: _t->Camera_open(); break;
        case 4: _t->Camera_save(); break;
        case 5: _t->Camera_close(); break;
        case 6: _t->File_close(); break;
        case 7: _t->Full_screen(); break;
        case 8: _t->Exit_screen(); break;
        case 9: _t->Select_skin(); break;
        case 10: _t->showWin(); break;
        case 11: _t->showImage(); break;
        case 12: _t->skin1(); break;
        case 13: _t->skin2(); break;
        case 14: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FaceDetection::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FaceDetection.data,
      qt_meta_data_FaceDetection,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FaceDetection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FaceDetection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FaceDetection.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FaceDetection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
