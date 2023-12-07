#include <precompiled.h>
#include "CommonPainter.h"
#include "const.h"
#include <cmath>
#include <QPainter>

void CommonPainter::selectChips(gbp_i64 coins, std::map<int, int> &chips)
{
    chips.clear();
    int index = 15;
    int chipsTypesCount = 0;
    int maxChipsCount = 0;

    while (coins > 0)
    {
        if (coins >= ns_const::chipDenomination[index])
        {
            chips[index] = coins / ns_const::chipDenomination[index];
            coins %= ns_const::chipDenomination[index];
            chipsTypesCount++;

            if (chips[index] > maxChipsCount) {
                maxChipsCount = chips[index];
            }
        }
        index--;
    }
}

CommonPainter::CommonPainter(QObject *parent)
    : QObject(parent)
    , m_chipSize(22, 22)
    , m_source(":textures/game/chipset.png")
    , m_fixedH(5)
//    , m_fixedColN(4)
    , m_fixedRowN(4)
{}

void CommonPainter::setSource(const QPixmap &source, QSize size)
{
    m_source = source;
    if (size.isValid()) {
        m_chipSize = size;
    }
}

void CommonPainter::setConstraints(int /*x*/, int y, int z)
{
    m_fixedH = z;
    m_fixedRowN = y;
//    m_fixedColN = x;
}

QPixmap CommonPainter::drawBetChipsPixmap(Money bet, StyleFlag styleFlag) const
{
    gbp_i64 coins = bet.amount();
    int index = 15;
    std::map<int/*index*/, int/*count*/> chips;
    int chipsTypesCount = 0;
    int maxChipsCount = 0;
    while (coins > 0)
    {
        if (coins >= ns_const::chipDenomination[index])
        {
            chips[index] = coins / ns_const::chipDenomination[index];
            coins %= ns_const::chipDenomination[index];
            chipsTypesCount++;

            if (chips[index] > maxChipsCount) {
                maxChipsCount = chips[index];
            }
        }
        index--;
    }

    int rowCols = int(sqrtf(float(chipsTypesCount)) + 0.99f);
    const int offsetSpace = maxChipsCount*2;
    const int textSpace = 0;

    QPixmap pix(m_chipSize * rowCols + QSize(textSpace, offsetSpace));
    pix.fill(Qt::transparent);
    drawBetChips(bet, pix, styleFlag);

    return pix;
}

void CommonPainter::drawBetChips(Money bet, QPaintDevice &target, StyleFlag styleFlag) const
{
    gbp_i64 coins = bet.amount();
    std::map<int/*index*/, int/*count*/> chips;
    selectChips(coins, chips);

    QPainter p;

//    int rowCols = int(sqrtf(float(chips.size())) + 0.99f);
    QSize size(target.width(), target.height());
//    QSize chipsSize = m_chipSize * rowCols;

    const int offsetSpace = /*maxChipsCount**/2;
    const int textSpace = 0;
    size += QSize(textSpace, offsetSpace);

//    QPixmap moneyPix = Money::moneyPaintHelper(bet, QSize(textSpace, 18));

    p.begin(&target);
//    p.drawPixmap(size.width()-textSpace, (size.height() - 18)/2, textSpace, 18, moneyPix);

    int x(0), y(offsetSpace);
    bool chess2 = false;
    for (std::map<int, int>::const_iterator it = chips.begin(); it != chips.end(); ++it)
    {
        QPoint origin((it->first % 8) * m_chipSize.width(), (it->first / 8) * m_chipSize.height());
        QPixmap sourceFragment = m_source.copy(QRect(origin, m_chipSize));

        int offsetY = 0;
        int h = (styleFlag & StyleHint::HeightConstraint) ? std::max(1, m_fixedH) : it->second;
        for (int i = 0; i < h; i++)
        {
            p.drawPixmap(QRect(QPoint(x, y - offsetY), m_chipSize), sourceFragment);
            offsetY += 2;
        }

        int area = (styleFlag & StyleHint::ColumnCountConstraint) ? m_chipSize.width()*m_fixedRowN : target.width();
        x += m_chipSize.width();
        if (area - x < m_chipSize.width()) {
            x = 0;
            y += m_chipSize.height();
            if (styleFlag & StyleHint::Chequerwise) {
                chess2 = !chess2;
            }
            if (chess2) {
                x = m_chipSize.width()/2 + 1;
            }
        }
    }

    p.end();
}
