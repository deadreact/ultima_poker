#pragma once
#include "api_models_global.h"
#include <qdatetime.h>
#include "defs.hpp"
#include <qlocale.h>
#include <qvariant.h>
#include <QMetaEnum>
#include <sstream>
#include <functional>
#include "api_util.hpp"
#include <api/gbp_int.hpp>
#include <QtWidgets/QStyle>

class concept
{
    Q_GADGET
public:
    enum type : gbp_u32
    {
          birthdate       = 0x00000001 
        , boolean         = 0x00000002 
        , card            = 0x00000004 
        , count           = 0x00000008 
        , country         = 0x00000010 
        , datetime        = 0x00000020 
        , duration        = 0x00000040 
        , enumerator      = 0x00000080 
        , flag            = 0x00000100 
        , game_limit      = 0x00000200 
        , game_speed      = 0x00000400 
        , game_type       = 0x00000800 
        , id              = 0x00001000 
        , image           = 0x00002000 
        , key             = 0x00004000 
        , language        = 0x00008000 
        , money           = 0x00010000 
        , name            = 0x00020000 
        , path            = 0x00040000 
        , persent         = 0x00080000 
        , phone           = 0x00100000 
        , position        = 0x00200000 
        , richtext        = 0x00400000 
        , sequence        = 0x00800000 
        , text            = 0x01000000 
        , time_utc        = 0x02000000 
        , tribool         = 0x04000000 
        , unique          = 0x08000000 
        , unique_sequence = unique | sequence
        , cards           = card | unique_sequence
    };
    Q_FLAG(type)
    Q_DECLARE_FLAGS(types, type)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(concept::types)

namespace ns_meta
{
    template <typename T>
    bool modify_common(T& target, const QVariant& value) {
        if (value.canConvert<T>()) {
            target = qvariant_cast<T>(value);
            return true;
        }
        return false;
    }

    template <concept::type type>
    struct typed_item
    {
        template <typename T>
        static QVariant data(const T& source, int role);
    };

    template <>
    struct typed_item<concept::id>
    {
        static QVariant data(const gbp_i64& source, int role) {
            switch (role) {
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
                return QString::number(source);
            default:
                return QVariant();
            }
        }
    };


    template <typename valuetype, typename edittype, typename displaytype
        , typename on_convert = std::function<valuetype(*)(const edittype&)>
        , typename on_edit    = std::function<edittype(*)(const valuetype&)>
        , typename on_display = std::function<displaytype(*)(const valuetype&)>>
      struct generic_type
      {
          using value_type = valuetype;
          using edit_type = edittype;
          using display_type = displaytype;
          static value_type convert(const edit_type& value);
          static edit_type edit(const value_type& value);
          static display_type display(const value_type& value);
      };

    struct API_MODEL_DLL money_type {
        using value_type   = gbp_i64;
        using edit_type    = qreal;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static value_type convert(const QVariant& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& source, const QVariant& value);
    };

    template <typename integral_type>

