/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created: Mon May 14 15:06:58 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,    8,    7,    7, 0x08,
      44,   40,    7,    7, 0x08,
      67,   40,    7,    7, 0x08,
      88,   40,    7,    7, 0x08,
     111,   40,    7,    7, 0x08,
     134,   40,    7,    7, 0x08,
     162,  158,    7,    7, 0x08,
     184,    7,    7,    7, 0x08,
     211,    7,    7,    7, 0x08,
     233,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Dialog[] = {
    "Dialog\0\0name\0onPortNameChanged(QString)\0"
    "idx\0onBaudRateChanged(int)\0"
    "onParityChanged(int)\0onDataBitsChanged(int)\0"
    "onStopBitsChanged(int)\0onQueryModeChanged(int)\0"
    "val\0onTimeoutChanged(int)\0"
    "onOpenCloseButtonClicked()\0"
    "onSendButtonClicked()\0onReadyRead()\0"
};

void Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dialog *_t = static_cast<Dialog *>(_o);
        switch (_id) {
        case 0: _t->onPortNameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onBaudRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onParityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onDataBitsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onStopBitsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onQueryModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onTimeoutChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onOpenCloseButtonClicked(); break;
        case 8: _t->onSendButtonClicked(); break;
        case 9: _t->onReadyRead(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog,
      qt_meta_data_Dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
