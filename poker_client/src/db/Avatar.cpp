#include <precompiled.h>
#include "Avatar.h"
#include <const.h>
#include <qdir.h>
#include <qpainter.h>
#include <qregularexpression.h>

Avatar::Avatar(const QIcon& icon)
    : m_icon(icon)
{

}

Avatar::~Avatar() {}

void Avatar::addPixmap(const QPixmap &pixmap) {
    m_icon.addPixmap(pixmap);
}

void Avatar::clearPixmaps()
{
    setIcon(QIcon());
}

void Avatar::setIcon(const QIcon &icon)
{
    if (m_icon.cacheKey() != icon.cacheKey()) {
        m_icon = icon;
    }
//    update();
}


const QImage &Avatar::alphaMask() {
    static const QImage defaultMask = QImage(":/textures/Common/icons/icon_avatar_cap.png").alphaChannel();

    return defaultMask;
}

QPixmap Avatar::placeholder(int size)
{
    static QIcon phldr;
    if (phldr.isNull()) {
        static const QString pathPattern(":/textures/Common/templates/defaultAvatar_%0.png");
        for (int h: std::vector<int>({66, 100, 120, 200, 244})) {
            QImage img(pathPattern.arg(h));
            img.setAlphaChannel(alphaMask().scaledToHeight(h, Qt::SmoothTransformation));
            phldr.addPixmap(QPixmap::fromImage(img));
        }
//        placeholder.addPixmap(QPixmap(pathPattern.arg(100)));
//        placeholder.addPixmap(QPixmap(pathPattern.arg(120)));
//        placeholder.addPixmap(QPixmap(pathPattern.arg(200)));
//        placeholder.addPixmap(QPixmap(pathPattern.arg(244)));
    }
    return phldr.pixmap(size);
}

bool Avatar::draw(QPainter *p, const QRect &rc)
{
    if (!p || !p->device() || !p->isActive() || !rc.isValid() || m_icon.isNull())  {
        return false;
    } else {
        QImage img = m_icon.pixmap(rc.size()).toImage();
        img.setAlphaChannel(alphaMask().scaled(img.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        p->drawImage(rc, img);
        return true;
    }
    return false;
}

QMap<QString, Avatar> Avatar::fromPath(const QString& path)
{
    QMap<QString, Avatar> lst;
    QDir dir(path);
    if ( dir.exists())
    {
        QStringList pics = dir.entryList(QStringList("*.png"), QDir::NoFilter, QDir::SortFlag::Name);
        // <name>_<size>_<hash>.png
        static const QRegularExpression re("^(\\w+)_(\\d+)_(\\w{40})\\.png$");
        if (!pics.isEmpty())
        {
            for (const QString& filename: pics) {
                QStringList captured = re.match(filename).capturedTexts();
                if (lst.contains(captured.at(1))) {
                    lst[captured.at(1)].addPixmap(QPixmap(path + filename));
                } else {
                    lst.insert(captured.at(1), Avatar(QIcon(path + filename)));
                }
            }
        }
    }
    return lst;
}

QString Avatar::findInPath(const QString &path, const sha1_t& hash)
{
    QDir dir(path);
    if ( dir.exists())
    {
        QStringList pics = dir.entryList(QStringList("*.png"), QDir::NoFilter, QDir::SortFlag::Name);
        // <name>_<size>_<hash>.png
        QString pattern = QString("^(\\w+)_\\d+_%0\\.png$").arg(hash.data());
        QRegularExpression re(pattern);
        if (!pics.isEmpty()) {
            for (const QString& filename: pics) {
                QRegularExpressionMatch match = re.match(filename);
                if (match.hasMatch()) {
                    return match.captured(1);
                }
            }
        }
    }
    return QString();
}
