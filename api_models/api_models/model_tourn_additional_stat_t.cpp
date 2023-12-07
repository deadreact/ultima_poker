#include "model_tourn_additional_stat_t.hpp"
#include "../poker_client/src/core/money.h"
#include "defs_metatype.hpp"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

#include <qdatetime.h>
#include <qlocale.h>
#include <basewidgets/types.h>
#include "api_util.hpp"
#include "itemtype_concept.hpp"
#include <tools/item_reference.hpp>

#include <iostream>
#include <QtWidgets/qapplication.h>
#include <qdebug.h>
#include <QtWidgets/qstyle.h>

struct ItemRef<struct gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info>;

template <>
struct ItemRef<gbp::ns_api::ns_statistic::tourn_additional_stat_t::game>
{
    using T = gbp::ns_api::ns_statistic::tourn_additional_stat_t::game;
    static T& invalidItem() {
        static T invalid;
        return invalid;
    }

    bool isValid() const { return this != nullptr && (const void*)this != (const void*)&invalidItem(); }

    T& _item;
    gbp_i64& _id;
    std::string& _title;
    std::vector<struct gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info>& _players;
    operator const T& () { return _item; }
    operator T& () { return _item; }

    ItemRef(T& item): _item(item), _id(item._id), _title(item._title), _players(item._players) {}
//        ItemRef(): _item(invalidItem), _players(invalidItem._players) {}

    struct gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info& player(int idx) {
        return _item._players.at(idx);
    }
};

Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::tourn_additional_stat_t)
Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_statistic::tourn_additional_stat_t::game>)
Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_statistic::tourn_additional_stat_t::blind>)
Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_statistic::tourn_additional_stat_t::prize>)
Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info>)

gbp::ns_api::ns_statistic::tourn_additional_stat_t createRandomItem(gbp_i64 id)
{
    gbp::ns_api::ns_statistic::tourn_additional_stat_t data;
    data._id = id;
    data._games.resize(rand() % 30);
    data._blinds.resize(rand() % 30);
    data._prizes.resize(rand() % 30);
    data._late_register_ends_in = rand() % 1000;
    data._rebuys_count = rand() % 100;
    data._addons_count = rand() % 100;
    data._current_pos = rand() % 20 + 1;

    gbp_i64 gameId = 1;
    for (auto& g: data._games)
    {
        gameId += (rand() % 20);
        g._id = gameId;
        g._title = "game_" + std::to_string(g._id);
        g._players.resize(rand() % 100);

        gbp_i64 playerId = 1;
        for (auto& pl: g._players) {
            playerId += (rand() % 20);
            pl._id = playerId;
            pl._nickname = "player_" + std::to_string(playerId);
            pl._stack = (rand() % 12345) * 3.67;
            pl._pos = gbp_i8(rand() % 9);
        }
    }
    gbp_i64 lvl = 0;
    float k = float((rand() % 100) + 3) / 17 + 1.f;
    for (auto& bl: data._blinds) {
        bl._level = lvl++;
        bl._big_blind = bl._level * 40 * k;
        bl._ante = (rand() % 10)*10;
        bl._duration = (rand() % 10) * 10;
    }

    int pos = 0;
    for (auto& pr: data._prizes) {
        pr._winner_pos = pos++;
        pr._prize = (rand() % 1000) * 71;
    }

    return data;
}



namespace
{
    using e_tournament_type     = gbp::ns_api::ns_tournament_types::e_tournament_type;
    using e_start_time          = gbp::ns_api::ns_tournament_types::e_start_time;
    using e_chips_rebuy         = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_win_result          = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_win_prize           = gbp::ns_api::ns_tournament_types::e_win_prize;
    using e_money_in            = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_registration        = gbp::ns_api::ns_tournament_types::e_registration;
    using e_tournament_status   = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_tournament_stack    = gbp::ns_api::ns_tournament_types::e_tournament_stack;

    using e_tourn_button_type   = gbp::ns_api::ns_statistic::e_tourn_button_type;

    using e_game_type           = gbp::ns_api::ns_game::e_game_type;
    using e_game_speed          = gbp::ns_api::ns_game::e_game_speed;
    using e_game_limit          = gbp::ns_api::ns_game::e_game_limit;

    struct Coord {
        int _1;
        int _2;
        Coord(int _1, int _2): _1(_1), _2(_2) {}
        Coord(const QModelIndex& idx): _1(idx.row()), _2(idx.column()) {}
        inline bool operator==(const Coord& c) const {
            return c._1 == _1 && c._2 == _2;
        }
        bool match(int _1, int _2) const {
            return *this == Coord(_1, _2);
        }
    };

//    Coord indexCoord(const QModelIndex& idx)
//    {
//        return Coord(idx.row(), idx.column());
//    }

}

