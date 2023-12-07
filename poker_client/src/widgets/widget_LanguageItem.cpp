#include <precompiled.h>
#include "widgets/widget_LanguageItem.h"
#include "ui_widget_LanguageItem.h"
#include <qpropertyanimation.h>

namespace
{
    QString&& capitalized(QString&& str)
    {
        str[0] = str[0].toUpper();
        return std::move(str);
    }
}

namespace ns_widget
{
    LanguageItem::LanguageItem(QAction *trigger, QWidget *parent)

        : SuperClass(parent)
        , m_ui(new Ui::LanguageItem)
        , m_language(trigger)
        , m_animation(new QPropertyAnimation(this, "fixedHeight", parent))
        , m_fixedHeight(46)
    {
        m_ui->setupUi(this);
        assignLocale(QLocale(trigger->data().toString()));
        if (!trigger->icon().isNull()) {
            setIcon(trigger->icon());
        }
//        connect(m_ui->bttn_icon, &QAbstractButton::toggled, this, [this](bool toggled){ if (toggled) this->trigger(); });
//        m_animation->setEasingCurve(QEasingCurve::OutSine);
    }

    void LanguageItem::assignLocale(const QLocale &locale)
    {
        QString lang = std::move(capitalized(std::move(locale.nativeLanguageName())));
        if (lang == "American English") {
            lang = "English";
        }
        setLocale(locale);
        m_ui->text->setText(lang);

        QString iconPath = QString(":textures/Common/icons/ico/%0.ico").arg(QLocale::countryToString(locale.country()));
        if (QFileInfo(iconPath).exists()) {
            m_ui->bttn_icon->setIcon(QIcon(iconPath));
        }
    }

    QLocale LanguageItem::assignedLocale() const {
        return locale();
    }

    QLocale::Language LanguageItem::language() const {
        return locale().language();
    }

    QAbstractButton *LanguageItem::button() const {
        return m_ui->bttn_icon;
    }

    QLabel *LanguageItem::indicator() const {
        return m_ui->check;
    }

    QIcon LanguageItem::icon() const {
        return m_ui->bttn_icon->icon();
    }

    int LanguageItem::fixedHeight() const
    {
        return m_fixedHeight;
    }

    QPropertyAnimation *LanguageItem::animation() const
    {
        return m_animation;
    }

    void LanguageItem::trigger()
    {
        m_language->trigger();
        emit triggered(m_language);
    }

    void LanguageItem::setIcon(const QIcon &icon)
    {
        m_ui->bttn_icon->setIcon(icon);
    }

    void LanguageItem::setIconSize(const QSize &size)
    {
        m_ui->bttn_icon->setIconSize(size);
    }

    void LanguageItem::setContentHeight(int h)
    {
        if (m_fixedHeight == h)
            return;

        m_fixedHeight = h;
//        SuperClass::setFixedHeight(h);
//        m_ui->bttn_icon->setFixedHeight(h);
        m_ui->bttn_icon->setIconSize(QSize(h, h));
        emit fixedHeightChanged(h);
    }

    void LanguageItem::setContentHeight(int h, int msec, QEasingCurve easing)
    {
        if (m_animation->state() == QAbstractAnimation::Running) {
            m_animation->stop();
        }
        m_animation->setEasingCurve(easing);
        m_animation->setStartValue(fixedHeight());
        m_animation->setEndValue(h);
        m_animation->setDuration(msec);
        m_animation->start();
    }

    QLabel *LanguageItem::textLabel() const
    {
        return m_ui->text;
    }

    LanguageItem::~LanguageItem()
    {
        delete m_animation;
        delete m_ui;
    }
    void LanguageItem::mouseDoubleClickEvent(QMouseEvent *e) {
        emit doubleClicked();
        SuperClass::mouseDoubleClickEvent(e);
    }

    void LanguageItem::focusInEvent(QFocusEvent *e) {
        emit focused();
        SuperClass::focusInEvent(e);
    }

    void LanguageItem::focusOutEvent(QFocusEvent *e) {
        emit unfocused();
        SuperClass::focusOutEvent(e);
    }
}


