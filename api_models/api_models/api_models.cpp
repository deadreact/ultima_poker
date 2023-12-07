#include "api_models.hpp"

#include <api_util.hpp>
#include <QtWidgets/qwidget.h>
#include <QDebug>



namespace ns_model
{
    Subscriber::Subscriber(QObject *owner, ModelMapper *mapper)
        : m_owner(owner)
        , m_mapper(mapper)
        , m_callback0Param(on_item_data_changed())
    {
        QObject::connect(mapper, &ModelMapper::destroyed, [this]{
            m_mapper = nullptr;
        });
        QObject::connect(owner, &ModelMapper::destroyed, [this]{
            m_owner = nullptr;
        });
    }

    Subscriber::~Subscriber()
    {
        if (m_mapper != nullptr) {
            m_mapper->removeSubscriber(this);
        }
    }

    void Subscriber::invoke(const QVariant &value) {
        if (m_callback0Param) {
            m_callback0Param(value);
        }
    }

    void Subscriber::invoke(const QVariant &value, int param)
    {
        if (m_callback1Param) {
            m_callback1Param(value, param);
        } else if (m_callback0Param) {
            m_callback0Param(value);
        }
    }

    void Subscriber::setCallback(on_item_data_changed callback) {
        m_callback0Param = std::move(callback);
//        if (m_callback0Param) {
//            m_callback1Param = on_slice_data_changed();
//        }
    }

    void Subscriber::setCallback(on_slice_data_changed callback) {
        m_callback1Param = std::move(callback);
//        if (m_callback1Param) {
//            m_callback0Param = on_item_data_changed();
//        }
    }

    ModelMapper::ModelMapper(QObject *parent)
        : QObject(parent)
        , m_model(nullptr)
        , m_indexParentGetter(on_get_parent())
        , m_nowResettingModel(false)
    {
//        resetIndexParentGetter(on_get_parent(), false);
    }


    void ModelMapper::setModel(QAbstractItemModel *model) {
        if (m_model != model) {
            onModelAboutToReset();
            m_nowResettingModel = true;
            disconnectFromModel();
            m_model = model;
            connectToModel();
            m_nowResettingModel = false;
            onModelReset();
            emit modelChanged(model);
        }
    }


    QAbstractItemModel *ModelMapper::model() const {
        return m_model;
    }

    void ModelMapper::resetIndexParentGetter(on_get_parent getter, bool resetData)
    {
        m_indexParentGetter = std::move((getter) ? getter : [this](QAbstractItemModel* /*model*/){ return QModelIndex(); });
        if (resetData && !m_nowResettingModel) {
            onModelReset();
        }
    }

    QModelIndex ModelMapper::indexParent() const {
        if (m_model && m_indexParentGetter) {
            return m_indexParentGetter(m_model);
        }
        return QModelIndex();
    }

    bool ModelMapper::acceptsParent(const QModelIndex &child) const {
        const QModelIndex& p = indexParent();
        return isAncestorOf(child, p);
    }

    bool ModelMapper::isAncestorOf(const QModelIndex &child, const QModelIndex &parent) {
        return child.isValid() && (child.parent() == parent || isAncestorOf(child.parent(), parent));
    }

    Subscriber* ModelMapper::addSubscriber(QObject *owner, int row, int column, int role, on_item_data_changed callback)
    {
        Subscriber* s = new Subscriber(owner, this);
        s->setCallback(std::move(callback));
        m_subscribers[row][column][role].insert(s);

        return s;
    }

    Subscriber *ModelMapper::addSubscriber(QObject *owner, int row, int column, int role, on_slice_data_changed callback)
    {
        Subscriber* s = new Subscriber(owner, this);
        s->setCallback(std::move(callback));
        m_subscribers[row][column][role].insert(s);

        return s;
    }
    int ModelMapper::removeSubscriber(Subscriber* s)
    {
        int counter = 0;
        for (auto& rowSubs: m_subscribers) {
            for (auto& colSubs: rowSubs) {
                for (auto& roleSubs: colSubs) {
                    auto it = roleSubs.find(s);
                    if (it != roleSubs.end()) {
                        roleSubs.erase(it);
                        counter++;
                    }
                }
            }
        }
        return counter;
    }

    QSet<Subscriber*> ModelMapper::takeSubscribers(int row, int column, int role) {
        QSet<Subscriber*> subs = m_subscribers[row][column][role];
        m_subscribers[row][column].remove(role);
        return subs;
    }

    SubscribersOnRole ModelMapper::takeSubscribers(int row, int column) {
        auto subs = m_subscribers[row][column];
        m_subscribers[row].remove(column);
        return subs;
    }

    SubscribersOnSlice ModelMapper::takeSubscribers(int row) {
        auto subs = m_subscribers[row];
        m_subscribers.remove(row);
        return subs;
    }

    void ModelMapper::forceUpdateAll()
    {
        onModelReset();
    }


