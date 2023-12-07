#pragma once

#include <QStyledItemDelegate>


namespace ns_delegate
{
    class PrivateDelegate;

    class CustomColumn : public QStyledItemDelegate
    {
        Q_OBJECT
    private:
        PrivateDelegate *m_delegate;
        int m_textRectAdjust[4];
    public:
        CustomColumn(int left=0,int top=0,int right=0,int bottom=0, QObject *parent = nullptr);

        virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &idx) const override;
    protected:
        virtual void initStyleOption(QStyleOptionViewItem *opt, const QModelIndex &idx) const override;
    };
} //namespace ns_delegate

