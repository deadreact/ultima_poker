#include <precompiled.h>
#include "AvatarsListWidget.h"
#include "ui_page_SelectAvatar.h"
#include <qpropertyanimation.h>
#include <animation/animation.h>


List::List(ns_model::AvatarsList *model)
    : m_first(nullptr)
    , m_last(nullptr)
    , m_model(model)
    , m_modelOffset(0)
{}

Element *List::indexElement(int index) const
{
    if (index < 0) {
        return nullptr;
    }
    Element* el = m_first;

    while (index != 0) {
        el = el->m_next;
        index--;
        if (el == nullptr) {
            return nullptr;
        }
    }
    return el;
}

QPropertyAnimation *List::adjustPositions(int xPos, int gridXSize, int msec)
{
    static const QRect defaultRc = AvatarsListWidget::quadInCenterOf(ns_const::ns_avatar_size::smallRadius, AvatarsListWidget::gridWidth(), 140);
    static const QRect bigRc     = AvatarsListWidget::quadInCenterOf(ns_const::ns_avatar_size::baseRadius,  AvatarsListWidget::gridWidth(), 140);
    static const QRect smallRc   = AvatarsListWidget::quadInCenterOf(ns_const::ns_avatar_size::microRadius, AvatarsListWidget::gridWidth(), 140);

    QPropertyAnimation* anim = ns_anim::moveTo(m_first->m_next->m_element, QPoint(xPos+gridXSize, 0), msec);

    QObject::connect(anim, &QPropertyAnimation::valueChanged, [this, gridXSize](const QVariant& val){
        Q_UNUSED(val)
        Element* target = m_first->m_next;
        int posIndex = target->m_element->x()/gridXSize;
        int xPos = target->m_element->x() - gridXSize;
        //                m_first->m_element->move(xPos - gridXSize, 0);
        target = m_first;

        const qreal diametr = 120.0;

        while (target != nullptr) {
            target->m_element->move(xPos, 0);
            if (xPos > 240 && xPos < 480) {
                qreal t = 1.0 - qreal(::abs(xPos-360))/diametr;
                QPoint extraOffset = defaultRc.topLeft() - bigRc.topLeft();
                QSize extraSize = bigRc.size() - defaultRc.size();
                QRect rc(defaultRc.x() - extraOffset.x() * t
                         , defaultRc.y() - extraOffset.y() * t
                         , defaultRc.width() + extraSize.width() * t
                         , defaultRc.height() + extraSize.height() * t);
                target->m_element->setViewport(rc);
                if (QGraphicsEffect* eff = target->m_element->graphicsEffect()) {
                    eff->setEnabled(xPos > 340 && xPos < 400);
                }
            } else if (xPos > 0 && xPos < 120) {
                qreal t = 1.0 - qreal(xPos)/diametr;
                QPoint extraOffset = defaultRc.topLeft() - smallRc.topLeft();
                QSize extraSize = smallRc.size() - defaultRc.size();
                QRect rc(defaultRc.x() - extraOffset.x() * t
                         , defaultRc.y() - extraOffset.y() * t
                         , defaultRc.width() + extraSize.width() * t
                         , defaultRc.height() + extraSize.height() * t);
                target->m_element->setViewport(rc);
            } else if (xPos > 600 && xPos < 720) {
                qreal t = 1.0 - qreal(720-xPos)/diametr;
                QPoint extraOffset = defaultRc.topLeft() - smallRc.topRight();
                QSize extraSize = smallRc.size() - defaultRc.size();
                QRect rc(defaultRc.x() - extraOffset.x() * t
                         , defaultRc.y() - extraOffset.y() * t
                         , defaultRc.width() + extraSize.width() * t
                         , defaultRc.height() + extraSize.height() * t);
                target->m_element->setViewport(rc);
            } else {
                if (QGraphicsEffect* eff = target->m_element->graphicsEffect()) {
                    eff->setEnabled(false);
                }
                target->m_element->setViewport(defaultRc);
            }
            target = target->m_next;

            xPos += gridXSize;
        }
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    return anim;
}
// -----------------------------------------------------------
// -----------------------------------------------------------


AvatarsListWidget::AvatarsListWidget(QWidget *parent)
    : EntityWidget(parent)
    , m_lst(nullptr)
//    , m_ui(new Ui::AvatarsList)
{
//    m_ui->setupUi(this);
}

AvatarsListWidget::~AvatarsListWidget()
{
//    delete m_ui;
}

void AvatarsListWidget::init(Ui::SelectAvatar *ui)
{
    ns_model::AvatarsList* model = new ns_model::AvatarsList(this);

    model->loadIcons(":textures/avatars/");
//    model->group()->setAlphaMask(*);


    Element* elements[] = { new Element(ui->item_0)
                          , new Element(ui->item_1)
                          , new Element(ui->item_2)
                          , new Element(ui->item_3)
                          , new Element(ui->item_4)
                          , new Element(ui->item_5)
                          , new Element(ui->item_6)};

    m_lst = new List(model);
    m_lst->m_first = elements[0];
    m_lst->m_last = elements[6];
    for (int i = 0; i < 6; i++) {
        elements[i]->m_next = elements[i+1];
        elements[i+1]->m_prev = elements[i];
    }
    m_lst->m_last->m_prev = elements[5];

    for (Element* el = m_lst->m_last; el != nullptr; el = el->prev()) {
        //qDebug() << el->m_element->objectName() << ", prev:" << ((el->m_prev) ? el->m_prev->m_element->objectName() : "nullptr") << ", next:" << ((el->m_next) ? el->m_next->m_element->objectName() : "nullptr");
    }

    m_lst->adjustPositions();

    connect(ui->bttn_prev, &QPushButton::clicked, this, [this]{
        m_lst->cycleRight();
        m_lst->adjustPositions();
    });
    connect(ui->bttn_next, &QPushButton::clicked, this, [this]{
        m_lst->cycleLeft();
        m_lst->adjustPositions();
    });
//    for (int i = 0; i < 11; i++) {
//        m_lst->cycleLeft();
//    }
    for (int i = 0; i < model->rowCount(); i++) {
        m_lst->cycleRight();
    }
    for (int i = 0; i < model->rowCount(); i++) {
        m_lst->cycleLeft();
    }

    for (int i = 0; i <= 6; i++) {
        elements[i]->m_element->setViewport(smallRect());
        setShadowEffect(elements[i]->m_element);
    }
    setShadowEffect(ui->bttn_prev);
    setShadowEffect(ui->bttn_next);

    ui->bttn_prev->graphicsEffect()->setEnabled(true);
    ui->bttn_next->graphicsEffect()->setEnabled(true);
    ui->item_3->graphicsEffect()->setEnabled(true);

}

QRect AvatarsListWidget::quadInCenterOf(int radius, int w, int h)
{
    return QRect(w/2 - radius, h/2 - radius, radius*2, radius*2);
}

ns_widget::ProfilePicture *AvatarsListWidget::avatarAt(const QPoint &pos) const
{
    int elementAreaId = pos.x() / gridWidth();
    int radius = gridWidth()/2;
    QPoint elementCenter(elementAreaId*gridWidth() + radius, height()/2);


    qreal distanceSq = QVector2D(pos - elementCenter).lengthSquared();
    qreal areaSq = (elementAreaId == 3) ? radius*radius : 45*45;

    if (distanceSq < areaSq) {
        if (Element* el = m_lst->indexElement(elementAreaId)) {
            return el->m_element;
        }
    }
    return nullptr;
}

void AvatarsListWidget::setShadowEffect(QWidget *w)
{
    QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect(this);
    w->setGraphicsEffect(eff);
    eff->setColor(QColor(0, 0, 0, 128));
    eff->setBlurRadius(21);
    eff->setOffset(0, 8);
    eff->setEnabled(false);
}


void AvatarsListWidget::mousePressEvent(QMouseEvent *e)
{
    m_pressedItem = avatarAt(e->pos());
    EntityWidget::mousePressEvent(e);
}

void AvatarsListWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_pressedItem == avatarAt(e->pos())) {
        onItemClicked(m_pressedItem);
        m_pressedItem = nullptr;
    }
    EntityWidget::mouseReleaseEvent(e);
}

