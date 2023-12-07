#include "money.h"

#include <QPainter>
#include <QRegularExpression>
#include "../../src/const.h"

/*static*/ QString Money::sm_currencySymbol = "$";
/*static*/ const Money Money::sm_zero = 0;
Money::Money()
    : m_ammount(0)
{}

Money::Money(money_t money)
    : m_ammount(money)
{}

Money Money::fromBills(int money) {
    return Money(static_cast<money_t>(money*100));
}
Money Money::fromBills(money_t money) {
    return Money(static_cast<money_t>(money*100));
}

Money Money::fromCoins(int coins)
{
    return Money(static_cast<money_t>(coins));
}

Money Money::fromCoins(money_t coins)
{
    return Money(coins);
}

Money Money::fromBillsAndCoins(int bills, int coins) {
    return Money(static_cast<money_t>(bills*100 + (coins%100)));
}

Money Money::fromFloat(float money) {
    return Money(static_cast<money_t>(money*100));
}

Money Money::fromDouble(double money) {
    return Money(static_cast<money_t>(money*100));
}

Money Money::fromString(const QString &str)
{
    static const QRegularExpression re("\\$?(\\d+)(\\.(\\d{2}))?");
    gbp_i64 mult = ns_const::n;
    QString trimmedStr = str.trimmed();
    QChar ch = trimmedStr.at(trimmedStr.size()-1);
    switch (ch.toLatin1()) {
    case 'M': mult = ns_const::m; break;
    case 'K': mult = ns_const::k; break;
    case 'c': mult = ns_const::c; break;
    default:
        break;
    }
    auto texts = re.match(trimmedStr).capturedTexts();
    if (texts.size() < 2) {
        return 0;
    }
    bool ok = false;
    gbp_i64 m = texts.at(1).toLongLong(&ok);
    if (!ok) {
        return 0;
    }
    m *= mult;
    if (texts.size() > 3) {
        m += texts.at(3).toInt(&ok);
    }
    return m;
}

float Money::toFloat() const
{
    return static_cast<float>(m_ammount) / 100;
}

double Money::toDouble() const
{
    return static_cast<double>(m_ammount) / 100;
}

int Money::bills() const {
    return static_cast<int>(m_ammount/100);
}

quint16 Money::coins() const {
    return static_cast<quint16>(qAbs(m_ammount)%100);
}

Money::money_t Money::amount() const {
    return m_ammount;
}

QString Money::toString(DisplayFlag flags) const
{
    QString currencySign = (flags & DisplayType::NoCurrencySign) ? "" : "$";
    QString coinsStr     = (flags & DisplayType::NoCoins) ? "" : QString(".%0%1").arg(coins()/10).arg(coins() % 10);

    if (flags & DisplayType::Normal) {
        QString strBills = QString::number(bills());
        if (strBills.length() <= sm_maxDigits - 2) {
            return QString("%0%1%2").arg(currencySign).arg(strBills).arg(coinsStr);
        } else if (strBills.length() <= sm_maxDigits + 1) {
            return QString("%0%1.%2K").arg(currencySign).arg(m_ammount/100000).arg((qAbs(m_ammount)/100)%1000);
        } else /*if (strBills.length() <= sm_maxDigits + 4)*/ {
            return QString("%0%1.%2M").arg(currencySign).arg(m_ammount/100000000).arg((qAbs(m_ammount)/100000)%1000);
        }
    } else if (flags & DisplayType::Short) {
        if (m_ammount > ns_const::m) {
            return QString("%0%1M").arg(currencySign).arg(m_ammount/ns_const::m);
        } else if (m_ammount > ns_const::k) {
            return QString("%0%1K").arg(currencySign).arg(m_ammount/ns_const::k);
        } else if (m_ammount > ns_const::n) {
            return QString("%0%1").arg(currencySign).arg(m_ammount/ns_const::n);
        } else {
            return QString("%0c").arg(m_ammount);
        }
    } else if (flags & DisplayType::Full) {
        money_t billsAll = bills();
        std::vector<money_t> billsGrouped;

        while (billsAll > 999) {
            billsGrouped.push_back(billsAll % 1000);
            billsAll /= 1000;
        }
        billsGrouped.push_back(billsAll);
        QString fullStr = "";

        int last = billsGrouped.size() - 1;
        for (int i = 0; i < last; i++) {
            fullStr = QString(" %1%2%3").arg(billsGrouped[i]/100).arg((billsGrouped[i]%100)/10).arg(billsGrouped[i]%10) + fullStr;
        }
        fullStr = QString("%0%1").arg(currencySign).arg(billsGrouped[last]) + fullStr;

        return QString("%0%1").arg(fullStr).arg(coinsStr);
    }
    return "";
}

std::string Money::toStdString(DisplayFlag flags) const
{
    return toString(flags).toStdString();
}

const char *Money::toCString(DisplayFlag flags) const
{
    return toStdString(flags).c_str();
}

QPixmap Money::moneyPaintHelper(const Money &money, const QSize& size, const QPixmap &currencyIcon, const QFont &font, const QColor &color)
{
    QPainter p;
    int shiftX = 0;
    int shiftY = 0;
    QPixmap result(size);
    result.fill(Qt::transparent);
    p.begin(&result);
    p.setFont(font);
    p.setPen(color);
    QString symb = sm_currencySymbol;
    sm_currencySymbol = "";
    QString str = money.toString();
    if (currencyIcon.isNull()) {
        str = QString("%0%1").arg('$').arg(str);
        shiftY += /*p.fontMetrics().height() - */1;
    } else {
        p.drawPixmap(shiftX, shiftY, currencyIcon.width(), currencyIcon.height(), currencyIcon);
        shiftX += currencyIcon.width() + 2;
        shiftY += /*currencyIcon.height() -*/ (currencyIcon.height() - p.fontMetrics().height())/2;
    }
    p.drawText(QRect(shiftX, shiftY, p.fontMetrics().width(str), p.fontMetrics().height()), Qt::AlignVCenter, str);
    p.end();
    sm_currencySymbol = symb;
    return result;
}
