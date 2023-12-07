#pragma once

#include <tools/types_fwd.hpp>
#include <qstring.h>

namespace ns_data
{
    struct PlayerBase
    {
        gbp_i64 m_playerID;
        QString m_nickname;
        QByteArray m_avatarHash;
        int m_avatarPixmapCacheId;
    };
} // namespace ns_data

