#pragma once

#include <basewidgets/framedentity.h>

#include <qdatetime.h>

namespace Ui {
class CalendarItem;
}

class QLabel;

struct CalendarItemShadowStyleData
{
    enum ShadowType {Inner, Outer};

    ShadowType type;
    QColor color;
    qreal blurRadius;
    QPoint shadowOffset;
    int deltaWidth;
    int deltaHeight;
    E::Blending blending;

    CalendarItemShadowStyleData()
        : type(ShadowType::Outer)
        , color(Qt::black)
        , blurRadius(0)
        , shadowOffset(0, 1)
        , deltaWidth(0)
        , deltaHeight(0)
        , blending(E::Blending::Overlay)
    {}
};

struct CalendarItemTextStyleData
{
    enum DisplayType {
        Auto,
        Number,
        Short,
        Long
    };
    DisplayType displayType;
    QRect rect;
    int pixelSize;
    QColor color;
    union {
        struct {
            int top, right, bottom, left;
        };
        int val[4];
    } padding;
    QList<CalendarItemShadowStyleData> shadowStyleData;

    CalendarItemTextStyleData()
        : displayType(Auto)
        , rect(0, 0, 80, 20)
        , pixelSize(12)
        , color(Qt::black)
        , padding{0, 0, 0, 0}
        , shadowStyleData({CalendarItemShadowStyleData()})
    {}
};

struct CalendarItemStyleData
{
    QRect rect;
    QPixmap pixmap;
    qreal opacity;
    E::Blending blending;
    CalendarItemTextStyleData dayOfWeekData;
    CalendarItemTextStyleData dayOfMonthData;
    CalendarItemTextStyleData monthData;

    CalendarItemStyleData()
        : rect(0, 0, 76, 90)
        , pixmap(QPixmap(":/textures/vip_system_page/big_calendar.png"))
        , opacity(1.)
        , blending(E::Blending::Normal)
        , dayOfWeekData()
        , dayOfMonthData()
        , monthData()
    {}
};

class CalendarItem : public FramedEntity
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
private:
    Ui::CalendarItem *m_ui;
    QDate m_date;
    CalendarItemStyleData m_pastStyleData;
    CalendarItemStyleData m_presentStyleData;
    CalendarItemStyleData m_futureStyleData;
protected:
    CalendarItemStyleData m_currentStyleData;
public:
    CalendarItem(QWidget *parent = nullptr);
    virtual ~CalendarItem();

    const QDate &date() const;
    inline bool isCurrentDate() const;

    /*virtual */void init();
public slots:
    void setDate(const QDate& date);
    inline void setTomorrowDate(const QDate& date) {
        setDate(date.addDays(1));
    }

    inline void setYesterdayDate(const QDate& date) {
        setDate(date.addDays(-1));
    }
protected slots:
    void updateData();
protected:
    void onCurrentStyleDataChanged();
    void updateDayOfWeek();
    void updateDayOfMonth();
    void updateMonth();
    void updateLabel(QLabel *label, const CalendarItemTextStyleData &data);
    void updateStrings();
signals:
    void dateChanged(const QDate& date);

protected:
    virtual void initPainter(QPainter *p) const override;
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void changeEvent(QEvent *e) override;
};



typedef CalendarItem CalendarItemWidget;
// ========================== INLINE ========================
inline bool CalendarItem::isCurrentDate() const {
    return date() == QDate::currentDate();
}

