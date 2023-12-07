#pragma once
#include <QDir>

namespace ns_util
{
    void MakeAtlas(QDir&& dir, const QString& saveTo, int rows = 1);
    void MakeAtlas(const std::vector<QPixmap*>& images, const QString& saveTo, int rows = 1);
}