    struct generic_count_type {
        using value_type   = typename std::enable_if<std::is_integral<integral_type>::value, integral_type>::type;
        using edit_type    = value_type;
        using display_type = value_type;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value)    { return value; }
        static edit_type edit(const value_type& value)       { return value; }
        static display_type display(const value_type& value) { return value; }

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& target, const QVariant& value) {
            if (value.canConvert<value_type>()) {
                target = qvariant_cast<value_type>(value);
                return true;
            }
            return false;
        }
    };

    struct API_MODEL_DLL count_type : generic_count_type<int> {};

    template <typename concept1, typename concept2, char sep = '/'>
    struct generic_pair_value {
        using value_type = std::pair<typename concept1::value_type, typename concept2::value_type>;
        using edit_type = QPair<typename concept1::edit_type, typename concept2::edit_type>;
        using display_type = QString;

        static value_type convert(const edit_type& value) {
            return std::make_pair(concept1::convert(value.first), concept2::convert(value.second));
        }
        static edit_type edit(const value_type& value) {
            return qMakePair(concept1::edit(value.first), concept2::edit(value.second));
        }

        static display_type display(const value_type& value) {
            return QString("%0%1%2").arg(concept1::display(value.first)).arg(sep).arg(concept2::display(value.second));
        }
    };

    struct API_MODEL_DLL bounded_count_type {
        using value_type   = std::pair<int, int>;
        using edit_type    = QVariant; // QPair<int, int>
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
    };

    struct API_MODEL_DLL id_type {
        using value_type   = gbp_i64;
        using edit_type    = gbp_i64;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& target, const QVariant& value);
    };

    struct API_MODEL_DLL persent_type {
        using value_type   = gbp_u8;
        using edit_type    = gbp_u8;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value) { return qBound(edit_type(0), value, edit_type(100)); }
        static edit_type edit(const value_type& value) { return qBound(value_type(0), value, value_type(100)); }
        static display_type display(const value_type& value) { return QString("%0%").arg(value); }

        static QVariant display(const QVariant& value) { return value; }
    };

    struct API_MODEL_DLL duration_type {
        using value_type   = gbp_i64;
        using edit_type    = gbp_i64;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& target, const QVariant& value);
    };

    struct API_MODEL_DLL datetime_type {
        using value_type   = gbp_i64;
        using edit_type    = QDateTime;
        using display_type = QDateTime;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
    };

    struct birthdate_type {
        using value_type   = gbp::ns_api::birth_date;
        using edit_type    = QDate;
        using display_type = QDate;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& source, const QVariant& value);
    };

    struct API_MODEL_DLL pos_type {
        using value_type   = gbp_i8;
        using edit_type    = qint16;
        using display_type = qint16;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& target, const QVariant& value);
    };

    struct API_MODEL_DLL card_type {
        using value_type   = gbp_i8;
        using edit_type    = gbp_i8;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

//        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& target, const QVariant& value);
    };

    struct API_MODEL_DLL cards_type {
        using value_type   = std::vector<gbp_i8>;
        using edit_type    = QVariant;
        using display_type = QString;

        constexpr static const bool is_container = true;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

//        static QVariant display(const QVariant& value) { return value; }
    };

    struct API_MODEL_DLL text_type {
        using value_type   = std::string;
        using edit_type    = QString;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& source, const QVariant& value);
    };

    struct API_MODEL_DLL name_type {
        using value_type   = std::string;
        using edit_type    = QString;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& source, const QVariant& value);
    };

    struct API_MODEL_DLL country_type {
        using value_type      = gbp_u16;
        using edit_type       = QLocale::Country;
        using display_type    = QString;
        using decoration_type = QIcon;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);
        static decoration_type decorate(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& source, const QVariant& value);
    };

    struct API_MODEL_DLL language_type {
        using value_type      = std::string;
        using edit_type       = QLocale::Language;
        using display_type    = QString;
        using decoration_type = QIcon;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);
        static decoration_type decorate(const value_type& value);

        static QVariant display(const QVariant& value);
        static bool modify(value_type& source, const QVariant& value);
    };

    struct API_MODEL_DLL standard_icon_type {
        using value_type      = QStyle::StandardPixmap;
        using edit_type       = value_type;
        using display_type    = QString;
        using decoration_type = QIcon;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);
        static decoration_type decorate(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
    };

    struct API_MODEL_DLL path_type {
        using value_type   = std::string;
        using edit_type    = QString;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
    };

    struct API_MODEL_DLL img_type {
        using value_type   = std::string;
        using edit_type    = QByteArray;
        using display_type = QPixmap;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& source, const QVariant& value);
    };

    struct API_MODEL_DLL key_type {
        using value_type   = std::string;
        using edit_type    = QByteArray;
        using display_type = QByteArray;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
    };

    template <typename valtype, typename edittype = valtype, typename displaytype = QString>
    struct generic_flag_type {
        using value_type = typename std::enable_if<std::is_integral<valtype>::value, valtype>::type;
        using edit_type = edittype;
        using display_type = displaytype;
    };



    struct API_MODEL_DLL flag_type {
        using value_type   = bool;
        using edit_type    = Qt::CheckState;
        using display_type = QString;

        constexpr static const bool is_container = false;

        static value_type convert(const edit_type& value);
        static edit_type edit(const value_type& value);
        static display_type display(const value_type& value);

        static QVariant display(const QVariant& value) { return value; }
        static bool modify(value_type& target, const QVariant& value) {
            if (value.canConvert<value_type>()) {
                target = qvariant_cast<value_type>(value);
                return true;
            }
            return false;
        }
    };

    template <typename ContT>
    struct generic_container_type {
        using value_type = ContT;
        using element_type = typename ContT::value_type;
        using edit_type    = QVariant;
        using display_type = QVariant;

        constexpr static const bool is_container = true;

        static value_type convert(const edit_type& value) { return value.value<value_type>();}
        static edit_type edit(const value_type& value) {return QVariant::fromValue(value); }
        static display_type display(const value_type& value) {return QVariant::fromValue(value); }
    };

    struct container_type {
        template <typename ContT>
        static typename generic_container_type<ContT>::display_type convert(const typename generic_container_type<ContT>::value_type& value) {
            return generic_container_type<ContT>::convert(value);
        }
        template <typename ContT>
        static typename generic_container_type<ContT>::display_type edit(const typename generic_container_type<ContT>::value_type& value) {
            return generic_container_type<ContT>::edit(value);
        }
        template <typename ContT>
        static typename generic_container_type<ContT>::display_type display(const typename generic_container_type<ContT>::value_type& value) {
            return generic_container_type<ContT>::display(value);
        }
    };

    template <typename valtype, bool useQtMetaEnum = QtPrivate::IsQEnumHelper<valtype>::Value>
    struct generic_enum_type {
        using value_type   = typename std::enable_if<std::is_enum<valtype>::value, valtype>::type;
        using edit_type    = typename std::underlying_type<value_type>::type;
        using display_type = QString;

        constexpr static const bool is_container = false;
        constexpr static const char* type_name() { return QMetaEnum::fromType<value_type>().name(); }

        static value_type convert(const edit_type& value) { return value_type(value); }
        static edit_type edit(const value_type& value)    { return underlying_cast(value); }

        static display_type display(const value_type &value) {
            return ns_util::enumToString(value);/*QMetaEnum::fromType<value_type>().valueToKey(static_cast<edit_type>(value));*/
        }
    };

    template <typename valtype>
    struct generic_enum_type<valtype, false>
    {
        using value_type   = typename std::enable_if<std::is_enum<valtype>::value, valtype>::type;
        using edit_type    = typename std::underlying_type<value_type>::type;
        using display_type = QString;

        constexpr static const bool is_container = false;
        static const char* type_name() {
            std::ostringstream os;
            os << value_type(0);
            return os.str().c_str();
        }

        static value_type convert(const edit_type& value) { return value_type(value); }
        static edit_type edit(const value_type& value)    { return underlying_cast(value); }

        static display_type display(const value_type &value) {
            std::ostringstream os;
            os << value;
            return os.str().c_str();
        }
    };

    struct enum_type {
        template <typename enum_type>
        static typename generic_enum_type<enum_type>::display_type display(enum_type value) {
            return generic_enum_type<enum_type>::display(value);
        }
        template <typename enum_type>
        static typename generic_enum_type<enum_type>::edit_type edit(enum_type value) {
            return generic_enum_type<enum_type>::edit(value);
        }
        static QVariant display(const QVariant& value) { return value; }
    };
    struct tr_enum_type {
        template <typename enum_type>
        static typename generic_enum_type<enum_type>::display_type display(enum_type value) {
            return ns_util::toString(value);
        }
        template <typename enum_type>
        static typename generic_enum_type<enum_type>::edit_type edit(enum_type value) {
            return generic_enum_type<enum_type>::edit(value);
        }
        static QVariant display(const QVariant& value) { return value; }
    };


    template <typename xyz_type, typename tp = typename xyz_type::edit_type>
    typename xyz_type::value_type convert(const QVariant& value) {
        return xyz_type::convert(value.value<tp>());
    }

    template <typename xyz_type>
    bool editModelData(typename xyz_type::value_type& target, const QVariant& value) {
        if (!value.canConvert<typename xyz_type::edit_type>()) {
            return false;
        }
        target = xyz_type::convert(value.value<typename xyz_type::edit_type>());
        return true;
    }

