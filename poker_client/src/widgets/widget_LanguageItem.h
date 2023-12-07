#pragma once

#include <QEasingCurve>
#include <QLabel>
#include <QLocale>
#include <basewidgets/entitywidget.h>
#include <basewidgets/framedbutton.h>

namespace Ui {
class LanguageItem;
}
class QAbstractButton;
class QPropertyAnimation;

namespace {
    using SuperClass = FramedButton;
}

namespace ns_widget
{
    class LanguageItem : public SuperClass
    {
        Q_OBJECT

        Q_PROPERTY(int fixedHeight READ fixedHeight WRITE setContentHeight NOTIFY fixedHeightChanged)
    private:
        Ui::LanguageItem *m_ui;
        QAction* m_language;
        QPropertyAnimation* m_animation;
        int m_fixedHeight;

    public:
        explicit LanguageItem(QAction* trigger, QWidget *parent=0);
        virtual ~LanguageItem();

        void assignLocale(const QLocale& locale);
        QLocale assignedLocale() const;

        QLocale::Language language() const;
        QAbstractButton *button() const;
        QLabel* indicator() const;

        QIcon icon() const;
        int fixedHeight() const;
        QPropertyAnimation* animation() const;
    public slots:
        void trigger();
        void setIcon(const QIcon& icon);
        void setIconSize(const QSize& size);
        void setContentHeight(int h);
        void setContentHeight(int h, int msec, QEasingCurve easing = QEasingCurve::Linear);
        QLabel* textLabel() const;
    signals:
        void triggered(QAction*);
        void focused();
        void unfocused();
        void doubleClicked();
        void scaleChanged(QSizeF scale);
        void fixedHeightChanged(int fixedHeight);

    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent *e) override;
        virtual void focusInEvent(QFocusEvent *e) override;
        virtual void focusOutEvent(QFocusEvent *e) override;
    };
}
