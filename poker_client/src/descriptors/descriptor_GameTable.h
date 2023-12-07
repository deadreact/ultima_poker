#pragma once

#include <tools/types_fwd.hpp>
#include <QString>
#include <vector>

namespace ns_descriptor {
    struct GameTable
    {
        using e_game_limit = gbp::ns_api::ns_game::e_game_limit;
        using e_game_type = gbp::ns_api::ns_game::e_game_type;

        const GameId id;
        const QString tableName;
        const QString title;
        //! true - free, false - busy or reserved
        const std::vector<bool> seats;
        const struct {
            e_game_limit limit;
            e_game_type  type;
//            e_game_speed speed;
        } game;


        GameTable(const GameId& gameID, const QString& tableName, const QString& title, const std::vector<bool>& seats, e_game_limit limit, e_game_type type)
            : id(gameID), tableName(tableName), title(title), seats(seats), game{limit, type}
        {}
    };

} //namespace ns_descriptor
