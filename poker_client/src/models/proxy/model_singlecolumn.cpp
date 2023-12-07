#include <precompiled.h>
#include <qpainter.h>
#include "model_singlecolumn.h"
#include <PokerApplication.h>
#include <utils/IconProvider.h>
#include <utils/Utils.h>
#include <core/money.h>

namespace
{

}

namespace ns_model
{
    template<>
    QSize SingleColumn::join(const QList<QSize> &lst, int /*role*/) const {
        QSize result(0, 0);
        for (const QSize& s: lst) {
            result.rwidth() += s.width();
            result.setHeight(qMax(result.height(), s.height()));
        }
        return result;
    }

    SingleColumn::SingleColumn(QObject *parent)
        : QIdentityProxyModel(parent)
        , m_roles({Qt::DisplayRole, Qt::ToolTipRole, Qt::DecorationRole, Qt::SizeHintRole})
        , m_singleColumnEnabled(false)
    {
//        initMapping();
    }

    int SingleColumn::columnCount(const QModelIndex &parent) const {
        if (isSingleColumnEnabled()) {
            return 1;//QIdentityProxyModel::columnCount(parent);
        }
        return QIdentityProxyModel::columnCount(parent);
    }

    QVariant SingleColumn::data(const QModelIndex &index, int role) const
    {
        if (!isSingleColumnEnabled()) {
            return QIdentityProxyModel::data(index, role);
        }
        if (!index.isValid()) {
            return QVariant();
        }
        int sourceColumnCount = QIdentityProxyModel::columnCount(index.parent());
        if (sourceColumnCount < 2 || role == UserType::RowDataRole || role == UserType::RowItemIdRole) {
            return QIdentityProxyModel::data(index, role);
        }


        if (index.column() == 0 && m_roles.contains(role)) {
            int row = index.row();
            QVariantList rowData;
            for (int column = 0; column < sourceColumnCount; column++) {
                rowData << QIdentityProxyModel::data(index.sibling(row, column), role);
            }
            switch (role) {
//            case UserType::RowDataRole:
//                return rowData;
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
            {
                QStringList lst;
                for (const QVariant& value: rowData) {
                    if (value.canConvert<QString>()) {
                        lst << value.toString();
                    }
                }
                return lst.join(" ");
            }
            case Qt::DecorationRole:
            {
                QList<QIcon> lst;
                QList<QSize> sizeLst;
                for (int column = 0; column < rowData.size(); column++) {
                    const auto& value = rowData.at(column);
                    if (value.canConvert<QIcon>()) {
                        lst << value.value<QIcon>();
                        sizeLst << QIdentityProxyModel::data(index.sibling(row, column), Qt::SizeHintRole).toSize();
                    }
                }
                QSize size = data(index, Qt::SizeHintRole).toSize();
                if (!size.isValid()) {
                    return QVariant();
                }
                QPixmap pix(size);
                pix.fill(Qt::transparent);
                QPainter p;
                QRect rc(QPoint(0, 0), QSize(16, 16));
                p.begin(&pix);
                if (p.isActive())
                {
                    for (int column = 0; column < rowData.size(); column++) {
                        rc.setSize(sizeLst.at(column));
                        lst[column].paint(&p, rc);
                        rc.moveRight(rc.width());
                    }
                }
                p.end();
                return QVariant::fromValue(pix);
            }
            case Qt::SizeHintRole:
            {
                QList<QSize> lst;
                for (const QVariant& value: rowData) {
                    if (value.canConvert<QSize>()) {
                        lst << value.value<QSize>();
                    }
                }
                return join(lst, Qt::SizeHintRole);
            }
            default:
                return QIdentityProxyModel::data(index, role);
            }
        }
        return QIdentityProxyModel::data(index, role);
    }

    bool SingleColumn::isSingleColumnEnabled() const {
        return m_singleColumnEnabled;
    }

    void SingleColumn::enableSingleColumn(bool enable)
    {
        if (m_singleColumnEnabled == enable) {
            return;
        }
        beginResetModel();
        m_singleColumnEnabled = enable;
        emit singleColumnEnabledToggled(enable);
        endResetModel();
    }



} //namespace ns_model





