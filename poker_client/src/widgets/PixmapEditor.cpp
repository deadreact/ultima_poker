#include <precompiled.h>
#include "PixmapEditor.h"
#include "ui_PixmapEditor.h"
#include <QDebug>

namespace {
    inline QPoint operator+(const QPoint &p, const QSize &s) {
        return QPoint(p.x()+s.width(), p.y()+s.height());
    }
    inline QPoint operator-(const QPoint &p, const QSize &s) {
        return QPoint(p.x()-s.width(), p.y()-s.height());
    }
}

PixmapEditor::PixmapEditor(QWidget *parent)
    : FramedEntity(parent)
    , m_ui(new Ui::PixmapEditor)
    , m_pressedCornerIndex(-1)
    , m_dragActive(false)
    , m_selection(0.0, 0.0, 0.0, 1.0)
    , m_displaySelection(SelectionType::RectSelection)
{
    m_ui->setupUi(this);
    m_corners[0] = m_ui->corner_0;
    m_corners[1] = m_ui->corner_1;
    m_corners[2] = m_ui->corner_2;
    m_corners[3] = m_ui->corner_3;

    connect(this, &PixmapEditor::pixmapChanged, this, [this](const QPixmap& pix){
        if (pix.width() > width()) {

        }
        m_scaledPixmap = pix.scaled(QSize(width(), width()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_scale = qreal(pix.width()) / m_scaledPixmap.width();
        setFixedHeight(m_scaledPixmap.height());
    });
}

PixmapEditor::~PixmapEditor()
{
    delete m_ui;
}

void PixmapEditor::init()
{
    int size = std::min(100., 100./m_scale);
    m_ui->squareFrame->setMinimumSize(size, size);
    m_ui->selectionFrame->setMinimumSize(size, size);
    setCornerSize(8);
    validatePositions();
}

QPixmap PixmapEditor::selectedFragment(int type) const
{
    if (!pixmapValid()) {
        return QPixmap();
    }
    if (type == -1) {
        type = m_displaySelection;
    }

    QRect targetRect = m_ui->selectionFrame->geometry();

    if (type != RectSelection) {
        if (targetRect.width() > targetRect.height()) {
            if (type & Qt::AlignRight) {
                targetRect.setX(targetRect.x() + targetRect.width() - targetRect.height());
            } else if (type & Qt::AlignHCenter) {
                targetRect.setX(targetRect.x() + (targetRect.width() - targetRect.height())/2);
            }
            targetRect.setWidth(targetRect.height());
        } else {
            if (type & Qt::AlignBottom) {
                targetRect.setY(targetRect.y() + targetRect.height() - targetRect.width());
            } else if (type & Qt::AlignVCenter) {
                targetRect.setY(targetRect.y() + (targetRect.height() - targetRect.width())/2);
            }
            targetRect.setHeight(targetRect.width());
        }
    }

    return pixmap().copy(targetRect.x()*m_scale, targetRect.y()*m_scale, targetRect.width()*m_scale, targetRect.height()*m_scale);
}

void PixmapEditor::setDisplaySelection(PixmapEditor::SelectionType displaySelection)
{
    m_displaySelection = displaySelection;
    m_ui->squareFrame->setVisible(m_displaySelection != RectSelection);
}

void PixmapEditor::setSelection(QRectF selection)
{
    if (m_selection == selection)
        return;

    m_selection = selection;
    emit selectionChanged(selection);
}

void PixmapEditor::setCornerSize(int size)
{
    m_corners[0]->setFixedSize(size, size);
    m_corners[1]->setFixedSize(size, size);
    m_corners[2]->setFixedSize(size, size);
    m_corners[3]->setFixedSize(size, size);
}

void PixmapEditor::cornerPressed() {
    QObject* corner = sender();
    for (int i = 0; i < 4; i++) {
        if (corner == m_corners[i]) {
            m_pressedCornerIndex = i;
            return;
        }
    }
}

void PixmapEditor::cornerReleased() {
    if (m_pressedCornerIndex != -1) {
        validatePositions();
        updateCursors();
//        QRect rc = m_ui->squareFrame->rect();
//        QPoint offset = QPoint(m_corners[0]->width()/2, m_corners[0]->height()/2);

//        m_corners[0]->move(offset + m_ui->squareFrame->mapToParent(rc.topLeft()));
//        m_corners[1]->move(offset + m_ui->squareFrame->mapToParent(rc.topRight()));
//        m_corners[2]->move(offset + m_ui->squareFrame->mapToParent(rc.bottomRight()));
//        m_corners[3]->move(offset + m_ui->squareFrame->mapToParent(rc.bottomLeft()));
//        validatePositions();


        m_pressedCornerIndex = -1;
    }
}

void PixmapEditor::beginDrag()
{
    m_dragActive = true;
    m_dragAnchor = QCursor::pos() - qobject_cast<QWidget*>(sender())->pos();
}

void PixmapEditor::endDrag()
{
    m_dragActive = false;
}

void PixmapEditor::updateSquareRect()
{
    if (!m_ui->squareFrame->isVisible()) {
        return;
    }
    QRect targetRect = m_ui->selectionFrame->rect();

    if (targetRect.width() > targetRect.height()) {
        if (m_displaySelection & Qt::AlignRight) {
            targetRect.setX(targetRect.x() + targetRect.width() - targetRect.height());
        } else if (m_displaySelection & Qt::AlignHCenter) {
            targetRect.setX(targetRect.x() + (targetRect.width() - targetRect.height())/2);
        }
        targetRect.setWidth(targetRect.height());
    } else {
        if (m_displaySelection & Qt::AlignBottom) {
            targetRect.setY(targetRect.y() + targetRect.height() - targetRect.width());
        } else if (m_displaySelection & Qt::AlignVCenter) {
            targetRect.setY(targetRect.y() + (targetRect.height() - targetRect.width())/2);
        }
        targetRect.setHeight(targetRect.width());
    }
    m_ui->squareFrame->setGeometry(targetRect);
}

void PixmapEditor::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragActive) {
//        QPoint newPos;
        int x = qBound(0, QCursor::pos().x() - m_dragAnchor.x(), width() - m_ui->selectionFrame->width());
        int y = qBound(0, QCursor::pos().y() - m_dragAnchor.y(), height() - m_ui->selectionFrame->height());
        m_ui->selectionFrame->move(x, y);
        m_corners[0]->move(m_ui->selectionFrame->pos() - m_corners[0]->size()/2);
        m_corners[2]->move(m_corners[0]->pos() + m_ui->selectionFrame->size());
        m_corners[1]->move(m_corners[2]->x(), m_corners[0]->y());
        m_corners[3]->move(m_corners[0]->x(), m_corners[2]->y());
        QRect newFrame = m_ui->selectionFrame->geometry();
        setSelection(QRectF(qreal(newFrame.x())/ width(), qreal(newFrame.y())/ height(), qreal(newFrame.width())/ width(), qreal(newFrame.height())/ height()));
    }
    if (m_pressedCornerIndex >= 0 && m_pressedCornerIndex <= 3) {
        switch (m_pressedCornerIndex) {
        case 0: setDisplaySelection(SquareSelectionRightBottom);break;
        case 1: setDisplaySelection(SquareSelectionRightTop);break;
        case 2: setDisplaySelection(SquareSelectionLeftTop);break;
        case 3: setDisplaySelection(SquareSelectionLeftBottom);break;
        }

        int minS = m_ui->selectionFrame->minimumHeight();

        QPoint targetPoint;
        if (m_pressedCornerIndex == 0) {
            targetPoint = QPoint(qBound(-4, e->x(), m_corners[2]->x() - minS), qBound(-4, e->y(), m_corners[2]->y() - minS));
            int diff = abs(targetPoint.x() - m_corners[2]->x()) - abs(targetPoint.y() - m_corners[2]->y());
            if (diff > 0) {
                targetPoint.rx() += diff;
            } else {
                targetPoint.ry() -= diff;
            }
            m_corners[0]->move(targetPoint);
            m_corners[3]->move(targetPoint.x(), m_corners[3]->y());
            m_corners[1]->move(m_corners[1]->x(), targetPoint.y());
        }
        if (m_pressedCornerIndex == 2) {
            targetPoint = QPoint(qBound(m_corners[0]->x() + minS, e->x(), width()), qBound(m_corners[0]->y() + minS, e->y(), height()));
            int diff = abs(targetPoint.x() - m_corners[0]->x()) - abs(targetPoint.y() - m_corners[0]->y());
            if (diff > 0) {
                targetPoint.rx() -= diff;
            } else {
                targetPoint.ry() += diff;
            }
            m_corners[2]->move(targetPoint);
            m_corners[1]->move(targetPoint.x(), m_corners[1]->y());
            m_corners[3]->move(m_corners[3]->x(), targetPoint.y());
        }

        if (m_pressedCornerIndex == 1) {
            targetPoint = QPoint(qBound(m_corners[3]->x() + minS, e->x(), width()), qBound(-4, e->y(), m_corners[3]->y() - minS));
            int diff = abs(targetPoint.x() - m_corners[3]->x()) - abs(targetPoint.y() - m_corners[3]->y());
            if (diff > 0) {
                targetPoint.rx() -= diff;
            } else {
                targetPoint.ry() -= diff;
            }
            m_corners[1]->move(targetPoint);
            m_corners[2]->move(targetPoint.x(), m_corners[2]->y());
            m_corners[0]->move(m_corners[0]->x(), targetPoint.y());
        }
        if (m_pressedCornerIndex == 3) {
            targetPoint = QPoint(qBound(-4, e->x(), m_corners[1]->x() - minS), qBound(m_corners[1]->y() + minS, e->y(), height()));
            int diff = abs(targetPoint.x() - m_corners[1]->x()) - abs(targetPoint.y() - m_corners[1]->y());
            if (diff > 0) {
                targetPoint.rx() += diff;
            } else {
                targetPoint.ry() += diff;
            }
            m_corners[3]->move(targetPoint);
            m_corners[0]->move(targetPoint.x(), m_corners[0]->y());
            m_corners[2]->move(m_corners[2]->x(), targetPoint.y());
        }

//        m_ui->selectionFrame->setGeometry(QRect(m_corners[0]->pos(), m_corners[2]->pos()));
        validatePositions();

        //  0 | 1
//        //  -----
//        //  3 | 2
//        int w = m_corners[m_pressedCornerIndex]->x() - m_corners[(m_pressedCornerIndex+2)%4]->x();
//        int h = m_corners[m_pressedCornerIndex]->y() - m_corners[(m_pressedCornerIndex+2)%4]->y();
//        if (m_pressedCornerIndex % 2) {
//            if (abs(w) > abs(h)) {
//                m_corners[m_pressedCornerIndex]->move(e->x()-(w + h), e->y());
//            } else {
//                m_corners[m_pressedCornerIndex]->move(e->x(), e->y()-(h+w));
//            }
//            m_corners[0]->move(m_corners[1]->x(), m_corners[3]->y());
//            m_corners[2]->move(m_corners[3]->x(), m_corners[1]->y());
//        } else {
//            if (abs(w) > abs(h)) {
//                m_corners[m_pressedCornerIndex]->move(e->x() - w + h, e->y());
//            } else {
//                m_corners[m_pressedCornerIndex]->move(e->x(), e->y() - h + w);
//            }
//            m_corners[1]->move(m_corners[0]->x(), m_corners[2]->y());
//            m_corners[3]->move(m_corners[2]->x(), m_corners[0]->y());
//        }
//        validatePositions();
    }
}

