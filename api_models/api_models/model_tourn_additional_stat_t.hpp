#pragma once

#include "api_models_global.h"
#include <tools/types_fwd.hpp>
#include <tools/accessor.hpp>
#include "model_apibase.hpp"


namespace ns_model
{
    class tourn_additional_stat_t : public ApiBase
    {
        Q_OBJECT
    public:
        using data_t = gbp::ns_api::ns_statistic::tourn_additional_stat_t;

        enum class field
        {
            _id = 0,
            _games,
            _blinds,
            _prizes,
            _late_register_ends_in,
            _rebuys_count,
            _addons_count,
            _current_pos,

            count
        };
        Q_ENUM(field)

        gbp_i64 idAt() const;

    private:
        struct PrivateData;
        PrivateData* m_dPtr;
    public slots:
        void resetData(const data_t& data);
        void updateData(const data_t& data);
    public:
        explicit tourn_additional_stat_t(gbp_i64 idAt = -1, QObject *parent = 0);
        virtual ~tourn_additional_stat_t();

        QVariant originData(const QModelIndex& idx = QModelIndex()) const;
        QVariant originData(const QVariant& parentOriginData, int row, int column = 0) const;

//        virtual QModelIndex indexOf(gbp_i64 id) const override;
        QModelIndex indexOf(field f) const;

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

        virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;

        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        virtual bool isItemOrientedData() const override { return false; }


        // IOModel interface
    public:
        virtual bool read(QIODevice *device) override;
        virtual bool write(QIODevice *device) const override;

        // ApiBase interface
    public:
        virtual int rowById(gbp_i64 id, const void *data, const QModelIndex &parent) const override;
        virtual int propertyIndex(const QByteArray &name, const QModelIndex &parent) const override;
        virtual bool removeItems(const QList<gbp_i64> &ids, const QModelIndex &parent) override;
        virtual bool insertItems(const QList<gbp_i64> &ids, const QModelIndex &parent) override;
    };


} //namespace ns_model

gbp::ns_api::ns_statistic::tourn_additional_stat_t createRandomItem(gbp_i64 id);
