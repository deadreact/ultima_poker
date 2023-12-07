#include <precompiled.h>
#include "delegate_customcolumn.h"

#include <QPainter>
#include <basewidgets/types.h>
#include <QItemDelegate>


namespace ns_delegate
{
    class PrivateDelegate : public QItemDelegate
    {
    public:
        PrivateDelegate(QObject* parent = nullptr): QItemDelegate(parent) {}
        virtual ~PrivateDelegate() {}
        virtual void drawDisplay(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QString &text) const override {
            QItemDelegate::drawDisplay(p, opt, rc, text);
        }
    };

    CustomColumn::CustomColumn(int left, int top, int right, int bottom, QObject *parent)
        : QStyledItemDelegate(parent)
        , m_delegate(new PrivateDelegate(this))
        , m_textRectAdjust{left, top, right, bottom}
    {

    }

    void CustomColumn::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &idx) const
    {
        QStyledItemDelegate::paint(p, opt, idx);
        if (opt.state & QStyle::State_Selected) {
            p->setPen(QColor(0x191919));
        } else {
            p->setPen(idx.data(Qt::ForegroundRole).value<QColor>());
        }
        QRect rc = opt.rect.adjusted(m_textRectAdjust[0], m_textRectAdjust[1], m_textRectAdjust[2], m_textRectAdjust[3]);
        p->drawText(rc, idx.data(Qt::DisplayRole).toString());
    }

    void CustomColumn::initStyleOption(QStyleOptionViewItem *opt, const QModelIndex &idx) const
    {
        QStyledItemDelegate::initStyleOption(opt, idx);
        opt->text = "";
    }
} //namespace ns_delegate




