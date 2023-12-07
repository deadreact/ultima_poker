#pragma once

#include <QIdentityProxyModel>

namespace ns_model
{
    class SingleColumn : public QIdentityProxyModel
    {
        Q_OBJECT
        Q_PROPERTY(bool singleColumnEnabled READ isSingleColumnEnabled WRITE enableSingleColumn NOTIFY singleColumnEnabledToggled)

        QVector<int> m_roles;
        bool m_singleColumnEnabled;
    public:
        SingleColumn(QObject* parent = nullptr);

    	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        bool isSingleColumnEnabled() const;
    public slots:
        void enableSingleColumn(bool enable);

    signals:
        void singleColumnEnabledToggled(bool);

    protected:
        template <typename T>
        T join(const QList<T>& lst, int role) const;
    };


} //namespace ns_model
