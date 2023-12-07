#include <precompiled.h>
#include <qlistview.h>
#include <qpainter.h>
#include <qitemdelegate.h>
#include <qsvgrenderer.h>
#include <qtimer.h>
#include "commongameviewhandler.h"
#include <db/modeldatabase.h>
#include <api_models/model_table_state_t.hpp>
#include <dialogs/dialog_gametable.h>
#include <widgets/widget_TableOver.h>
#include <widgets/MainWindow.h>
#include <widgets/CardWidget.h>
#include <game/tableItems/potitem.h>
#include <models/model_CardsDeck.h>
#include <QSvgRenderer>
// _seats_count
//_players, BETS   ->_is_dealer (0 - _seats_count, 9)->(4, 0)->(0, 0), _round_bet (0 - _seats_count, 11)->(4, 0)->(0, 0)
//_current_user_pos, ?
//_table_cards, CARDS -> (0 - 5, 0)->(6, 0)->(0, 0)
//_all_pots, POTS -> (0 - (_seats_count-1), 0)->(7, 0)->(0, 0)


class CardDelegate : public QItemDelegate
{
    QScopedPointer<ns_model::CardsDeck> m_deck;
    QScopedPointer<QSvgRenderer> m_renderer;
    QString m_prefixPath;
public:
    CardDelegate(QObject* parent = nullptr)
        : QItemDelegate(parent)
        , m_deck(new ns_model::CardsDeck)
        , m_renderer(new QSvgRenderer)
        , m_prefixPath(":/textures/svg/01")
    {
        m_deck->loadCards(3, 3);
    }

    void setPrefixPath(const QString& path) {
        if (path != m_prefixPath) {
            m_prefixPath = path;
        }
    }
    QString prefixPath() const {
        return m_prefixPath;
    }

    virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override {
        if (!p || !p->isActive() || !index.isValid()) {
            return;
        }
        gbp_i8 card = qvariant_cast<gbp_i8>(index.data(UserType::OriginDataRole));
        QByteArray data = m_deck->deck(card);
        m_renderer->load(data);
        m_renderer->render(p, QRect(opt.rect.topLeft(), opt.decorationSize));
    }
};


TableSurfaceViewHandler::ItemType TableSurfaceViewHandler::type(const QModelIndex &index) const
{
    if (!GameViewHandler::indexAffectsView(index)) {
        return ItemType::Unknown;
    }
    if (index.parent().isValid() && index.parent().parent().isValid() && !index.parent().parent().parent().isValid()
     && index.parent().column() == 0 && index.parent().parent().row() == 0 && index.parent().parent().column() == 0)
    {
        switch (index.parent().row()) {
        case 4:
            if (index.row() >= 0 && index.row() < 9) {
                switch (index.column()) {
                case 9:
                    return ItemType::Dealer;
                case 11:
                    return ItemType::Bet;
                default:
                    return ItemType::Unknown;
                }
            }
            return ItemType::Unknown;
        case 6:
            return (index.row() >= 0 && index.row() < 5 && index.column() == 0) ? ItemType::Card : ItemType::Unknown;
        case 7:
            return (index.row() >= 0 && index.row() < 9 && index.column() == 0) ? ItemType::Pot : ItemType::Unknown;
        default:
            return ItemType::Unknown;
        }
    }
    return ItemType::Unknown;
}

TableSurfaceViewHandler::ItemType TableSurfaceViewHandler::type(const QModelIndexRange &range) const
{
    return type(range.first);
}

//TableOver *TableSurfaceViewHandler::widget() const
//{
//    QWidgetList lst = QApplication::topLevelWidgets();
//    for (QWidget* w: lst) {
//        if (GameTable* table = qobject_cast<GameTable*>(w)) {
//            if (table->id() == id()) {
//                return table->layers();
//            }
//        }
//    }
//    qDebug() << "Table not found!\n\n";
//    qDebug() << lst;
//    return nullptr;
//}

TableSurfaceViewHandler::TableSurfaceViewHandler(const GameId &gameID, QObject *parent)
    : GameViewHandler(gameID, parent)
{}

TableSurfaceViewHandler::~TableSurfaceViewHandler()
{

}


bool TableSurfaceViewHandler::indexAffectsView(const QModelIndex &index) const
{
    return type(index) != ItemType::Unknown;
}


void TableSurfaceViewHandler::timelineEvent(const TimeLineEvent &ev)
{
//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("()");

}

void TableSurfaceViewHandler::onModelAboutToBeReset()
{

//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("()");

}

void TableSurfaceViewHandler::onModelReset()
{
//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("()");

}

