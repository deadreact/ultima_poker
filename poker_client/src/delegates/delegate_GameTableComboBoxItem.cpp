#include <precompiled.h>
#include "delegate_GameTableComboBoxItem.h"

#include <QPainter>
#include <QDebug>
#include <qsvgrenderer.h>
#include <basewidgets/types.h>

namespace {
//    QPoint operator*(const QPoint& p, const QSize& s) {
//        return QPoint(p.x()*s.width(), p.y()*s.height());
//    }
//    QSize operator*(const QSize& s, const QPoint& p) {
//        return QSize(p.x()*s.width(), p.y()*s.height());
//    }

    QPoint operator-(const QPoint& p, const QSize& s) {
        return QPoint(p.x() - s.width(), p.y() - s.height());
    }
}

namespace ns_delegate
{
    ColorBoxItem::ColorBoxItem(QObject *parent)
        : QStyledItemDelegate(parent)
        , m_baseSize(49)
    {

    }
    void ColorBoxItem::paint(QPainter *p, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {

        p->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());

        QStyledItemDelegate::paint(p, option, index);
    }

    // --------------------------------------------------------------------------------------------------------- //
    CardItem::CardItem(QObject *parent)
        : QStyledItemDelegate(parent)
        , m_renderer(new QSvgRenderer)
    {}
    void CardItem::paint(QPainter *p, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        static const QSize sizeOfFrame(47, 68);
        m_renderer->load(index.data(UserType::SvgDataRole).toByteArray());
        m_renderer->render(p, QRectF(option.rect.center() - sizeOfFrame/2, sizeOfFrame));

        QStyledItemDelegate::paint(p, option, index);
    }

    FilterItem::FilterItem(QObject *parent)
        : QItemDelegate(parent)
    {
//        setClipping(false);
    }

    void FilterItem::paint(QPainter *p, const QStyleOptionViewItem &o, const QModelIndex &i) const
    {
        QStyleOptionViewItem opt(o);
        opt.rect.adjust(-10, 0, 10, 0);

        QItemDelegate::paint(p, opt, i);
    }

} //namespace ns_delegate
