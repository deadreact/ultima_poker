#pragma once

#include <tools/types_fwd.hpp>
#include <QLocale>
#include "data_playerbase.h"
#include <tools/accessor.hpp>

//struct TableData;

namespace ns_model
{
    class table_state_t;
} // namespace ns_model

namespace ns_data
{
    struct TablePlayerMapping {
    private:
        TablePlayerMapping(gbp_i64 tableID, const gbp::ns_api::ns_table::table_player_info_t& data);
    public:
        const gbp_i64 tableID;
        const gbp::ns_api::ns_table::table_player_info_t& data;
        static TablePlayerMapping* create(gbp_i64 table_id, const PlayerBase& baseData);
    };

    struct TablePlayer
    {
        PlayerBase* m_baseData;
        GameId m_gameID;

        ns_model::table_state_t* tableModel() const;

//        Accessor<gbp::ns_api::ns_table::table_player_info_t> getTableData() const;


//        gbp::ns_api::ns_table::table_state_t* m_tableData;
//        gbp::ns_api::ns_table::table_player_info_t* m_playerData;

//        QLocale::Country m_country;
//        GameAction m_lastCourse;
//        std::array<gbp_i64, 5> m_roundBet;
//        gbp_i64 m_distributionBet;
//        gbp_i64 m_gameMoney;
//            gbp_i16 m_seatIndex;    // 0 - 8 (? 9)
//            gbp_i16 m_roundIndex; // 0 - 4
//            gbp_i16 m_waiterIndex;
//            bool m_online;
//            bool m_frozen;
//            bool m_ingame;
//            bool m_folded;
//            bool m_current;
    };
} // namespace ns_data

//class TablePlayer : public QAction
//{
//    Q_OBJECT
//    Q_PROPERTY(GamePlayerId id READ id MEMBER m_id CONSTANT)
//    Q_PROPERTY(long long playerID READ playerID)
//    Q_PROPERTY(GameId gameID READ gameID)
//    Q_PROPERTY(long long tableID READ tableID)
//    Q_PROPERTY(QString nickname READ text WRITE setText)
//    Q_PROPERTY(QPixmap avatar READ avatar WRITE setAvatar NOTIFY avatarUpdated)
//    Q_PROPERTY(QLocale::Country country MEMBER m_country)

//    Q_PROPERTY(GameAction lastCourse READ lastCourse WRITE course)

//    Q_PROPERTY(Money roundBet        READ roundBet        WRITE setRoundBet        NOTIFY moneyChanged)
//    Q_PROPERTY(Money distributionBet READ distributionBet WRITE setDistributionBet NOTIFY moneyChanged)
//    Q_PROPERTY(Money gameMoney       READ gameMoney       WRITE setGameMoney       NOTIFY moneyChanged)

//    Q_PROPERTY(short seatIndex  READ seatIndex  WRITE setSeatIndex  NOTIFY indexChanged)
//    Q_PROPERTY(short roundIndex READ roundIndex WRITE setRoundIndex NOTIFY indexChanged)
//    Q_PROPERTY(short waiterIndex READ waiterIndex WRITE setWaiterIndex NOTIFY indexChanged)
//    Q_PROPERTY(short waitersCount READ waitersCount WRITE setWaitersCount)

//    Q_PROPERTY(bool online  READ isOnline  WRITE setOnline   NOTIFY statusChanged)
//    Q_PROPERTY(bool sitOut  READ isSitOut  WRITE sitOut      NOTIFY statusChanged)
//    Q_PROPERTY(bool frozen  READ isFrozen  WRITE freeze      NOTIFY statusChanged)
//    Q_PROPERTY(bool ingame  READ isInGame  WRITE setInGame   NOTIFY statusChanged)
//    Q_PROPERTY(bool folded  READ isFolded  WRITE fold        NOTIFY statusChanged)
//    Q_PROPERTY(bool current READ isCurrent WRITE makeCurrent NOTIFY statusChanged)