    void ModelMapper::connectToModel()
    {
        if (m_model != nullptr)
        {
            qDebug() << "connect to model" << m_model;
            connect(m_model, &QAbstractItemModel::dataChanged, this, &ModelMapper::onDataChanged);
            connect(m_model, &QAbstractItemModel::modelAboutToBeReset, this, &ModelMapper::onModelAboutToReset);
            connect(m_model, &QAbstractItemModel::modelReset, this, &ModelMapper::onModelReset);
        }
    }


    void ModelMapper::disconnectFromModel()
    {
        if (m_model != nullptr)
        {
            qDebug() << "disconnect from model" << m_model;
            disconnect(m_model, &QAbstractItemModel::dataChanged, this, &ModelMapper::onDataChanged);
            disconnect(m_model, &QAbstractItemModel::modelAboutToBeReset, this, &ModelMapper::onModelAboutToReset);
            disconnect(m_model, &QAbstractItemModel::modelReset, this, &ModelMapper::onModelReset);
        }
    }

    /*-------------------*/
    void updateSubscriberGroupAtColumn(const SubscribersOnRole& subscriberGroup, const QModelIndex& index, const QVector<int>& roles = ALL_ROLES)
    {
        if (subscriberGroup.isEmpty()) {
            return;
        }
        QVariant data;
        int column = index.column();
        if (!roles.isEmpty()) {
            if (index.isValid()) {
                for (int role: roles) {
                    data = index.data(role);
                    for (Subscriber* subscriber : subscriberGroup.value(role)) {
                        subscriber->invoke(data, column);
                    }
                }
            } else {
                for (int role: roles) {
                    for (Subscriber* subscriber : subscriberGroup.value(role)) {
                        subscriber->invoke(data, column);
                    }
                }
            }
        } else {
            if (index.isValid()) {
                for (auto roleIt = subscriberGroup.begin(); roleIt != subscriberGroup.end(); roleIt++) {
                    data = index.data(roleIt.key());
                    for (Subscriber* subscriber: roleIt.value()) {
                        subscriber->invoke(data, column);
                    }
                }
            } else {
                for (auto roleIt = subscriberGroup.begin(); roleIt != subscriberGroup.end(); roleIt++) {
                    for (Subscriber* subscriber: roleIt.value()) {
                        subscriber->invoke(data, column);
                    }
                }
            }
        }
    }
    void updateSubscriberGroupAtRow(const SubscribersOnRole& subscriberGroup, const QModelIndex& index, const QVector<int>& roles = ALL_ROLES)
    {
        if (subscriberGroup.isEmpty()) {
            return;
        }

        QVariant data;
        int row = index.row();
        if (!roles.isEmpty()) {
            if (index.isValid()) {
                for (int role: roles) {
                    data = index.data(role);
                    for (Subscriber* subscriber : subscriberGroup.value(role)) {
                        subscriber->invoke(data, row);
                    }
                }
            } else {
                for (int role: roles) {
                    for (Subscriber* subscriber : subscriberGroup.value(role)) {
                        subscriber->invoke(data, row);
                    }
                }
            }
        } else {
            if (index.isValid()) {
                for (auto roleIt = subscriberGroup.begin(); roleIt != subscriberGroup.end(); roleIt++) {
                    data = index.data(roleIt.key());
                    for (Subscriber* subscriber: roleIt.value()) {
                        subscriber->invoke(data, row);
                    }
                }
            } else {
                for (auto roleIt = subscriberGroup.begin(); roleIt != subscriberGroup.end(); roleIt++) {
                    for (Subscriber* subscriber: roleIt.value()) {
                        subscriber->invoke(data, row);
                    }
                }
            }
        }
    }
    void updateSubscriberGroup(const SubscribersOnRole& subscriberGroup, const QModelIndex& index, const QVector<int>& roles = ALL_ROLES)
    {
        if (subscriberGroup.isEmpty()) {
            return;
        }
        QVariant data;
        if (!roles.isEmpty()) {
            if (index.isValid()) {
                for (int role: roles) {
                    data = index.data(role);
                    for (Subscriber* subscriber : subscriberGroup.value(role)) {
                        subscriber->invoke(data);
                    }
                }
            } else {
                for (int role: roles) {
                    for (Subscriber* subscriber : subscriberGroup.value(role)) {
                        subscriber->invoke(data);
                    }
                }
            }
        } else {
            if (index.isValid()) {
                for (auto roleIt = subscriberGroup.begin(); roleIt != subscriberGroup.end(); roleIt++) {
                    data = index.data(roleIt.key());
                    for (Subscriber* subscriber: roleIt.value()) {
                        subscriber->invoke(data);
                    }
                }
            } else {
                for (auto roleIt = subscriberGroup.begin(); roleIt != subscriberGroup.end(); roleIt++) {
                    for (Subscriber* subscriber: roleIt.value()) {
                        subscriber->invoke(data);
                    }
                }
            }
        }
    }

