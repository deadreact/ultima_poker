#pragma once

#include <QWidget>
#include <QProxyStyle>

namespace Ui {
class VipSystemDescription;
}
namespace ns_model {
class VipStatusInfo;
}

class VipSystemToolBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    VipSystemToolBoxStyle(QStyle* parent = nullptr);
    virtual ~VipSystemToolBoxStyle();
    virtual void drawItemText(QPainter *p, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

    virtual void drawControl(ControlElement element, const QStyleOption *opt1, QPainter *p, const QWidget *w) const override;
    virtual QRect itemTextRect(const QFontMetrics &fm, const QRect &r, int flags, bool enabled, const QString &text) const override;
};

namespace ns_page
{
    class VipSystemDescription : public QWidget
    {
        Q_OBJECT
    private:
        Ui::VipSystemDescription *m_ui;
        QScopedPointer<ns_model::VipStatusInfo> m_model;
    public:
        explicit VipSystemDescription(QWidget *parent = nullptr);
        virtual ~VipSystemDescription();

        virtual void init();

    protected:
        void initShadows();
        virtual void changeEvent(QEvent* e) override;
    };

} //namespace ns_page
