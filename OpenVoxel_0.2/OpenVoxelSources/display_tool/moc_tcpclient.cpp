/****************************************************************************
** Meta object code from reading C++ file 'tcpclient.h'
**
** Created: Thu Jun 7 13:39:00 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tcpclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcpClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   11,   10,   10, 0x08,
      46,   10,   10,   10, 0x08,
      68,   56,   10,   10, 0x08,
     111,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_tcpClient[] = {
    "tcpClient\0\0host,port\0requestCam(QString*,int)\0"
    "readCam()\0socketError\0"
    "displayError(QAbstractSocket::SocketError)\0"
    "sessionOpened()\0"
};

void tcpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        tcpClient *_t = static_cast<tcpClient *>(_o);
        switch (_id) {
        case 0: _t->requestCam((*reinterpret_cast< QString*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->readCam(); break;
        case 2: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->sessionOpened(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData tcpClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject tcpClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_tcpClient,
      qt_meta_data_tcpClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tcpClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tcpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tcpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tcpClient))
        return static_cast<void*>(const_cast< tcpClient*>(this));
    return QObject::qt_metacast(_clname);
}

int tcpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
