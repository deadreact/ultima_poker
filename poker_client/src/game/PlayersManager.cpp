#include <precompiled.h>
#include "PlayersManager.h"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <rpc/network_client.h>
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_lobby.h>
#endif //!Q_MOC_RUN

#include <PokerApplication.h>
#include <basewidgets/util.h>

//PlayerData::PlayerData()
//    : id(std::make_pair(-1, QString()))
//    , avatar(std::make_pair(sha1_t(), base64_t()))
//{}

PlayersManager::PlayerData::PlayerData(gbp_i64 id, const QString &nick, const sha1_t &hash)
    : id(std::make_pair(id, nick))
    , avatar(std::make_pair(hash, pApp->db().getAvatar(hash)))
{}

PlayersManager::PlayersManager(QObject *parent)
    : QObject(parent)
{}

PlayersManager::~PlayersManager() {}

void PlayersManager::addPlayer(gbp_i64 id, const QString &nickname, const sha1_t &hashAvatar, QLocale::Country)
{
    auto it = m_players.find(id);
    if (it == m_players.end())
    {
        PlayerData data(id, nickname, hashAvatar);
        base64_t& avatar = data.avatar.second;
        auto res = m_players.insert(std::make_pair(id, std::move(data)));
        if (avatar.isNull() || avatar.isEmpty()) {
            m_requestList.insert(&res.first->second);
        }
    }
}

QString PlayersManager::nickname(gbp_i64 id) const {
    return m_players.at(id).id.second;
}

QPixmap PlayersManager::avatar(gbp_i64 id) const {
    return ns_util::fromBase64(m_players.at(id).avatar.second);
}

sha1_t PlayersManager::hash(gbp_i64 id) const {
    return m_players.at(id).avatar.first;
}

gbp_i64 PlayersManager::id(const QString &nickname) const {
    for (const std::pair<gbp_i64, PlayerData>& d: m_players)
    {
        if (d.second.id.second == nickname) {
            return d.second.id.first;
        }
    }
    return -1;
}

QPixmap PlayersManager::avatar(const QString &nickname) const {
    return avatar(id(nickname));
}

sha1_t PlayersManager::hash(const QString &nickname) const {
    return hash(id(nickname));
}

void PlayersManager::requestAvatars()
{
    if (m_requestList.empty()) {
        return;
    }

    std::vector<gbp_i64> v;
    for (PlayerData* plData: m_requestList) {
        v.push_back(plData->id.first);
    }
    rpc_lobby* rpc = rpc_object<rpc_lobby>();
    rpc->get_avatars(v, [this](reply_lobby::e_lobby status, const QHash<gbp_i64, QByteArray>/*std::unordered_map<gbp_i64, std::string>*/& avatars)
    {
        if (ns_helpers::isOk(status))
        {
            std::vector<gbp_i64> v;
            for (PlayerData* plData : m_requestList)
            {
                auto it = avatars.find(plData->id.first);
                if (it != avatars.end()) {
                    plData->avatar.second = *it;
                    sha1_t hash = pApp->db().addAvatar(*it);

                    Q_ASSERT(hash == plData->avatar.first);
                    v.push_back(plData->id.first);
                }
            }
            m_requestList.clear();
            emit avatarsUpdated(v);
        }
    });
}