//signals:
//    void avatarUpdated(const QPixmap&);
//    void moneyChanged(const QMetaProperty& hint);
//    void statusChanged(const QMetaProperty& hint);
//    void indexChanged(const QMetaProperty& hint);
//private:
//    TablePlayer(GamePlayerId id
//               , const QString& nickname
//               , const sha1_t& hashAvatar
//               , QLocale::Country country = QLocale::AnyCountry
//               , QObject* parent = nullptr);
//private:
//    std::vector<QMetaProperty> m_changes;
//public:
//    const QMetaProperty& lastChanges() const;
//    const QMetaProperty& changes(int idx) const;
//    int changesCountFrom(int rev) const;
//    std::vector<QMetaProperty> changesFrom(int rev) const;
//    const std::vector<QMetaProperty>& allChanges() const;
//protected:
//    const GamePlayerId m_id;
//    QLocale::Country m_country;
//    GameAction m_lastCourse;
//    std::array<Money, 5> m_roundBet;
//    Money m_distributionBet;
//    Money m_gameMoney;
//    gbp_i16 m_seatIndex;    // 0 - 8 (? 9)
//    gbp_i16 m_roundIndex; // 0 - 4
//    gbp_i16 m_waiterIndex;
//    bool m_online;
//    bool m_frozen;
//    bool m_ingame;
//    bool m_folded;
//    bool m_current;
//public:
//    static TablePlayer *create(const GamePlayerId& gameID
//                             , const QString& nickname
//                             , const sha1_t& hashAvatar
//                             , QLocale::Country country = QLocale::AnyCountry
//                             , QObject* parent = nullptr);

//    inline static TablePlayer *create(gbp_i64 playerID
//                                    , const GameId& gameID
//                                    , const QString& nickname
//                                    , const sha1_t& hashAvatar
//                                    , QLocale::Country country = QLocale::AnyCountry
//                                    , QObject* parent = nullptr);


//    virtual ~TablePlayer();
//    QByteArray info() const;
//    inline const GamePlayerId& id() const;
//    inline gbp_i64 playerID() const;
//    inline const GameId& gameID() const;
//    inline gbp_i64 tableID() const;

//    QPixmap avatar(const QSize& size = QSize()) const;
//    void setAvatar(const QPixmap& avatar, bool override = true);

//    const GameAction &lastCourse() const;
//    gbp_i16 seatIndex() const;
//    void setSeatIndex(gbp_i16 seatIndex);
//    gbp_i16 roundIndex() const;
//    void setRoundIndex(gbp_i16 round);
//    gbp_i16 waiterIndex() const;
//    void setWaiterIndex(gbp_i16 waiter);
//    gbp_i16 waitersCount() const;
//    void setWaitersCount(gbp_i16 count);

//    Money roundBet(gbp_i16 round = -1/* -1 - currentRound*/) const;
//    Money distributionBet() const;
//    Money gameMoney() const;
//    Money totalIngameMoney() const;

//    void setRoundBet(Money m);
//    void setDistributionBet(Money m);
//    void setGameMoney(Money m);

//    inline bool isSitOnTable() const { return seatIndex() != -1; }
//    inline bool isOnline() const     { return m_online; }
//    inline bool isSitOut() const     { return !isOnline(); }
//    inline bool isFrozen() const     { return m_frozen; }
//    inline bool isInGame() const     { return m_ingame; }
//    inline bool isCurrent() const    { return m_current; }
//    inline bool isFolded() const     { return m_folded; }

//    void setOnline(bool v = true);
//    void sitOut(bool v = true);
//    void freeze(bool v = true);
//    void setInGame(bool v = true);
//    void fold(bool v = true);
//    void makeCurrent(bool v = true);
//    void course(const GameAction& course);
//public slots:

//};

//typedef QList<TablePlayer *> TablePlayerList;
//typedef QMap<GamePlayerId, TablePlayer *> TablePlayerMap;
//typedef QPair<GamePlayerId, TablePlayer *> TablePlayerItem;

//inline TablePlayer *TablePlayer::create(gbp_i64 playerID, const GameId &gameID, const QString &nickname, const sha1_t &hashAvatar, QLocale::Country country, QObject *parent)
//{
//    return TablePlayer::create(GamePlayerId(gameID, playerID), nickname, hashAvatar, country, parent);
//}

//inline const GamePlayerId& TablePlayer::id() const {
//    return m_id;
//}
//inline gbp_i64 TablePlayer::playerID() const {
//	return m_id.m_playerID;
//}
//inline const GameId& TablePlayer::gameID() const {
//	return m_id.m_gameID;
//}
//inline gbp_i64 TablePlayer::tableID() const {
//	return m_id.m_gameID.m_tableID;
//}
