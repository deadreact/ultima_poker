#pragma once

#include "api_models_global.h"
#include <QAbstractListModel>
#include "../../poker_client/src/core/money.h"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/lobby/types.h>
#endif // !Q_MOC_RUN

#include <basewidgets/types.h>
#include <QLocale>
#include <QMetaMethod>
#include <functional>

Q_DECLARE_METATYPE(gbp::ns_details::e_gender)
Q_DECLARE_METATYPE(gbp::ns_api::e_verifying)
Q_DECLARE_METATYPE(gbp::ns_vip_system::e_status)
Q_DECLARE_METATYPE(gbp::ns_api::ns_lobby::user_info_t)
class QLineEdit;

namespace ns_lobby
{
    struct rpc_object;
} //namespace ns_lobby


namespace ns_model
{
    class API_MODEL_DLL user_info_t : public QAbstractListModel
    {
        Q_OBJECT
    public:
        using data_type = gbp::ns_api::ns_lobby::user_info_t;

        enum class field : int {
            begin = 0,

            _user_id = begin,
            _nickname,
            _hash_avatar,
            _email,
            _username,
            _last_name,
            _birth_date,
            _city,
            _mail_index,
            _mail_address,
            _country,
            _lang,
            _phone,
            _money,
            _in_game_money,
            _wait_out_money,
            _gender,
            _verifying,
            _vip_status,

            end,
            count = end
        };
        Q_ENUM(field)

    private:
        data_type m_cash;
        QScopedPointer<data_type> m_data;
        ns_lobby::rpc_object* m_rpc;
        QIcon m_avatar;
    public:
        explicit user_info_t(QObject *parent = nullptr);
        virtual ~user_info_t();

        void setRpcObject(ns_lobby::rpc_object* rpc);

        gbp_i64 userId() const;

        void resetData(gbp::ns_api::ns_lobby::user_info_t* newData = nullptr);

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        QModelIndex indexOf(field f) const;
        field fieldOf(const QModelIndex& idx) const;

        QVariant valueOf(field f) const;
        template <typename T>
        T valueOf(field f) const {
            return valueOf(f).value<T>();
        }

        QString text(field f) const;
        QIcon icon(field f) const;
        gbp_i64 money(field f) const;

        inline const gbp::ns_api::ns_lobby::user_info_t* nativeData() const { return m_data.data(); }

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        bool modify(field f, const QVariant& newValue, int role = Qt::EditRole);
    public slots:
        void updateEmail(const QString& email);
        void updatePhoneNumber(const QString& countryCode, const QString& phoneNumber);
        void updatePhoneNumber(const QString& phoneNumber);
        void updateAvatar(const QIcon& avatar);
        void updateAddress(QLocale::Country country, const QString& city, const QString& address, const QString& mailIndex);
        void updateLanguage(const QString& lang);
        void updateVerification(gbp::ns_api::e_verifying verifying);
    public slots:
        // пополнение счета
        bool addMoney(gbp_i64 m);
        // внесение бай-ина
        bool moveMoneyToIngameMoney(gbp_i64 m);
        // вышел из игры, забрав выигрыш
        bool moveMoneyFromIngameMoney(gbp_i64 m);
        // подана заявка на вывод средств
        bool moveMoneyToWaitOutMoney(gbp_i64 m);
        // изменения в процессе игры
        bool updateIngameMoney(gbp_i64 m);
        // деньги были выведены
        bool substractWaitOutMoney(gbp_i64 m);
        // отменена заявка на вывод средств
        bool cancelWaitOutMoney(gbp_i64 m);
    public:
        virtual bool registerDisplayWidget(QWidget* displayWidget, field f, const QByteArray& property = "text", int role = Qt::DisplayRole);

        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
        virtual QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;

        // QAbstractItemModel interface
    public slots:
        virtual bool submit() override;
        virtual void revert() override;
    };

    namespace ns_test
    {
        gbp::ns_api::ns_lobby::user_info_t* createRandomUser();
    }


} //namespace ns_model


//template <ns_model::user_info_t::field f>
//struct fieldInfo;
//template <ns_model::user_info_t::field f, typename T = typename fieldInfo<f>::type>
//const T& getValueOf(const ns_model::user_info_t::data_type& data);

// ----------------- INLINE ----------------------
