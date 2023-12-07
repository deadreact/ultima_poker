#include <precompiled.h>
#include "widget_TableOver.h"
//#include "ui_Player.h"
#include "ui_widget_TableOver.h"
#include "ui_tablelayer_9seats.h"
#include "ui_tablelayer_6seats.h"
#include "ui_tablelayer_2seats.h"

#include <PokerApplication.h>
#include <models/model_ChatSmilesList.h>
#include <game/userbox.h>
#include <game/tableItems/betitem.h>
#include <models/model_Chat.h>
#include "CardWidget.h"
#include <dialogs/dialog_gametable.h>
#include <basewidgets/types.h>
#include <notes/notescompactview.h>
#include <settingsmanager.h>
#include <qpropertyanimation.h>
#include <animation/animation.h>
#include <models/model_CardsDeck.h>

/** *************************************/
struct TableOver::TableOverData : Ui::TableOver
{
    Handle m_layer;
    QImage m_background;
    QList<Userbox*> m_usrboxes;
    QButtonGroup* m_bttnGrp_usrboxes;
    QList<BetItem*> m_betItems;
//    Player* m_controller;

    QList<PotItem*> m_tablePots;
    std::array<CardWidget*, 5> m_cards;

    QLabel* m_dealer;

    int m_seatCount;
    QScopedPointer<ns_model::CardsDeck> m_deckModel;

    TableOverData(::TableOver* owner)
        : m_layer(nullptr)
        , m_background()
        , m_usrboxes()
        , m_bttnGrp_usrboxes(new QButtonGroup(owner))
        , m_cards{nullptr, nullptr, nullptr, nullptr, nullptr}
        , m_dealer(nullptr)
        , m_seatCount(9)
        , m_deckModel(new ns_model::CardsDeck)
    {
        Ui::TableOver::setupUi(owner);
    }
};
/** *************************************/


template <>
TableOver::Handle TableOver::initWithSeatCount<2>()
{
    Ui::layer_2seats* ui = new Ui::layer_2seats;
    ui->setupUi(m_dPtr->layer_gameTable);
    m_dPtr->m_betItems << ui->bet_0 << ui->bet_1;
    m_dPtr->m_tablePots << ui->table_pot_0;
    m_dPtr->m_cards = {ui->card_0, ui->card_1, ui->card_2, ui->card_3, ui->card_4};
    m_dPtr->m_dealer = ui->icon_dealer;

    const bool orientation[] = {0, 0};
    for (Userbox* usrbox : userboxes()) {
        usrbox->setIsRightToLeft(orientation[usrbox->seatIndex()]);
    }
    ui->boxes_pos->hide();

    userbox(0)->setHorizontalOffset(50);
    userbox(1)->setHorizontalOffset(50);

    connect(this, &TableOver::destroyed, [ui]{ delete ui; });

    return ui;
}
template <>
TableOver::Handle TableOver::initWithSeatCount<6>()
{
    Ui::layer_6seats* ui = new Ui::layer_6seats;
    ui->setupUi(m_dPtr->layer_gameTable);
    m_dPtr->m_betItems << ui->bet_0 << ui->bet_1 << ui->bet_2 << ui->bet_3 << ui->bet_4 << ui->bet_5;
    m_dPtr->m_tablePots << ui->table_pot_0 << ui->table_pot_1 << ui->table_pot_2 << ui->table_pot_3 << ui->table_pot_4;
    m_dPtr->m_cards = {ui->card_0, ui->card_1, ui->card_2, ui->card_3, ui->card_4};
    m_dPtr->m_dealer = ui->icon_dealer;

    const bool orientation[] = {0, 1, 1, 0, 0, 0};
    for (Userbox* usrbox : userboxes()) {
        usrbox->setIsRightToLeft(orientation[usrbox->seatIndex()]);
    }
    ui->boxes_pos->hide();

    userbox(0)->setHorizontalOffset(50);
    userbox(3)->setHorizontalOffset(50);

    connect(this, &TableOver::destroyed, [ui]{ delete ui; });

    return ui;
}
template <>
TableOver::Handle TableOver::initWithSeatCount<9>()
{
    Ui::layer_9seats* ui = new Ui::layer_9seats;
    ui->setupUi(m_dPtr->layer_gameTable);
    m_dPtr->m_betItems << ui->bet_0 << ui->bet_1 << ui->bet_2 << ui->bet_3 << ui->bet_4 << ui->bet_5 << ui->bet_6 << ui->bet_7 << ui->bet_8;
    m_dPtr->m_tablePots << ui->table_pot_0 << ui->table_pot_1 << ui->table_pot_2 << ui->table_pot_3 << ui->table_pot_4 << ui->table_pot_5 << ui->table_pot_6 << ui->table_pot_7;
    m_dPtr->m_cards = {ui->card_0, ui->card_1, ui->card_2, ui->card_3, ui->card_4};
    m_dPtr->m_dealer = ui->icon_dealer;

    const bool orientation[] = {0, 1, 1, 1, 1, 0, 0, 0, 0};
    for (Userbox* usrbox : userboxes()) {
        usrbox->setIsRightToLeft(orientation[usrbox->seatIndex()]);
    }
    ui->boxes_pos->hide();

    userbox(0)->setHorizontalOffset(50);

    connect(this, &TableOver::destroyed, [ui]{ delete ui; });

    return ui;
}

