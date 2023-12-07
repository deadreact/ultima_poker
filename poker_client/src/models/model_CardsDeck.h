#pragma once

#include <QAbstractTableModel>

namespace ns_model
{

    class CardsDeck : public QAbstractTableModel
    {
        Q_OBJECT

        QVector<QByteArray> m_deck;
        QByteArray m_shirt;
    public:
        constexpr static const char* sm_cardsPath = ":textures/svg/cards_0%0/";
        enum class suit {
            clubs = 0,
            spades,
            diamonds,
            hearts,

            shirt
        };
        Q_ENUM(suit)
        enum class card {
            two = 0,
            three,
            four,
            five,
            six,
            seven,
            eight,
            nine,
            ten,
            jack,
            queen,
            king,
            ace,

            shirt
        };
        Q_ENUM(card)
    public:
        CardsDeck(QObject* parent = nullptr);

    public:
        void loadCards(int deckIndex, int shirtIndex);
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        const QByteArray &svgData(const QModelIndex &index) const;

        void setShirt(const QByteArray& shirt);
        const QByteArray &shirt() const;
        const QByteArray &deck(int index) const;

        QModelIndex indexOf(int id) const;
        QModelIndex index(suit _suit, card _card = card::shirt, const QModelIndex &parent = QModelIndex()) const;
    protected:
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    public:
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;
    private:
        int getCardId(const QString& cardName, const QString& suitName);
    };
}
