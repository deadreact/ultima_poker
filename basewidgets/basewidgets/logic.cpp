#include "logic.h"
#include "framedbutton.h"

Logic::Logic(FramedButton *parent)
    : QObject(nullptr)
    , m_owner(parent)
{}

Logic::~Logic() {}

DefaultFrameChangeLogic::DefaultFrameChangeLogic(FramedButton *owner)
    : Logic(owner)
    , m_effect(owner)
    , m_lastState(0)
{
    m_effect.setColor(Qt::gray);
    this->owner()->setGraphicsEffect(&m_effect);
    m_effect.setEnabled(false);
    m_blend = owner->blending();
}

DefaultFrameChangeLogic::~DefaultFrameChangeLogic() {}

void DefaultFrameChangeLogic::onStateChanged(E::ButtonFlag flag)
{
    m_effect.setEnabled(owner()->hasState(E::DISABLED));
    if (!owner()->hasState(E::DISABLED)) {
        if (owner()->hasState(E::PRESSED) | owner()->hasState(E::CHECKED)) {
            owner()->setFrameIndex(2);
        } else if (owner()->hasState(E::HOVER)) {
            owner()->setFrameIndex(1);
        } else {
            owner()->setFrameIndex(0);
        }
    }
    m_lastState = flag;
}

FramedButton *Logic::owner() const
{
    return m_owner;
}

MainMenubarLogic::MainMenubarLogic(FramedButton *_owner)
    : DefaultFrameChangeLogic(_owner)
    , m_icon(nullptr)
//    , m_text(nullptr)
    , m_shadow(new QGraphicsDropShadowEffect(this))
    , m_defaultIconFrame(0)
{
    _owner->setStyle(new MyProxyStyle);
    QString iconName = _owner->objectName().replace("bttn_", "icon_");
//    QString textName = owner->objectName().replace("bttn_", "text_");

    m_icon = _owner->findChild<FramedEntity*>(iconName);
//    m_text = owner->findChild<QLabel*>(textName);
    if (m_icon != nullptr) {
        m_shadow->setOffset(0, 1);
        m_shadow->setBlurRadius(5);
        m_shadow->setColor(QColor(0, 0, 0, 150));
        m_shadow->setEnabled(false);
        m_icon->setGraphicsEffect(m_shadow);
        m_defaultIconFrame = m_icon->frameIndex();

        connect(_owner, &FramedButton::entered, this, [this]{ m_shadow->setEnabled(true); });
        connect(_owner, &FramedButton::leaved, this, [this]{ m_shadow->setEnabled(false); });
    }

    for (QObject* child : _owner->children()) {
        if (QWidget* w = qobject_cast<QWidget*>(child)) {
            w->setAttribute(Qt::WA_TransparentForMouseEvents);
        }
    }

//    updateTexture();
}

MainMenubarLogic::~MainMenubarLogic()
{

}

void MainMenubarLogic::updateTexture()
{

}

void MainMenubarLogic::onStateChanged(E::ButtonFlag flag)
{
    if (!owner()->isEnabled()) {
        return;
    }
    if (m_icon != nullptr) {
        if (!owner()->hasState(E::DISABLED)) {
            if (owner()->hasState(E::PRESSED) | owner()->hasState(E::CHECKED)) {
                m_icon->setFrameIndex(m_defaultIconFrame+2);
            } else if (owner()->hasState(E::HOVER)) {
                m_icon->setFrameIndex(m_defaultIconFrame+1);
            } else {
                m_icon->setFrameIndex(m_defaultIconFrame);
            }
            if (lastState() == E::HOVER && flag != E::HOVER)
            {
//                m_icon->setBlending(E::Blending::Normal);
//                m_icon->graphicsEffect()->setEnabled(false);
//                m_icon->move(m_icon->pos() + QPoint(0, 1));
            }
            else if (lastState() != E::HOVER && flag == E::HOVER)
            {
//                m_icon->setBlending(E::Blending::Add);
//                m_icon->graphicsEffect()->setEnabled(true);
//                m_icon->move(m_icon->pos() - QPoint(0, 1));
            }
        }
    }
    DefaultFrameChangeLogic::onStateChanged(flag);
}

void MyProxyStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    FramedButton* button = dynamic_cast<FramedButton*>(painter->device());
    if (button && textRole == QPalette::ButtonText)
    {
        QPalette palShadow(pal);
        if (button->hasState(E::PRESSED) || button->hasState(E::CHECKED)) {
            QColor col(0x90a4ae);
            col.setAlphaF(0.5);
            palShadow.setColor(QPalette::ButtonText, col);
            QProxyStyle::drawItemText(painter, rect.adjusted(0, 1, 0, 1), flags, palShadow, enabled, text, textRole);
        } else if (button->hasState(E::HOVER)) {
            palShadow.setColor(QPalette::ButtonText, QColor(0, 0, 0, 150));
            QProxyStyle::drawItemText(painter, rect, flags, palShadow, enabled, text, textRole);
            QProxyStyle::drawItemText(painter, rect.adjusted(0, -1, 0, -1), flags, pal, enabled, text, textRole);
        } else {
            QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
        }
    } else {
        QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
    }
}