template <>
struct IdGetter<GameTable> {
    using id_type = GameId;
    using T = GameTable;

    constexpr inline static GameId invalidID() {
        return GameId::invalidID();
    }

    inline static const id_type& get_id(const T& obj) {
        return obj.id();
    }
};

TableOver::TableOver(QWidget *parent)
    : InitableWidget(parent)
    , GameIdRefObject(ns_util::inheritedID<GameTable>(parent))
    , m_dPtr(new TableOverData(this))
{}

TableOver::TableOver(const GameId& id, QWidget *parent)
    : InitableWidget(parent)
    , GameIdRefObject(id)
    , m_dPtr(new TableOverData(this))
{
}

TableOver::~TableOver()
{
    // bttnGroup создана на стеке, controller тоже, еще и в другом классе
    //  если оставить прежнего родителя получим pure virtual call
//    m_controller->setParent(nullptr);
//    qDebug() << "~TableOver()";
}

void TableOver::init()
{
    if (isInitialized()) {
        return;
    }

    for (int i = m_dPtr->m_usrboxes.size(); i < m_dPtr->m_seatCount; i++) {
        m_dPtr->m_usrboxes.push_back(new Userbox(id(), i, m_dPtr->hud_low_level));
        m_dPtr->m_usrboxes.back()->init();
        m_dPtr->m_usrboxes.back()->setObjectName(QString("userbox_%0").arg(i));

    }

    switch (m_dPtr->m_seatCount) {
    case 9: m_dPtr->m_layer = initWithSeatCount<9>(); break;
    case 6: m_dPtr->m_layer = initWithSeatCount<6>(); break;
    case 2: m_dPtr->m_layer = initWithSeatCount<2>(); break;
    default:
        qCritical("Fatal error in function %s, cannot init with %i seats", __FUNCTION__, m_dPtr->m_seatCount);
        qFatal("    Loc: [%s:%i]", __FILE__, __LINE__);
    }

    setModelKeys();

    for (int i = 0; i < m_dPtr->m_seatCount; i++)
    {
        Userbox* usrbox = m_dPtr->m_usrboxes.at(i);
        usrbox->setDeckModel(m_dPtr->m_deckModel.data());
        m_dPtr->m_bttnGrp_usrboxes->addButton(usrbox->buttonTakeASit(), i);
        int uniqueID = m_dPtr->hud->registerExternalControl(usrbox->buttonTakeASit());
        connect(usrbox->buttonTakeASit(), &QAbstractButton::clicked, [i, uniqueID]{
            qDebug() << "triggered take a seat at" << i << "by control with id" << uniqueID;
        });
    }

    for (Userbox* usrbox : userboxes()) {
        usrbox->setBetItem(m_dPtr->m_betItems.at(usrbox->seatIndex()));
        usrbox->raise();

        connect(usrbox->actionAddNote(), &QAction::triggered, this, [this, usrbox]{
            NotesCompactView* w = pApp->settingsManager()->create<NotesCompactView>(this);
            w->setWindowTitle(tr("@title_notes: %0").arg(usrbox->nickname()));
            w->init();
            //qDebug() << usrbox->nickname();
            w->addNote(usrbox->nickname());
            w->open();
        });
    }

    gbp_i8 counter = 0;
    for (BetItem* bet: m_dPtr->m_betItems) {
        bet->clear();
        bet->setAttribute(Qt::WA_TransparentForMouseEvents);
        bet->setModelKeyNumber(counter);
        counter++;
    }

    counter = 0;
    for (PotItem* pot: m_dPtr->m_tablePots) {
        pot->clear();
        pot->setAttribute(Qt::WA_TransparentForMouseEvents);
        pot->setModelKeyNumber(counter);
        counter++;
    }
    if (m_dPtr->m_cards[0]) {
        for (CardWidget* card: m_dPtr->m_cards) {
            card->setDeckModel(m_dPtr->m_deckModel.data());
        }
    }

    clearTableCards();
    m_dPtr->hud->init();
    m_dPtr->hud->raise();
    arrangeItems();
    m_dPtr->m_deckModel->loadCards(4, 2);



#ifdef ENABLE_VISUAL_DEBUGGING
    m_dPtr->sdg->show();
#else
    m_dPtr->sdg->hide();
#endif // !ENABLE_VISUAL_DEBUGGING

    onInitialized();
}

