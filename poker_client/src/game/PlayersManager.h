#pragma once

#include <tools/types_fwd.hpp>
#include <QObject>
#include <QLocale>
#include <set>

class PlayersManager : public QObject
{
    Q_OBJECT
private:
    struct PlayerData
    {
        std::pair<gbp_i64, QString> id;
        std::pair<sha1_t, base64_t> avatar;
        PlayerData(gbp_i64 id, const QString& nick, const sha1_t &hash);
    };
    std::map<gbp_i64, PlayerData> m_players;
//    std::set<gbp_i64> m_requestList;
    std::set<PlayerData*> m_requestList;
public:
    void requestAvatars();
public:
    explicit PlayersManager(QObject *parent = nullptr);
    virtual ~PlayersManager();

    void addPlayer(gbp_i64 id, const QString& nickname, const sha1_t& hashAvatar, QLocale::Country = QLocale::AnyCountry);
    bool contains(gbp_i64 playerID) const;
    bool contains(const QString& nickname) const;

    QString nickname(gbp_i64 id) const;
    QPixmap avatar(gbp_i64 id) const;
    sha1_t hash(gbp_i64 id) const;
    gbp_i64 id(const QString& nickname) const;
    QPixmap avatar(const QString& nickname) const;
    sha1_t hash(const QString& nickname) const;

    QPixmap avatar(const sha1_t& hash);
signals:
    void avatarsUpdated(const std::vector<gbp_i64>& ids);
};

