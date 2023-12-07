#pragma once

#include <basewidgets/framedentity.h>

namespace Ui {
class PixmapEditor;
}

class PixmapEditor : public FramedEntity
{
    Q_OBJECT
    Q_PROPERTY(QRectF selection READ selection WRITE setSelection NOTIFY selectionChanged)
    Q_PROPERTY(int cornerSize READ cornerSize WRITE setCornerSize)
public:
    enum SelectionType :int{
        RectSelection               = 0x0000
      , SquareSelectionLeftTop      = int(Qt::AlignLeft    | Qt::AlignTop)
      , SquareSelectionLeftCenter   = int(Qt::AlignLeft    | Qt::AlignVCenter)
      , SquareSelectionLeftBottom   = int(Qt::AlignLeft    | Qt::AlignBottom)
      , SquareSelectionCenterTop    = int(Qt::AlignHCenter | Qt::AlignTop)
      , SquareSelectionCenter       = int(Qt::AlignCenter )
      , SquareSelectionCenterBottom = int(Qt::AlignHCenter | Qt::AlignBottom)
      , SquareSelectionRightTop     = int(Qt::AlignRight   | Qt::AlignTop)
      , SquareSelectionRightCenter  = int(Qt::AlignRight   | Qt::AlignVCenter)
      , SquareSelectionRightBottom  = int(Qt::AlignRight   | Qt::AlignBottom)
      , SquareSelectionNear         = 0x0008
      , SquareSelectionFar          = 0x0010
    };

private:
    Ui::PixmapEditor *m_ui;
    QPixmap m_scaledPixmap;
    qreal m_scale;
    EntityWidget* m_corners[4];
    int m_pressedCornerIndex;
    bool m_dragActive;
    QPoint m_dragAnchor;
    //  0 | 1
    //  -----
    //  3 | 2
    QRectF m_selection;
    SelectionType m_displaySelection;

public:
    explicit PixmapEditor(QWidget *parent = 0);
    virtual ~PixmapEditor();

    void init();
    QPixmap selectedFragment(int type = -1) const;
    void setDisplaySelection(SelectionType displaySelection);
public slots:
    void setSelection(QRectF selection);
    void setCornerSize(int size);

private slots:
    void cornerPressed();
    void cornerReleased();
    void beginDrag();
    void endDrag();
    void updateSquareRect();
signals:
    void selectionChanged(QRectF selection);

protected:
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;

    void updateCursors();
    void validatePositions();
public:
    virtual int heightForWidth(int) const override;
    virtual bool hasHeightForWidth() const override;
    QRectF selection() const;
    int cornerSize() const;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *e) override;
};