UserControls *TableOver::hud() const {
    return m_dPtr->hud;
}

void TableOver::setSeatCount(int n) {
    Q_ASSERT(n == 2 || n == 6 || n == 9);
    if (m_dPtr->m_seatCount != n) {
        m_dPtr->m_seatCount = n;
    }
}

int TableOver::seatCount() const { return m_dPtr->m_seatCount; }

const ns_model::CardsDeck *TableOver::deckModel() const {
    return m_dPtr->m_deckModel.data();
}

void TableOver::updateUserboxes()
{
    QList<Userbox*> m_freeSeats;
    bool currentPlayerSits = false;
    for (Userbox* p: m_dPtr->m_usrboxes) {
        p->betItem()->setVisible(p->betItem()->money() > 0);
        p->update();
        if (p->playerId() == -1) {
            m_freeSeats << p;
        } else if (p->playerId() == pApp->client().userId()) {
            currentPlayerSits = true;
        }
    }
    for (Userbox* p: m_freeSeats) {
        p->setDisabled(currentPlayerSits);
    }
}

const QImage &TableOver::background() const
{
    return m_dPtr->m_background;
}

const QList<Userbox *> &TableOver::userboxes() const {
    return m_dPtr->m_usrboxes;
}

Userbox *TableOver::userbox(gbp_i8 pos) const {
    return (pos >= 0 && pos < m_dPtr->m_usrboxes.size()) ? m_dPtr->m_usrboxes[pos] : nullptr;
}

Userbox *TableOver::userboxByPlayerId(gbp_i64 playerID) const
{
    for (Userbox* usrbox: m_dPtr->m_usrboxes) {
        if (usrbox->playerId() == playerID) {
            return usrbox;
        }
    }
    return nullptr;
}

gbp_i8 TableOver::playerPos(gbp_i64 playerID) const
{
    for (gbp_i8 i = 0; i < m_dPtr->m_usrboxes.size(); ++i) {
        if (userbox(i)->playerId() == playerID) {
            return i;
        }
    }
    return -1;
}

void TableOver::arrangeItems()
{
    QWidgetList lst = findChildren<QWidget*>(QRegularExpression("pos_\\d"));

    std::sort(lst.begin(), lst.end(), [](QWidget* w1, QWidget* w2) { return w1->objectName() < w2->objectName(); });

    for (int i = 0; i < lst.size(); i++) {
        QPoint globalPos1 = lst.at(i)->mapToGlobal(lst.at(i)->rect().center());
        QPoint globalPos2 = userbox(i)->mapToGlobal(userbox(i)->rect().center());
        QPoint diff = globalPos1 - globalPos2 + QPoint(2, -12);
        userbox(i)->move(userbox(i)->pos() + diff);
        userbox(i)->raise();
    }
}

const QList<BetItem *> &TableOver::betItems() const {
    return m_dPtr->m_betItems;
}

BetItem *TableOver::betItem(int index) const {
    return (index >= 0 && index < m_dPtr->m_betItems.size()) ? m_dPtr->m_betItems[index] : nullptr;
}

void TableOver::clearBetItems()
{
    for (BetItem* betItem: m_dPtr->m_betItems) {
        betItem->clear();
    }
}

void TableOver::setPotsCount(int count)
{
    count = qBound(1, count, m_dPtr->m_tablePots.size());
    for (int i = 0; i < m_dPtr->m_tablePots.size(); i++) {
        m_dPtr->m_tablePots[i]->setVisible(i < count);
    }
}

PotItem *TableOver::tablePot(int index) const {
    return (index >= 0 && index < m_dPtr->m_tablePots.size()) ? m_dPtr->m_tablePots[index] : nullptr;
}

void TableOver::clearPots()
{
    for (PotItem* pot: m_dPtr->m_tablePots) {
        pot->clear();
        pot->hide();
    }
}

QTextBrowser *TableOver::chatBrowser() const {
    return m_dPtr->hud->uiElement(UiElement::ChatBrowser);
}

CardWidget *TableOver::tableCard(int index) const
{
    Q_ASSERT_X(index >= 0 && index < 5, "Table over", QString::number(index).toLatin1());
    return m_dPtr->m_cards[index];
}

//void TableOver::setDistributionNumber(gbp_i64 number) {
//    m_controller->distributionNumber()->setText(tr("@game_id_%0").arg(number));
//}