namespace ns_meta
{

} //namespace ns_meta

namespace ns_model
{
    struct tourn_additional_stat_t::PrivateData
    {
        using data_t = tourn_additional_stat_t::data_t;
    private:
        data_t data;
    public:
        QVariant allData() const { return QVariant::fromValue(data); }
        void reset(const data_t& d) {
            data = d;
        }

        const gbp_i64& _id;
        ItemRefContainer<struct gbp::ns_api::ns_statistic::tourn_additional_stat_t::game>  _games;
        ItemRefContainer<struct gbp::ns_api::ns_statistic::tourn_additional_stat_t::blind> _blinds;
        ItemRefContainer<struct gbp::ns_api::ns_statistic::tourn_additional_stat_t::prize> _prizes;
        const gbp_i64& _late_register_ends_in;
        const int& _rebuys_count;
        const int& _addons_count;
        const int& _current_pos;



        PrivateData(gbp_i64 id = -1)
            : data()
            , _id(data._id)
            , _games(data._games)
            , _blinds(data._blinds)
            , _prizes(data._prizes)
            , _late_register_ends_in(data._late_register_ends_in)
            , _rebuys_count(data._rebuys_count)
            , _addons_count(data._addons_count)
            , _current_pos(data._current_pos)
        {
            data._id = id;
//            /*QList<simple_item*> keeper = */build_layout(data);

        }
    };


    tourn_additional_stat_t::tourn_additional_stat_t(gbp_i64 id, QObject *parent)
        : ApiBase(parent)
        , m_dPtr(new PrivateData(id))
    {
//        ItemRef<gbp::ns_api::ns_statistic::tourn_additional_stat_t::game> a = m_dPtr->_games.at(0);
    }

    tourn_additional_stat_t::~tourn_additional_stat_t() {
        delete m_dPtr;
    }

    QVariant tourn_additional_stat_t::originData(const QModelIndex &idx) const
    {
        if (!idx.isValid()) {
            return m_dPtr->allData();
        }
        QVariant parentOriginData = originData(idx.parent());
        if (parentOriginData.isValid()) {
            return originData(parentOriginData, idx.row(), idx.column());
        }
        return QVariant();
    }

    QVariant tourn_additional_stat_t::originData(const QVariant &parentOriginData, int row, int column) const
    {
        if (!parentOriginData.isValid()) {
            if (row == 0 && column == 0) {
                return originData(QModelIndex());
            }
            return QVariant();
        }

        if (parentOriginData.canConvert<data_t>())
        {
            if (column != 0) {
                return QVariant();
            }
            data_t parentData = parentOriginData.value<data_t>();
            switch (field(row)) {
            case field::_id:                    return QVariant::fromValue(parentData._id);
            case field::_games:                 return QVariant::fromValue(parentData._games);
            case field::_blinds:                return QVariant::fromValue(parentData._blinds);
            case field::_prizes:                return QVariant::fromValue(parentData._prizes);
            case field::_late_register_ends_in: return QVariant::fromValue(parentData._late_register_ends_in);
            case field::_rebuys_count:          return QVariant::fromValue(parentData._rebuys_count);
            case field::_addons_count:          return QVariant::fromValue(parentData._addons_count);
            case field::_current_pos:           return QVariant::fromValue(parentData._current_pos);
            default:
                return QVariant();
            }
        }
        using games_type = decltype(data_t::_games);
        if (parentOriginData.canConvert<games_type>()) {
            auto parentData = parentOriginData.value<games_type>();
            if (row >= parentData.size()) {
                return QVariant();
            }
            switch (column) {
            case 0: return QVariant::fromValue(parentData.at(row)._id);
            case 1: return QVariant::fromValue(parentData.at(row)._title);
            case 2: return QVariant::fromValue(parentData.at(row)._players);
            default:
                return QVariant();
            }
        }
        if (parentOriginData.canConvert<decltype(data_t::_blinds)>()) {
            auto parentData = parentOriginData.value<decltype(data_t::_blinds)>();
            if (row >= static_cast<int>(parentData.size())) {
                return QVariant();
            }
            switch (column) {
            case 0: return QVariant::fromValue(parentData.at(row)._level);
            case 1: return QVariant::fromValue(parentData.at(row)._big_blind);
            case 2: return QVariant::fromValue(parentData.at(row)._ante);
            case 3: return QVariant::fromValue(parentData.at(row)._duration);
            default:
                return QVariant();
            }
        }
        if (parentOriginData.canConvert<decltype(data_t::_prizes)>()) {
            auto parentData = parentOriginData.value<decltype(data_t::_prizes)>();
            if (row >= static_cast<int>(parentData.size())) {
                return QVariant();
            }
            switch (column) {
            case 0: return QVariant::fromValue(parentData.at(row)._winner_pos);
            case 1: return QVariant::fromValue(parentData.at(row)._prize);
            default:
                return QVariant();
            }
        }
        if (parentOriginData.canConvert<decltype(games_type::value_type::_players)>()) {
            auto parentData = parentOriginData.value<decltype(games_type::value_type::_players)>();
            if (row >= static_cast<int>(parentData.size())) {
                return QVariant();
            }
            switch (column) {
            case 0: return QVariant::fromValue(parentData.at(row)._id);
            case 1: return QVariant::fromValue(parentData.at(row)._nickname);
            case 2: return QVariant::fromValue(parentData.at(row)._stack);
            case 3: return QVariant::fromValue(parentData.at(row)._pos);
            default:
                return QVariant();
            }
        }
        return QVariant();
    }



