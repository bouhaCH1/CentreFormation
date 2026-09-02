/****************************************************************************
** Meta object code from reading C++ file 'CoursWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../views/CoursWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CoursWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSCoursWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSCoursWidgetENDCLASS = QtMocHelpers::stringData(
    "CoursWidget",
    "onAjouter",
    "",
    "onModifier",
    "onSupprimer",
    "onSave",
    "onCancel",
    "onSearchToggle",
    "onApplySearch",
    "onClearSearch",
    "onApplySort",
    "onShowStats",
    "onGeneratePDF",
    "onDetectConflits",
    "onCalculChargeHoraire",
    "onTableSelectionChanged"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCoursWidgetENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x08,    1 /* Private */,
       3,    0,   99,    2, 0x08,    2 /* Private */,
       4,    0,  100,    2, 0x08,    3 /* Private */,
       5,    0,  101,    2, 0x08,    4 /* Private */,
       6,    0,  102,    2, 0x08,    5 /* Private */,
       7,    0,  103,    2, 0x08,    6 /* Private */,
       8,    0,  104,    2, 0x08,    7 /* Private */,
       9,    0,  105,    2, 0x08,    8 /* Private */,
      10,    0,  106,    2, 0x08,    9 /* Private */,
      11,    0,  107,    2, 0x08,   10 /* Private */,
      12,    0,  108,    2, 0x08,   11 /* Private */,
      13,    0,  109,    2, 0x08,   12 /* Private */,
      14,    0,  110,    2, 0x08,   13 /* Private */,
      15,    0,  111,    2, 0x08,   14 /* Private */,

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

       0        // eod
};

Q_CONSTINIT const QMetaObject CoursWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSCoursWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCoursWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCoursWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CoursWidget, std::true_type>,
        // method 'onAjouter'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onModifier'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSupprimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSave'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCancel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchToggle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onApplySearch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearSearch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onApplySort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onShowStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onGeneratePDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDetectConflits'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCalculChargeHoraire'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTableSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CoursWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CoursWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAjouter(); break;
        case 1: _t->onModifier(); break;
        case 2: _t->onSupprimer(); break;
        case 3: _t->onSave(); break;
        case 4: _t->onCancel(); break;
        case 5: _t->onSearchToggle(); break;
        case 6: _t->onApplySearch(); break;
        case 7: _t->onClearSearch(); break;
        case 8: _t->onApplySort(); break;
        case 9: _t->onShowStats(); break;
        case 10: _t->onGeneratePDF(); break;
        case 11: _t->onDetectConflits(); break;
        case 12: _t->onCalculChargeHoraire(); break;
        case 13: _t->onTableSelectionChanged(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *CoursWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CoursWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCoursWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CoursWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