void TableSurfaceViewHandler::onModelDataChanged(const QModelIndexRange &range)
{
    for (const QModelIndex& index: range) {
        auto it = m_mappings.find(m_model->indexToKey(index));
        if (it != m_mappings.end()) {
            QWidget* w = it.value();
            bool ok = false;
            int role = w->property("indexDataRole").toInt(&ok);
            role = (ok) ? role : Qt::DisplayRole;
            QByteArray  prop = w->property("indexProperty").toByteArray();
            w->setProperty((!prop.isEmpty() ? prop : QByteArray(w->metaObject()->userProperty().name())), index.data(role));
        }
    }

//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("()");
//    TableOver* w = widget();
//    if (!w) {
//        return;
//    }
//    for (const QModelIndex& index: range)
//    {
//        ItemType itemType = type(index);
//        const ns_model::table_state_t* model = qmodelindexmodel_cast<ns_model::table_state_t>(index);
//        if (itemType != ItemType::Unknown) {
//            switch (itemType) {
//            case ItemType::Card:
//            {
//                if (TableOver* w = widget()) {
//                    w->cardsView()->setModel((QAbstractItemModel* )range.model());
//                    w->cardsView()->setRootIndex(range.parent());
//                    w->cardsView()->setItemDelegate(new CardDelegate);
//                }
//            } break;
//            case ItemType::Bet:
//            {
//                BetItem* betItem = w->betItem(index.row());
//                betItem->updateData(index.model());
//            } break;
//            case ItemType::Pot:
//            {
//                PotItem* potItem = w->tablePot(index.row());
//                potItem->updateData(index.model());
//                qDebug() << "POTPOTPOTPOTPOTPOTPOT" << potItem->money();
//                qDebug() << potItem->modelKey() << potItem->index(model);
//                qDebug() << model->indexToKey(index);
//            } break;
//            case ItemType::Dealer:
//            {

//            } break;
//            default:
//                Q_UNREACHABLE();
//            }


//            qDebug() << ns_util::printItemInfo(index);
//        }
//    }
}

void TableSurfaceViewHandler::onModelAboutToInsertItems(const QModelIndex &parent)
{
//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QString("(const QModelIndex&: %0)").arg(ns_util::traceModelIndexWIthData(parent));
}

void TableSurfaceViewHandler::onModelInsertItems(const QModelIndexRangeList &items)
{
//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("(const QModelIndexRangeList&: %0)");
    for (const QModelIndexRange& range: items) {
        switch (type(range.first)) {
        case ItemType::Card:   break;
        case ItemType::Bet:    break;
        case ItemType::Pot:    break;
        case ItemType::Dealer: break;
        default:
            break;
        }
    }
}

void TableSurfaceViewHandler::onModelAboutToRemoveItems(const QModelIndexRangeList &items)
{
//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("(const QModelIndexRangeList&: %0)");
    for (const QModelIndexRange& range: items) {

    }
}

void TableSurfaceViewHandler::onModelRemoveItems(const QModelIndex &parent)
{
//    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QString("(const QModelIndex&: %0)").arg(ns_util::traceModelIndexWIthData(parent));

}

//bool TableSurfaceViewHandler::assignWidget(QWidget *widget, const QModelIndex& index)
//{
////    qDebug() << QStringLiteral("TableSurfaceViewHandler::") % qPrintable(__FUNCTION__) % QStringLiteral("(QWidget*: %0, const QModelIndex&: %1)");
//    if (!index.isValid() || widget == nullptr) {
//        return false;
//    }
//    QAbstractItemModel* m = (QAbstractItemModel*)index.model();
//    if (QAbstractItemView* itemView = qobject_cast<QAbstractItemView*>(widget)) {
//        itemView->setModel(m);
//        itemView->setRootIndex(index);
//        connect(m, &QAbstractItemModel::modelReset, [itemView, index]{ itemView->setRootIndex(index); });
//        return true;
//    }
//}

void TableSurfaceViewHandler::assign(GameTable *gameTable)
{
    ns_model::table_state_t* model = modelsDB().getTableState(m_id);
    int maxBets = gameTable->layers()->seatCount();
    int maxPots = maxBets - 1;

    for (int i = 0; i < 5; i++) {
        QWidget* w = gameTable->layers()->tableCard(i);
        setIndexWidget(w, model->keyToIndex(w->property("modelKey").toByteArray()));
    }
    for (int i = 0; i < maxPots; i++) {
        QWidget* w = gameTable->layers()->tablePot(i);
        setIndexWidget(w, model->keyToIndex(w->property("modelKey").toByteArray()));
    }
    for (int i = 0; i < maxBets; i++) {
        QWidget* w = gameTable->layers()->betItem(i);
        setIndexWidget(w, model->keyToIndex(w->property("modelKey").toByteArray()));
    }
}

QAbstractItemDelegate *TableSurfaceViewHandler::getCardDelegate() const
{
    static CardDelegate delegate;
//    delegate.setPrefixPath();
    return &delegate;
}

//void TableSurfaceViewHandler::connectPots(const QModelIndexRange &range)
//{
//    if (!range.isValid() || range.isEmpty()) {
//        return;
//    }

//    auto *w = q_check_ptr(widget());
//    for (const QModelIndex& index: range) {
//        w->tablePot(index.row())->setModelKeyNumber(index.row());
//        w->tablePot(index.row())->updateData(index.model());
//    }
//}

void TableSurfaceViewHandler::openTable()
{
    // FIXME: Implement me!
}

void TableSurfaceViewHandler::closeTable()
{
    // FIXME: Implement me!
}


void TableSurfaceViewHandler::initialize()
{
    GameViewHandler::initialize();
}
