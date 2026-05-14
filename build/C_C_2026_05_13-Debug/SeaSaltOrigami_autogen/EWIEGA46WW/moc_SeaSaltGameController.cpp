/****************************************************************************
** Meta object code from reading C++ file 'SeaSaltGameController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../SeaSaltGameController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SeaSaltGameController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN21SeaSaltGameControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto SeaSaltGameController::qt_create_metaobjectdata<qt_meta_tag_ZN21SeaSaltGameControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SeaSaltGameController",
        "stateChanged",
        "",
        "titleChanged",
        "requestQuit",
        "gameActiveChanged",
        "singlePlayerModeChanged",
        "drawAnimationRequested",
        "sourcePile",
        "cardName",
        "discardAnimationRequested",
        "targetPile",
        "pendingDrawAnimationRequested",
        "firstCardName",
        "secondCardName",
        "zoneAnimationRequested",
        "sourceArea",
        "targetArea",
        "onTurnTimeOut",
        "startGame",
        "singlePlayer",
        "returnToMenu",
        "drawMain",
        "selectPendingKeep",
        "index",
        "finishPendingMainDraw",
        "discardPileNo",
        "drawDiscard1",
        "drawDiscard2",
        "play",
        "endTurn",
        "endRound",
        "toggleHandCard",
        "moveHandCard",
        "fromIndex",
        "toIndex",
        "showScoreDetail",
        "dismissToast",
        "quitApp",
        "scoreText",
        "playerText",
        "mainPileText",
        "discard1Text",
        "discard2Text",
        "handCardNames",
        "handSelected",
        "QVariantList",
        "selectedHandCount",
        "collectCardLines",
        "buffCardNames",
        "playedPairCardNames",
        "allInPending",
        "drawDiscard1Enabled",
        "drawDiscard2Enabled",
        "roundEndVisible",
        "roundEndHighlight",
        "windowTitleText",
        "gameActive",
        "singlePlayerMode",
        "computerTurn",
        "pendingDrawActive",
        "pendingDrawCardNames",
        "pendingKeepIndex",
        "toastText",
        "toastVisible"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'stateChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'titleChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestQuit'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameActiveChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'singlePlayerModeChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'drawAnimationRequested'
        QtMocHelpers::SignalData<void(QString, QString)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 },
        }}),
        // Signal 'discardAnimationRequested'
        QtMocHelpers::SignalData<void(QString, QString)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 9 },
        }}),
        // Signal 'pendingDrawAnimationRequested'
        QtMocHelpers::SignalData<void(QString, QString)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
        }}),
        // Signal 'zoneAnimationRequested'
        QtMocHelpers::SignalData<void(QString, QString, QString)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 }, { QMetaType::QString, 17 }, { QMetaType::QString, 9 },
        }}),
        // Slot 'onTurnTimeOut'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'startGame'
        QtMocHelpers::MethodData<void(bool)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 20 },
        }}),
        // Method 'returnToMenu'
        QtMocHelpers::MethodData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'drawMain'
        QtMocHelpers::MethodData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'selectPendingKeep'
        QtMocHelpers::MethodData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'finishPendingMainDraw'
        QtMocHelpers::MethodData<void(int)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 },
        }}),
        // Method 'drawDiscard1'
        QtMocHelpers::MethodData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'drawDiscard2'
        QtMocHelpers::MethodData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'play'
        QtMocHelpers::MethodData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'endTurn'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'endRound'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'toggleHandCard'
        QtMocHelpers::MethodData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'moveHandCard'
        QtMocHelpers::MethodData<void(int, int)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 34 }, { QMetaType::Int, 35 },
        }}),
        // Method 'showScoreDetail'
        QtMocHelpers::MethodData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'dismissToast'
        QtMocHelpers::MethodData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'quitApp'
        QtMocHelpers::MethodData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'scoreText'
        QtMocHelpers::PropertyData<QString>(39, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'playerText'
        QtMocHelpers::PropertyData<QString>(40, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'mainPileText'
        QtMocHelpers::PropertyData<QString>(41, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'discard1Text'
        QtMocHelpers::PropertyData<QString>(42, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'discard2Text'
        QtMocHelpers::PropertyData<QString>(43, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'handCardNames'
        QtMocHelpers::PropertyData<QStringList>(44, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'handSelected'
        QtMocHelpers::PropertyData<QVariantList>(45, 0x80000000 | 46, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'selectedHandCount'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'collectCardLines'
        QtMocHelpers::PropertyData<QStringList>(48, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'buffCardNames'
        QtMocHelpers::PropertyData<QStringList>(49, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'playedPairCardNames'
        QtMocHelpers::PropertyData<QStringList>(50, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'allInPending'
        QtMocHelpers::PropertyData<bool>(51, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'drawDiscard1Enabled'
        QtMocHelpers::PropertyData<bool>(52, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'drawDiscard2Enabled'
        QtMocHelpers::PropertyData<bool>(53, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'roundEndVisible'
        QtMocHelpers::PropertyData<bool>(54, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'roundEndHighlight'
        QtMocHelpers::PropertyData<bool>(55, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'windowTitleText'
        QtMocHelpers::PropertyData<QString>(56, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'gameActive'
        QtMocHelpers::PropertyData<bool>(57, QMetaType::Bool, QMC::DefaultPropertyFlags, 3),
        // property 'singlePlayerMode'
        QtMocHelpers::PropertyData<bool>(58, QMetaType::Bool, QMC::DefaultPropertyFlags, 4),
        // property 'computerTurn'
        QtMocHelpers::PropertyData<bool>(59, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'pendingDrawActive'
        QtMocHelpers::PropertyData<bool>(60, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'pendingDrawCardNames'
        QtMocHelpers::PropertyData<QStringList>(61, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'pendingKeepIndex'
        QtMocHelpers::PropertyData<int>(62, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'toastText'
        QtMocHelpers::PropertyData<QString>(63, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'toastVisible'
        QtMocHelpers::PropertyData<bool>(64, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SeaSaltGameController, qt_meta_tag_ZN21SeaSaltGameControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SeaSaltGameController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21SeaSaltGameControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21SeaSaltGameControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21SeaSaltGameControllerE_t>.metaTypes,
    nullptr
} };

void SeaSaltGameController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SeaSaltGameController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->titleChanged(); break;
        case 2: _t->requestQuit(); break;
        case 3: _t->gameActiveChanged(); break;
        case 4: _t->singlePlayerModeChanged(); break;
        case 5: _t->drawAnimationRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->discardAnimationRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->pendingDrawAnimationRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 8: _t->zoneAnimationRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 9: _t->onTurnTimeOut(); break;
        case 10: _t->startGame((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->returnToMenu(); break;
        case 12: _t->drawMain(); break;
        case 13: _t->selectPendingKeep((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->finishPendingMainDraw((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->drawDiscard1(); break;
        case 16: _t->drawDiscard2(); break;
        case 17: _t->play(); break;
        case 18: _t->endTurn(); break;
        case 19: _t->endRound(); break;
        case 20: _t->toggleHandCard((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->moveHandCard((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 22: _t->showScoreDetail(); break;
        case 23: _t->dismissToast(); break;
        case 24: _t->quitApp(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)()>(_a, &SeaSaltGameController::stateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)()>(_a, &SeaSaltGameController::titleChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)()>(_a, &SeaSaltGameController::requestQuit, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)()>(_a, &SeaSaltGameController::gameActiveChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)()>(_a, &SeaSaltGameController::singlePlayerModeChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)(QString , QString )>(_a, &SeaSaltGameController::drawAnimationRequested, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)(QString , QString )>(_a, &SeaSaltGameController::discardAnimationRequested, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)(QString , QString )>(_a, &SeaSaltGameController::pendingDrawAnimationRequested, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (SeaSaltGameController::*)(QString , QString , QString )>(_a, &SeaSaltGameController::zoneAnimationRequested, 8))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->scoreText(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->playerText(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->mainPileText(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->discard1Text(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->discard2Text(); break;
        case 5: *reinterpret_cast<QStringList*>(_v) = _t->handCardNames(); break;
        case 6: *reinterpret_cast<QVariantList*>(_v) = _t->handSelected(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->selectedHandCount(); break;
        case 8: *reinterpret_cast<QStringList*>(_v) = _t->collectCardLines(); break;
        case 9: *reinterpret_cast<QStringList*>(_v) = _t->buffCardNames(); break;
        case 10: *reinterpret_cast<QStringList*>(_v) = _t->playedPairCardNames(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->allInPending(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->drawDiscard1Enabled(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->drawDiscard2Enabled(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->roundEndVisible(); break;
        case 15: *reinterpret_cast<bool*>(_v) = _t->roundEndHighlight(); break;
        case 16: *reinterpret_cast<QString*>(_v) = _t->windowTitleText(); break;
        case 17: *reinterpret_cast<bool*>(_v) = _t->gameActive(); break;
        case 18: *reinterpret_cast<bool*>(_v) = _t->singlePlayerMode(); break;
        case 19: *reinterpret_cast<bool*>(_v) = _t->computerTurn(); break;
        case 20: *reinterpret_cast<bool*>(_v) = _t->pendingDrawActive(); break;
        case 21: *reinterpret_cast<QStringList*>(_v) = _t->pendingDrawCardNames(); break;
        case 22: *reinterpret_cast<int*>(_v) = _t->pendingKeepIndex(); break;
        case 23: *reinterpret_cast<QString*>(_v) = _t->toastText(); break;
        case 24: *reinterpret_cast<bool*>(_v) = _t->toastVisible(); break;
        default: break;
        }
    }
}

const QMetaObject *SeaSaltGameController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeaSaltGameController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21SeaSaltGameControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SeaSaltGameController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void SeaSaltGameController::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SeaSaltGameController::titleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SeaSaltGameController::requestQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SeaSaltGameController::gameActiveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SeaSaltGameController::singlePlayerModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SeaSaltGameController::drawAnimationRequested(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void SeaSaltGameController::discardAnimationRequested(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void SeaSaltGameController::pendingDrawAnimationRequested(QString _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void SeaSaltGameController::zoneAnimationRequested(QString _t1, QString _t2, QString _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
