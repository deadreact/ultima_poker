#pragma once

#include "basewidgets_global.h"
#include <qabstractitemmodel.h>
#include <QtGui/qicon.h>
#include <tools/types_fwd.hpp>

typedef QByteArrayList HashList;
typedef QByteArrayList Data64List;

typedef QSet<int> QIndexSet;




class BASEWIDGETS_DLL E {
    Q_GADGET
    Q_ENUMS(Blending)
    Q_ENUMS(ButtonState)
    Q_ENUMS(ForceCorrectState CorrectInputClassType)
public:

    enum class VipStatus : short
    {
        Beginner = 0,
        SemiPro,
        Professional,
        Elite,
        Master,
        Legend,

        count
    };
    Q_ENUM(VipStatus)

    enum Blending
    {
        Normal=0,
        Add,
        Multiply,
        Overlay,
        Screen,
        Darken,
        Lighten,
        ColorDodge,
        ColorBurn,
        HardLight,
        SoftLight,
        Difference,
        Exclusion,
    };
    Q_ENUM(Blending)
    enum ButtonState : gbp_u8
    {
        ENABLED         = 0x00,
        DISABLED        = 0x01,
        HOVER           = 0x02,
        PRESSED         = 0x04,
        CHECKED         = 0x08
    };
    Q_DECLARE_FLAGS(ButtonFlag, ButtonState)
    Q_FLAG(ButtonFlag)

    enum ForceCorrectState
    {
        None,
        Correct,
        Incorrect
    };
    Q_ENUM(ForceCorrectState)

    enum class Placeholder
    {
        Avatar
    };

    enum Currency
    {
        Bux = 0,
        Points,
        Bonus
    };
    Q_ENUM(Currency)

    enum ParentPainting {
        IgnoreParentPainting = 0,
        PaintParentBefore,
        PaintParentAfter
    };
    Q_ENUM(ParentPainting)

};
Q_DECLARE_OPERATORS_FOR_FLAGS(E::ButtonFlag)

class BASEWIDGETS_DLL CountryInfo {
    Q_GADGET
public:
    CountryInfo(): id(-1), name(""), code(""), icon() {}
    CountryInfo(int id, QString name, QString code): id(id), name(name), code(code), icon(QString(":textures/Common/icons/ico/%0.ico").arg(name)) {}
    int id;
    QString name;
    QString code;
    QString icon;
    QString toString() const {
        return QString("%0 Country(%1), phone code: +%2)").arg(id).arg(name, code);
    }
};
Q_DECLARE_METATYPE(CountryInfo)


class BASEWIDGETS_DLL UserType {
    Q_GADGET
    Q_ENUMS(ERoles)
    Q_FLAGS(EFilters)
public:
    enum ERoles : uint//typename std::underlying_type<Qt::ItemDataRole>::type
    {
        RowItemIdRole = 0x0101/*Qt::UserRole*/,   // gbp_i64
        InternalIdRole,                 // gbp_i64, QByteArray
        SortRole,                       // number or string
        FilterRole,                     // EFilters
        RowDataRole,                    // cash_stat_t, tournament_stat_t
        OriginDataRole,                 // origin types
        VisibilityRole,                 // bool

        SvgDataRole,                    // QByteArray
        PathRole,

        DateTimeRole,
        PositionRole,
        HashRole,

        UserRolesCount
    };
    Q_ENUM(ERoles)

    enum /*class*/ EFilters : uint
    {
        AllUnchecked = 0,
        Page_Cash               = 0x00000001,
        Page_Tourn              = 0x00000002,
        Page_SitNGo             = 0x00000004,
        User_DoesNotParticipate = 0x00000008,

        GameType_Holdem         = 0x00000010,
        GameType_Omaha          = 0x00000020,
        BuyIn_FreeRoll          = 0x00000040,
        BuyIn_Micro             = 0x00000080,
        BuyIn_Low               = 0x00000100,
        BuyIn_Medium            = 0x00000200,
        BuyIn_High              = 0x00000400,
        Speed_Normal            = 0x00000800,
        Speed_Turbo             = 0x00001000,