void AvatarsListWidget::mouseMoveEvent(QMouseEvent *e)
{
    EntityWidget::mouseMoveEvent(e);

    if (avatarAt(e->pos())) {
        setCursor(Qt::PointingHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void AvatarsListWidget::wheelEvent(QWheelEvent *e)
{
    EntityWidget::wheelEvent(e);
}

void AvatarsListWidget::enterEvent(QEvent *e)
{
    EntityWidget::enterEvent(e);
}

void AvatarsListWidget::leaveEvent(QEvent *e)
{
    EntityWidget::leaveEvent(e);
}

void AvatarsListWidget::onItemClicked(ns_widget::ProfilePicture *item, bool emitSignal)
{
    if (item != nullptr) {
        int index = item->x()/gridWidth();

        switch (index) {
        case 0: break;
        case 1:
        {
            m_lst->cycleRight();
            connect(m_lst->adjustPositions(0, 120, 150), &QPropertyAnimation::finished, this, [this, item] { onItemClicked(item, false); });
            break;
        }
        case 2:
        {
            m_lst->cycleRight();
            m_lst->adjustPositions(0, 120, (emitSignal) ? 300 : 150);
            break;
        }
        case 3: break;
        case 5:
        {
            m_lst->cycleLeft();
            connect(m_lst->adjustPositions(0, 120, 150), &QPropertyAnimation::finished, this, [this, item] { onItemClicked(item, false); });
            break;
        }
        case 4:
        {
            m_lst->cycleLeft();
            m_lst->adjustPositions(0, 120, (emitSignal) ? 300 : 150);
            break;
        }
        case 6: break;
        }
        if (emitSignal) {
            emit avatarClicked(item->avatar());
        }
    }
}


void AvatarsListWidget::showEvent(QShowEvent *e)
{
    static const QRect bigRc = AvatarsListWidget::quadInCenterOf(ns_const::ns_avatar_size::baseRadius,  AvatarsListWidget::gridWidth(), 140);
    m_lst->m_first->m_next->m_next->m_next->m_element->setViewport(bigRc);
//    onItemClicked(m_element, false);
    EntityWidget::showEvent(e);
}
