#pragma once

#include "basewidgets_global.h"
#include <QGraphicsEffect>
#include <QObject>
#include <QProxyStyle>
#include "util.h"

class FramedButton;
class FramedEntity;

class BASEWIDGETS_DLL Logic : public QObject
{
    Q_OBJECT
private:
    FramedButton* m_owner;
public:
    explicit Logic(FramedButton* parent);
    virtual ~Logic();
    FramedButton* owner() const;
public slots:
    virtual void onStateChanged(E::ButtonFlag flag) = 0;
};

class BASEWIDGETS_DLL DefaultFrameChangeLogic : public Logic
{
    Q_OBJECT

    QGraphicsColorizeEffect m_effect;
    E::ButtonFlag m_lastState;
    E::Blending m_blend;
public:
    DefaultFrameChangeLogic(FramedButton* owner);
    virtual ~DefaultFrameChangeLogic();
protected:
    E::ButtonFlag lastState() const { return m_lastState; }
public slots:
    virtual void onStateChanged(E::ButtonFlag flag) override;
};

class BASEWIDGETS_DLL MainMenubarLogic : public DefaultFrameChangeLogic
{
    Q_OBJECT

    FramedEntity* m_icon;
//    QLabel* m_text;
    QGraphicsDropShadowEffect* m_shadow;
    int m_defaultIconFrame;
public:
    MainMenubarLogic(FramedButton* owner);
    virtual ~MainMenubarLogic();
public slots:
    void updateTexture();
    virtual void onStateChanged(E::ButtonFlag flag) override;
};

class BASEWIDGETS_DLL MyProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole /* = QPalette::NoRole */) const;
};
