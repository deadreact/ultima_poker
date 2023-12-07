#pragma once

#include <QObject>
#include <qpaintdevice.h>
#include <core/money.h>

class CommonPainter : public QObject
{
    Q_OBJECT

    QSize m_chipSize;
    QPixmap m_source;
    int m_fixedH;
    int m_fixedRowN;
//    int m_fixedColN;
private:
    static void selectChips(gbp_i64 coins, std::map<int, int>& chips);
public:
    enum StyleHint
    {
        Nothing               = 0x00,
        MixChips              = 0x01,
        HeightConstraint      = 0x02,
        RowCountConstraint    = 0x04,
        ColumnCountConstraint = 0x08,
        Chequerwise           = 0x10
    };
    Q_DECLARE_FLAGS(StyleFlag, StyleHint)
    Q_FLAG(StyleFlag)

    explicit CommonPainter(QObject *parent = 0);

    void setSource(const QPixmap& source, QSize size = QSize());
    void setConstraints(int x, int y, int z);

    QPixmap drawBetChipsPixmap(Money bet, StyleFlag styleFlag = StyleHint::Nothing) const;
    void drawBetChips(Money bet, QPaintDevice& target, StyleFlag styleFlag = StyleHint::Nothing) const;
//    void drawBetChips(Money bet, QPaintDevice& target) const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CommonPainter::StyleFlag)
