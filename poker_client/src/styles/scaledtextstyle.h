#pragma once

#include "styles_global.h"
#include <QWidget>
#include <QProxyStyle>

//namespace ns_styles
//{
    class STYLES_DLL ScaledTextStyle : public QProxyStyle
    {
        Q_OBJECT

        Q_PROPERTY(int minimumSize READ minimumSize WRITE setMinimumSize)
        Q_PROPERTY(int defaultSize READ defaultSize WRITE setDefaultSize)
    private:
        int m_minimumSize; // in pixels
        int m_defaultSize; // in pixels
    public:
        ScaledTextStyle(int min=10, int def=14, QStyle* style = nullptr);

        int minimumSize() const;
        int defaultSize() const;
    public:
        virtual ~ScaledTextStyle() {}

        void setMinimumSize(int minSize);
        void setDefaultSize(int size);

        virtual void drawItemText(QPainter *p
                                 , const QRect &rect
                                 , int alignment
                                 , const QPalette &pal
                                 , bool enabled
                                 , const QString &text
                                 , QPalette::ColorRole textRole/* = QPalette::NoRole*/) const override;


    };


//} //namespace ns_styles

