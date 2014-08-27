/****************************************************************************
** Meta object code from reading C++ file 'qttreepropertybrowser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qttreepropertybrowser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qttreepropertybrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtTreePropertyBrowser_t {
    QByteArrayData data[23];
    char stringdata[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtTreePropertyBrowser_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtTreePropertyBrowser_t qt_meta_stringdata_QtTreePropertyBrowser = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 9),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 14),
QT_MOC_LITERAL(4, 48, 4),
QT_MOC_LITERAL(5, 53, 8),
QT_MOC_LITERAL(6, 62, 13),
QT_MOC_LITERAL(7, 76, 12),
QT_MOC_LITERAL(8, 89, 29),
QT_MOC_LITERAL(9, 119, 26),
QT_MOC_LITERAL(10, 146, 16),
QT_MOC_LITERAL(11, 163, 11),
QT_MOC_LITERAL(12, 175, 15),
QT_MOC_LITERAL(13, 191, 20),
QT_MOC_LITERAL(14, 212, 13),
QT_MOC_LITERAL(15, 226, 10),
QT_MOC_LITERAL(16, 237, 10),
QT_MOC_LITERAL(17, 248, 16),
QT_MOC_LITERAL(18, 265, 28),
QT_MOC_LITERAL(19, 294, 11),
QT_MOC_LITERAL(20, 306, 7),
QT_MOC_LITERAL(21, 314, 5),
QT_MOC_LITERAL(22, 320, 16)
    },
    "QtTreePropertyBrowser\0collapsed\0\0"
    "QtBrowserItem*\0item\0expanded\0slotCollapsed\0"
    "slotExpanded\0slotCurrentBrowserItemChanged\0"
    "slotCurrentTreeItemChanged\0QTreeWidgetItem*\0"
    "indentation\0rootIsDecorated\0"
    "alternatingRowColors\0headerVisible\0"
    "resizeMode\0ResizeMode\0splitterPosition\0"
    "propertiesWithoutValueMarked\0Interactive\0"
    "Stretch\0Fixed\0ResizeToContents"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtTreePropertyBrowser[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       7,   64, // properties
       1,   85, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   50,    2, 0x08 /* Private */,
       7,    1,   53,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x08 /* Private */,
       9,    2,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 10,    2,    2,

 // properties: name, type, flags
      11, QMetaType::Int, 0x00095103,
      12, QMetaType::Bool, 0x00095103,
      13, QMetaType::Bool, 0x00095103,
      14, QMetaType::Bool, 0x00095103,
      15, 0x80000000 | 16, 0x0009510b,
      17, QMetaType::Int, 0x00095103,
      18, QMetaType::Bool, 0x00095103,

 // enums: name, flags, count, data
      16, 0x0,    4,   89,

 // enum data: key, value
      19, uint(QtTreePropertyBrowser::Interactive),
      20, uint(QtTreePropertyBrowser::Stretch),
      21, uint(QtTreePropertyBrowser::Fixed),
      22, uint(QtTreePropertyBrowser::ResizeToContents),

       0        // eod
};

void QtTreePropertyBrowser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtTreePropertyBrowser *_t = static_cast<QtTreePropertyBrowser *>(_o);
        switch (_id) {
        case 0: _t->collapsed((*reinterpret_cast< QtBrowserItem*(*)>(_a[1]))); break;
        case 1: _t->expanded((*reinterpret_cast< QtBrowserItem*(*)>(_a[1]))); break;
        case 2: _t->d_func()->slotCollapsed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->d_func()->slotExpanded((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->d_func()->slotCurrentBrowserItemChanged((*reinterpret_cast< QtBrowserItem*(*)>(_a[1]))); break;
        case 5: _t->d_func()->slotCurrentTreeItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QtTreePropertyBrowser::*_t)(QtBrowserItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtTreePropertyBrowser::collapsed)) {
                *result = 0;
            }
        }
        {
            typedef void (QtTreePropertyBrowser::*_t)(QtBrowserItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtTreePropertyBrowser::expanded)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject QtTreePropertyBrowser::staticMetaObject = {
    { &QtAbstractPropertyBrowser::staticMetaObject, qt_meta_stringdata_QtTreePropertyBrowser.data,
      qt_meta_data_QtTreePropertyBrowser,  qt_static_metacall, 0, 0}
};


const QMetaObject *QtTreePropertyBrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtTreePropertyBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtTreePropertyBrowser.stringdata))
        return static_cast<void*>(const_cast< QtTreePropertyBrowser*>(this));
    return QtAbstractPropertyBrowser::qt_metacast(_clname);
}

int QtTreePropertyBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtAbstractPropertyBrowser::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = indentation(); break;
        case 1: *reinterpret_cast< bool*>(_v) = rootIsDecorated(); break;
        case 2: *reinterpret_cast< bool*>(_v) = alternatingRowColors(); break;
        case 3: *reinterpret_cast< bool*>(_v) = isHeaderVisible(); break;
        case 4: *reinterpret_cast< ResizeMode*>(_v) = resizeMode(); break;
        case 5: *reinterpret_cast< int*>(_v) = splitterPosition(); break;
        case 6: *reinterpret_cast< bool*>(_v) = propertiesWithoutValueMarked(); break;
        default: break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setIndentation(*reinterpret_cast< int*>(_v)); break;
        case 1: setRootIsDecorated(*reinterpret_cast< bool*>(_v)); break;
        case 2: setAlternatingRowColors(*reinterpret_cast< bool*>(_v)); break;
        case 3: setHeaderVisible(*reinterpret_cast< bool*>(_v)); break;
        case 4: setResizeMode(*reinterpret_cast< ResizeMode*>(_v)); break;
        case 5: setSplitterPosition(*reinterpret_cast< int*>(_v)); break;
        case 6: setPropertiesWithoutValueMarked(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtTreePropertyBrowser::collapsed(QtBrowserItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtTreePropertyBrowser::expanded(QtBrowserItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
