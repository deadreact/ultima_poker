#include <precompiled.h>
#include "CardWidget.h"
#include <utils/logger.h>
#include <styles/colorizedstyle.h>
#include <models/model_CardsDeck.h>


namespace
{
    QByteArray placeholder = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<!-- Generator: Adobe Illustrator 16.0.0, SVG Export Plug-In . SVG Version: 6.00 Build 0)  -->"
            "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"
            "<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\""
            "     width=\"512px\" height=\"512px\" viewBox=\"0 0 512 512\" enable-background=\"new 0 0 512 512\" xml:space=\"preserve\">"
            "<g>"
            "    <path d=\"M446.844,208.875C447.625,203.313,448,197.656,448,192c0-70.563-57.406-128-128-128"
            "        c-40.938,0-78.531,19.344-102.344,51.063C209.25,113.031,200.688,112,192,112c-61.75,0-112,50.25-112,112"
            "        c0,1.563,0.031,3.094,0.094,4.625C33.813,242.375,0,285.313,0,336c0,61.75,50.25,112,112,112h272c70.594,0,128-57.406,128-128"
            "        C512,273.344,486.344,231.188,446.844,208.875z M384,416H112c-44.188,0-80-35.813-80-80s35.813-80,80-80"
            "        c2.438,0,4.75,0.5,7.125,0.719c-4.5-10-7.125-21.031-7.125-32.719c0-44.188,35.813-80,80-80c14.438,0,27.813,4.125,39.5,10.813"
            "        C246,120.25,280.156,96,320,96c53.031,0,96,42.969,96,96c0,12.625-2.594,24.625-7.031,35.688C449.813,238.75,480,275.688,480,320"
            "        C480,373.031,437.031,416,384,416z\"/>"
            "    <polygon points=\"288,192 224,192 224,288 160,288 256,384 352,288 288,288 	\"/>"
            "</g>"
            "</svg>";
}

CardWidget::CardWidget(QWidget *parent)
    : SvgWidget(parent)
    , m_cardIndex(-1)
    , m_deckModel(nullptr)
{
    setSourceCode(::placeholder);
}

ns_model::CardsDeck *CardWidget::deckModel() const {
    return m_deckModel;
}

int CardWidget::cardIndex() const {
    return m_cardIndex;
}


bool CardWidget::cardIndexValid(int index) {
    return index >= -1 && index < 52;
}

void CardWidget::setDeckModel(ns_model::CardsDeck *model)
{
    if (m_deckModel == model) {
        return;
    }
    m_deckModel = model;

    if (m_deckModel != nullptr) {
        setSourceCode(m_deckModel->svgData(m_deckModel->indexOf(m_cardIndex)));

        connect(m_deckModel, &ns_model::CardsDeck::modelReset, this, &CardWidget::updateView);
        connect(m_deckModel, &ns_model::CardsDeck::dataChanged, this, &CardWidget::updateView);

    }
}

void CardWidget::setCardIndex(int cardIndex) {
    if (cardIndex != m_cardIndex)
    {
        if (!cardIndexValid(cardIndex)) {
            Logger::error(QString("WRONG CARD INDEX %0").arg(cardIndex), __FILELINE__);
            return;
        }
        m_cardIndex = cardIndex;
        if (m_deckModel != nullptr) {
            setSourceCode(m_deckModel->svgData(m_deckModel->indexOf(m_cardIndex)));
        }
    }
}

void CardWidget::updateView()
{
    setSourceCode(::placeholder);
    if (m_cardIndex != -1) {
        setSourceCode(m_deckModel->deck(m_cardIndex));
    } else {
        setSourceCode(m_deckModel->shirt());
    }
}