void TableOver::onSettingsChanged(QAction* settings)
{
    m_dPtr->m_deckModel->loadCards(0, 0);
    if (settings->data().canConvert<QVariantMap>()) {
        m_dPtr->m_deckModel->loadCards(1, 1);
    }

    for (CardWidget* card: m_dPtr->m_cards) {
        card->updateView();
    }
    for (Userbox* usrbox: userboxes()) {
        usrbox->onSettingsChanged(settings);
    }
}

void TableOver::clearTableCards()
{
    m_dPtr->m_cards[0]->hide();
    m_dPtr->m_cards[1]->hide();
    m_dPtr->m_cards[2]->hide();
    m_dPtr->m_cards[3]->hide();
    m_dPtr->m_cards[4]->hide();
}

void TableOver::setDealerPos(gbp_i8 pos)
{
    BetItem* bet = betItem(pos);
    QPoint newPos = bet->pos();
    if (bet->y() > m_dPtr->m_dealer->height() + 10) {
        newPos.ry() -= m_dPtr->m_dealer->height();
    } else {
        newPos.ry() += bet->height();
    }

    QPropertyAnimation* anim = ns_anim::moveTo(m_dPtr->m_dealer, newPos, 250);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

QWidget *TableOver::layer(Layer layerType) const
{
    switch (layerType) {
    case Layer::Background: return m_dPtr->_bg;
    case Layer::GameTable: return m_dPtr->layer_gameTable;
    case Layer::MiddleSpace: return m_dPtr->layer_middleSpace;
    case Layer::HudControls: return m_dPtr->hud;
    }
    return nullptr;
}

void TableOver::setModelKeys()
{
    for (int i = 0; i < m_dPtr->m_cards.size(); i++) {
        QWidget* w = m_dPtr->m_cards.at(i);
        w->setProperty("modelKey", QString("[%0,0[6,0[0,0,0x0]]]").arg(i).toLocal8Bit());
        w->setProperty("indexDataRole", UserType::OriginDataRole);
        w->setProperty("indexProperty", "cardIndex");
    }
    for (int i = 0; i < m_dPtr->m_tablePots.size(); i++) {
        QWidget* w = m_dPtr->m_tablePots.at(i);
        w->setProperty("modelKey", QString("[%0,0[7,0[0,0,0x0]]]").arg(i).toLocal8Bit());
        w->setProperty("indexDataRole", UserType::OriginDataRole);
        w->setProperty("indexProperty", "money");

    }
    for (int i = 0; i < m_dPtr->m_betItems.size(); i++) {
        QWidget* w = m_dPtr->m_betItems.at(i);
        w->setProperty("modelKey", QString("[%0,9[4,0[0,0,0x0]]]").arg(i).toLocal8Bit());
        w->setProperty("indexDataRole", UserType::OriginDataRole);
        w->setProperty("indexProperty", "money");
    }
}

void TableOver::setBackground(const QImage &background)
{
    if (m_dPtr->m_background.cacheKey() == background.cacheKey())
        return;

    m_dPtr->m_background = background;
//    m_ui->_bg->setPixmap(QPixmap::fromImage(m_background));
    update();
    emit backgroundChanged(background);
}

void TableOver::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_dPtr->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TableOver::actionEvent(QActionEvent *e)
{
    if (e->type() == QEvent::ActionAdded)
    {
        if (e->action()->text() == "UpdateGameSettings") {
            m_dPtr->hud->addAction(e->action());
            for (Userbox* usrbox: m_dPtr->m_usrboxes) {
                usrbox->addAction(e->action());
            }
            QAction* a = e->action();
            connect(a, &QAction::triggered, this, [this, a]{ onSettingsChanged(a); });
        }
    }
    else if (e->type() == QEvent::ActionRemoved)
    {
        if (e->action()->text() == "UpdateGameSettings") {
            QAction* a = e->action();
            disconnect(a);
            for (Userbox* usrbox: m_dPtr->m_usrboxes) {
                usrbox->removeAction(e->action());
            }
            m_dPtr->hud->removeAction(e->action());

        }
    }
}


void TableOver::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void TableOver::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.drawImage(rect(), m_dPtr->m_background);

    QWidget::paintEvent(e);
}

CardWidget *TableOver::findCard(gbp_i8 cardId, bool onlyTableCards) const
{
    for (CardWidget* card: m_dPtr->m_cards) {
        if (card->cardIndex() == cardId) {
            return card;
        }
    }
    if (!onlyTableCards)
    {
        for (Userbox* p: m_dPtr->m_usrboxes) {
            if (CardWidget* card = p->findCard(cardId)) {
                return card;
            }
        }
    }
    return nullptr;
}
