#include <precompiled.h>
#include "CalendarItem.h"
#include "ui_CalendarItem.h"
#include <styles/shadowedtextstyle.h>
#include <styles/colorizedstyle.h>
#include <qpainter.h>
#include <QEvent>

namespace Ui
{
//    struct CalendarItem
//    {
//        QLabel *dayOfWeek;
//        QLabel *dayOfMonth;
//        QLabel *month;

//        void setupUi(FramedEntity *calendarItem)
//        {
//            calendarItem->setObjectName(QStringLiteral("CalendarItem"));
//            dayOfWeek  = new QLabel(calendarItem);
//            dayOfMonth = new QLabel(calendarItem);
//            month      = new QLabel(calendarItem);
//            dayOfWeek->show();
//            dayOfMonth->show();
//            month->show();

//            calendarItem->setStyleSheet(QLatin1String("QLabel {\n"
//                                                      "    border: none;\n"
//                                                      "    font: 600 40px \"Open Sans\";\n"
//                                                      "    color: rgba(25, 25, 25, 255);\n"
//                                                      "    margin: 0px;\n"
//                                                      "    padding: 0px;\n"
//                                                      "}"));
//            dayOfWeek->setStyleSheet(QLatin1String("QLabel {\n"
//                                                    "    font: 600 16px \"Open Sans\";\n"
//                                                    "    color: rgba(250, 250, 250, 255);\n"
//                                                    "}"));
//            dayOfMonth->setStyleSheet(QLatin1String("QLabel {\n"
//                                                    "    padding-top: 6px;\n"
//                                                    "    padding-left: 6px;\n"
//                                                    "}"));
//            month->setStyleSheet(QLatin1String("QLabel {\n"
//                                                "font: 600 12px \"Open Sans\";\n"
//                                                "color: rgba(84, 110, 122, 255);\n"
//                                                "padding: 2px;\n"
//                                                "}"));
//        }
//        void retranslateUi(FramedEntity *) {}
//    };
}

namespace // данные хардкод
{
    CalendarItemStyleData presentData()
    {
        CalendarItemStyleData styleData;
        styleData.opacity = 1.;
        styleData.rect = QRect(9, 0, 76, 90);
        styleData.blending = E::Blending::Normal;
        styleData.pixmap = QPixmap(":/textures/vip_system_page/big_calendar.png");
        // day of Week
        {
            styleData.dayOfWeekData.pixelSize = 16;
            styleData.dayOfWeekData.displayType = CalendarItemTextStyleData::DisplayType::Short;
            styleData.dayOfWeekData.color = QColor(0xfafafa);
            styleData.dayOfWeekData.rect = QRect(0, 0, 76, 24);
            styleData.dayOfWeekData.padding = {0, 0, 0, 0};
            styleData.dayOfWeekData.shadowStyleData << CalendarItemShadowStyleData();

            CalendarItemShadowStyleData &shadowData = styleData.dayOfWeekData.shadowStyleData.first();
            shadowData.blending = E::Blending::Normal;
            shadowData.blurRadius = 0;
            shadowData.color = QColor(25, 25, 25, 255*0.9);
            shadowData.deltaHeight = 0;
            shadowData.deltaWidth = 0;
            shadowData.shadowOffset = QPoint(0, 1);
            shadowData.type = CalendarItemShadowStyleData::ShadowType::Outer;
        }
        // day of Month
        {
            styleData.dayOfMonthData.pixelSize = 40;
            styleData.dayOfMonthData.displayType = CalendarItemTextStyleData::DisplayType::Number;
            styleData.dayOfMonthData.color = QColor(0x191919);
            styleData.dayOfMonthData.rect = QRect(11, 26, 52, 39);
            styleData.dayOfMonthData.padding = {6, 0, 0, 6};
            styleData.dayOfMonthData.shadowStyleData << CalendarItemShadowStyleData();

            CalendarItemShadowStyleData &shadowData = styleData.dayOfMonthData.shadowStyleData.first();
            shadowData.blending = E::Blending::Normal;
            shadowData.blurRadius = 0;
            shadowData.color = QColor(255, 255, 255, 255*0.9);
            shadowData.deltaHeight = 0;
            shadowData.deltaWidth = 0;
            shadowData.shadowOffset = QPoint(0, 1);
            shadowData.type = CalendarItemShadowStyleData::ShadowType::Outer;
        }
        // month
        {
            styleData.monthData.pixelSize = 12;
            styleData.monthData.displayType = CalendarItemTextStyleData::DisplayType::Long;
            styleData.monthData.color = QColor(0x546e7a);
            styleData.monthData.rect = QRect(6, 66, 63, 21);
            styleData.monthData.padding = {2, 2, 2, 2};
            styleData.monthData.shadowStyleData << CalendarItemShadowStyleData();

            CalendarItemShadowStyleData &shadowData = styleData.monthData.shadowStyleData.first();
            shadowData.blending = E::Blending::Normal;
            shadowData.blurRadius = 0;
            shadowData.color = QColor(255, 255, 255, 255*0.9);
            shadowData.deltaHeight = 0;
            shadowData.deltaWidth = 0;
            shadowData.shadowOffset = QPoint(0, 1);
            shadowData.type = CalendarItemShadowStyleData::ShadowType::Outer;
        }
        return styleData;
    }

