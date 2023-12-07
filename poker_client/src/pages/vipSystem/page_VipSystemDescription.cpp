#include <precompiled.h>
#include "page_VipSystemDescription.h"
#include "ui_page_vipSystemDescription.h"
#include <QPainter>
#include <QBitmap>
#include <basewidgets/util.h>
#include <qabstractbutton.h>
#include <QGraphicsEffect>
#include <models/model_VipStatusInfo.h>
#include <styles/shadowedtextstyle.h>

// =================== VipSystemToolBoxStyle ===================

namespace
{
    QGradient toolBoxGradient(const QColor& colorFrom, const QColor& colorTo)
    {
        QLinearGradient gradient(0, 0, 0, 1);
        gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
        gradient.setColorAt(0., Qt::transparent);
        gradient.setColorAt(0.148, Qt::transparent);
        gradient.setColorAt(0.15, colorFrom);
        gradient.setColorAt(0.85, colorTo);
        gradient.setColorAt(0.852, Qt::transparent);
        gradient.setColorAt(1., Qt::transparent);

        return gradient;
    }

    QString tabTitleText(int i)
    {
        switch (i) {
        case 0: return ns_page::VipSystemDescription::tr("@BEGINNER");
        case 1: return ns_page::VipSystemDescription::tr("@SEMI-PRO");
        case 2: return ns_page::VipSystemDescription::tr("@PROFESSIONAL");
        case 3: return ns_page::VipSystemDescription::tr("@ELITE");
        case 4: return ns_page::VipSystemDescription::tr("@MASTER");
        case 5: return ns_page::VipSystemDescription::tr("@LEGEND");
        default:
            return QString();
        }
    }

    QBrush brushForTab(int i)
    {
        static QList<QBrush> brushLst;
        if (brushLst.isEmpty())
        {
            brushLst << toolBoxGradient(0xffc792, 0xa35f18) << toolBoxGradient(0x898989, 0x555555)
                     << toolBoxGradient(0xffcd78, 0xbd7822) << toolBoxGradient(0x969cae, 0x60687d)
                     << toolBoxGradient(0x8baac1, 0x497097) << toolBoxGradient(0x363636, 0x111111);
        }
        return brushLst.at(i);
    }

    QImage imageForTab(int i, const QRect& rc = QRect(0, 0, 950, 42))
    {
        static QList<QImage> imgLst;
        auto drawImg = [rc](int i)
        {
            QImage img(rc.size(), QImage::Format_RGBA8888);
            img.fill(Qt::transparent);
            QPainter p;
            p.begin(&img);

            p.fillRect(rc, brushForTab(i));
            static const QPixmap pix(":textures/vip_system_page/mask.png");
            static const QPixmap ico(":textures/vip_system_page/icon_beginner.png");

            p.setCompositionMode(QPainter::CompositionMode_Overlay);
            p.setPen(QColor(255, 255, 255, 255*0.9));
            QLine borderTop(rc.topLeft(), rc.topRight());
            borderTop.translate(0, 5);
            p.drawLine(borderTop);
            ns_util::setBlendingOption(E::Blending::Normal, p);
            p.setPen(QColor(0x191919));
            p.drawLine(borderTop.translated(0, 31));
            p.setPen(QColor(0x34424a));
            p.drawLine(borderTop.translated(0, 32));

            p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            p.drawPixmap(52, rc.center().y() - pix.height()/2 + 1, pix);

            ns_util::setBlendingOption(E::Blending::Normal, p);
            p.drawPixmap(54, rc.center().y() - pix.height()/2 + 1, ico);

            p.end();
            return img;
        };
        if (imgLst.isEmpty())
        {
            imgLst << drawImg(0) << drawImg(1) << drawImg(2) << drawImg(3) << drawImg(4) << drawImg(5);
        }

        return imgLst.at(i);
    }

    QColor textColor(int i)
    {
        static QList<uint> colorLst;
        if (colorLst.isEmpty()) {
            colorLst << 0x7b430c << 0x363636 << 0x825a0e << 0x404655 << 0x37474f << 0xb71c1c;
        }
        return QColor(colorLst.at(i));
    }
}


VipSystemToolBoxStyle::VipSystemToolBoxStyle(QStyle *parent):
    QProxyStyle(parent)
{}

VipSystemToolBoxStyle::~VipSystemToolBoxStyle() {}

