#include "itemtype_concept.hpp"
#include <money.h>
#include <functional>
#include "api_util.hpp"
#include <QtGui/QPixmap>

#ifndef Q_MOC_RUN
#include <api/api_common.h>
#endif // !Q_MOC_RUN

#include <qfileinfo.h>
#include <qicon.h>
#include <QtWidgets/qapplication.h>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(ns_meta::birthdate_type::value_type)

namespace
{
    QString cardToString(gbp_i8 cardIndex) {
        if (cardIndex < 0 || cardIndex > 51) {
            return "▄▀";
        }
        QString suit = "?";
        switch (cardIndex / 13) {
        case 0: suit = "♣"; break;
        case 1: suit = "♠"; break;
        case 2: suit = "♦"; break;
        case 3: suit = "♥"; break;
        default:
            break;
        }
        return QString("%0%1").arg(suit).arg(std::vector<const char*>({"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"})[cardIndex % 13]);
    }

    template <typename StringType>
    StringType capitalized(const StringType& source) {
        StringType target(source);
        target[0] = target[0].toUpper();
        return target;
    }

    template <typename TContainer>
    QStringList toStringList(const TContainer& container) {
        QStringList result;
        for (const typename TContainer::value_type& value: container) {
            result << QString("%0").arg(value);
        }
        return result;
    }

    template <typename TContainer> using on_convert = std::function<QString(typename TContainer::value_type)>;

    template <typename TContainer, typename Fn>
    QStringList toStringList(const TContainer& container, Fn conv_func) {
        QStringList result;
        for (const typename TContainer::value_type& value: container) {
            result << conv_func(value);
        }
        return result;
    }


} //namespace

namespace ns_meta
{
    money_type::value_type money_type::convert(const edit_type &value) {
        return 100 * value;
    }

    money_type::value_type money_type::convert(const QVariant &value)
    {
        switch (value.userType()) {
        case QMetaType::Double:
                return value.toDouble() * 100;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Short:
        case QMetaType::UShort:
        case QMetaType::Char:
        case QMetaType::UChar:
        case QMetaType::SChar:
            return value.toLongLong();
        case QMetaType::QString:
            return Money::fromString(value.toString()).amount();
        default:
            return 0;
        }
    }

    money_type::edit_type money_type::edit(const value_type &value) {
        return edit_type(value) / 100;
    }

    money_type::display_type money_type::display(const value_type &value) {
        return Money(value).toString();
    }

    bool money_type::modify(value_type &source, const QVariant &value)
    {
        switch (value.userType()) {
        case QMetaType::Double:
            source = value.toDouble() * 100;
            return true;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Short:
        case QMetaType::UShort:
        case QMetaType::Char:
        case QMetaType::UChar:
        case QMetaType::SChar:
            source = value.toLongLong();
            return true;
        case QMetaType::QString:
            source = Money::fromString(value.toString()).amount();
            return true;
        default:
            return false;
        }
    }

    id_type::value_type id_type::convert(const edit_type &value) {
        return value;
    }

    id_type::edit_type id_type::edit(const value_type &value) {
        return value;
    }

    id_type::display_type id_type::display(const value_type &value) {
        return QString::number(value);
    }

    bool id_type::modify(id_type::value_type &target, const QVariant &value)
    {
        if (value.canConvert<value_type>()) {
            target = qvariant_cast<value_type>(value);
            return true;
        } else if (value.canConvert<edit_type>()) {
            target = convert(qvariant_cast<edit_type>(value));
            return true;
        }
        return false;
    }

    duration_type::value_type duration_type::convert(const edit_type &value) {
        return value;
    }

    duration_type::edit_type duration_type::edit(const value_type &value) {
        return value;
    }

    duration_type::display_type duration_type::display(const value_type &value) {
        return QString::number(value) + "sec";
    }

    bool duration_type::modify(value_type &target, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            target = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            target = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }

    datetime_type::value_type datetime_type::convert(const edit_type &value) {
        return value.toTime_t();
    }

    datetime_type::edit_type datetime_type::edit(const value_type &value) {
        return edit_type::fromTime_t(value);
    }

    datetime_type::display_type datetime_type::display(const value_type &value) {
        return display_type::fromTime_t(value);
    }

    pos_type::value_type pos_type::convert(const edit_type &value) {
        return value;
    }

    pos_type::edit_type pos_type::edit(const value_type &value) {
        return value;
    }

    pos_type::display_type pos_type::display(const value_type &value) {
        return value;
    }

    bool pos_type::modify(value_type &target, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            target = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            target = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }

    card_type::value_type card_type::convert(const edit_type &value) {
        return qBound(edit_type(-1), value, edit_type(51));
    }

    card_type::edit_type card_type::edit(const value_type &value) {
        return value;
    }

    card_type::display_type card_type::display(const value_type &value) {
        return cardToString(value);
    }

    bool card_type::modify(value_type &target, const QVariant &value)
    {
        if (value.canConvert<value_type>()) {
            value_type convertedValue = qvariant_cast<value_type>(value);
            if (convertedValue >= -1 && convertedValue < 52) {
                target = convertedValue;
                return true;
            }
        }
        return false;
    }

    name_type::value_type name_type::convert(const edit_type &value)    { return value.toStdString(); }
    name_type::edit_type name_type::edit(const value_type &value)       { return edit_type::fromStdString(value); }
    name_type::display_type name_type::display(const value_type &value) { return capitalized(edit_type::fromStdString(value)); }
    bool name_type::modify(value_type &source, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            source = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            source = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }

