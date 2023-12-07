#pragma once

#include <QIcon>
#include <QAction>

class Avatar
{
private:
    QIcon m_icon;
public:
    // ----------------------
    Avatar(const QIcon &icon = QIcon());
    virtual ~Avatar();

    void addPixmap(const QPixmap& pixmap);
    void clearPixmaps();
    void setIcon(const QIcon &icon);
    inline QIcon icon() const {
        return m_icon;
    }
    static const QImage& alphaMask();
    static QPixmap placeholder(int size);

    bool draw(QPainter* p, const QRect& rc = QRect());

    static QMap<QString, Avatar> fromPath(const QString &path);
    static QString findInPath(const QString &path, const sha1_t &hash);
};