    CalendarItemStyleData futureData()
    {
        CalendarItemStyleData styleData;
        styleData.opacity = 1.;
        styleData.rect = QRect(0, 0, 48, 58);
        styleData.blending = E::Blending::Normal;
        styleData.pixmap = QPixmap(":/textures/vip_system_page/small_calendar.png");
        // day of Week
        {
            styleData.dayOfWeekData.pixelSize = 11;
            styleData.dayOfWeekData.displayType = CalendarItemTextStyleData::DisplayType::Short;
            styleData.dayOfWeekData.color = QColor(0xfafafa);
            styleData.dayOfWeekData.rect = QRect(1, 1, 46, 14);
            styleData.dayOfWeekData.padding = {0, 0, 0, 0};
            styleData.dayOfWeekData.shadowStyleData << CalendarItemShadowStyleData();

            CalendarItemShadowStyleData &shadowData = styleData.dayOfWeekData.shadowStyleData.first();
            shadowData.blending = E::Blending::Normal;
            shadowData.blurRadius = 0;
            shadowData.color = QColor(25, 25, 25, 255*0.9);
            shadowData.deltaHeight = 0;
            shadowData.deltaWidth = 0;
            shadowData.shadowOffset = QPoint(0, 1);
            shadowData.type = CalendarItemShadowStyleData::ShadowType::Outer;
        }
        // day of Month
        {
            styleData.dayOfMonthData.pixelSize = 24;
            styleData.dayOfMonthData.displayType = CalendarItemTextStyleData::DisplayType::Number;
            styleData.dayOfMonthData.color = QColor(0x191919);
            styleData.dayOfMonthData.rect = QRect(10, 16, 28, 25);
            styleData.dayOfMonthData.padding = {4, 0, 0, 0};
            styleData.dayOfMonthData.shadowStyleData << CalendarItemShadowStyleData();

            CalendarItemShadowStyleData &shadowData = styleData.dayOfMonthData.shadowStyleData.first();
            shadowData.blending = E::Blending::Normal;
            shadowData.blurRadius = 0;
            shadowData.color = QColor(255, 255, 255, 255*0.9);
            shadowData.deltaHeight = 0;
            shadowData.deltaWidth = 0;
            shadowData.shadowOffset = QPoint(0, 1);
            shadowData.type = CalendarItemShadowStyleData::ShadowType::Outer;
        }
        // month
        {
            styleData.monthData.pixelSize = 10;
            styleData.monthData.displayType = CalendarItemTextStyleData::DisplayType::Short;
            styleData.monthData.color = QColor(0x546e7a);
            styleData.monthData.rect = QRect(14, 41, 20, 14);
            styleData.monthData.padding = {0, 0, 0, 0};
            styleData.monthData.shadowStyleData << CalendarItemShadowStyleData();

            CalendarItemShadowStyleData &shadowData = styleData.monthData.shadowStyleData.first();
            shadowData.blending = E::Blending::Normal;
            shadowData.blurRadius = 0;
            shadowData.color = QColor(255, 255, 255, 255*0.9);
            shadowData.deltaHeight = 0;
            shadowData.deltaWidth = 0;
            shadowData.shadowOffset = QPoint(0, 1);
            shadowData.type = CalendarItemShadowStyleData::ShadowType::Outer;
        }

        return styleData;
    }

    CalendarItemStyleData pastData()
    {
        CalendarItemStyleData styleData = futureData();
        styleData.opacity = 0.5;
        styleData.dayOfWeekData.color.setAlphaF(0.5);
        styleData.dayOfMonthData.color.setAlphaF(0.7);
        styleData.monthData.color.setAlphaF(0.7);

        for (CalendarItemShadowStyleData& d: styleData.dayOfWeekData.shadowStyleData) {
            d.color.setAlphaF(0.5);
        }
        for (CalendarItemShadowStyleData& d: styleData.dayOfMonthData.shadowStyleData) {
            d.color.setAlphaF(0.2);
        }
        for (CalendarItemShadowStyleData& d: styleData.monthData.shadowStyleData) {
            d.color.setAlphaF(0.1);
        }

        return styleData;
    }
}


CalendarItem::CalendarItem(QWidget *parent)
    : FramedEntity(parent)
    , m_ui(new Ui::CalendarItem)
    , m_date(QDate::currentDate())
    , m_pastStyleData()
    , m_presentStyleData()
    , m_futureStyleData()
    , m_currentStyleData(m_presentStyleData)
{
    m_ui->setupUi(this);
    connect(this, &CalendarItem::dateChanged, this, &CalendarItem::updateData);
    init();
}

CalendarItem::~CalendarItem()
{
    delete m_ui;
}

const QDate& CalendarItem::date() const
{
    return m_date;
}

void CalendarItem::init()
{
    m_pastStyleData = ::pastData();
    m_presentStyleData = ::presentData();
    m_futureStyleData = ::futureData();

    onCurrentStyleDataChanged();
}