    /*-------------------*/


    void ModelMapper::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
    {
        if (m_nowResettingModel) {
            return;
        }

        if (topLeft.isValid() && bottomRight.isValid() && acceptsParent(topLeft))
        {
            QModelIndex p1 = topLeft.parent();
            QModelIndex p2 = bottomRight.parent();
            Q_ASSERT((!p1.isValid() && !p2.isValid()) || p1 == p2);
            for (int row = topLeft.row(); row <= bottomRight.row(); row++) {
                for (int column = topLeft.column(); column <= bottomRight.column(); column++) {
                    updateSubscriberGroup(m_subscribers.value(row).value(column), topLeft.sibling(row, column), roles);
                    updateSubscriberGroupAtRow(m_subscribers.value(ALL_ROWS).value(column), topLeft.sibling(row, column), roles);
                    updateSubscriberGroupAtColumn(m_subscribers.value(row).value(ALL_COLUMNS), topLeft.sibling(row, column), roles);
                }
            }
        }
    }


    void ModelMapper::onModelAboutToReset() {
        qDebug() << "about to reset";
    }


    void ModelMapper::onModelReset() {
        qDebug() << "reset";
        QModelIndex p = indexParent();
        int lastRow = m_model->rowCount(p) - 1;
        int lastCol = m_model->columnCount(p) - 1;
        qDebug() << "parent" << p;
        qDebug() << "rows" << "0 -" << lastRow;
        qDebug() << "columns" << "0 -" << lastCol;
        if (lastRow >= 0 && lastCol >= 0) {
            QModelIndex topLeft = m_model->index(0, 0, p);
            QModelIndex bottomRight = m_model->index(lastRow, lastCol, p);
            qDebug() << topLeft << "-" << bottomRight;
            onDataChanged(topLeft, bottomRight);
        }
    }


} // namespace ns_model



namespace ns_model
{
bool registerDisplayWidget(ns_model::user_info_t *model, QWidget *displayWidget, user_info_t::field f, int role, const QByteArray &property)
{
    displayWidget->setProperty(property, model->data(model->index(underlying_cast(f)), role));
    displayWidget->connect(model, &user_info_t::dataChanged, displayWidget
                           , [model, displayWidget, f, property, role](const QModelIndex& idx, const QModelIndex&, const QVector<int>& roles) {
        if ((user_info_t::field(idx.row()) == f) && (roles.empty() || roles.contains(role) || roles.contains(Qt::EditRole))) {
            QByteArray target = property;
            if ((property.isEmpty() || property.isNull()) && displayWidget->metaObject()->userProperty().isValid()) {
                target = displayWidget->metaObject()->userProperty().name();
            }
            displayWidget->setProperty(target, model->data(model->index(underlying_cast(f)), role));
        }
    });
    displayWidget->connect(model, &user_info_t::modelReset, displayWidget
                           , [model, displayWidget, f, property, role] {
        QByteArray target = property;
        if ((property.isEmpty() || property.isNull()) && displayWidget->metaObject()->userProperty().isValid()) {
            target = displayWidget->metaObject()->userProperty().name();
        }
        displayWidget->setProperty(target, model->data(model->index(underlying_cast(f)), role));
    });

    return true;
}

bool bind(QWidget *widget, const QModelIndex &index, int role, const QByteArray &property)
{
    if (!index.isValid()) {
        return false;
    }
    widget->setProperty("modelIndex", index);
    const QAbstractItemModel* model = index.model();
        QVariant id = index.data(UserType::InternalIdRole);
        widget->setProperty(property, index.data(role));

        auto updateFn = [widget, id, property, role](const QModelIndex& idx, const QModelIndex&, const QVector<int>& roles) {
            qDebug() << widget->objectName() << "updateFn";
            if ((id == idx.data(UserType::InternalIdRole)) && (roles.empty() || roles.contains(role))) {
                QByteArray target = property;
                if ((property.isEmpty() || property.isNull()) && widget->metaObject()->userProperty().isValid()) {
                    target = widget->metaObject()->userProperty().name();
                }
                widget->setProperty(target, idx.data(role));
            }
        };
        auto resetFn = [model, widget, id, property, role] {
            qDebug() << widget->objectName() << "resetFn";
            auto lst = model->match(model->index(0, 0), UserType::InternalIdRole, id);
            if (!lst.isEmpty()) {
                QModelIndex idx = lst.first();
                QByteArray target = property;
                if ((property.isEmpty() || property.isNull()) && widget->metaObject()->userProperty().isValid()) {
                    target = widget->metaObject()->userProperty().name();
                }
                widget->setProperty(target, idx.data(role));
            }
        };


        widget->setProperty(property, index.data(role));
        widget->connect(model, &QAbstractItemModel::dataChanged, widget, updateFn);
        widget->connect(model, &QAbstractItemModel::modelReset, widget, resetFn);

        return true;
    }
} //namespace ns_model