    gbp_i64 tourn_additional_stat_t::idAt() const {
        return m_dPtr->_id;
    }

    void tourn_additional_stat_t::resetData(const data_t &data)
    {
        beginResetModel();
        m_dPtr->reset(data);

        m_dPtr->_games.setInsertHandlers( [this](int first, int count){beginInsertRows(index(1, 0), first, first + count - 1);}
                                        , [this](){endInsertRows();});
        m_dPtr->_blinds.setInsertHandlers([this](int first, int count){beginInsertRows(index(2, 0), first, first + count - 1);}
                                        , [this](){endInsertRows();});
        m_dPtr->_prizes.setInsertHandlers([this](int first, int count){beginInsertRows(index(3, 0), first, first + count - 1);}
                                        , [this](){endInsertRows();});
        resetInternalData();
        endResetModel();
    }

    void tourn_additional_stat_t::updateData(const data_t &data)
    {
        //qDebug() << "tourn_additional_stat_t::updateData()" << data._id << idAt();
//        std::cout << data << std::endl;
        if (idAt() != data._id)
        {
            resetData(data);
        }
        else
        {
            setData(indexOf(field::_games),                 QVariant::fromValue(data._games),                 UserType::OriginDataRole);
            setData(indexOf(field::_blinds),                QVariant::fromValue(data._blinds),                UserType::OriginDataRole);
            setData(indexOf(field::_prizes),                QVariant::fromValue(data._prizes),                UserType::OriginDataRole);
            setData(indexOf(field::_late_register_ends_in), QVariant::fromValue(data._late_register_ends_in), UserType::OriginDataRole);
            setData(indexOf(field::_rebuys_count),          QVariant::fromValue(data._rebuys_count),          UserType::OriginDataRole);
            setData(indexOf(field::_addons_count),          QVariant::fromValue(data._addons_count),          UserType::OriginDataRole);
            setData(indexOf(field::_current_pos),           QVariant::fromValue(data._current_pos),           UserType::OriginDataRole);

        }
    }

    QModelIndex tourn_additional_stat_t::indexOf(field f) const
    {
        return index(underlying_cast(f), 0);
    }


    int tourn_additional_stat_t::rowCount(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return 8;
        }

        if (!parent.parent().isValid())
        {
            if (parent.column() != 0) {
                return 0;
            }
            switch (parent.row()) {
            case 1: return m_dPtr->_games.size();
            case 2: return m_dPtr->_blinds.size();
            case 3: return m_dPtr->_prizes.size();

            default:
                return 0;
            }
        }
        if (!parent.parent().parent().isValid() && Coord(parent.parent()).match(1, 0) && parent.column() == 2)
        {
            return m_dPtr->_games.at(parent.row())._players.size();
        }

