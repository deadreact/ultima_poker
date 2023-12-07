#include <precompiled.h>
#include "IconProvider.h"

#ifdef Q_OS_WIN
    #include <QtWinExtras/QtWin>
#endif
#include <qpainter.h>
#include <PokerApplication.h>
#include <db/Database.h>
#include <api/lobby_stat/types.h>

QIcon IconProvider::fileIcon(const QString &filename)
{
    QFileInfo fileInfo(filename);
    QPixmap pixmap;

#ifdef Q_OS_WIN
    if (fileInfo.suffix().isEmpty() || ((fileInfo.suffix() == "exe") && fileInfo.exists())) {
        return m_iconProvider.icon(fileInfo);
    }

    if (!m_iconCache.find(fileInfo.suffix(), &pixmap))
    {
        static HRESULT comInit = CoInitialize(NULL);
        Q_UNUSED(comInit);

        SHFILEINFO shFileInfo;
        unsigned long val = SHGetFileInfo((const wchar_t *)("foo." + fileInfo.suffix()).utf16(), 0, &shFileInfo,
                            sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

        if (val && shFileInfo.hIcon)
        {
            pixmap = QtWin::fromHICON(shFileInfo.hIcon);
            if (!pixmap.isNull()) {
                m_iconCache.insert(fileInfo.suffix(), pixmap);
            }
            DestroyIcon(shFileInfo.hIcon);
        }
        else
        {
            m_iconProvider.icon(QFileIconProvider::File);
        }
    }

#else //Q_OS_WIN
    return m_iconProvider.icon(fileInfo);
#endif //Q_OS_WIN

    return QIcon(pixmap);
}

QIcon IconProvider::dirIcon() {
    return m_iconProvider.icon(QFileIconProvider::Folder);
}

QIcon IconProvider::countryIcon(QLocale::Country country) const
{
    return QIcon(QString(":textures/Common/icons/ico/%0.ico").arg(QLocale::countryToString(QLocale::Country(country))));
}

QPixmap IconProvider::gameTypeIcon(const games_stat_t::first_type::value_type &item) const
{
    static const QPixmap &pix = *pApp->db().loadToCache(":textures/Common/icons/icons_5x2_tourn.png");

    int seats = item._players_count.second;
    bool isFlash = item._is_flash;

    static bool flashDrawn = false;
    static QPixmap flashPix(15, 15);

     if (isFlash && !flashDrawn) {
         flashPix.fill(Qt::transparent);
         QPainter p(&flashPix);
         p.setFont(QFont("Elephant", 10, 600));
         p.setPen(0xffb906);
         p.drawText(QRect(0, 0, 15, 15), Qt::AlignCenter, "F");
         flashDrawn = true;
     }

    std::vector<QRect> iconsStack;

    static QPixmap surface(75, 15);
    surface.fill(Qt::transparent);
    QPainter p;
    QSize frameSize(15, 15);

    p.begin(&surface);

    if (seats == 9) iconsStack.push_back(QRect(QPoint(1, 1)*15, frameSize));
    else if (seats == 6) iconsStack.push_back(QRect(QPoint(4, 0)*15, frameSize));
    else if (seats == 2) iconsStack.push_back(QRect(QPoint(1, 0)*15, frameSize));
    else if (seats == 3) iconsStack.push_back(QRect(QPoint(2, 0)*15, frameSize));
    else if (seats == 5) iconsStack.push_back(QRect(QPoint(3, 0)*15, frameSize));
    else if (seats == 8) iconsStack.push_back(QRect(QPoint(0, 1)*15, frameSize));
    else {
        p.setFont(QFont("Open Sans", 10, 600));
        p.drawText(QRect(QPoint(0, 0), frameSize), Qt::AlignCenter, QString::number(seats));
    }

    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    int shift = 0;
    for (QRect& rc : iconsStack)
    {
        p.drawPixmap(QRect(shift, 0, 15, 15), pix, rc);
        shift += 15;
    }
    if (isFlash) {
        p.drawPixmap(QRect(shift, 0, 15, 15), flashPix);

    }
    p.end();
    return surface;
}

QPixmap IconProvider::gameTypeIcon(const games_stat_t::second_type::value_type &item) const
{
    static const QPixmap &pix = *pApp->db().loadToCache(":textures/Common/icons/icons_5x2_tourn.png");
    int seats = item._seats_count;
    auto type = item._types;
    auto speed = item._game_speed;

    QPainter p;
    bool needPaintNumber = false;
    std::vector<QRect> iconsStack;


    QSize frameSize(15, 15);
    if (seats == 9) iconsStack.push_back(QRect(QPoint(1, 1)*15, frameSize));
    else if (seats == 6) iconsStack.push_back(QRect(QPoint(4, 0)*15, frameSize));
    else if (seats == 2) iconsStack.push_back(QRect(QPoint(1, 0)*15, frameSize));
    else if (seats == 3) iconsStack.push_back(QRect(QPoint(2, 0)*15, frameSize));
    else if (seats == 5) iconsStack.push_back(QRect(QPoint(3, 0)*15, frameSize));
    else if (seats == 8) iconsStack.push_back(QRect(QPoint(0, 1)*15, frameSize));
    else {
        iconsStack.push_back(QRect(QPoint(0, 0)*15, frameSize));
        needPaintNumber = true;
    }

    if (type & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_rebuy) {
        iconsStack.push_back(QRect(QPoint(3, 1)*15, frameSize));
    }

    if (type & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_bounty) {
        iconsStack.push_back(QRect(QPoint(4, 1)*15, frameSize));
    }

    if (speed != gbp::ns_api::ns_game::e_game_speed::normal) {
        iconsStack.push_back(QRect(QPoint(0, 0)*15, frameSize));
    }

    QPixmap surface(15 * iconsStack.size(), 15);
    surface.fill(Qt::transparent);
    p.begin(&surface);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    int shift = 0;
    for (QRect& rc : iconsStack)
    {
        p.drawPixmap(QRect(shift, 0, 15, 15), pix, rc);
        shift += 15;
    }
    if (needPaintNumber)
    {
        p.setPen(Qt::black);
        p.setFont(QFont("Open Sans",10));
        p.drawText(QRect(0, 0, 15, 15), Qt::AlignCenter, QString::number(seats));
    }

    p.end();
    return surface;
}
