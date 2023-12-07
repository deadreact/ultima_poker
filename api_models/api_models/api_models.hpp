#pragma once

#include "api_models_global.h"
#include "qnamespace.h"
#include "model_user_info_t.hpp"
#include <functional>
#include <qset.h>
#include <qmap.h>

#define ALL_ROLES QVector<int>()
#define ALL_ROWS -1
#define ALL_COLUMNS -1

class QWidget;
class QByteArray;

namespace ns_model
{
    bool API_MODEL_DLL registerDisplayWidget(user_info_t* model, QWidget *displayWidget, user_info_t::field f, int role = Qt::DisplayRole, const QByteArray &property = "text");
    bool API_MODEL_DLL bind(QWidget *widget, const QModelIndex& index, int role = Qt::DisplayRole, const QByteArray &property = "text");

    using on_item_data_changed = std::function<void(const QVariant&)>;
    using on_slice_data_changed = std::function<void(const QVariant&, int)>;
    using on_get_parent = std::function<QModelIndex(QAbstractItemModel* model)>;

    class ModelMapper;

    class API_MODEL_DLL Subscriber
    {
        QObject* m_owner;
        ModelMapper* m_mapper;
        on_item_data_changed m_callback0Param;
        on_slice_data_changed m_callback1Param;
    public:
        Subscriber(QObject* owner, ModelMapper* mapper);
        ~Subscriber();

        void invoke(const QVariant& value);
        void invoke(const QVariant& value, int param);
        void setCallback(on_item_data_changed callback);
        void setCallback(on_slice_data_changed callback);
    };

    using SubscribersOnRole  = QMap<int, QSet<Subscriber*>>;
    using SubscribersOnSlice = QMap<int, SubscribersOnRole>;
    using SubscribersOnIndex = QMap<int, SubscribersOnSlice>;

    class API_MODEL_DLL ModelMapper : public QObject
    {
        Q_OBJECT
    private:
        QAbstractItemModel* m_model;
        SubscribersOnIndex m_subscribers; // set[row][column][role]
        on_get_parent m_indexParentGetter;
        bool m_nowResettingModel;
    public:
        ModelMapper(QObject *parent = nullptr);

        void setModel(QAbstractItemModel* model);
        QAbstractItemModel* model() const;

        static bool isAncestorOf(const QModelIndex& child, const QModelIndex &parent);

        void resetIndexParentGetter(on_get_parent getter = on_get_parent(), bool resetData = true);
        QModelIndex indexParent() const;
        bool acceptsParent(const QModelIndex& child) const;

        Subscriber* addSubscriber(QObject* owner, int row, int column, int role, on_item_data_changed callback);
        Subscriber* addSubscriber(QObject* owner, int row, int column, int role, on_slice_data_changed callback);
        inline Subscriber* addSubscriber(QObject* owner, int row, int column, on_item_data_changed callback) {
            return addSubscriber(owner, row, column, callback);
        }
        inline Subscriber* addSubscriber(QObject* owner, int row, on_item_data_changed callback) {
            return addSubscriber(owner, row, 0, callback);
        }

        inline Subscriber* addSubscriberOnRow(QObject* owner, int column, int role, on_slice_data_changed callback) {
            return addSubscriber(owner, ALL_ROWS, column, role, callback);
        }
        inline Subscriber* addSubscriberOnRow(QObject* owner, int column, on_slice_data_changed callback) {
            return addSubscriberOnRow(owner, column, Qt::DisplayRole, callback);
        }
        inline Subscriber* addSubscriberOnColumn(QObject* owner, int row, int role, on_slice_data_changed callback) {
            return addSubscriber(owner, row, ALL_COLUMNS, role, callback);
        }
        inline Subscriber* addSubscriberOnColumn(QObject* owner, int row, on_slice_data_changed callback) {
            return addSubscriberOnColumn(owner, row, Qt::DisplayRole, callback);
        }

        int removeSubscriber(Subscriber* s);
        QSet<Subscriber*>  takeSubscribers(int row, int column, int role);
        SubscribersOnRole  takeSubscribers(int row, int column);
        SubscribersOnSlice takeSubscribers(int row);
    public slots:
        void forceUpdateAll();
    protected:
        virtual void connectToModel();
        virtual void disconnectFromModel();

    protected slots:
        virtual void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = ALL_ROLES);
        virtual void onModelAboutToReset();
        virtual void onModelReset();
    signals:
        void modelChanged(QAbstractItemModel*);
    };

} //namespace ns_model