        Type_Regular            = 0x00002000,
        Type_Rebuy              = 0x00004000,
        Type_Knockout           = 0x00008000,
        Type_Qualifier          = 0x00010000,
        Type_Private            = 0x00020000,
        // для турниров
        Status_Announced        = 0x00040000 | Page_Tourn,
        Status_Registration     = 0x00080000 | Page_Tourn,
        Status_LateRegistration = 0x00100000 | Page_Tourn,
        Status_Started          = 0x00200000 | Page_Tourn,
        Status_Finished         = 0x00400000 | Page_Tourn,
        // для снг
        SNGSize_Pvp             = 0x00040000 | Page_SitNGo,
        SNGSize_Single          = 0x00080000 | Page_SitNGo,
        SNGSize_Multiple        = 0x00100000 | Page_SitNGo,

        Limit_NoLimit           = 0x00800000,
        Limit_PotLimit          = 0x01000000,
        Limit_FixedLimit        = 0x02000000,

        SeatsCount_2            = 0x04000000,
        SeatsCount_6            = 0x08000000,
        SeatsCount_9            = 0x10000000,

        FreeSeats_Full          = 0x20000000,
        FreeSeats_Empty         = 0x40000000,

        Indicator               = 0x80000000,


        GameType_Mask   = GameType_Holdem | GameType_Omaha,
        BuyIn_Mask      = BuyIn_FreeRoll | BuyIn_Micro | BuyIn_Low | BuyIn_Medium | BuyIn_High,
        Speed_Mask      = Speed_Normal | Speed_Turbo,
        Type_Mask       = Type_Rebuy | Type_Regular | Type_Knockout | Type_Qualifier | Type_Private,
        Status_Mask     = Status_Announced | Status_Registration | Status_LateRegistration | Status_Started | Status_Finished,
        SNGSize_Mask	= SNGSize_Pvp | SNGSize_Single | SNGSize_Multiple,
        Limit_Mask      = Limit_FixedLimit | Limit_PotLimit | Limit_NoLimit,
        SeatsCount_Mask = SeatsCount_2 | SeatsCount_6 | SeatsCount_9,
        FreeSeats_Mask  = FreeSeats_Full | FreeSeats_Empty,

        Holdem_FixedLimit = GameType_Holdem | Limit_FixedLimit,
        Holdem_NoLimit    = GameType_Holdem | Limit_NoLimit,
        Omaha_PotLimit    = GameType_Omaha | Limit_PotLimit,

        CashFilters_Mask  = Page_Cash | GameType_Mask | (BuyIn_Mask ^ BuyIn_FreeRoll) | Limit_Mask | SeatsCount_Mask | FreeSeats_Mask,

        TournCommon_Mask = GameType_Mask | Limit_Mask | BuyIn_Mask | Speed_Mask | Type_Mask | User_DoesNotParticipate,
		TournFilters_Mask  = Page_Tourn | TournCommon_Mask | Status_Mask,
		SitNGoFilters_Mask = Page_SitNGo | TournCommon_Mask | SNGSize_Mask,

        AllChecked    = 0xffffffff
    };

//    Q_ENUM(EFilters)
    Q_DECLARE_FLAGS(FilterFlags, EFilters)
    Q_FLAG(FilterFlags)

    static FilterFlags convert(const QString& str)
    {
        bool ok = false;
        int v = str.toUInt(&ok, 2);
        return FilterFlags(ok ? (v ^ /*underlying_cast*/uint(EFilters::Indicator)) : 0);
    }
    static QString convert(FilterFlags filter)
    {
        return QString::number(filter | /*underlying_cast*/uint(EFilters::Indicator), 2);
    }

    static QRegExp toRegExp(FilterFlags filter)
    {
        QString str = convert(filter);
        QRegExp re(str.replace("1", "[01]"));
        return re;
    }

};
Q_DECLARE_OPERATORS_FOR_FLAGS(UserType::FilterFlags)

QString
#ifdef Q_OS_WIN
BASEWIDGETS_DLL
#endif
roleToString(int role);
