#pragma once

#include <QString>
#include <QMetaType>
#include "../../../common/api/gbp_int.hpp"

class Money
{
    Q_GADGET
public:
    typedef gbp_i64 money_t;

    static const Money sm_zero;
    static const int sm_maxDigits = 7;
    enum DisplayType
    {
        Normal         = 0x01,
        Short          = 0x02,
        Full           = 0x04,
        NoCoins        = 0x08,
        NoCurrencySign = 0x10
    };
    Q_FLAG(DisplayType)
    Q_DECLARE_FLAGS(DisplayFlag, DisplayType)
private:
    money_t m_ammount;
    static QString sm_currencySymbol;

public:
    Money();
    Money(money_t);
    static Money fromBills(int);
    static Money fromBills(money_t);
    static Money fromCoins(int);
    static Money fromCoins(money_t);
    static Money fromBillsAndCoins(int, int);
    static Money fromFloat(float);
    static Money fromDouble(double);
    static Money fromString(const QString&);

    inline bool isPositive() const { return m_ammount > 0; }
    inline bool isNegative() const { return m_ammount < 0; }
    inline bool isZero() const { return m_ammount == 0; }

    float toFloat() const;
    double toDouble() const;

    int bills() const;
    quint16 coins() const;
    money_t amount() const;
    operator money_t() const { return amount(); }
    QString toString(DisplayFlag flags = DisplayType::Normal) const;
    std::string toStdString(DisplayFlag flags = DisplayType::Normal) const;
    const char* toCString(DisplayFlag flags = DisplayType::Normal) const;

    inline bool operator==(const Money& money) const;
    inline bool operator!=(const Money& money) const;
    inline bool operator>=(const Money& money) const;
    inline bool operator<=(const Money& money) const;
    inline bool operator>(const Money& money) const;
    inline bool operator<(const Money& money) const;

    inline bool operator==(gbp_i64 money) const;
    inline bool operator!=(gbp_i64 money) const;
    inline bool operator>=(gbp_i64 money) const;
    inline bool operator<=(gbp_i64 money) const;
    inline bool operator>(gbp_i64 money) const;
    inline bool operator<(gbp_i64 money) const;

    static QPixmap moneyPaintHelper(const Money& money, const QSize& size, const QPixmap& currencyIcon, const QFont& font, const QColor& color);
};


// inline operations

inline bool Money::operator==(const Money& money) const { return m_ammount == money.m_ammount; }
inline bool Money::operator!=(const Money& money) const { return m_ammount != money.m_ammount; }
inline bool Money::operator>=(const Money& money) const { return m_ammount >= money.m_ammount; }
inline bool Money::operator<=(const Money& money) const { return m_ammount <= money.m_ammount; }
inline bool Money::operator>(const Money& money) const  { return m_ammount >  money.m_ammount; }
inline bool Money::operator<(const Money& money) const  { return m_ammount <  money.m_ammount; }

inline bool Money::operator==(gbp_i64 money) const { return m_ammount == money; }
inline bool Money::operator!=(gbp_i64 money) const { return m_ammount != money; }
inline bool Money::operator>=(gbp_i64 money) const { return m_ammount >= money; }
inline bool Money::operator<=(gbp_i64 money) const { return m_ammount <= money; }
inline bool Money::operator>(gbp_i64 money) const  { return m_ammount >  money; }
inline bool Money::operator<(gbp_i64 money) const  { return m_ammount <  money; }

Q_DECLARE_METATYPE(Money)
Q_DECLARE_OPERATORS_FOR_FLAGS(Money::DisplayFlag)
