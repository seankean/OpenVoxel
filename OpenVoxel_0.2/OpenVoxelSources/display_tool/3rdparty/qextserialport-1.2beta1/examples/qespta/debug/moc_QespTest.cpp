/****************************************************************************
** Meta object code from reading C++ file 'QespTest.h'
**
** Created: Mon May 14 15:06:39 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QespTest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QespTest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QespTest[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      24,    9,    9,    9, 0x08,
      37,    9,    9,    9, 0x08,
      48,    9,    9,    9, 0x08,
      59,    9,    9,    9, 0x08,
      71,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QespTest[] = {
    "QespTest\0\0transmitMsg()\0receiveMsg()\0"
    "appendCR()\0appendLF()\0closePort()\0"
    "openPort()\0"
};

void QespTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QespTest *_t = static_cast<QespTest *>(_o);
        switch (_id) {
        case 0: _t->transmitMsg(); break;
        case 1: _t->receiveMsg(); break;
        case 2: _t->appendCR(); break;
        case 3: _t->appendLF(); break;
        case 4: _t->closePort(); break;
        case 5: _t->openPort(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QespTest::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QespTest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QespTest,
      qt_meta_data_QespTest, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QespTest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QespTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QespTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QespTest))
        return static_cast<void*>(const_cast< QespTest*>(this));
    return QWidget::qt_metacast(_clname);
}

int QespTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