        return 0;
    }

    int tourn_additional_stat_t::columnCount(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return 1;
        }
        QVariant d = originData(parent);
        if (d.isValid()) {
            if (d.canConvert<std::vector<data_t::game>>()) {
                return 3;
            }
            if (d.canConvert<std::vector<data_t::user_info>>()) {
                return 4;
            }
            if (d.canConvert<std::vector<data_t::blind>>()) {
                return 4;
            }
            if (d.canConvert<std::vector<data_t::prize>>()) {
                return 2;
            }
        }
        return 0;
    }

    QVariant tourn_additional_stat_t::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }

        if (role == UserType::VisibilityRole) {
            return true;
        }
        if (role == UserType::OriginDataRole) {
            return originData(index.parent(), index.row(), index.column());
        }


        if (index.parent().isValid()) {
            if (index.parent().parent().isValid() && field(index.parent().parent().row()) == field::_games && index.parent().column() == 2) {

                const auto& player = m_dPtr->_games.refAt(index.parent().row()).player(index.row());
                switch (role) {
                case Qt::DisplayRole:
                    switch (index.column()) {
                    case 0: return ns_meta::id_type::display(player._id);
                    case 1: return ns_meta::name_type::display(player._nickname);
                    case 2: return ns_meta::money_type::display(player._stack);
                    case 3: return ns_meta::pos_type::display(player._pos);
                    default:
                        return QVariant();
                    }
                case Qt::EditRole:
                    switch (index.column()) {
                    case 0: return ns_meta::id_type::edit(player._id);
                    case 1: return ns_meta::name_type::edit(player._nickname);
                    case 2: return ns_meta::money_type::edit(player._stack);
                    case 3: return ns_meta::pos_type::edit(player._pos);
                    default:
                        return QVariant();
                    }
                default:
                    return QVariant();
                }
                return QVariant();
            }

            switch (field(index.parent().row())) {
            case field::_games:
            {
                auto gamesItem = m_dPtr->_games.at(index.row());
                switch (role) {
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                    switch (index.column()) {
                    case 0: return QVariant::fromValue(gamesItem._id);
                    case 1: return QVariant::fromValue(gamesItem._title);
                    case 2: return QVariant::fromValue(gamesItem._players.size());
                    default:
                        return QVariant();
                    }
                default:
                    return QVariant();
                }
            }
            case field::_blinds:
            {
                const auto& item = m_dPtr->_blinds.at(index.row());
                switch (role) {
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                    switch (index.column()) {
                    case 0: return ns_meta::id_type::display(item._level);
                    case 1: return ns_meta::money_type::display(item._big_blind);
                    case 2: return ns_meta::money_type::display(item._ante);
                    case 3: return ns_meta::duration_type::display(item._duration);
                    default:
                        return QVariant();
                    }
                case Qt::EditRole:
                    switch (index.column()) {
                    case 0: return ns_meta::id_type::edit(item._level);
                    case 1: return ns_meta::money_type::edit(item._big_blind);
                    case 2: return ns_meta::money_type::edit(item._ante);
                    case 3: return ns_meta::duration_type::edit(item._duration);
                    default:
                        return QVariant();
                    }
                default:
                    return QVariant();
                }
            }
            case field::_prizes:
            {
                auto item = m_dPtr->_prizes.at(index.row());
                switch (role) {
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                    switch (index.column()) {
                    case 0: return ns_meta::pos_type::display(item._winner_pos);
                    case 1: return ns_meta::money_type::display(item._prize);
                    default:
                        return QVariant();
                    }
                case Qt::EditRole:
                    switch (index.column()) {
                    case 0: return ns_meta::pos_type::edit(item._winner_pos);
                    case 1: return ns_meta::money_type::edit(item._prize);
                    default:
                        return QVariant();
                    }
                default:
                    return QVariant();
                }
            }
            default:
                return QVariant();
            }
        }

        if (!index.parent().isValid())
        {
            switch (role) {
            case Qt::DisplayRole:
                switch (field(index.row())) {
                case field::_id:                    return ns_meta::id_type::display(m_dPtr->_id);
                case field::_games:                 return QVariant::fromValue(m_dPtr->_games.size());
                case field::_blinds:                return QVariant::fromValue(m_dPtr->_blinds.size());
                case field::_prizes:                return QVariant::fromValue(m_dPtr->_prizes.size());
                case field::_late_register_ends_in: return ns_meta::duration_type::display(m_dPtr->_late_register_ends_in);
                case field::_rebuys_count:          return ns_meta::count_type::display(m_dPtr->_rebuys_count);
                case field::_addons_count:          return ns_meta::count_type::display(m_dPtr->_addons_count);
                case field::_current_pos:           return ns_meta::pos_type::display(m_dPtr->_current_pos);

                default:
                    return QVariant();
                }
            case Qt::EditRole:
                switch (field(index.row())) {
                case field::_id:                    return ns_meta::id_type::edit(m_dPtr->_id);
                case field::_games:                 return ns_meta::generic_container_type<std::vector<data_t::game>>::edit((std::vector<data_t::game>)m_dPtr->_games);
                case field::_blinds:                return ns_meta::generic_container_type<std::vector<data_t::blind>>::edit(m_dPtr->_blinds);
                case field::_prizes:                return ns_meta::generic_container_type<std::vector<data_t::prize>>::edit(m_dPtr->_prizes);
                case field::_late_register_ends_in: return ns_meta::duration_type::edit(m_dPtr->_late_register_ends_in);
                case field::_rebuys_count:          return ns_meta::count_type::edit(m_dPtr->_rebuys_count);
                case field::_addons_count:          return ns_meta::count_type::edit(m_dPtr->_addons_count);
                case field::_current_pos:           return ns_meta::pos_type::edit(m_dPtr->_current_pos);

                default:
                    return QVariant();
                }
            default:
                return QVariant();
            }
        }


        return QVariant();
    }

    bool tourn_additional_stat_t::setData(const QModelIndex &idx, const QVariant &value, int role)
    {
        if (!idx.isValid()) {
            return false;
        }

        return false;
    }

    Qt::ItemFlags tourn_additional_stat_t::flags(const QModelIndex &index) const
    {
        if (!index.isValid()) {
            return Qt::NoItemFlags;
        }

        auto commonFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        return commonFlags | Qt::ItemIsEditable;
    }

    bool tourn_additional_stat_t::insertRows(int row, int count, const QModelIndex &parent)
    {
        if (!parent.isValid() || parent.parent().isValid() || parent.column() != 0) {
            return false;
        }

        switch (field(parent.row())) {
        case field::_games:  return m_dPtr->_games.insert(row, count);
        case field::_blinds: return m_dPtr->_blinds.insert(row, count);
        case field::_prizes: return m_dPtr->_prizes.insert(row, count);
        case field::_id:
        case field::_late_register_ends_in:
        case field::_rebuys_count:
        case field::_addons_count:
        case field::_current_pos:
        default:
            return false;
        }
    }

    bool tourn_additional_stat_t::removeRows(int row, int count, const QModelIndex &parent)
    {
        if (!parent.isValid() || parent.parent().isValid() || parent.column() != 0) {
            return false;
        }

        switch (field(parent.row())) {
        case field::_games:  return m_dPtr->_games.remove(row, count);
        case field::_blinds: return m_dPtr->_blinds.remove(row, count);
        case field::_prizes: return m_dPtr->_prizes.remove(row, count);
        case field::_id:
        case field::_late_register_ends_in:
        case field::_rebuys_count:
        case field::_addons_count:
        case field::_current_pos:
        default:
            return false;
        }
    }

    QModelIndex tourn_additional_stat_t::index(int row, int column, const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            if (column != 0) {
                return QModelIndex();
            }
            switch (field(row)) {
            case field::_id:                    return createIndex(row, column, nullptr);
            case field::_games:                 return createIndex(row, column, nullptr);
            case field::_blinds:                return createIndex(row, column, nullptr);
            case field::_prizes:                return createIndex(row, column, nullptr);
            case field::_late_register_ends_in: return createIndex(row, column, nullptr);
            case field::_rebuys_count:          return createIndex(row, column, nullptr);
            case field::_addons_count:          return createIndex(row, column, nullptr);
            case field::_current_pos:           return createIndex(row, column, nullptr);
            default:
                return QModelIndex();
            }
        }

        if (parent.internalPointer() == nullptr && parent.column() == 0)
        {
            switch (parent.row()) {
            case 1: return createIndex(row, column, (void*)&m_dPtr->_games);
            case 2: return createIndex(row, column, (void*)&m_dPtr->_blinds);
            case 3: return createIndex(row, column, (void*)&m_dPtr->_prizes);
            default:
                return QModelIndex();
            }
        }

        if (parent.internalPointer() == (void*)&m_dPtr->_games && parent.column() == 2) {
            return createIndex(row, column, (void*)&m_dPtr->_games.at(parent.row())._players);
        }
        return QModelIndex();
    }

    QModelIndex tourn_additional_stat_t::parent(const QModelIndex &child) const
    {
        if (!child.isValid() || child.internalPointer() == nullptr) {
            return QModelIndex();
        }

        if (child.internalPointer() == &m_dPtr->_games) {
            return index(1, 0);
        }
        if (child.internalPointer() == &m_dPtr->_blinds) {
            return index(2, 0);
        }
        if (child.internalPointer() == &m_dPtr->_prizes) {
            return index(3, 0);
        }

        for (size_t i = 0; i < m_dPtr->_games.size(); i++) {
            if (child.internalPointer() == &m_dPtr->_games.at(i)._players) {
                return createIndex(i, 2, (void*)&m_dPtr->_games);
            }
        }
        return QModelIndex();
    }

    bool tourn_additional_stat_t::hasChildren(const QModelIndex &parent) const
    {
//        const simple_item* p = parent.isValid() ? static_cast<const simple_item*>(parent.internalPointer()) : simple_item::root(id());
//        return !p->children.empty();
        if (!parent.isValid()) {
            return true;
        }
        if (!parent.parent().isValid()) {
            return parent.column() == 0 && parent.row() >=1 && parent.row() <= 3;
        }
        if (!parent.parent().parent().isValid()) {
            return parent.parent().column() == 0 && parent.parent().row() == 1  && parent.column() == 2;
        }
        return false;
    }

    QVariant tourn_additional_stat_t::headerData(int section, Qt::Orientation orientation, int role) const
    {
        switch (role) {
        case Qt::DisplayRole:
            return section;
        case Qt::ToolTipRole:
            return ns_util::enumToString(field(section));
        default:
            return QVariant();
        }

        return QVariant();
    }

} //namespace ns_model