void VipSystemToolBoxStyle::drawItemText(QPainter *p, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    static const int offset = 106;
    QWidget* w = dynamic_cast<QWidget*>(p->device());

    QPalette mainPal = pal;
    int index = text.toInt();
    mainPal.setColor(textRole, textColor(index));
    if (w && (QString(w->metaObject()->className()) == "QToolBoxButton"))
    {
        QFont fnt("Open Sans Extrabold");
        fnt.setPixelSize(16);
        p->setFont(fnt);
        QString trText = tabTitleText(index);
        if (enabled)
        {
            ns_util::setBlendingOption(E::Blending::Overlay, *p);
            QPalette shadowPal = pal;
            shadowPal.setColor(textRole, QColor(255, 255, 255, 255*0.4));
            QProxyStyle::drawItemText(p, rect.adjusted(offset, 1, 0, 1), flags, shadowPal, enabled, trText, textRole);
        }
        ns_util::setBlendingOption(E::Blending::Normal, *p);
        QProxyStyle::drawItemText(p, rect.adjusted(offset, 0, 0, 0), flags, mainPal, enabled, trText, textRole);
        {// inner swadow
            QPalette shadowPal = pal;
            shadowPal.setColor(textRole, QColor(25, 25, 25, 255*0.4));
            p->setClipRect(rect.adjusted(0, 0, 0, 16 - rect.height()));
            QProxyStyle::drawItemText(p, rect.adjusted(offset, 0, 0, 0), flags, shadowPal, enabled, trText, textRole);
        }
    }
    else
    {
        QProxyStyle::drawItemText(p, rect, flags, pal, enabled, text, textRole);
    }
}


void VipSystemToolBoxStyle::drawControl(QStyle::ControlElement element, const QStyleOption *opt1, QPainter *p, const QWidget *w) const
{
    if (element == QStyle::CE_ToolBoxTabShape)
    {
        QStyleOptionToolBox *opt = new QStyleOptionToolBox(*static_cast<const QStyleOptionToolBox*>(opt1));

//        opt->rect.adjust(0, 4, 0, -4);
        if (const QStyleOptionToolBox* myOpt = qstyleoption_cast<const QStyleOptionToolBox*>(opt)) {
            QImage img = imageForTab(myOpt->text.toInt(), opt->rect);
            if (opt->state & QStyle::State_Sunken) {
//                QImage img2(img.size(), QImage::Format_RGBA8888);
//                img2.fill(QColor(100, 100, 100, 210));
//                img.setAlphaChannel(img2);
                p->drawImage(opt->rect, img);
            } else {
                p->drawImage(opt->rect, img);
            }
            if (opt->state & QStyle::State_MouseOver) {
//                p->setPen(QColor(250, 250, 250, 55));
//                p->setCompositionMode(QPainter::CompositionMode_Screen);
//                p->drawRect(opt->rect.adjusted(1, 4, -1, -6));
//                p->setCompositionMode(QPainter::CompositionMode_SourceOver);
            }

            static const QPixmap arrows(":textures/vip_system_page/arrows.png");

            if (opt->state & QStyle::State_Selected) {
                p->drawPixmap(opt->rect.right() - 35,  opt->rect.center().y() - arrows.height()/4, arrows, 0, 0, arrows.width(), arrows.height()/2);
            } else {
                p->drawPixmap(opt->rect.right() - 35,  opt->rect.center().y() - arrows.height()/4, arrows, 0, arrows.height()/2, arrows.width(), arrows.height()/2);
            }
        }
    }
    else
    {
        QProxyStyle::drawControl(element, opt1, p, w);
    }
}

QRect VipSystemToolBoxStyle::itemTextRect(const QFontMetrics &fm, const QRect &r, int flags, bool enabled, const QString &text) const
{
    return QProxyStyle::itemTextRect(fm, r, flags, enabled, text).adjusted(50, 10, 10, 10);
}

// =================== VipSystemToolBoxStyle ===================



namespace ns_page
{
    VipSystemDescription::VipSystemDescription(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::VipSystemDescription)
        , m_model(new ns_model::VipStatusInfo)
    {
        m_ui->setupUi(this);
        imageForTab(0);

        m_ui->toolBox->setStyle(new VipSystemToolBoxStyle(m_ui->toolBox->style()));
        QList<QAbstractButton*> lst = m_ui->toolBox->findChildren<QAbstractButton*>();
        for (QAbstractButton* bttn: lst) {
            if (QString(bttn->metaObject()->className()) == "QToolBoxButton") {
                bttn->setFixedHeight(42);
                bttn->setStyle(new VipSystemToolBoxStyle);
//                bttn->setBackgroundRole(QPalette::ColorRole::Dark);
            }
        }
    }



    VipSystemDescription::~VipSystemDescription()
    {
        delete m_ui;
    }


    void VipSystemDescription::init()
    {
        QList<VipStatusItem*> pages;
        pages << m_ui->page_beginner << m_ui->page_semi_pro << m_ui->page_professional << m_ui->page_elite << m_ui->page_master << m_ui->page_legend;

        int row = 0;
        for (VipStatusItem* item: pages) {
            item->init();
            item->setModel(m_model.data(), row++);
        }
        QList<QAbstractScrollArea*> areas = findChildren<QAbstractScrollArea*>();

        for (QAbstractScrollArea* scroll: areas) {
            scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

        initShadows();


    }

    void VipSystemDescription::initShadows()
    {
        setTextDropShadowStyle(m_ui->text_vipStatusInfo, QPoint(0, -1), QColor(25, 25, 25, 125));
    }

    void VipSystemDescription::changeEvent(QEvent *e)
    {
        if (e && e->type() == QEvent::LanguageChange)
        {
            m_ui->retranslateUi(this);
        }
        QWidget::changeEvent(e);
    }

} //namespace ns_page