    text_type::value_type text_type::convert(const edit_type &value)    { return value.toStdString(); }
    text_type::edit_type text_type::edit(const value_type &value)       { return edit_type::fromStdString(value); }
    text_type::display_type text_type::display(const value_type &value) { return edit_type::fromStdString(value); }
    bool text_type::modify(value_type &source, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            source = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            source = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }

    key_type::value_type key_type::convert(const edit_type &value)    { return value.toStdString(); }
    key_type::edit_type key_type::edit(const value_type &value)       { return edit_type::fromStdString(value); }
    key_type::display_type key_type::display(const value_type &value) { return edit_type::fromStdString(value); }

    cards_type::value_type cards_type::convert(const edit_type &value) {
        return value.value<value_type>();
    }
    cards_type::edit_type cards_type::edit(const value_type &value) {
        return edit_type::fromValue(value);
    }
    cards_type::display_type cards_type::display(const value_type &value) {
        return toStringList(value, cardToString).join(" ");
    }

    flag_type::value_type flag_type::convert(const edit_type &value) {
        return value;
    }
    flag_type::edit_type flag_type::edit(const value_type &value) {
        return value ? Qt::Checked : Qt::Unchecked;
    }
    flag_type::display_type flag_type::display(const value_type &value) {
        return value ? "true" : "false";
    }

    country_type::value_type country_type::convert(const edit_type &value) {
        return underlying_cast(value);
    }
    country_type::edit_type country_type::edit(const value_type &value) {
        return QLocale::Country(value);
    }
    country_type::display_type country_type::display(const value_type &value) {
        return QLocale::countryToString(QLocale::Country(value));
    }

    country_type::decoration_type country_type::decorate(const value_type &value) {
        QString path = QString(":/ico/%0.ico").arg(display(value));
        if (QFileInfo(path).exists()) {
            return QIcon(path);
        }
        static const QIcon unknown(":/ico/Unknown.ico");
        return unknown;
    }

    bool country_type::modify(value_type &source, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            source = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            source = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }

    img_type::value_type img_type::convert(const edit_type &value) {
        return value.toStdString();
    }

    img_type::edit_type img_type::edit(const value_type &value) {
        return edit_type::fromStdString(value);
    }

    img_type::display_type img_type::display(const value_type &value) {
        return display_type::fromImage(QImage::fromData(QByteArray::fromBase64(QByteArray::fromStdString(value))));
    }

    bool img_type::modify(value_type &source, const QVariant &value)
    {
        // TODO: fixme
        if (value.canConvert<edit_type>()) {
            source = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            source = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }


    birthdate_type::value_type birthdate_type::convert(const edit_type &value) {
        value_type result_value;
        result_value._year = value.year();
        result_value._month = value.month();
        result_value._day = value.day();
        return result_value;
    }

    birthdate_type::edit_type birthdate_type::edit(const birthdate_type::value_type &value) {
        return display(value);
    }

    birthdate_type::display_type birthdate_type::display(const birthdate_type::value_type &value)  {
        QDate result_value;
        result_value.setDate(value._year, value._month, value._day);
        return result_value;
    }

    bool birthdate_type::modify(birthdate_type::value_type &source, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            source = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            source = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }
    //    template <>
    //    QDate birthdate_type<gbp::ns_api::birth_date>::display(const gbp::ns_api::birth_date &value)

    path_type::value_type path_type::convert(const edit_type &value) {
        return value.toStdString();
    }

    path_type::edit_type path_type::edit(const value_type &value) {
        return value.c_str();
    }

    path_type::display_type path_type::display(const value_type &value) {
        return value.c_str();
    }

    bounded_count_type::value_type bounded_count_type::convert(const edit_type &value) {
        QPair<int, int> val = value.value<QPair<int, int>>();
        return std::make_pair(qMin(val.first, val.second), val.second);
    }

    bounded_count_type::edit_type bounded_count_type::edit(const value_type &value) {
        return QVariant::fromValue(qMakePair(qMin(value.first, value.second), value.second));
    }

    bounded_count_type::display_type bounded_count_type::display(const value_type &value) {
        return QString("%0/%1").arg(value.first).arg(value.second);
    }

    standard_icon_type::value_type standard_icon_type::convert(const standard_icon_type::edit_type &value) {
        return value;
    }

    standard_icon_type::edit_type standard_icon_type::edit(const standard_icon_type::value_type &value) {
        return value;
    }

    standard_icon_type::display_type standard_icon_type::display(const standard_icon_type::value_type &value) {
        return ns_util::enumToString(value);
    }

    standard_icon_type::decoration_type standard_icon_type::decorate(const value_type &value) {
        return QApplication::style()->standardIcon(value);
    }

    language_type::value_type language_type::convert(const language_type::edit_type &value) {
        return QLocale(value).name().toStdString();
    }

    language_type::edit_type language_type::edit(const language_type::value_type &value) {
        return QLocale(value.c_str()).language();
    }

    language_type::display_type language_type::display(const language_type::value_type &value) {
        return QLocale(value.c_str()).nativeLanguageName();
    }

    language_type::decoration_type language_type::decorate(const language_type::value_type &value) {
        return country_type::decorate(QLocale(value.c_str()).country());
    }

    QVariant language_type::display(const QVariant &value) { return value; }

    bool language_type::modify(value_type &source, const QVariant &value)
    {
        if (value.canConvert<edit_type>()) {
            source = convert(qvariant_cast<edit_type>(value));
            return true;
        } else if (value.canConvert<value_type>()) {
            source = qvariant_cast<value_type>(value);
            return true;
        }
        return false;
    }

} //namespace ns_meta
