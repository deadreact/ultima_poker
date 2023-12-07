#include <precompiled.h>
#include "widget_PlayerInfo.h"
#include <QPainter>
#include <QFileInfo>


namespace ns_widget
{
    PlayerInfo::PlayerInfo(QWidget *parent)
        : QWidget(parent)
        , m_playerName("")
        , m_playerMoney("")
        , m_playerCountry(QLocale::AnyCountry)
        , m_flag(nullptr)
        , m_index(0)
    {}
    PlayerInfo::PlayerInfo(const gbp::ns_api::ns_statistic::player_t& playerInfo, QWidget *parent)
        : QWidget(parent)
        , m_playerName(playerInfo._nick.c_str())
        , m_playerMoney(QString("$%0").arg(float(playerInfo._money) / 100))
        , m_playerCountry(QLocale::Country(playerInfo._country))
        , m_flag(nullptr)
        , m_index(playerInfo._pos)
    {}
    void PlayerInfo::assignLocale(const QLocale& locale) {
        setPlayerCountry(locale.country());
    }

    void PlayerInfo::update(const gbp::ns_api::ns_statistic::player_t &playerInfo)
    {
        setPlayerName(playerInfo._nick.c_str());
        setPlayerMoney(QString("$%0").arg(float(playerInfo._money) / 100));
        setPlayerCountry(QLocale::Country(playerInfo._country));
        repaint();
        emit infoUpdated();
    }

    void PlayerInfo::init()
    {

    }

    QString PlayerInfo::playerName() const {
        return m_playerName;
    }

    QString PlayerInfo::playerMoney() const {
        return m_playerMoney;
    }

    QLocale::Country PlayerInfo::playerCountry() const {
        return m_playerCountry;
    }

    int PlayerInfo::index() const {
        return m_index;
    }

    void PlayerInfo::setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(index);
    }

    void PlayerInfo::clear()
    {
        setPlayerName("");
        setPlayerMoney("");
        setPlayerCountry(QLocale::AnyCountry);
        repaint();
    }

    void PlayerInfo::setPlayerName(const QString& playerName) {
        m_playerName = playerName;
    }

    void PlayerInfo::setPlayerMoney(const QString &playerMoney) {
        m_playerMoney = playerMoney;
    }

    void PlayerInfo::setPlayerCountry(QLocale::Country playerCountry)
    {
        if (m_playerCountry == playerCountry)
            return;

        m_playerCountry = playerCountry;
        QString filePath = QString(":textures/Common/locale/%0.png").arg(QLocale::countryToString(m_playerCountry));
        if (m_flag != nullptr) delete m_flag;
        if (QFileInfo(filePath).exists())
            m_flag = new QPixmap(filePath);
        else
            m_flag = nullptr;
    }


    void PlayerInfo::paintEvent(QPaintEvent* /*e*/)
    {
//        QWidget::paintEvent(e);
        QPainter p(this);
        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        qreal scale = 0.6;

//        QColor bgColor(1, 33+(m_index%2)*10, 10+(m_index%2)*10);
        QColor bgColor(0, 0, 0, 255 / (2 * (m_index%2 + 1)));

        p.fillRect(rect(), bgColor);

        p.setPen(QColor(210, 210, 210));
        p.drawText(20, 16, m_playerName);

        if (m_flag != nullptr)
        {
            p.drawPixmap(QRect(width()/2+10, 1, m_flag->width()*scale, m_flag->height()*scale), m_flag->scaled(m_flag->width()*scale, m_flag->height()*scale));
        }
        p.setPen(QColor(210, 210, 180));
        p.drawText(width() - m_playerMoney.size() * 5 - 20, 16, m_playerMoney);
    }

} // namespace widget
