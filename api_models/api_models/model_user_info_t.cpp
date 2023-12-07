#include "model_user_info_t.hpp"
#include "visitors.hpp"
#ifndef Q_MOC_RUN
//    #include <rpc/network_client.h>
#endif // !Q_MOC_RUN

#include <QBuffer>
#include <QCryptographicHash>
#include <qdatetime.h>
#include "api_util.hpp"
#include "itemtype_concept.hpp"
#include "defs_metatype.hpp"
#include <qdebug.h>
#include <rpcbase/rpcbase_lobby.h>
#include <iostream>

Q_DECLARE_METATYPE(gbp::ns_api::birth_date)

//struct Comp {
//    QMap<QString, QPair<QVariant, QVariant>> m_diff;
//    template <typename T>
//    void operator()(const char* member, const T& first, const T& second) {
//        std::cout << member << " " << first << ", " << second << std::endl;
//    }
//};

#define FOR_EACH_USER_INFO_FIELD(F, ...) \
F(_user_id, __VA_ARGS__)\
F(_nickname, __VA_ARGS__)\
F(_hash_avatar, __VA_ARGS__)\
F(_email, __VA_ARGS__)\
F(_username, __VA_ARGS__)\
F(_last_name, __VA_ARGS__)\
F(_birth_date, __VA_ARGS__)\
F(_city, __VA_ARGS__)\
F(_mail_index, __VA_ARGS__)\
F(_mail_address, __VA_ARGS__)\
F(_country, __VA_ARGS__)\
F(_lang, __VA_ARGS__)\
F(_phone, __VA_ARGS__)\
F(_money, __VA_ARGS__)\
F(_in_game_money, __VA_ARGS__)\
F(_wait_out_money, __VA_ARGS__)\
F(_gender, __VA_ARGS__)\
F(_verifying, __VA_ARGS__)\
F(_vip_status, __VA_ARGS__)

namespace
{
    QIcon getIcon(QLocale::Country c) {
        return QIcon(QString(":/ico/%0.ico").arg(QLocale::countryToString(QLocale::Country(c))));
    }
    base64_t toBase64(const QPixmap &pix)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        pix.save(&buffer, "PNG");

        return byteArray.toBase64();
    }

//    using field = ns_model::user_info_t::field;
//    using data_type = ns_model::user_info_t::data_type;

//    template <field f1, field ...f2>
//    bool isDifferent(const data_type& item1, const data_type& item2) {
//        return isDifferent<f1>(item1, item2) || isDifferent<f2...>(item1, item2);
//    }

//    template <field f> struct next {
//        static const field value = field(underlying_cast(f)+1);
//        static const bool is_last = (underlying_cast(f) == underlying_cast(field::count) + 1);
//    };
//    template <> struct next<field::count> {};

//    template <field f>
//    bool diff_from(const data_type& item1, const data_type& item2) {
//        return isDifferent<f>(item1, item2) || diff_from<next<f>::value>(item1, item2);
//    }
//    template <>
//    bool diff_from<field::count>(const data_type&, const data_type&) {
//        return false;
//    }

//    template <field f>
//    void foreach_from(const std::function<void(data_type&, field)> &func, data_type& data) {
//        func(data, f);
//        foreach_from<next<f>::value>(func, data);
//    }
//    template <>
//    void foreach_from<field::count>(const std::function<void(data_type&, field)> &/*func*/, data_type& /*data*/) {}
}


//bool compare(const data_type& item1, const data_type& item2) {
//    return diff_from<field::begin>(item1, item2);
//}

//void for_each(data_type& data, const std::function<void(data_type&, field)> &func) {
//    foreach_from<field::begin>(func, data);
//}



//bool operator!=(const gbp::ns_api::birth_date& bd1, const gbp::ns_api::birth_date& bd2)
//{
//    return bd1._day != bd2._day || bd1._month != bd2._month || bd1._year != bd2._year;
//}

//template <field f> struct getTypeOf { using type = void;};
//template <field f, typename T = typename getTypeOf<f>::type>
//const T& getValueOf(const data_type& data);

