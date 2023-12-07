#include <precompiled.h>
#include "GameTablePreview.h"
#include "ui_GameTablePreview.h"

GameTablePreview::GameTablePreview(QWidget *parent)
    : QFrame(parent)
    , m_ui(new Ui::GameTablePreview)
{
    m_ui->setupUi(this);
}

GameTablePreview::~GameTablePreview()
{
    delete m_ui;
}

QColor GameTablePreview::backgroundColor() const
{
    return m_ui->bgcolor->backgroundColor();
}

QColor GameTablePreview::feltColor() const
{
    return m_feltColor;
}

const QPixmap *GameTablePreview::frontDeck() const {
    return &m_ui->tablecard_0->pixmap();
}

const QPixmap *GameTablePreview::backDeck() const {
    return &m_ui->tablecardback_0->pixmap();
}

void GameTablePreview::setBackgroundColor(QColor color)
{
    if (backgroundColor() == color)
        return;

    m_ui->bgcolor->setBackgroundColor(color);
    emit backgroundColorChanged(color);
}

void GameTablePreview::setFeltColor(QColor color)
{
    if (feltColor() == color)
        return;

    m_feltColor = color;
    m_ui->widget->setStyleSheet(QString(".QWidget { border-radius: 120px 82px; background-color: rgba(%0, %1, %2, %3); }").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    emit feltColorChanged(color);
}

void GameTablePreview::setFrontDeck(const QPixmap &pixmap)
{
    if (frontDeck() && frontDeck()->cacheKey() == pixmap.cacheKey())
        return;

    QList<FramedEntity*> lst = findChildren<FramedEntity*>(QRegExp("tablecard_\\d+"));

    for (FramedEntity* card: lst) {
        card->setPixmap(pixmap);
    }

    emit frontDeckChanged(pixmap);
}

void GameTablePreview::setBackDeck(const QPixmap &pixmap)
{
    if (backDeck() && backDeck()->cacheKey() == pixmap.cacheKey())
        return;

    m_ui->tablecardback_0->setPixmap(pixmap);
    m_ui->tablecardback_1->setPixmap(pixmap);
    emit backDeckChanged(pixmap);
}