bool ns_model::tourn_additional_stat_t::read(QIODevice *device)
{
    if (device != nullptr && device->open(QIODevice::ReadOnly))
    {
        QDataStream stream(device);
        games_stat_t::second_type buff;
//        stream >> buff;
        //TODO: Implement (stream >> tourn_additional_stat_t)
        device->close();
//        updateItems(*this, m_data, buff);
        return true;
    }
    std::cout << "Read Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
    return false;
}

bool ns_model::tourn_additional_stat_t::write(QIODevice *device) const
{
    if (device != nullptr && device->open(QIODevice::WriteOnly))
    {
        QDataStream stream(device);
//        stream << m_data;
        //TODO: Implement (stream << tourn_additional_stat_t)
        device->close();
        return true;
    }
    std::cout << "Write Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
    return false;
}

int ns_model::tourn_additional_stat_t::rowById(gbp_i64 id, const void *data, const QModelIndex &parent) const
{
    if (!hasChildren(parent)) {
        return -1;
    }
    if (parent.column() == 1) {
        const auto* d = (data != nullptr) ? static_cast<const std::vector<gbp::ns_api::ns_statistic::tourn_additional_stat_t::game>*>(data) : (&m_dPtr->_games._items);
        for (size_t row = 0; row < d->size(); row++) {
            if (get_id(&d->at(row)).ref() == id) {
                return row;
            }
        }
    }
    else if (parent.parent().column() == 0 && parent.parent().row() == 1  && parent.column() == 2) {
        const auto* d = (data != nullptr) ? static_cast<const std::vector<gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info>*>(data) : (&m_dPtr->_games.at(parent.row())._players);
        for (size_t row = 0; row < d->size(); row++) {
            if (get_id(&d->at(row)).ref() == id) {
                return row;
            }
        }
    }
    return -1;
}

