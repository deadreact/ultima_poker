#pragma once
#include <QDataStream>
#include "qvector.h"
#include "defs.hpp"

template <typename EnumType>
constexpr typename std::underlying_type<EnumType>::type underlying_cast(EnumType enumValue);

template <typename enum_type>
void read_enum(QDataStream& stream, enum_type data) {
    stream << underlying_cast(data);
}

template <typename enum_type>
void write_enum(QDataStream& stream, enum_type &data) {
    typename std::underlying_type<enum_type>::type underlying;
    stream >> underlying;
    data = enum_type(underlying);
}
template <typename value_type>
void read_vector(QDataStream& stream, const std::vector<value_type> &data) {
    stream << QVector<value_type>::fromStdVector(data);
}

template <typename value_type>
void write_vector(QDataStream& stream, std::vector<value_type> &data) {
    QVector<value_type> v;
    stream >> v;
    data = v.toStdVector();
}

namespace ns_io
{
    template <typename T>
    void write(QDataStream& stream, const T& arg) {
        stream << arg;
    }
    template <typename T, typename ...Args>
    void write(QDataStream& stream, const T& arg, const Args&... args) {
        stream << arg;
        write(stream, args...);
    }

} //namespace api_io

template <typename T>
QDataStream& operator<<(QDataStream& stream, const std::vector<T>& data) { return read_vector(stream, data), stream; }
template <typename T>
QDataStream& operator>>(QDataStream& stream, std::vector<T>& data) { return write_vector(stream, data), stream; }

QDataStream& operator<<(QDataStream& stream, const std::string& data);
QDataStream& operator>>(QDataStream& stream, std::string& data);

QDataStream& operator<<(QDataStream& stream, gbp::ns_api::ns_game::e_game_limit data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_game::e_game_limit& data);
QDataStream& operator<<(QDataStream& stream, gbp::ns_api::ns_game::e_game_type data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_game::e_game_type& data);
QDataStream& operator<<(QDataStream& stream, gbp::ns_api::ns_game::e_game_speed data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_game::e_game_speed& data);

QDataStream& operator<<(QDataStream& stream, gbp::ns_api::ns_tournament_types::e_tournament_status data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_tournament_types::e_tournament_status& data);

QDataStream& operator<<(QDataStream& stream, gbp::ns_api::ns_statistic::e_tourn_button_type data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_statistic::e_tourn_button_type& data);

QDataStream& operator<<(QDataStream& stream, const std::pair<gbp_i8, gbp_i8>& data);
QDataStream& operator>>(QDataStream& stream, std::pair<gbp_i8, gbp_i8>& data);

QDataStream& operator<<(QDataStream& stream, const gbp::ns_api::ns_statistic::player_t& data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_statistic::player_t& data);
QDataStream& operator<<(QDataStream& stream, const gbp::ns_api::ns_statistic::cash_stat_t& data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_statistic::cash_stat_t& data);
QDataStream& operator<<(QDataStream& stream, const gbp::ns_api::ns_statistic::tournament_stat_t& data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_statistic::tournament_stat_t& data);
QDataStream& operator<<(QDataStream& stream, const gbp::ns_api::rebuys_t& data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::rebuys_t& data);
QDataStream& operator<<(QDataStream& stream, const gbp::ns_api::addon_t& data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::addon_t& data);

QDataStream& operator<<(QDataStream& stream, const gbp::ns_api::ns_table::table_state_t& data);
QDataStream& operator>>(QDataStream& stream, gbp::ns_api::ns_table::table_state_t& data);
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::user_state_t &data);
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::table_player_info_t &data);
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::tournament_state_t &data);
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::round_conditions_t &data);
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::user_state_t &data);
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::table_player_info_t &data);
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::tournament_state_t &data);
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::round_conditions_t &data);


QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tourn_additional_stat_t &data);
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tourn_additional_stat_t &data);

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_lobby::user_info_t &data);
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_lobby::user_info_t &data);
