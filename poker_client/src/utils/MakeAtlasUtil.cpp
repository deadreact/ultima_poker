#include <precompiled.h>
#include "MakeAtlasUtil.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>

namespace ns_util
{


    void MakeAtlas(const std::vector<QPixmap *> &images, const QString &saveTo, int /*rows*/)
    {
//        int img_count = images.size();
        int max_width = 0;
        int height_sum = 0;
        for (auto&& pix : images)
        {
            height_sum += pix->height();
            max_width = (pix->width() > max_width) ? pix->width() : max_width;
        }
//        QPixmap resultImg(width_sum, max_height);

//        resultImg.fill(QColor(255, 255, 255, 1));

        QImage res_img(max_width, height_sum, QImage::Format_ARGB32);

//        uchar* buffer = res_img.bits();
//        uchar* buffer_it = buffer;

        int xOffset = 0;
        int yOffset = 0;

        for (auto&& pix : images)
        {
            QImage img = pix->toImage();

            for (int i = 0; i < img.width(); i++)
            {
                for (int j = 0; j < img.height(); j++)
                {
                    res_img.setPixel(i+xOffset, j+yOffset, img.pixel(i, j));
                }
            }
            yOffset += img.height();
        }
        res_img.save(saveTo);
        //        delete[] buffer;
    }

    void MakeAtlas(QDir &&dir, const QString &saveTo, int rows)
    {
        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);

        QFileInfoList list = dir.entryInfoList(QStringList("*.png"));

        std::vector<QPixmap*> v;
        v.reserve(list.size());

        for (int i = 0; i < list.size(); ++i)
        {
           QFileInfo fileInfo = list.at(i);

           v.push_back(new QPixmap(dir.absoluteFilePath(fileInfo.fileName())));

        }
        MakeAtlas(v, saveTo, rows);
        for (auto&& pix: v)
        {
            delete pix;
        }
    }

}