int ns_model::tourn_additional_stat_t::propertyIndex(const QByteArray &name, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        static const PropertyIndex<gbp::ns_api::ns_statistic::tourn_additional_stat_t> idx;
        return idx.indexOf(name);
    }
    if (parent.parent().isValid()) {
        if (parent.parent().column() == 2) {
            static const PropertyIndex<gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info> idx;
            return idx.indexOf(name);
        }
        return -1;
    }
    if (parent.column() != 0) {
        return -1;
    }
    static const PropertyIndex<gbp::ns_api::ns_statistic::tourn_additional_stat_t::game> idx1;
    static const PropertyIndex<gbp::ns_api::ns_statistic::tourn_additional_stat_t::blind> idx2;
    static const PropertyIndex<gbp::ns_api::ns_statistic::tourn_additional_stat_t::prize> idx3;

    switch (parent.row()) {
    case 1:
        return idx1.indexOf(name);
    case 2:
        return idx2.indexOf(name);
    case 3:
        return idx3.indexOf(name);
    default:
        return -1;
    }
}

bool ns_model::tourn_additional_stat_t::removeItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
{
    //TODO: Implement ns_model::tourn_additional_stat_t::removeItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
    return false;
}

bool ns_model::tourn_additional_stat_t::insertItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
{
    //TODO: Implement ns_model::tourn_additional_stat_t::insertItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
    return false;
}
