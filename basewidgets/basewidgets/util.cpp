#include "util.h"
#include <QBuffer>
#include <QCryptographicHash>
#include <QPainter>
#include <QVector2D>
#include <cmath>
#include <QDebug>

namespace ns_util
{
    QPixmap fromBase64(const base64_t &data)
    {
        return QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(data)));
    }

    base64_t toBase64(const QPixmap &pix)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        pix.save(&buffer, "PNG");

        return byteArray.toBase64();
    }

    template <>
    sha1_t toSha1(const QString &string) {
        return QCryptographicHash::hash(string.toLocal8Bit(), QCryptographicHash::Sha1).toHex();
    }
    template <>
    sha1_t toSha1(const QPixmap &pixmap) {
        return QCryptographicHash::hash(toBase64(pixmap), QCryptographicHash::Sha1);
    }

//    /*static*/ PixmapCache* PixmapCache::sm_instance = nullptr;


//    PixmapCache& PixmapCache::instance()
//    {
//        if (sm_instance == nullptr) {
//            sm_instance = new PixmapCache;
//        }
//        return *sm_instance;
//    }

//    const QPixmap* PixmapCache::cached(const base64_t &pixmap, const QPixmap* originPixmap)
//    {
//        return restore(cachePixmap(pixmap, originPixmap));
//    }

//    sha1_t PixmapCache::cachePixmap(const base64_t &pixmap, const QPixmap* originPixmap)
//    {
//        auto it = m_cacheByPixmap.find(pixmap);
//        if (it != m_cacheByPixmap.end()) {
//            return it->second;
//        }
//        sha1_t hash = QCryptographicHash::hash(pixmap, QCryptographicHash::Sha1);
//        m_cacheByHash[hash] = new QPixmap((originPixmap == nullptr) ? fromBase64(pixmap) : *originPixmap);
//        m_cacheByPixmap.insert(std::make_pair(pixmap, hash));
//        return hash;
//    }

//    const QPixmap *PixmapCache::restore(const sha1_t &hash)
//    {
//        auto it = m_cacheByHash.find(hash);
//        if (it == m_cacheByHash.end()) {
//            return nullptr;
//        }
//        return it->second;
//    }

    void setBlendingOption(E::Blending blending, QPainter &p)
    {
        switch (blending) {
        case E::Blending::Normal:
            p.setCompositionMode(QPainter::CompositionMode_SourceOver);
            break;
        case E::Blending::Add:
            p.setCompositionMode(QPainter::CompositionMode_Plus);
            break;
        case E::Blending::Multiply:
            p.setCompositionMode(QPainter::CompositionMode_Multiply);
            break;
        case E::Blending::Overlay:
            p.setCompositionMode(QPainter::CompositionMode_Overlay);
            break;
        case E::Blending::Screen:
            p.setCompositionMode(QPainter::CompositionMode_Screen);
            break;
        case E::Blending::Darken:
            p.setCompositionMode(QPainter::CompositionMode_Darken);
            break;
        case E::Blending::Lighten:
            p.setCompositionMode(QPainter::CompositionMode_Lighten);
            break;
        case E::Blending::ColorBurn:
            p.setCompositionMode(QPainter::CompositionMode_ColorBurn);
            break;
        case E::Blending::ColorDodge:
            p.setCompositionMode(QPainter::CompositionMode_ColorDodge);
            break;
        case E::Blending::HardLight:
            p.setCompositionMode(QPainter::CompositionMode_HardLight);
            break;
        case E::Blending::SoftLight:
            p.setCompositionMode(QPainter::CompositionMode_SoftLight);
            break;
        case E::Blending::Exclusion:
            p.setCompositionMode(QPainter::CompositionMode_Exclusion);
            break;

        default:
            break;
        }
    }

    int getPasswordWeight(const std::string &pass)
    {
        // http://korzh.net/2011-04-eshhyo-ob-ocenke-stojkosti-parolya.html
        int numeric(0);
        int lower(0);
        int upper(0);
        int numsymbols(0);

        for (auto symb: pass)
        {
            if ( (symb >= '0' && symb <= '9') ) {
                ++numeric; continue;
            }
            if ( (symb >= 'a' && symb <= 'z') || symb == '_' ) {
                ++lower; continue;
            }
            if ( (symb >= 'A' && symb <= 'Z') ) {
                ++upper; continue;
            }
            ++numsymbols;
        }
        int pwlength = std::min(pass.size(), size_t(5));

        return ((pwlength * 10) - 20) + (std::min(numeric, 3) * 10) + (std::min(numsymbols, 3) * 15) + (std::min(upper, 3) * 10);
    }

//    void sincosf(float __x, float *p_sin, float *p_cos)
//    {
//#ifdef Q_OS_MAC
//        __sincosf(__x, p_sin, p_cos);
//#else
//        ::sincosf(__x, p_sin, p_cos);
//#endif // MAC_OS
//    }

//    QRect square(const QRect &rc) {
//        int h = std::min(rc.height(), rc.width());
//        return QRect(rc.x(), rc.y(), h, h);
//    }

    QString shortMonthNativeName(int month)
    {
        QStringList lst( { QObject::tr("@shortMonthNativeName_1")
                         , QObject::tr("@shortMonthNativeName_2")
                           , QObject::tr("@shortMonthNativeName_3")
                           , QObject::tr("@shortMonthNativeName_4")
                           , QObject::tr("@shortMonthNativeName_5")
                           , QObject::tr("@shortMonthNativeName_6")
                           , QObject::tr("@shortMonthNativeName_7")
                           , QObject::tr("@shortMonthNativeName_8")
                           , QObject::tr("@shortMonthNativeName_9")
                           , QObject::tr("@shortMonthNativeName_10")
                           , QObject::tr("@shortMonthNativeName_11")
                           , QObject::tr("@shortMonthNativeName_12")
                         });
        return lst[month-1];
    }

    QString longMonthNativeName(int month)
    {
        QStringList lst( { QObject::tr("@longMonthNativeName_1")
                         , QObject::tr("@longMonthNativeName_2")
                           , QObject::tr("@longMonthNativeName_3")
                           , QObject::tr("@longMonthNativeName_4")
                           , QObject::tr("@longMonthNativeName_5")
                           , QObject::tr("@longMonthNativeName_6")
                           , QObject::tr("@longMonthNativeName_7")
                           , QObject::tr("@longMonthNativeName_8")
                           , QObject::tr("@longMonthNativeName_9")
                           , QObject::tr("@longMonthNativeName_10")
                           , QObject::tr("@longMonthNativeName_11")
                           , QObject::tr("@longMonthNativeName_12")
                         });
        return lst[month-1];
    }

    QString shortDayOfWeekNativeName(int dayOfWeek)
    {
        QStringList lst( { QObject::tr("@short_mon")
                         , QObject::tr("@short_tue")
                           , QObject::tr("@short_wen")
                           , QObject::tr("@short_thu")
                           , QObject::tr("@short_fri")
                           , QObject::tr("@short_sat")
                           , QObject::tr("@short_sun")
                         });
        return lst[dayOfWeek-1];
    }
    QString longDayOfWeekNativeName(int dayOfWeek)
    {
        QStringList lst( { QObject::tr("@long_mon")
                         , QObject::tr("@long_tue")
                           , QObject::tr("@long_wen")
                           , QObject::tr("@long_thu")
                           , QObject::tr("@long_fri")
                           , QObject::tr("@long_sat")
                           , QObject::tr("@long_sun")
                         });
        return lst[dayOfWeek-1];
    }

}
