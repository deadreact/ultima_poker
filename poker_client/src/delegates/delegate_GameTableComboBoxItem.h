#pragma once

#include <QItemDelegate>
#include <QStyledItemDelegate>

class QSvgRenderer;
namespace ns_delegate
{
    class ColorBoxItem : public QStyledItemDelegate
    {
        Q_OBJECT
    private:
        int m_baseSize;
    public:
        ColorBoxItem(QObject *parent = nullptr);

        virtual void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    };

    class CardItem : public QStyledItemDelegate
    {
        Q_OBJECT
    private:
        QSvgRenderer* m_renderer;
    public:
        CardItem(QObject *parent = nullptr);

        virtual void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    };

    class FilterItem : public QItemDelegate
    {
        Q_OBJECT
    private:
    public:
        FilterItem(QObject *parent = nullptr);

        virtual void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    };

} //namespace ns_delegate