//          {
//              using on_convert = on_convert_t;
//              using on_edit = on_edit_t;
//              using on_display = on_display_t;
//            on_convert convert;// = [](const edit_type& value)  { return static_cast<value_type>(value); };
//            on_edit    edit;//    = [](const value_type& value) { return static_cast<edit_type>(value); };
//            on_display display;// = [](const value_type& value) { return static_cast<display_type>(value); };
//          };
//              , std::function<value_type(const edit_type&)> &convert
//              , std::function<edit_type(const value_type&)> &edit
//              , std::function<display_type(const value_type&)> &display>


//    {
//    };

    //    template <typename T, bool can_trivially_compare>
    //    struct comparator;

    //    template <typename T>
    //    struct comparator<T, true> {
    //        static bool isDifferent(const T& val1, const T& val2) {
//            return val1 != val2;
//        }
//    };

//    template <typename T>
//    struct comparator<T, false> {
//        static bool isDifferent(const T& val1, const T& val2) {
//            if (val1.size() != val2.size()) {
//                return true;
//            }
//            for (int i = 0; i < val1.size(); i++) {
//                if (comparator<T::value_type, false>::isDifferent(val1.at(i), val2.at(i))) {
//                    return true;
//                }
//            }
//            return false;
//        }
//    };

} //namespace ns_meta