//#define SPEC_ISDIFF(item_field, ...) template <> bool isDifferent<field::item_field>(const data_type& item1, const data_type& item2) { return item1.item_field != item2.item_field; }
//#define SPEC_TYPE(item_field, ...) template <> struct fieldInfo<field::item_field> { using type = decltype(gbp::ns_api::ns_lobby::user_info_t::item_field);};
//#define SPEC_GETTER(item_field, ...) template <> const decltype(data_type::item_field)& getValueOf<field::item_field>(const data_type& data) { return data.item_field; }
namespace {
//FOR_EACH_USER_INFO_FIELD(SPEC_ISDIFF)
}
//FOR_EACH_USER_INFO_FIELD(SPEC_TYPE)
//FOR_EACH_USER_INFO_FIELD(SPEC_GETTER)

    
//    template <field f1, field f2>
//    bool isAnyDifferent(const data_type& item1, const data_type& item2) {
//        return isAnyDifferent<f1>(item1, item2) && isDifferent<f2>(item1, item2);
//    }

//    template <field f1, field f2>
//    bool isDifferent(const data_type& item1, const data_type& item2) {
//        return isDifferent<f1>(item1, item2) || isDifferent<f2>(item1, item2);
//    }

//template <typename T>
//void fun(const char* name, const T& f) {
//    std::cout << name << std::endl;
//}


namespace ns_model
{
    user_info_t::user_info_t(QObject* parent)
        : QAbstractListModel(parent)
        , m_cash()
        , m_data(nullptr)
        , m_rpc(nullptr)
        , m_avatar()
    {
        qRegisterMetaType<gbp::ns_api::ns_lobby::user_info_t>("gbp::ns_api::ns_lobby::user_info_t");
        qRegisterMetaType<gbp::ns_api::ns_lobby::user_info_t>("user_info_t");
    }

    user_info_t::~user_info_t() {}

    void user_info_t::setRpcObject(ns_lobby::rpc_object *rpc)
    {
        m_rpc = rpc;
    }

    gbp_i64 user_info_t::userId() const {
        return m_data.isNull() ? -1 : m_data->_user_id;
    }

    void user_info_t::resetData(gbp::ns_api::ns_lobby::user_info_t *newData)
    {
        beginResetModel();
        resetInternalData();
        m_data.reset(newData);
        if (newData) {
            m_cash = *m_data.data();
        } else {
            m_cash = decltype(m_cash)();
            m_cash._user_id = -1;
        }
        m_avatar = /*(newData == nullptr) ? */QIcon()/* : QIcon(pApp->db().getAvatarPixmap(sha1_t::fromStdString(newData->_hash_avatar)))*/;

        endResetModel();
    }

