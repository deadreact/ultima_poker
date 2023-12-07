#include <precompiled.h>
#include <qabstractproxymodel.h>
#include <qpropertyanimation.h>
#include <api_models/algorithm.hpp>
#include "potitem.h"
#include "ui_potitem.h"

PotItem::PotItem(QWidget *parent)
    : EntityWidget(parent)
    , m_ui(new Ui::PotItem)
    , m_modelKeyNumber(-1)
    , m_money(0)
{
    m_ui->setupUi(this);
}

PotItem::~PotItem()
{
    delete m_ui;
}

QByteArray PotItem::modelKey() const {
    return QString(sm_modelKey).arg(m_modelKeyNumber).toLocal8Bit();
}

PotItem *PotItem::clone() const
{
    PotItem* myClone = new PotItem(parentWidget());
    myClone->setMirrored(this->mirrored());
    myClone->setStyleSheet(styleSheet());
    myClone->setGeometry(geometry());

    return myClone;
}

bool PotItem::mirrored() const {
    return m_ui->text->property("mirrored").toBool();
}

gbp_i64 PotItem::money() const
{
    return m_money;
}

void PotItem::clear() {
    setMoney(0);
}

void PotItem::setMirrored(bool mirrored) {
    m_ui->text->setProperty("mirrored", mirrored);
}

void PotItem::updateData(const QAbstractItemModel* model)
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

void PotItem::setMoney(gbp_i64 money)
{
    if (m_money != money)
    {
        m_money = money;
        m_ui->text->setString(Money(money).toString(), 1);
        setVisible(money > 0);
    }
}

void PotItem::setModelKeyNumber(gbp_i8 n)
{
    m_modelKeyNumber = n;
}


void PotItem::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        m_ui->retranslateUi(this);
        m_ui->text->setString(Money(m_money).toString(), 1);
        e->accept();
    }
    EntityWidget::changeEvent(e);
}
