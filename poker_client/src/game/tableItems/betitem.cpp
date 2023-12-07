#include <precompiled.h>
#include "betitem.h"
#include "ui_betitem.h"

#include <utils/CommonPainter.h>
#include <api_models/model_apibase.hpp>
#include <qabstractproxymodel.h>
#include <qpropertyanimation.h>

BetItem::BetItem(QWidget *parent)
    : EntityWidget(parent)
    , m_ui(new Ui::BetItem)
    , m_modelKeyNumber(0)
    , m_money(-1)
{
    m_ui->setupUi(this);
}

BetItem::BetItem(gbp_i8 n, QWidget *parent)
    : EntityWidget(parent)
    , m_ui(new Ui::BetItem)
    , m_modelKeyNumber(n)
    , m_money(-1)
{
    m_ui->setupUi(this);
}

BetItem::~BetItem() {
    delete m_ui;
}

gbp_i64 BetItem::money() const {
    return m_money;
}

QByteArray BetItem::modelKey() const {
    return QString(sm_modelKey).arg(m_modelKeyNumber).toLatin1();
}

void BetItem::updateData(const QAbstractItemModel* model)
{
    QPropertyAnimation* eff = new QPropertyAnimation(this, "money", this);
    eff->setStartValue(money());
    eff->setEndValue(index(model).data(UserType::OriginDataRole));
    eff->setDuration(400);
    eff->setEasingCurve(QEasingCurve::Linear);
    m_animQueue.enqueue(eff);
    connect(eff, &QPropertyAnimation::finished, this, [this]{
        m_animQueue.dequeue()->deleteLater();
        if (!m_animQueue.isEmpty()) {
           m_animQueue.head()->start();
        }
    });
    if (m_animQueue.head()->state() != QAbstractAnimation::Running) {
        m_animQueue.head()->start();
    }
}

void BetItem::setMoney(gbp_i64 money)
{
    if (m_money == money)
        return;

    m_money = money;
    m_ui->label->setText(Money(m_money).toString());
    setVisible(m_money > 0);
    update();
    emit moneyChanged(money);
}

void BetItem::clear() {
    setMoney(0);
}

void BetItem::setModelKeyNumber(gbp_i8 n) {
    m_modelKeyNumber = n;
}

void BetItem::moveEvent(QMoveEvent *e)
{
    if (parentWidget() != nullptr)
    {
        int parentWidth = parentWidget()->width();
        m_ui->label->setProperty("mirrored", e->pos().x() > parentWidth/2);
        m_ui->label->setStyleSheet(m_ui->label->styleSheet());
    }
    EntityWidget::moveEvent(e);
}