    int user_info_t::rowCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return (m_data.isNull()) ? 1 : underlying_cast(field::count);
    }

    int user_info_t::columnCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return 1;
    }

    QVariant user_info_t::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.parent().isValid() || index.column() != 0) {
            return QVariant();
        }
        field f = field(index.row());
        switch (role)
        {
            case UserType::RowItemIdRole:
                return QVariant::fromValue((m_data.isNull()) ? -1 : m_data->_user_id);
            case UserType::RowDataRole:
                return (m_data.isNull()) ? QVariant() : QVariant::fromValue(*m_data.data());
            case UserType::OriginDataRole:
                return valueOf(f);
            case UserType::DateTimeRole:
                return field(index.row()) == field::_birth_date ? QDate(m_data->_birth_date._year, m_data->_birth_date._month, m_data->_birth_date._day) : QDate();
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
                return text(f);
            case Qt::DecorationRole:
            {
                QIcon ico = icon(f);
                return ico.isNull() ? QVariant() : QVariant(ico);
            }
            case Qt::EditRole:
            {
                if (flags(index) & Qt::ItemIsEditable) {
                    field f = field(index.row());
                    switch (f) {
                    case field::_hash_avatar:    return ns_meta::img_type::edit(m_data->_hash_avatar);
                    case field::_birth_date:     return ns_meta::birthdate_type::edit(m_data->_birth_date);
                    case field::_country:        return ns_meta::country_type::edit(m_data->_country);
                    case field::_lang:           return QLocale(m_data->_lang.c_str()).language();
                    case field::_phone:          return text(f);
                    case field::_money:
                    case field::_in_game_money:
                    case field::_wait_out_money:
                        return ns_meta::money_type::edit(money(f));
                    case field::_verifying:      return ns_meta::tr_enum_type::edit(m_data->_verifying);
                    case field::_vip_status:     return ns_meta::tr_enum_type::edit(m_data->_vip_status);
                    default:
                        return text(f);
                    }
                }
                return QVariant();
            }
            case Qt::CheckStateRole:
            {
                if (field(index.row()) != field::_verifying) {
                    return QVariant();
                }
                switch (m_data->_verifying) {
                case gbp::ns_api::e_verifying::verified: return Qt::Checked;
                case gbp::ns_api::e_verifying::in_process: return Qt::PartiallyChecked;
                case gbp::ns_api::e_verifying::not_verified: return Qt::Unchecked;
                default:
                    return QVariant();
                }
            }
        }

        return QVariant();
    }

    QModelIndex user_info_t::indexOf(field f) const {
        return index(underlying_cast(f), 0);
    }

    user_info_t::field user_info_t::fieldOf(const QModelIndex &idx) const
    {
        if (idx.isValid() && !idx.parent().isValid() && idx.model() == this && idx.column() == 0) {
            return field(idx.row());
        }
        return field(-1);
    }

    QString user_info_t::text(field f) const
    {
        if (m_data.isNull()) {
            return f == field::_user_id ? QString::number(-1) : QString();
        }
        switch (f)
        {
            case field::_user_id:           return QString::number(m_data->_user_id);
            case field::_nickname:          return QString::fromStdString(m_data->_nickname);
            case field::_hash_avatar:       return QString::fromStdString(m_data->_hash_avatar);
            case field::_email:             return QString::fromStdString(m_data->_email);
            case field::_username:          return QString::fromStdString(m_data->_username);
            case field::_last_name:         return QString::fromStdString(m_data->_last_name);
            case field::_birth_date:        return QDate(m_data->_birth_date._year, m_data->_birth_date._month, m_data->_birth_date._day).toString(QLocale().dateFormat(QLocale::ShortFormat) );
            case field::_city:              return QString::fromStdString(m_data->_city);
            case field::_mail_index:        return QString::fromStdString(m_data->_mail_index);
            case field::_mail_address:      return QString::fromStdString(m_data->_mail_address);
            case field::_country:           return QLocale::countryToString(QLocale::Country(m_data->_country));
            case field::_lang:              return QLocale().languageToString(QLocale(m_data->_lang.c_str()).language());
            case field::_phone:             return QString::fromStdString(m_data->_phone);
            case field::_money:             return Money(m_data->_money).toString();
            case field::_in_game_money:     return Money(m_data->_in_game_money).toString();
            case field::_wait_out_money:    return Money(m_data->_wait_out_money).toString();
            case field::_vip_status:        return ns_util::toString(m_data->_vip_status);
            case field::_gender:            return ns_util::toString(m_data->_gender);
            case field::_verifying:         return ns_util::toString(m_data->_verifying);
        default:
            return QString();
        }
    }

    QVariant user_info_t::valueOf(field f) const
    {
        if (m_data.isNull()) {
            return f == field::_user_id ? QVariant::fromValue(-1) : QVariant();
        }
        const data_type& data = *m_data.data();
        switch (f) {
        case field::_user_id:           return QVariant::fromValue(data.get_member<underlying_cast(field::_user_id)>());
        case field::_nickname:          return QVariant::fromValue(data.get_member<underlying_cast(field::_nickname)>());
        case field::_hash_avatar:       return QVariant::fromValue(data.get_member<underlying_cast(field::_hash_avatar)>());
        case field::_email:             return QVariant::fromValue(data.get_member<underlying_cast(field::_email)>());
        case field::_username:          return QVariant::fromValue(data.get_member<underlying_cast(field::_username)>());
        case field::_last_name:         return QVariant::fromValue(data.get_member<underlying_cast(field::_last_name)>());
        case field::_birth_date:        return QVariant::fromValue(data.get_member<underlying_cast(field::_birth_date)>());
        case field::_city:              return QVariant::fromValue(data.get_member<underlying_cast(field::_city)>());
        case field::_mail_index:        return QVariant::fromValue(data.get_member<underlying_cast(field::_mail_index)>());
        case field::_mail_address:      return QVariant::fromValue(data.get_member<underlying_cast(field::_mail_address)>());
        case field::_country:           return QVariant::fromValue(data.get_member<underlying_cast(field::_country)>());
        case field::_lang:              return QVariant::fromValue(data.get_member<underlying_cast(field::_lang)>());
        case field::_phone:             return QVariant::fromValue(data.get_member<underlying_cast(field::_phone)>());
        case field::_money:             return QVariant::fromValue(data.get_member<underlying_cast(field::_money)>());
        case field::_in_game_money:     return QVariant::fromValue(data.get_member<underlying_cast(field::_in_game_money)>());
        case field::_wait_out_money:    return QVariant::fromValue(data.get_member<underlying_cast(field::_wait_out_money)>());
        case field::_vip_status:        return QVariant::fromValue(data.get_member<underlying_cast(field::_vip_status)>());
        case field::_gender:            return QVariant::fromValue(data.get_member<underlying_cast(field::_gender)>());
        case field::_verifying:         return QVariant::fromValue(data.get_member<underlying_cast(field::_verifying)>());
        default:
            return QVariant();
        }
    }

    QIcon user_info_t::icon(user_info_t::field f) const
    {
        if (m_data.isNull()) {
            return QIcon();
        }
        switch (f) {
        case field::_hash_avatar: return m_avatar;
        case field::_country:
        case field::_lang:
            return getIcon(QLocale::Country(m_data->_country));
//        case field::_money:
//            return QIcon(":/ico2/icon_money.png");
//        case field::_vip_status:
//            return QIcon(":/ico2/icon_vip.png");
        default:
            return QIcon();
        }
    }
    gbp_i64 user_info_t::money(field f) const
    {
        if (m_data.isNull()) {
            return -1;
        }
        switch (f)
        {
        case field::_money:             return (m_data->_money);
        case field::_in_game_money:     return (m_data->_in_game_money);
        case field::_wait_out_money:    return (m_data->_wait_out_money);
        default:
            return -1;
        }
    }

    bool user_info_t::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (m_data.isNull() || !value.isValid()) {
            return false;
        }
        if (!index.isValid() || index.parent().isValid() || index.column() != 0) {
            return false;
        }
        bool changed  = false;
        switch (field(index.row())) {
        case field::_user_id:           return false;
        case field::_nickname:          return false;
        case field::_hash_avatar:       changed = ns_meta::img_type::modify(m_data->_hash_avatar, value); break;
        case field::_email:             changed = ns_meta::text_type::modify(m_data->_email, value); break;
        case field::_username:          changed = ns_meta::name_type::modify(m_data->_username, value); break;
        case field::_last_name:         changed = ns_meta::name_type::modify(m_data->_last_name, value); break;
        case field::_birth_date:        changed = ns_meta::birthdate_type::modify(m_data->_birth_date, value); break;
        case field::_city:              changed = ns_meta::name_type::modify(m_data->_city, value); break;
        case field::_mail_index:        changed = ns_meta::text_type::modify(m_data->_mail_index, value); break;
        case field::_mail_address:      changed = ns_meta::text_type::modify(m_data->_mail_address, value); break;
        case field::_country:           changed = ns_meta::country_type::modify(m_data->_country, value); break;
        case field::_lang:              changed = ns_meta::language_type::modify(m_data->_lang, value); break;
        case field::_phone:             changed = ns_meta::text_type::modify(m_data->_phone, value); break;
        case field::_money:             changed = ns_meta::money_type::modify(m_data->_money, value); break;
        case field::_in_game_money:     changed = ns_meta::money_type::modify(m_data->_in_game_money, value); break;
        case field::_wait_out_money:    changed = ns_meta::money_type::modify(m_data->_wait_out_money, value); break;
        case field::_vip_status:        m_data->_vip_status     = gbp::ns_vip_system::e_status(value.value<typename std::underlying_type<gbp::ns_vip_system::e_status>::type >()); break;
        case field::_gender:            return false;/*m_data->_gender = value.value<gbp::ns_details::e_gender>(); break;*/
        case field::_verifying:         m_data->_verifying      = value.value<gbp::ns_api::e_verifying>(); break;
        default:
            return false;
        }
        if (!changed) {
            return false;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    bool user_info_t::modify(user_info_t::field f, const QVariant &newValue, int role) {
        return setData(index(underlying_cast(f), 0), newValue, role);
    }

    void user_info_t::updateEmail(const QString& email) {
        modify(field::_email, email);
    }
    void user_info_t::updatePhoneNumber(const QString& countryCode, const QString& phoneNumber) {
        modify(field::_phone, countryCode + phoneNumber);
    }

    void user_info_t::updatePhoneNumber(const QString &phoneNumber)
    {
        modify(field::_phone, phoneNumber);
    }
    void user_info_t::updateAvatar(const QIcon& avatar)
    {
        int actualSize = 0;
        for (const QSize& s: avatar.availableSizes()) {
            if (s.width() > actualSize) {
                actualSize = s.width();
            }
        }
        if (modify(field::_hash_avatar, QVariant::fromValue(avatar.pixmap(actualSize)))) {
            m_avatar = avatar;
        }
    }

    void user_info_t::updateAddress(QLocale::Country country, const QString& city, const QString& address, const QString& mailIndex) {
        modify(field::_country, country);
        modify(field::_city, city);
        modify(field::_mail_address, address);
        modify(field::_mail_index, mailIndex);
    }

    void user_info_t::updateLanguage(const QString& lang) {
        modify(field::_lang, lang);
    }

    void user_info_t::updateVerification(gbp::ns_api::e_verifying verifying)
    {
        modify(field::_verifying, QVariant::fromValue(verifying));
    }

    bool user_info_t::addMoney(gbp_i64 m) {
        if (m > 0) {
            modify(field::_money, QVariant::fromValue(money(field::_money) + m));
            return true;
        }
        return false;
    }
    bool user_info_t::moveMoneyToIngameMoney(gbp_i64 m) {
        gbp_i64 currMoney = money(field::_money);
        if (m <= currMoney) {
            modify(field::_money, QVariant::fromValue(currMoney - m));
            modify(field::_in_game_money, QVariant::fromValue(money(field::_in_game_money) + m));
            return true;
        }
        return false;
    }
    bool user_info_t::moveMoneyFromIngameMoney(gbp_i64 m) {
        gbp_i64 currIngameMoney = money(field::_in_game_money);
        if (m <= currIngameMoney && m > 0) {
            modify(field::_money, QVariant::fromValue(money(field::_money) + m));
            modify(field::_in_game_money, QVariant::fromValue(currIngameMoney - m));
            return true;
        }
        return false;
    }
    bool user_info_t::moveMoneyToWaitOutMoney(gbp_i64 m) {
        gbp_i64 currMoney = money(field::_money);
        if (m <= currMoney) {
            modify(field::_money, QVariant::fromValue(currMoney - m));
            modify(field::_wait_out_money, QVariant::fromValue(money(field::_wait_out_money) + m));
            return true;
        }
        return false;
    }
    bool user_info_t::updateIngameMoney(gbp_i64 m) {
        if (m >= 0) {
            modify(field::_in_game_money, QVariant::fromValue(m));
            return true;
        }
        return false;
    }
    bool user_info_t::substractWaitOutMoney(gbp_i64 m) {
        gbp_i64 waitOutMoney = money(field::_wait_out_money);
        if (m > 0 && m <= waitOutMoney) {
            modify(field::_wait_out_money, QVariant::fromValue(waitOutMoney - m));
            return true;
        }
        return false;
    }
    bool user_info_t::cancelWaitOutMoney(gbp_i64 m) {
        gbp_i64 waitOutMoney = money(field::_wait_out_money);
        if (m > 0 && m <= waitOutMoney) {
            modify(field::_wait_out_money, QVariant::fromValue(waitOutMoney - m));
            modify(field::_money, QVariant::fromValue(money(field::_money) + m));
            return true;
        }
        return false;
    }



    bool user_info_t::registerDisplayWidget(QWidget *displayWidget, user_info_t::field f, const QByteArray &property, int role) {
        Q_UNUSED(displayWidget); Q_UNUSED(f); Q_UNUSED(property); Q_UNUSED(role);
        return false;
    }

    std::string gen(int minlen, int maxlen, std::vector<int> flags)
    {
        std::string str;
        uint len = minlen + rand() % (maxlen - minlen + 1);
        for (int flag: flags)
        {
            std::string symbols = "";
            if (flag & 1) {
                symbols += "AEIOUY";
            }
            if (flag & 2) {
                symbols += "BCDFGHJKLMNPQRSTVWXZ";
            }
            if (flag & 4) {
                symbols += "aeiouy";
            }
            if (flag & 8) {
                symbols += "bcdfghjklmnpqrstvwxz";
            }
            if (flag & 16) {
                symbols += "_-";
            }
            if (flag & 32) {
                symbols += "0123456789";
            }
            if (flag & 64) {
                symbols += ".[]()!@#$^&*?";
            }
            str += symbols.at(rand() % symbols.size());
        }
        while (str.size() < len) {
            str += "abcnvgrtpk"[rand()%10];
        }
        str.resize(len);
        return str;
    }

    gbp::ns_api::ns_lobby::user_info_t *ns_test::createRandomUser()
    {
        gbp::ns_api::ns_lobby::user_info_t* d = new gbp::ns_api::ns_lobby::user_info_t;

        d->_user_id      = rand() % 10000;
        d->_nickname       = gen(8, 12, {15, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63});
        d->_hash_avatar    = "";
        d->_email          = d->_nickname + "@" + std::vector<std::string>({"gmail.com", "ukr.net", "ya_zadrot.uk"})[rand() % 3];
        d->_username       = rand() % 2;
        d->_last_name      = rand() % 2;
        d->_birth_date     = ns_util::apiDate((QDate::currentDate().year() - 60) + rand()%42, rand()%12, rand()%28);
        d->_city           = gen(4, 12, {2, 4, 12, 12, 12, 12, 12, 12, 12, 12, 12});
        d->_mail_index     = gen(4, 6, {34, 34, 34, 34, 34, 34});
        d->_mail_address   = "";
        d->_country        = rand() % 257;
        d->_lang           = "en";
        d->_phone          = std::string("+") + gen(12, 12, {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32});
        d->_money          = rand() % 2000000 + 250;
        d->_in_game_money  = rand() % 2000000 + 250;
        d->_wait_out_money = rand() % 2000000 + 250;
        d->_vip_status     = gbp::ns_vip_system::e_status(1 + (rand() % 6));
        d->_gender         = gbp::ns_details::e_gender(rand() % 2);
        d->_verifying      = gbp::ns_api::e_verifying(rand() % 3);

        return d;
    }

}