void CalendarItem::setDate(const QDate& date)
{
    if (m_date == date)
        return;

    m_date = date;
    emit dateChanged(date);
}

void CalendarItem::updateData()
{
    onCurrentStyleDataChanged();
    if (QDate::currentDate() > m_date) {
        m_currentStyleData = m_pastStyleData;
    } else if (QDate::currentDate() < m_date) {
        m_currentStyleData = m_futureStyleData;
    } else {
        m_currentStyleData = m_presentStyleData;
    }
    onCurrentStyleDataChanged();

    updateStrings();
}

void CalendarItem::onCurrentStyleDataChanged()
{
//    setPixmap(m_currentStyleData.pixmap);
    setSizeOfFrame(m_currentStyleData.pixmap.size());
    const QRect& rc = m_currentStyleData.rect;
    setFixedSize(rc.width() + rc.x()*2, rc.height() + rc.y()*2);
    setOpacity(m_currentStyleData.opacity);
    setBlending(m_currentStyleData.blending);

    updateDayOfWeek();
    updateDayOfMonth();
    updateMonth();

    adjustSize();

    update();
}

void CalendarItem::updateLabel(QLabel* label, const CalendarItemTextStyleData& data)
{
    static const QString pattern = "QLabel {"
                                   "font: 600 %0px \"Open Sans\";"
                                   "padding: %1px %2px %3px %4px;"
                                   "color: rgba(%5, %6, %7, %8);"
                                   "}";
    int xOff = m_currentStyleData.rect.x();
    int yOff = m_currentStyleData.rect.y();
    label->setGeometry(data.rect.adjusted(xOff, yOff, xOff, yOff));
    label->setStyleSheet(pattern.arg(data.pixelSize)
                                .arg(data.padding.top)
                                .arg(data.padding.right)
                                .arg(data.padding.bottom)
                                .arg(data.padding.left)
                                .arg(data.color.red())
                                .arg(data.color.green())
                                .arg(data.color.blue())
                                .arg(data.color.alpha()));


    if (!data.shadowStyleData.isEmpty()) {
        const auto& shadowStyleData = data.shadowStyleData.first();
        QRect rc = QRect(shadowStyleData.shadowOffset, QSize(data.rect.width()+shadowStyleData.deltaWidth, data.rect.height()+shadowStyleData.deltaHeight));
        setTextDropShadowStyle(label, rc, shadowStyleData.color, shadowStyleData.blending);
    }
}

void CalendarItem::updateStrings()
{
    switch (m_currentStyleData.dayOfWeekData.displayType) {
    case CalendarItemTextStyleData::DisplayType::Number:
    {
        m_ui->dayOfWeek->setNum(m_date.dayOfWeek());
    } break;
    case CalendarItemTextStyleData::DisplayType::Auto:
    case CalendarItemTextStyleData::DisplayType::Short:
    {
        m_ui->dayOfWeek->setText(ns_util::shortDayOfWeekNativeName(m_date.dayOfWeek()));
    } break;
    case CalendarItemTextStyleData::DisplayType::Long:
    {
        m_ui->dayOfWeek->setText(ns_util::longDayOfWeekNativeName(m_date.dayOfWeek()));
    } break;
    default:
        break;
    }

    switch (m_currentStyleData.monthData.displayType) {
    case CalendarItemTextStyleData::DisplayType::Number:
    {
        m_ui->month->setNum(m_date.month());
    } break;
    case CalendarItemTextStyleData::DisplayType::Auto:
    case CalendarItemTextStyleData::DisplayType::Short:
    {
        m_ui->month->setText(ns_util::shortMonthNativeName(m_date.month()));
    } break;
    case CalendarItemTextStyleData::DisplayType::Long:
    {
        m_ui->month->setText(ns_util::longMonthNativeName(m_date.month()));
    } break;
    default:
        break;
    }

    m_ui->dayOfMonth->setNum(m_date.day());
}


void CalendarItem::updateDayOfWeek()
{
    updateLabel(m_ui->dayOfWeek, m_currentStyleData.dayOfWeekData);
}
void CalendarItem::updateDayOfMonth()
{
    updateLabel(m_ui->dayOfMonth, m_currentStyleData.dayOfMonthData);
}
void CalendarItem::updateMonth()
{
    updateLabel(m_ui->month, m_currentStyleData.monthData);
}


void CalendarItem::initPainter(QPainter *p) const
{
    FramedEntity::initPainter(p);
}

void CalendarItem::paintEvent(QPaintEvent *e)
{
    const QPixmap &bg = m_currentStyleData.pixmap;
    if (!bg.isNull())
    {
        QPainter p(this);
        p.drawPixmap((width() - bg.width())/2, (height() - bg.height())/2, bg.width(), bg.height(), bg);
    }

    FramedEntity::paintEvent(e);
}

void CalendarItem::changeEvent(QEvent *e)
{
    FramedEntity::changeEvent(e);

    if (e != nullptr) {
        if (e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
            updateStrings();

        }/* else if (e->type() == QEvent::LocaleChange) {
            setLocale(QLocale::system());
            m_ui->retranslateUi(this);
        }*/
    }
}
