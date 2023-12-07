#pragma once

#include <basewidgets/svgwidget.h>

namespace ns_model
{
    class CardsDeck;
}

class CardWidget : public SvgWidget
{
    Q_OBJECT
    Q_PROPERTY(int cardIndex READ cardIndex WRITE setCardIndex)

    int m_cardIndex;
    ns_model::CardsDeck* m_deckModel;
public:
    explicit CardWidget(QWidget *parent = nullptr);

    ns_model::CardsDeck* deckModel() const;
    int cardIndex() const;
    static bool cardIndexValid(int index);

public slots:
    void setDeckModel(ns_model::CardsDeck* model);
    void setCardIndex(int cardIndex);
    void updateView();
};

