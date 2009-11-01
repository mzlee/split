/****************************************************************************
** Meta object code from reading C++ file 'WebWindow.h'
**
** Created: Sat Oct 31 21:54:12 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../WebWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WebWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   11,   10,   10, 0x05,
      34,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   62,   10,   10, 0x08,
      89,   10,   10,   10, 0x08,
      94,   10,   10,   10, 0x08,
     106,   10,   10,   10, 0x08,
     126,   10,   10,   10, 0x08,
     145,   10,   10,   10, 0x08,
     155,   10,   10,   10, 0x08,
     162,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WebWindow[] = {
    "WebWindow\0\0ww\0wwFocus(WebWindow*)\0"
    "wwRestoreWindow(WebWindow*)\0region\0"
    "createMask(QRegion)\0go()\0newWindow()\0"
    "startClippingMode()\0exitClippingMode()\0"
    "forward()\0back()\0restoreClip()\0"
};

const QMetaObject WebWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WebWindow,
      qt_meta_data_WebWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebWindow))
        return static_cast<void*>(const_cast< WebWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WebWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: wwFocus((*reinterpret_cast< WebWindow*(*)>(_a[1]))); break;
        case 1: wwRestoreWindow((*reinterpret_cast< WebWindow*(*)>(_a[1]))); break;
        case 2: createMask((*reinterpret_cast< QRegion(*)>(_a[1]))); break;
        case 3: go(); break;
        case 4: newWindow(); break;
        case 5: startClippingMode(); break;
        case 6: exitClippingMode(); break;
        case 7: forward(); break;
        case 8: back(); break;
        case 9: restoreClip(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void WebWindow::wwFocus(WebWindow * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WebWindow::wwRestoreWindow(WebWindow * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
