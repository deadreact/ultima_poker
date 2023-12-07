#include <precompiled.h>
#include "model_CardsDeck.h"

#include <QByteArrayList>
#include <QPixmap>
#include <db/Database.h>
#include <QDebug>
#include <qregularexpression.h>
#include <basewidgets/util.h>

namespace
{

}


namespace ns_model
{
    CardsDeck::CardsDeck(QObject *parent)
        : QAbstractTableModel(parent)
        , m_deck()
    {
        m_deck.resize(52);
    }

    void CardsDeck::loadCards(int deckIndex, int shirtIndex)
    {
        QString path = QString(sm_cardsPath).arg(deckIndex);

        QDir dir(path);
        if (dir.exists())
        {
            beginResetModel();
            QStringList lst = dir.entryList(QStringList("*.svg"));
            if (!lst.isEmpty())
            {
                for (const QString& filename: lst) {
                    QFile file(path + filename);
                    if (file.open(QIODevice::ReadOnly)) {
                        static const QRegularExpression re("^(2|3|4|5|6|7|8|9|10|J|Q|K|A)_(clubs|hearts|diamonds|spades).svg$");
                        QStringList captured = re.match(filename).capturedTexts();
                        if (!captured.isEmpty())
                        {
                            int id = getCardId(captured.at(1), captured.at(2));
                            m_deck[id] = file.readAll();
                        }
                        file.close();
                    }
                }

            }

            QString shirtFilePath = QString(":textures/svg/shirts/0%0.svg").arg(shirtIndex);
            QFile file(shirtFilePath);
            //qDebug() << "FILE" << shirtFilePath;
            if (file.exists() && file.open(QIODevice::ReadOnly)) {
                m_shirt = file.readAll();
                file.close();
            }
            endResetModel();
        }
    }

    int CardsDeck::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return 4;
    }

    int CardsDeck::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return 13;
    }
    
    QVariant CardsDeck::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.parent().isValid()) {
            switch (role) {
            case UserType::SvgDataRole:
            case Qt::DecorationRole:
                return shirt();
            default:
                return QVariant();
            }
        }

        switch (role) {
        case UserType::SvgDataRole:
        case Qt::DecorationRole:
            return svgData(index);
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return QString("%0 of %1").arg(ns_util::enumToString(card(index.column()))).arg(ns_util::enumToString(suit(index.row())));
        default:
            QVariant();
        }

//        return index.row()*columnCount() + index.column();
        return QVariant();
    }

    const QByteArray& CardsDeck::svgData(const QModelIndex &index) const {
        if (suit(index.row()) == suit::shirt) {
            return m_shirt;
        }
        if (!index.isValid()) {
            return m_shirt;
        }
        return m_deck.at(index.row() * columnCount() + index.column());
    }

    void CardsDeck::setShirt(const QByteArray &shirt)
    {
        if (m_shirt != shirt) {
            m_shirt = shirt;

            emit dataChanged(index(suit::shirt), index(suit::shirt), (QVector<int>() << UserType::SvgDataRole << Qt::DecorationRole));
        }
    }

    const QByteArray& CardsDeck::shirt() const {
        return m_shirt;
    }

    const QByteArray &CardsDeck::deck(int index) const {
        return m_deck.at(index);
    }


    QModelIndex CardsDeck::indexOf(int id) const {
        if (id >= 0 && id < 52) {
            return index(id / columnCount(), id % columnCount());
        }
        return index(suit::shirt);
    }


    QModelIndex CardsDeck::index(suit _suit, card _card, const QModelIndex &parent) const {
        return index(underlying_cast(_suit), underlying_cast(_card), parent);
    }

    QModelIndex CardsDeck::index(int row, int column, const QModelIndex &parent) const {
        return QAbstractTableModel::index(row, column, parent);
    }

    QVariant CardsDeck::headerData(int section, Qt::Orientation orientation, int role) const
    {
        
        if (orientation == Qt::Horizontal) {
            if (role == Qt::DisplayRole) {
                
            }
            return QVariant();
        }
        return QVariant();
    }

    int CardsDeck::getCardId(const QString &cardName, const QString &suitName)
    {
        static QMap<QString, card> cardDict;
        static QMap<QString, suit> suitDict;
        if (cardDict.empty())
        {
            cardDict.insert("2", card::two);
            cardDict.insert("3", card::three);
            cardDict.insert("4", card::four);
            cardDict.insert("5", card::five);
            cardDict.insert("6", card::six);
            cardDict.insert("7", card::seven);
            cardDict.insert("8", card::eight);
            cardDict.insert("9", card::nine);
            cardDict.insert("10", card::ten);
            cardDict.insert("J", card::jack);
            cardDict.insert("Q", card::queen);
            cardDict.insert("K", card::king);
            cardDict.insert("A", card::ace);

            suitDict.insert("clubs", suit::clubs);
            suitDict.insert("hearts", suit::hearts);
            suitDict.insert("diamonds", suit::diamonds);
            suitDict.insert("spades", suit::spades);
        }

        int i = underlying_cast(cardDict.value(cardName));
        int j = underlying_cast(suitDict.value(suitName));

        return j*columnCount() + i;
    }

} // namespace ns_model






