#include <precompiled.h>
#include "initable.h"
#include "dialogs/dialog_okmessage.h"
#include "utils/logger.h"




InitableDialog::InitableDialog(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{}

InitableDialog::InitableDialog(InitableDialog *linkedFrom, Qt::WindowFlags flags)
    : QDialog(linkedFrom->parentWidget(), flags)
    , Linkable(linkedFrom)
{
}

InitableDialog::~InitableDialog() { }

void InitableDialog::onInitialized()
{
    if (!m_isInitialized)
    {
        m_isInitialized = true;
        emit initialized();
    }
    else
    {
        Logger::warning(QString("Trying to initialize object of type %0, that is already initialized").arg(this->metaObject()->className()).toLatin1(), __FILE__, __LINE__);
    }
}

void InitableDialog::cancel()
{
    if (InitableDialog* dialog = dynamic_cast<InitableDialog*>(m_prev))
    {
        dialog->cancel();
    }
    emit canceled();
}

//---------------------------------------- INITABLE_WIDGET ---------------------------------
InitableWidget::InitableWidget(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , Initable()
{}

InitableWidget::~InitableWidget() {  }

void InitableWidget::onInitialized()
{
    if (!m_isInitialized)
    {
        m_isInitialized = true;
        emit initialized();
    }
    else
    {
        Logger::warning(QString("Trying to initialize object of type %0, that is already initialized").arg(this->metaObject()->className()).toLatin1(), __FILE__, __LINE__);
    }
}

void Linkable::setNext(Linkable *next)
{
    m_next = next;
}

bool Linkable::hasPrev() const
{
    return m_prev != nullptr;
}

bool Linkable::hasNext() const
{
    return m_next != nullptr;
}

Linkable::~Linkable()
{

}

void Linkable::setPrev(Linkable *prev)
{
    m_prev = prev;
}

Linkable *Linkable::getNext() const
{
    return m_next;
}

Linkable *Linkable::getPrev() const
{
    return m_prev;
}

const Linkable *Linkable::getFirst() const
{
    if (hasPrev()) {
        return m_prev->getFirst();
    }
    return this;
}
Linkable *Linkable::getFirst()
{
    if (hasPrev()) {
        return m_prev->getFirst();
    }
    return this;
}

QWidget *Linkable::prevWidget() const {
    return dynamic_cast<QWidget*>(getPrev());
}
QWidget *Linkable::nextWidget() const {
    return dynamic_cast<QWidget*>(getNext());
}


bool InitableDialog::stepForward()
{
    if (m_next == nullptr)
    {
        QString errMsgText = QString("Dialog of type %0, named %1 has no link to next one\nFile: (%2), line (%3)").arg(metaObject()->className()).arg(objectName()).arg(__FILE__).arg(__LINE__);
        return ns_dialog::OkMessage::execMessage(errMsgText, "Assert : Missing dialog link!", QRect(100, 200, 600, 300));
    }

    if (QDialog* next = dynamic_cast<QDialog*>(m_next))
    {
        this->hide();
        next->show();
        return true;
    }
    return false;
}

bool InitableDialog::stepBackward()
{
    if (m_prev == nullptr)
    {
        QString errMsgText = QString("Dialog of type %0, named %1 has no link to previous one\nFile: (%2), line (%3)").arg(metaObject()->className()).arg(objectName()).arg(__FILE__).arg(__LINE__);
        return ns_dialog::OkMessage::execMessage(errMsgText, "Assert : Missing dialog link!", QRect(100, 200, 600, 300));
    }


    if (QDialog* prev = dynamic_cast<QDialog*>(m_prev))
    {
        this->hide();
        prev->show();
        return true;
    }
    return false;
}

Initable::~Initable()
{

}