int PixmapEditor::heightForWidth(int w) const
{
    return w * qreal(pixmap().height())/pixmap().width();
}

bool PixmapEditor::hasHeightForWidth() const
{
    return pixmapValid();
}

QRectF PixmapEditor::selection() const
{
    return m_selection;
}

int PixmapEditor::cornerSize() const
{
    return m_corners[0]->width();
}

void PixmapEditor::resizeEvent(QResizeEvent *e)
{
    FramedEntity::resizeEvent(e);
}

void PixmapEditor::updateCursors()
{

}

void PixmapEditor::validatePositions()
{
    int halfSize = cornerSize()/2;
//    QString text("p0(%0, %1);\np1(%2, %3);\np2(%4, %5);\np3(%6, %7);\n");
    for (int i = 0; i < 4; i++) {
        m_corners[i]->move( qBound(-halfSize, m_corners[i]->x(), width() - halfSize)
                          , qBound(-halfSize, m_corners[i]->y(), height() - halfSize) );
//        text = text.arg(m_corners[i]->x()).arg(m_corners[i]->y());
    }
//    m_ui->label->setText(text);


    QRect newFrame(m_corners[0]->pos(), m_corners[2]->pos());
//    int minW = m_ui->selectionFrame->minimumWidth();
//    int minH = m_ui->selectionFrame->minimumHeight();

//    if (newFrame.width() < minW) {
//        if (m_pressedCornerIndex == 0 || m_pressedCornerIndex == 3) {
//            m_corners[0]->move(m_corners[1]->x() - minW - halfSize, m_corners[0]->y());
//            m_corners[3]->move(m_corners[1]->x() - minW - halfSize, m_corners[3]->y());
//        } else {
//            m_corners[1]->move(m_corners[0]->x() + minW - halfSize, m_corners[1]->y());
//            m_corners[2]->move(m_corners[0]->x() + minW - halfSize, m_corners[2]->y());
//        }
//    }
//    if (newFrame.height() < minH) {
//        if (m_pressedCornerIndex == 0 || m_pressedCornerIndex == 1) {
//            m_corners[0]->move(m_corners[0]->x(), m_corners[3]->y() - minH);
//            m_corners[1]->move(m_corners[1]->x(), m_corners[3]->y() - minH);
//        } else {
//            m_corners[3]->move(m_corners[3]->x(), m_corners[0]->y() + minH);
//            m_corners[2]->move(m_corners[2]->x(), m_corners[0]->y() + minH);
//        }
//    }
    newFrame = QRect(m_corners[0]->pos() + m_corners[0]->size()/2, m_corners[2]->pos() + m_corners[2]->size()/2);

    m_ui->selectionFrame->setGeometry(newFrame);
    updateSquareRect();

    setSelection(QRectF(qreal(newFrame.x())/ width(), qreal(newFrame.y())/ height(), qreal(newFrame.width())/ width(), qreal(newFrame.height())/ height()));

}


void PixmapEditor::paintEvent(QPaintEvent *e)
{
//    FramedEntity::paintEvent(e);
    QPainter p(this);

    p.drawPixmap(rect(), m_scaledPixmap);
    QImage img(size(), QImage::Format_ARGB32);
    QColor darkAreaColor(34, 42, 48, 102);
    img.fill(darkAreaColor);
    int w = std::min(width(), m_ui->selectionFrame->x() + m_ui->selectionFrame->width());
    int h = std::min(height(), m_ui->selectionFrame->y() + m_ui->selectionFrame->height());
    for (int i = m_ui->selectionFrame->x(); i < w; i++) {
        for (int j = m_ui->selectionFrame->y(); j < h; j++) {
//            img.setPixelColor(QPoint(i, j), Qt::transparent);
        }

    }
    p.drawImage(rect(), img);
//    QPainter::PixmapFragment fragment = QPainter::PixmapFragment::create(QPointF(0, 0), QRectF(0, 0, width(), height()));

//    p.drawPixmapFragments(&fragment, 1, pixmap(), QPainter::OpaqueHint);
}
