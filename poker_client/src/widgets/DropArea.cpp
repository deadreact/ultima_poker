#include <precompiled.h>
#include <qevent.h>
#include "DropArea.h"
#include <QFileInfo>

DropArea::DropArea(QWidget *parent)
    : QWidget(parent)
{

}

const QStringList &DropArea::acceptedFormats() const {
    return m_acceptedFormats;
}

void DropArea::setAcceptedFormats(const QStringList &acceptedFormats) {
    m_acceptedFormats = acceptedFormats;
}

void DropArea::addAcceptedFormat(const QString &acceptedFormat) {
    m_acceptedFormats << acceptedFormat;
}


void DropArea::dragEnterEvent(QDragEnterEvent *e)
{
    for (const QUrl& url: e->mimeData()->urls()) {
        if (m_acceptedFormats.contains(QFileInfo(url.toLocalFile()).completeSuffix(), Qt::CaseInsensitive))
        {
            e->acceptProposedAction();
            return;
        }
    }
    QWidget::dragEnterEvent(e);
}

void DropArea::dragMoveEvent(QDragMoveEvent *e)
{
	QWidget::dragMoveEvent(e);
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *e)
{
	QWidget::dragLeaveEvent(e);
}

void DropArea::dropEvent(QDropEvent *e)
{
    emit objectDropped(e->mimeData());
	QWidget::dropEvent(e);
}
