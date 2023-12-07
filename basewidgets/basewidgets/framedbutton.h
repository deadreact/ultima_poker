#pragma once

#include "basewidgets_global.h"
#include <QtWidgets/QPushButton>
#include "framedentity.h"
#include <QGraphicsEffect>
#include <QTextLength>
//#include "util.h"
#include "logic.h"

class BASEWIDGETS_DLL FramedButton : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible RESET setVisibleTrue)
    Q_PROPERTY(E::ButtonFlag state READ state WRITE setState NOTIFY stateChanged)

    Q_PROPERTY(int frameIndex READ frameIndex WRITE setFrameIndex NOTIFY frameIndexChanged DESIGNABLE false)
    Q_PROPERTY(QSize sizeOfFrame READ sizeOfFrame WRITE setSizeOfFrame NOTIFY sizeOfFrameChanged)
    Q_PROPERTY(int widthOfFrame READ widthOfFrame WRITE setWidthOfFrame STORED false DESIGNABLE false)
    Q_PROPERTY(int heightOfFrame READ heightOfFrame WRITE setHeightOfFrame STORED false DESIGNABLE false)
    Q_PROPERTY(QSize sizeInFrames READ sizeInFrames WRITE setSizeInFrames NOTIFY sizeInFramesChanged)
    Q_PROPERTY(int cols READ cols WRITE setCols STORED false DESIGNABLE false)
    Q_PROPERTY(int rows READ rows WRITE setRows STORED false DESIGNABLE false)
    Q_PROPERTY(int maxFrameIndex READ maxFrameIndex WRITE setMaxFrameIndex RESET unsetMaxFrameIndex)
    Q_PROPERTY(QRect textRegion READ textRegion WRITE moveText NOTIFY textMoved)
    Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment DESIGNABLE true)
    Q_PROPERTY(E::Blending blending READ blending WRITE setBlending)
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode)

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

//    Q_DECLARE_FLAGS(ButtonFlags, E::ButtonState)

    QPixmap m_pixmap;
    Logic* m_logic;
    QRect m_textRegion;
    QSize m_sizeOfFrame;
    QSize m_sizeInFrames;
    int m_frameIndex;
    int m_maxFrameIndex;
    QColor m_backgroundColor;
    E::Blending m_blending;
    Qt::AspectRatioMode m_aspectRatioMode;
    E::ButtonFlag m_state;
    Qt::Alignment m_textAlignment;

    void setVisibleTrue() { setVisible(true);}

signals:
    void entered();
    void leaved();
    void pixmapChanged(const QPixmap& pixmap);
    void stateChanged(E::ButtonFlag state);
    void sizeOfFrameChanged(QSize sizeOfFrame);
    void sizeInFramesChanged(QSize sizeInFrames);
    void textMoved(QRect textRegion);
    void frameIndexChanged(int);
    void backgroundColorChanged(QColor);
public:

    Q_INVOKABLE FramedButton(QWidget* parent = nullptr);
    virtual ~FramedButton();

    void setLogic(Logic *logic);

    template <class TLogic>
    TLogic* resetLogic() {
        setLogic(nullptr);
        TLogic *logic = new TLogic(this);
        setLogic(logic);
        return logic;
    }

    virtual bool event(QEvent *) override;
    int maxFrameIndex() const;
    virtual QSize sizeHint() const override;
    const QPixmap& pixmap() const;

    bool isEnabled() const;
    bool isVisible() const;
    E::ButtonFlag state() const;
    bool hasState(E::ButtonState state) const;
    bool hasState(E::ButtonFlag state) const;

    QSize sizeOfFrame() const;
    QSize sizeInFrames() const;
    inline int widthOfFrame() const;
    inline int heightOfFrame() const;
    inline int cols() const;
    inline int rows() const;

    QRect textRegion() const;

    int frameIndex() const;

    E::Blending blending() const;

    Qt::AspectRatioMode aspectRatioMode() const;

    void setSizeOfFrame(const QSize& sizeOfFrame);
    inline void setSizeOfFrame(int w, int h);
    inline void setWidthOfFrame(int widthOfFrame);
    inline void setHeightOfFrame(int heightOfFrame);

    void setSizeInFrames(const QSize& sizeInFrames);
    inline void setSizeInFrames(int cols, int rows);
    inline void setCols(int cols);
    inline void setRows(int rows);
    void setBlending(E::Blending blending);
    void setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode);
    inline void moveText(int x, int y, int w, int h);
    void unsetMaxFrameIndex();
    QColor backgroundColor() const;

    Qt::Alignment textAlignment() const;
public slots:
    void adjustProperties();
    void setPixmap(const QPixmap& pixmap);
    virtual void setEnabled(bool enabled);
    virtual void setDisabled(bool disabled);
    virtual void setVisible(bool visible) override;

    void setMaxFrameIndex(int maxFrameIndex);

    void moveText(const QRect& region);

    void setFrameIndex(int frameIndex);
    void setBackgroundColor(QColor color);

    void setTextAlignment(Qt::Alignment textAlignment);
private slots:
    void setState(E::ButtonFlag state);
    void setState(E::ButtonState state, bool enabled = true);
protected:
    bool pixmapValid() const;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *) override;
};

inline int FramedButton::widthOfFrame() const {
    return sizeOfFrame().width();
}
inline int FramedButton::heightOfFrame() const {
    return sizeOfFrame().height();
}
inline int FramedButton::cols() const {
    return sizeInFrames().width();
}
inline int FramedButton::rows() const {
    return sizeInFrames().height();
}

/*********************** SETTERS ************************/

inline void FramedButton::moveText(int x, int y, int w, int h) {
    moveText(QRect(x, y, w, h));
}
inline void FramedButton::setSizeOfFrame(int w, int h) {
    setSizeOfFrame(QSize(w, h));
}
inline void FramedButton::setWidthOfFrame(int w) {
    setSizeOfFrame(w, heightOfFrame());
}
inline void FramedButton::setHeightOfFrame(int h) {
    setSizeOfFrame(widthOfFrame(), h);
}
inline void FramedButton::setSizeInFrames(int cols, int rows) {
    setSizeInFrames(QSize(cols, rows));
}
inline void FramedButton::setCols(int cols) {
    setSizeInFrames(cols, rows());
}
inline void FramedButton::setRows(int rows) {
    setSizeInFrames(cols(), rows);
}