QVariant ns_model::user_info_t::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
    {
        switch (role) {
        case Qt::DisplayRole:
            return QString("user_info.") + ns_util::enumToString(field(section));
        case Qt::ToolTipRole:
            return QString("gbp::ns_api::ns_lobby::user_info_t::") + ns_util::enumToString(field(section));
        default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags ns_model::user_info_t::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.parent().isValid() || index.column() != 0) {
        return Qt::NoItemFlags;
    }
    auto commonFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    auto dragNDrop = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    switch (field(index.row())) {
    case field::_user_id:        return commonFlags;
    case field::_nickname:       return commonFlags;
    case field::_hash_avatar:    return commonFlags | Qt::ItemIsEditable;
    case field::_email:          return commonFlags | Qt::ItemIsEditable;
    case field::_username:       return commonFlags;
    case field::_last_name:      return commonFlags;
    case field::_birth_date:     return commonFlags | Qt::ItemIsEditable;
    case field::_city:           return commonFlags | Qt::ItemIsEditable;
    case field::_mail_index:     return commonFlags | Qt::ItemIsEditable;
    case field::_mail_address:   return commonFlags | Qt::ItemIsEditable;
    case field::_country:        return commonFlags | Qt::ItemIsEditable | dragNDrop;
    case field::_lang:           return commonFlags | Qt::ItemIsEditable | dragNDrop;
    case field::_phone:          return commonFlags | Qt::ItemIsEditable | dragNDrop;
    case field::_money:          return commonFlags | Qt::ItemIsEditable;
    case field::_in_game_money:  return commonFlags | Qt::ItemIsEditable;
    case field::_wait_out_money: return commonFlags | Qt::ItemIsEditable;
    case field::_gender:         return commonFlags;
    case field::_verifying:      return commonFlags | Qt::ItemIsEditable | Qt::ItemIsTristate | Qt::ItemIsUserTristate;
    case field::_vip_status:     return commonFlags | Qt::ItemIsEditable;
    default:
        return Qt::NoItemFlags;
    }
}

QModelIndexList ns_model::user_info_t::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    return QAbstractListModel::match(start, role, value, hits, flags);
}



bool ns_model::user_info_t::submit()
{
    if (m_data.isNull() || m_rpc == nullptr) {
        return false;
    }
    const data_type& data = *m_data.data();
    auto submitOk = [](ns_lobby::e_status s){
        std::cout << s << std::endl;
    };



//    if (isDifferent<field::_hash_avatar>(data, m_cash)) {
//        m_rpc->change_avatar();
//    }
    if (m_cash._phone != data._phone) {
        m_rpc->update_phone_number(data._phone, submitOk);
    }
    if (m_cash._country != data._country || m_cash._city != data._city || m_cash._mail_index != data._mail_index || m_cash._mail_address != data._mail_address) {
        m_rpc->update_address(data._country, data._city.c_str(), data._mail_index.c_str(), data._mail_address.c_str(), submitOk);
    }

    return true;
}

void ns_model::user_info_t::revert()
{
    if (!m_data.isNull())
    {
        *m_data.data() = m_cash;
    }
}
