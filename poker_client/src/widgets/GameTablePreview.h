#pragma once
#include <QFrame>

namespace Ui {
class GameTablePreview;
}

class GameTablePreview : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor feltColor READ feltColor WRITE setFeltColor NOTIFY feltColorChanged)
    Q_PROPERTY(QPixmap frontDeck READ frontDeck WRITE setFrontDeck NOTIFY frontDeckChanged)
    Q_PROPERTY(QPixmap backDeck READ backDeck WRITE setBackDeck NOTIFY backDeckChanged)
private:
    Ui::GameTablePreview *m_ui;
    QColor m_feltColor;
public:
    explicit GameTablePreview(QWidget *parent = nullptr);
    virtual ~GameTablePreview();

    QColor backgroundColor() const;
    QColor feltColor() const;

    const QPixmap* frontDeck() const;
    const QPixmap* backDeck() const;

public slots:
    void setBackgroundColor(QColor color);
    void setFeltColor(QColor feltColor);
    void setFrontDeck(const QPixmap& pixmap);
    void setBackDeck(const QPixmap &pixmap);

signals:
    void backgroundColorChanged(QColor backgroundColor);
    void feltColorChanged(QColor feltColor);
    void frontDeckChanged(QPixmap frontDeck);
    void backDeckChanged(QPixmap backDeck);
};

