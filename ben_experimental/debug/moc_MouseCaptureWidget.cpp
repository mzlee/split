/****************************************************************************
** Meta object code from reading C++ file 'MouseCaptureWidget.h'
**
** Created: Fri Oct 30 22:51:47 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MouseCaptureWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MouseCaptureWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MouseCaptureWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   20,   19,   19, 0x05,
      74,   59,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MouseCaptureWidget[] = {
    "MouseCaptureWidget\0\0mc\0"
    "mcStartCapture(MouseCaptureWidget*)\0"
    "capturedRegion\0mcEndCapture(QRegion)\0"
};

const QMetaObject MouseCaptureWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MouseCaptureWidget,
      qt_meta_data_MouseCaptureWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MouseCaptureWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MouseCaptureWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MouseCaptureWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MouseCaptureWidget))
        return static_cast<void*>(const_cast< MouseCaptureWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MouseCaptureWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mcStartCapture((*reinterpret_cast< MouseCaptureWidget*(*)>(_a[1]))); break;
        case 1: mcEndCapture((*reinterpret_cast< QRegion(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MouseCaptureWidget::mcStartCapture(MouseCaptureWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MouseCaptureWidget::mcEndCapture(QRegion _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
