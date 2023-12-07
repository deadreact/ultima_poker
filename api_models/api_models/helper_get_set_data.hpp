#pragma once

#include <basewidgets/types.h>
#include <qabstractitemmodel.h>
#include "defs_metatype.hpp"

namespace ns_model
{
    class SetDataInfo
    {
        Q_GADGET
    public:
        enum Result
        {
            Empty                    = 0x000000, // 1 - 0xffff номер строки
            Success                  = 0x010000,
            InvalidIndex             = 0x020000,
            SameValue                = 0x040000,
            TypesMismatchCanConvert  = 0x080000,
            TypesMismatchNoConverter = 0x100000,
//            Mixed
//            CannotSetAtLine
            Fail                     = 0xffff
        };
        Q_FLAG(Result)
        Q_DECLARE_FLAGS(Results, Result)

        static const char* thisFile();
    };

    template <int R, typename T>
    struct Helper
    {
        using Type = T;
        static const int role = R;
        static QVariant getData(const QModelIndex& index, const T& data);
        static SetDataInfo::Results setData(const QModelIndex& index, T& data, const QVariant& value);
    };

    /*******************************************************************************************************/
    /****************************************** DECLARATIONS ***********************************************/
    /*******************************************************************************************************/
    template <int R> using Helper_TS = Helper<R, gbp::ns_api::ns_table::table_state_t>;
    using Helper_TS_OriginRole       = Helper_TS<UserType::OriginDataRole>;
    using Helper_TS_RowDataRole      = Helper_TS<UserType::RowDataRole>;
    using Helper_TS_InternalIdRole   = Helper_TS<UserType::InternalIdRole>;
    using Helper_TS_DisplayRole      = Helper_TS<Qt::DisplayRole>;
    using Helper_TS_DecorationRole   = Helper_TS<Qt::DecorationRole>;
    using Helper_TS_ToolTipRole      = Helper_TS<Qt::ToolTipRole>;
    using Helper_TS_WhatsThisRole    = Helper_TS<Qt::WhatsThisRole>;
    using Helper_TS_EditRole         = Helper_TS<Qt::EditRole>;
    using Helper_TS_BgRole           = Helper_TS<Qt::BackgroundRole>;
    using Helper_TS_FgRole           = Helper_TS<Qt::ForegroundRole>;


    QList<QVariantList> childrenData(const QModelIndex& parent, int role = Qt::DisplayRole);
    QString makeString(const QList<QVariantList>& childrenData);

    inline QString displayChildren(const QModelIndex& index) {
        return makeString(childrenData(index));
    }

    template <typename T>
    int get_metatype_id() {
        static const int id = qRegisterMetaType<T>();
        return id;
    }

    template <typename T>
    int get_metatype_id(const T& obj) {
        return get_metatype_id<T>();
    }

    namespace ns_detail
    {
        struct table_state_parent_type
        {
            static const quintptr invalid        = 0;
            static const quintptr root           = 1;
            static const quintptr players        = 2;
            static const quintptr tableCards     = 3;
            static const quintptr tablePots      = 4;
            static const quintptr player_cards[];
            static const quintptr player_state[];
        };
    }

} // namespace ns_model
Q_DECLARE_OPERATORS_FOR_FLAGS(ns_model::SetDataInfo::Results)

