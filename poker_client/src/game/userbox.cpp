#include <precompiled.h>
#include "userbox.h"
#include "ui_userbox.h"
#include <styles/shadowedtextstyle.h>
#include "TimelineHandler.h"
#include <PokerApplication.h>
#include <qtimer.h>
#include <utils/Utils.h>
#include <array>
#include <game/tableItems/betitem.h>
#include <db/GameTableSettingsData.h>
#include <widgets/CardWidget.h>
#include <qpropertyanimation.h>
#include <animation/animation.h>
#include <api_models/model_table_state_t.hpp>

namespace
{
//    static const char* cards = "234567891JQKA";
//    static const char* suits = "♥♦♣♠";

    inline bool isInRange(char c, const void* range)
    {
        for (const uchar* it = reinterpret_cast<const uchar*>(range); *it != 0; it++) {
            if (c == *it) {
                return true;
            }
        }
        return false;
    }

    int suit_index(const void* marker) {
        const uchar* it = reinterpret_cast<const uchar*>(marker);
        for (; !(*it == 0 || (*it == 0xE2 && *(it+1) == 0x99)); it++) {
//            qDebug() << (const char*)marker << *it << it;
        }
        if (*it == 0) {
            return -1;
        }
        switch (*(it+2)) {
        case 0xA5: return 0;
        case 0xA6: return 1;
        case 0xA3: return 2;
        case 0xA0: return 3;
        default:
            return -1;
        }
    }

    int card_index(const void* marker) {
        const uchar* it = reinterpret_cast<const uchar*>(marker);
        if (suit_index(marker) != -1) {
            return card_index(it+3);
        }
        if (*it >= '2' && *it <= '9') {
            return *it - '2';
        } else if (*it == '1' && *(it+1) == '0') {
            return 8;
        } else if (isInRange(*it, "Jj")) {
            return 9;
        } else if (isInRange(*it, "Qq")) {
            return 10;
        } else if (isInRange(*it, "Kk")) {
            return 11;
        } else if (isInRange(*it, "Aa")) {
            return 12;
        }
        return -1;
    }

    std::vector<ns_table::table_player_info_t>::iterator findPlayerOnPos(gbp_i8 pos, const GameId& id, bool* ok = nullptr)
    {
        auto* m = modelsDB().getTableState(id);
        std::vector<ns_table::table_player_info_t>& players = m->origin()._players;
        for (auto it = players.begin(); it != players.end(); ++it) {
            if (it->_pos == pos) {
                if (ok) *ok = true;
                return it;
            }
        }
        if (ok) *ok = false;
        return players.end();
    }

}
//---------------------PLAYER FUNCTIONS ---------------------------------


struct UserboxData  : public Ui::Userbox
{
//    const TablePlayer* m_player;
//    QModelIndex m_rootIndex;
    gbp_i64 m_playerID;
    GameId m_gameID;
    std::array<CardWidget*, 4> m_cards;
    BetItem* m_betItem;
    QWidget* m_glowWgt;

//    ns_model::CardsDeck* m_deck;

    qreal m_opacity;
    gbp_i64 m_stackMoney;
    gbp_i64 m_distrBet;
    gbp_i64 m_winnings;

    int m_cardsCount;
    int m_horizontalOffset;

    struct
    {
        gbp_u8 initialized     : 1;
        gbp_u8 needUpdateState : 1;
        gbp_u8 isRightToLeft   : 1;
        gbp_u8                 : 1;
        gbp_u8                 : 1;
        gbp_u8                 : 3;
    } m_flags;

    UserboxData(const GameId& id, QStackedWidget* owner)
        : Ui::Userbox()
        , m_playerID(-1)
        , m_gameID(id)
//        , m_player(nullptr)
        , m_cards()
        , m_betItem(nullptr)
        , m_glowWgt(nullptr)
        , m_opacity(1.0)
        , m_stackMoney(0)
        , m_distrBet(0)
        , m_winnings(-1)
        , m_cardsCount(0)
        , m_horizontalOffset(0)
        , m_flags{false, true, false}
    {
        setupUi(owner);
        timeline->hide();
        owner->setCurrentIndex(0);
        m_cards[0] = card_1;
        m_cards[1] = card_2;
        m_cards[2] = card_3;
        m_cards[3] = card_4;
    }
};


//--------------------------------
//--------------------------------

Userbox::Userbox(const GameId &gameID, int seatIndex, QWidget *parent)
    : QStackedWidget(parent)
    , m_dPtr(new UserboxData(gameID, this))
    , m_seatIndex(seatIndex)
{
    Q_ASSERT(seatIndex >= 0 && seatIndex <= 9);
}

Userbox::~Userbox()
{
    delete m_dPtr;
}

void Userbox::updateModelData()
{
//    ns_model::table_state_t* m = modelsDB().getTableState(m_dPtr->m_gameID);
    bool ok = false;
    auto it = findPlayerOnPos(m_seatIndex, m_dPtr->m_gameID, &ok);
    if (ok)
    {
        if (m_dPtr->m_playerID == -1) {
            reserve(it->_id);
            setNickname(QString::fromStdString(it->_nickname));
        }
        if (isPlayerReady())
        {
        }
    }
    else
    {
        if (m_dPtr->m_playerID != -1) {
            free();
        }
    }
}

bool Userbox::updateModelData(const QModelIndex &index, int role)
{
    if (!index.isValid() || !index.parent().isValid()) {
        return false;
    }

    ns_model::table_state_t* model = modelsDB().getTableState(m_dPtr->m_gameID);

    if (index.parent() == model->index_players())
    {
        using element = ns_model::table_state_t::element_of_players;
        gbp_i8 pos = qvariant_cast<gbp_i8>(index.sibling(index.row(), underlying_cast(element::_pos)).data(UserType::OriginDataRole));
        if (pos != m_seatIndex) {
            return false;
        }
        switch (element(index.column())) {
        case element::_id:
        {
            gbp_i64 id = qvariant_cast<gbp_i8>(index.sibling(index.row(), underlying_cast(element::_id)).data(UserType::OriginDataRole));
            if (m_dPtr->m_playerID != id)
            {
                if (m_dPtr->m_playerID == -1) {
                    reserve(id);
                    return m_dPtr->m_playerID != -1;
                } else {
                    // reset all data
                    onModelReset();
                    return true;
                }
            }
            return false;
        }
        case element::_is_user:
        case element::_pos:
        case element::_country:
            return false;
        case element::_nickname:
            return updateNickname(index.data(role), role);
        case element::_money:
            return updateStack(index.data(role), role);
        case element::_start_stack:
            return updateStartStack(index.data(role), role);
        case element::_bounty_progressive_money:
            return updateBounty(index.data(role), role);
        case element::_hash_avatar:
            return updateAvatar(index.data(role));
        case element::_is_dealer:
            return updateIsDealer(index.data(role), role);
        case element::_cards:
            return updateCards(index.data(role), role);
        case element::_round_bet:
            return updateRoundBet(index.data(role), role);
        case element::_is_current:
            return updateIsCurrent(index.data(role), role);
        case element::_vip_status:
            return updateVipStatus(index.data(role), role);
        case element::_state:
            return updateState(index.data(role), role);
        default:
            return false;
        }
    }
    return false;
}

bool Userbox::updateStack(const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    {
        m_dPtr->imsg_moneyOrState->setCurrentMsgIndex(0);
        m_dPtr->imsg_moneyOrState->setMessage(0, value.toString());
        return false;
    }
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateStartStack(const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateNickname(const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
        if (value.canConvert<QString>()) {
            setNickname(value.toString());
        }
        return false;
    default:
        return false;
    };
    return false;
}
bool Userbox::updateBounty(    const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateAvatar(    const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateIsDealer(  const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateIsCurrent( const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateVipStatus( const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateState(     const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateRoundBet(  const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}
bool Userbox::updateCards(     const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::DisplayRole:
    case UserType::OriginDataRole:
    {

    }
    default:
        return false;
    };
    return false;
}

void Userbox::init()
{
    if (m_dPtr && m_dPtr->m_flags.initialized) {
        return;
    }
//    m_dPtr.reset(new UserboxData(this));

    // connect(this, &Userbox::stackChanged, this, &Userbox::updateStack);

    setCardsCount(4);

    m_dPtr->page_busy->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_dPtr->page_busy->addAction(m_dPtr->actionAddNote);
    m_dPtr->page_busy->addAction(m_dPtr->actionEditNote);
    m_dPtr->actionAddNote->setVisible(playerId() != netClient().userId());
    m_dPtr->actionEditNote->setVisible(playerId() != netClient().userId());
//    m_dPtr->avatar->addAction(m_dPtr->actionAddNote);
//    m_dPtr->bttn_sit->addAction(m_dPtr->actionAddNote);

    for (int i = 0; i < 4; i++) {
        setCard(i, -1);
        m_dPtr->m_cards[i]->setMask(m_dPtr->m_cards[i]->rect().adjusted(0, 0, 0, -20));
//        CardWidget* w = m_dPtr->m_cards[i];
//        connect(w, &CardWidget::sourceCodeChanged, );
    }

    ShadowedTextStyle* style = setTextDropShadowStyle(m_dPtr->nickname, QPoint(0, 1));
    connect(this, &Userbox::opacityChanged, style, &ShadowedTextStyle::setTextOpacity);

    style = setTextDropShadowStyle(m_dPtr->imsg_moneyOrState, QPoint(0, 1));
    connect(this, &Userbox::opacityChanged, style, &ShadowedTextStyle::setTextOpacity);
    style = setTextDropShadowStyle(m_dPtr->imsg_action, QPoint(0, 1));
    connect(this, &Userbox::opacityChanged, style, &ShadowedTextStyle::setTextOpacity);

    m_dPtr->m_flags.initialized = true;
}

int Userbox::seatIndex() const {
    return m_seatIndex;
}

bool Userbox::isPlayerReady() const
{
//    if (m_dPtr->m_player != nullptr)
//    {
//        if (id().isValid() && m_dPtr->m_player->playerID() == playerId()) {
//            return !m_dPtr->m_player->avatar().isNull() && m_dPtr->m_player->gameMoney().isPositive();
//        }
//    }
    return m_dPtr->m_playerID != -1 && stack() > 0;
}

gbp_i64 Userbox::playerId() const
{
    return m_dPtr->m_playerID;
}

void Userbox::reserve(gbp_i64 playerID)
{
    if (playerID == playerId()) {
        return;
    }
//    Q_ASSERT_X(playerId() == -1 && playerID != -1, "setPlayer", QString("This seat is busy by player with id %0").arg(playerId()).toLatin1());
    m_dPtr->m_playerID = playerID;
    setCurrentIndex(2);
}

//void Userbox::setPlayer(TablePlayer *player)
//{
//    addAction(player);
//}

void Userbox::free()
{
//    Q_ASSERT_X(playerId() != -1, "free", "This seat is already free");
    m_dPtr->m_playerID = -1;
    setCurrentIndex(0);
}

QString Userbox::nickname() const {
    return  m_dPtr->nickname->text();
}

QIcon Userbox::avatar() const {
    return m_dPtr->avatar->icon();
}

gbp_i64 Userbox::stack() const {
    return m_dPtr->m_stackMoney;
}

gbp_i64 Userbox::roundBet() const {
    if (!m_dPtr || !m_dPtr->m_betItem) {
        return -1;
    }
    return m_dPtr->m_betItem->money();
}

gbp_i64 Userbox::distributionBet() const {
    if (m_dPtr->m_playerID == -1) {
        return -1;
    }
    return m_dPtr->m_distrBet;
}

gbp_i64 Userbox::totalMoney() const {
    return m_dPtr->m_stackMoney + /*roundBet() + */distributionBet() + winnings();
}

gbp_i64 Userbox::winnings() const {
    return m_dPtr->m_winnings;
}

int Userbox::cardsCount() const {
    return m_dPtr->m_cardsCount;
}


gbp_i8 Userbox::card(int index) const
{
    Q_ASSERT(index >= 0 && index <= 3);

    return m_dPtr->m_cards.at(index)->cardIndex();
}

CardWidget *Userbox::findCard(gbp_i8 cardId) const
{
    for (int i = 0; i < cardsCount(); i++) {
        if (card(i) == cardId) {
            return m_dPtr->m_cards[i];
        }
    }
    return nullptr;
}

void Userbox::setCard(int index, gbp_i8 card)
{
    Q_ASSERT_X(card >= -1 && card < 52, "setCard", QString("Card %0 out of range").arg(card).toLatin1());
    Q_ASSERT_X(index >= 0 && index < 4, "setCard", QString("Card %0 index(%1) out of range").arg(card).arg(index).toLatin1());

    m_dPtr->m_cards[index]->setCardIndex(card);
}

void Userbox::setCard(int index, const char *repr)
{
    if (repr == 0) {
        setCard(index, -1);
    }

    gbp_i8 suitIdx = suit_index(repr);
    gbp_i8 cardIdx = card_index(repr);

//    qDebug() << "Wrong card passed! Ignoring. Card:" << repr;

    if (suitIdx == -1 || cardIdx == -1) {
        setCard(index, -1);
    } else {
        setCard(index, suitIdx*13 + cardIdx);
    }
}

QAbstractButton *Userbox::buttonTakeASit() const {
    return m_dPtr->bttn_sit;
}


void Userbox::setIsRightToLeft(bool rightToLeft)
{
    if (isRightToLeft() != rightToLeft) {
        m_dPtr->m_flags.isRightToLeft = rightToLeft;

        QWidgetList lst = findChildren<QWidget*>();
        for (QWidget* w : lst) {
            w->setProperty("isRightToLeft", isRightToLeft());
        }
        m_dPtr->box->setFrameIndex(rightToLeft ? 1 : 0);
        m_dPtr->orientationDependent->move(rightToLeft ? -14 : 149, rightToLeft ? 3 : 0);
        m_dPtr->cards->move(rightToLeft ? -17 : 144, rightToLeft ? -6 : -9);

//        m_dPtr->
//        QMenuBar* mb = new QMenuBar;
//        mb->addAction("Cho bliat", this, SLOT(clear()));
//        m_dPtr->layout_userInfo->setMenuBar(mb);
    }
}

bool Userbox::isRightToLeft() const {
    return m_dPtr->m_flags.isRightToLeft;
}

ns_model::CardsDeck *Userbox::deckModel() const {
    return m_dPtr->card_1->deckModel();
}

BetItem *Userbox::betItem() const {
    return m_dPtr->m_betItem;
}

void Userbox::setBetItem(BetItem *betItem) {
    m_dPtr->m_betItem = betItem;
}

qreal Userbox::opacity() const {
    return m_dPtr->m_opacity;
}

void Userbox::setOpacity(qreal newOpacity)
{
    if (!m_dPtr || !m_dPtr->m_flags.initialized) {
        return;
    }
    if (!qFuzzyCompare(m_dPtr->m_opacity, newOpacity)) {

        m_dPtr->m_opacity = newOpacity;
        m_dPtr->page_busy->setOpacity(newOpacity);
        emit opacityChanged(newOpacity);
    }
}

void Userbox::setAvatar(const QIcon &avatar)
{
    m_dPtr->avatar->setIcon(avatar);
    update();
//    if (!avatar.isNull()) {
//        update();
//    }
}

void Userbox::setNickname(QString name)
{
    if (name.length() > 12) {
        name.resize(12);
        name += "...";
    }
    m_dPtr->nickname->setText(name);
    update();
}

void Userbox::setStack(gbp_i64 money)
{
    if (m_dPtr->m_stackMoney == money)
        return;

    m_dPtr->m_stackMoney = money;
    update();
    setCurrentIndex(totalMoney() > 0 ? 1 : 2);
    emit stackChanged(money);
}

void Userbox::gainMoney(gbp_i64 m)
{
    QPropertyAnimation* anim = new QPropertyAnimation(this, "stack", this);
    anim->setDuration(250);
    anim->setStartValue(stack());
    anim->setEndValue(m);
    anim->setEasingCurve(QEasingCurve::InCubic);

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Userbox::setWinnings(gbp_i64 winnings)
{
    if (m_dPtr->m_winnings != winnings)
    {
        m_dPtr->m_winnings = winnings;
        update();
    }
}

void Userbox::setCardsCount(int count)
{
    if (m_dPtr->m_cardsCount == count) {
        return;
    }

    m_dPtr->m_cardsCount = count;
    m_dPtr->card_1->setVisible(count > 0);
    m_dPtr->card_2->setVisible(count > 1);
    m_dPtr->card_3->setVisible(count > 2);
    m_dPtr->card_4->setVisible(count > 3);

    update();

    for (int i = 0; i < 4; i++) {
        m_dPtr->m_cards[i]->setMask(m_dPtr->m_cards[i]->rect().adjusted(0, 0, 0, ((card(i) == -1) ? -22 : -10)));
    }

    emit cardsCountChanged(count);
}

void Userbox::clear()
{
    setWinnings(0);
    setStack(0);
    setNickname("<empty>");
    setAvatar(pApp->db().placeholder(E::Placeholder::Avatar));
}

void Userbox::timelineEvent(const TimeLineEvent &ev)
{
    gbp_i8 currUsrPos = qvariant_cast<gbp_i8>(modelsDB().getTableState(m_dPtr->m_gameID)->index_current_user_pos());
    m_dPtr->timeline->setVisible(currUsrPos == m_seatIndex && ev.type != ev.s);

    if (FramedEntity* glow = qobject_cast<FramedEntity*>(m_dPtr->m_glowWgt)) {
        if (ev.type == ev.s) {
            float val = float((ev.handler->durationShort() % 800)) / 800;
            glow->setOpacity(fabs(val - 0.5f)*2);
            glow->setBlending(E::Add);
        } else {
            glow->setOpacity(1.0);
            glow->setBlending(E::Overlay);
        }
    }
    if (ev.type == ev.s)
    {
    }
    else if (ev.type == ev.m)
    {
        m_dPtr->progressbar_timeline->setRange(0, (ev.handler->timeMark().m() - ev.handler->timeMark().s())*1000);
        m_dPtr->progressbar_timeline->setValue(ev.handler->durationMain());
        m_dPtr->timeline->setStyleSheet("");
        m_dPtr->lbl_timer->setNum(static_cast<int>(ev.handler->durationMain()/1000));

    }
    else if (ev.type == ev.a)
    {
        m_dPtr->progressbar_timeline->setRange(0, (ev.handler->timeMark().a() - ev.handler->timeMark().m())*1000);
        m_dPtr->progressbar_timeline->setValue(ev.handler->durationFull());
        QString s = "QProgressBar::chunk {"
                    "background-color: qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:0,"
                    "stop:0 rgba(66, 5, 5, 66), stop:0.46 rgba(210, 9, 9, 248), stop:1 #a10707);}";
        m_dPtr->timeline->setStyleSheet(s);
        m_dPtr->lbl_timer->setNum(static_cast<int>(ev.handler->durationFull()/1000));
    }
}

void Userbox::setHorizontalOffset(int offset)
{
    if (m_dPtr->m_horizontalOffset != offset)
    {
        int diff = offset - m_dPtr->m_horizontalOffset;
        m_dPtr->bttn_sit->move(m_dPtr->bttn_sit->pos() + QPoint(diff, 0));
        m_dPtr->label->move(m_dPtr->label->pos() + QPoint(diff, 0));
        m_dPtr->m_horizontalOffset = offset;
    }
}

int Userbox::horizontalOffset() const
{
    return m_dPtr->m_horizontalOffset;
}

void Userbox::displayAction(GameAction action, int durationMsec)
{
    auto actionId = underlying_cast(action.first);

    if (m_dPtr->imsg_action->messages().size() <= actionId) {
        return;
    }
    m_dPtr->imsg_action->setCurrentMsgIndex(actionId);
    if (action.second > 0) {
        m_dPtr->imsg_action->setText(m_dPtr->imsg_action->text().arg(Money(action.second).toString()));
    }

    m_dPtr->stackOfDisplays->setCurrentIndex(1);
    QTimer::singleShot(durationMsec, this, SLOT(displayUserInfoPage()));
}

void Userbox::setDeckModel(ns_model::CardsDeck* model)
{
    if (deckModel() != model) {
        for (CardWidget* card: m_dPtr->m_cards) {
            card->setDeckModel(model);
        }
    }
}

void Userbox::displayUserInfoPage()
{
    m_dPtr->stackOfDisplays->setCurrentIndex(0);
}

void Userbox::onModelReset()
{

}

/*void Userbox::onStatusChanged(const QMetaProperty &hint)
{
    QString nick = nickname();
    nick.resize(15);

    const TablePlayer* pl = m_dPtr->m_player;
    QString hName = hint.name();
    bool value = hint.read(pl).toBool();

    playerChangeEvent(pl);

    if (hName == "current" && !value) {
//        m_dPtr->glow->hide();
        m_dPtr->timeline->hide();
    }

}

void Userbox::onMoneyChanged(const QMetaProperty &hint)
{
    const TablePlayer* pl = m_dPtr->m_player;

    QString hName(hint.name());
    gbp_i64 m = qvariant_cast<gbp_i64>(hint.read(pl));

    if (hName == "gameMoney") {
        gainMoney(m);
    } else if (hName == "roundBet") {
        if (m_dPtr->m_betItem != nullptr) {
            m_dPtr->m_betItem->setMoney(m);
        }
    }
}*/

// void Userbox::updateStack(gbp_i64 stack)
// {
//     m_dPtr->imsg_moneyOrState->setMessage(0, Money(stack).toString());
//     m_dPtr->imsg_moneyOrState->setCurrentMsgIndex(0);
// }

void Userbox::onSettingsChanged(QAction* settings)
{
    if (settings->data().canConvert<QVariantMap>())
    {
        QVariantMap d = settings->data().value<QVariantMap>();
        QString theme = d.value("theme").toString();
        QWidgetList lst = findChildren<QWidget*>();
        for (QWidget* w: lst) {
            w->setProperty("theme", theme);
        }
        for (QWidget* w: lst) {
            w->setStyleSheet(w->styleSheet());
        }
        for (CardWidget* card: m_dPtr->m_cards) {
//            card->setSourceCode();
        }
    }

    update();
}

void Userbox::setGlowWidget(QWidget *glow)
{
    m_dPtr->m_glowWgt = glow;
    glow->setParent(m_dPtr->avatar);
    glow->move(0, 0);
    glow->show();
    glow->raise();
}

QWidget *Userbox::takeGlowWidget()
{
    QWidget* glow = m_dPtr->m_glowWgt;
    m_dPtr->m_glowWgt = nullptr;

    glow->setParent(nullptr);
    glow->hide();

    return glow;
}

QAction *Userbox::actionAddNote() const {
    return m_dPtr->actionAddNote;
}
QAction *Userbox::actionEditNote() const {
    return m_dPtr->actionEditNote;
}

void Userbox::actionEvent(QActionEvent *e)
{
//    if (const TablePlayer* pl = qobject_cast<const TablePlayer*>(e->action()))
//    {
//        switch (e->type()) {
//        case QEvent::ActionAdded:   playerAddEvent(pl); break;
//        case QEvent::ActionChanged: playerChangeEvent(pl); break;
//        case QEvent::ActionRemoved: playerRemoveEvent(pl); break;
//        default:
//            break;
//        }
//        m_dPtr->actionAddNote->setVisible(this->playerId() != netClient().userId());
//    }
//    else
//    {
//        if (e->action()->text() == "UpdateGameSettings" && e->type() == QEvent::ActionAdded) {
//            QAction* a = e->action();
//            connect(a, &QAction::triggered, this, [this, a]{ onSettingsChanged(a); });
//        }
//    }
    QStackedWidget::actionEvent(e);
}

//void Userbox::playerAddEvent(const TablePlayer *pl)
//{
////    Q_ASSERT(!m_dPtr->m_player);

//    reserve(pl->playerID());
//    m_dPtr->m_player = pl;
//    connect(pl, &TablePlayer::avatarUpdated, this, &Userbox::setAvatar);
//    connect(pl, &TablePlayer::moneyChanged, this, &Userbox::onMoneyChanged);
//    connect(pl, &TablePlayer::statusChanged, this, &Userbox::onStatusChanged);
//    m_dPtr->lbl_frozen->setVisible(pl->isFrozen());
////    m_dPtr->glow->setVisible(pl->isCurrent());

//    setEnabled(true);
//    setStack(pl->gameMoney());
//    setAvatar(pl->avatar());
//    setNickname(pl->text());

//    if (pl->isInGame()) {
//        setOpacity(1.0);
//    } else if (pl->isFolded()) {
//        setOpacity(0.55);
//    } else if (pl->isSitOut()) {
//        setOpacity(0.35);
//    }

//    setCurrentIndex(totalMoney().isPositive() ? 1 : 2);
//}
//void Userbox::playerChangeEvent(const TablePlayer *pl)
//{
//    Q_ASSERT(pl == m_dPtr->m_player);

//    setNickname(pl->text());
//    setAvatar(pl->avatar());
//    gainMoney(pl->gameMoney());
//    m_dPtr->lbl_frozen->setVisible(pl->isFrozen());
//    if (pl->isFolded()) {
//        if (cardsCount() > 0) {
//            QPoint pos1 = m_dPtr->card_1->pos();
//            QPoint pos2 = m_dPtr->card_2->pos();
//            QPoint p = m_dPtr->card_1->pos()/2 + m_dPtr->card_2->pos()/2;
//            QPropertyAnimation* anim1 = ns_anim::moveTo(m_dPtr->card_1, p, 300);
//            QPropertyAnimation* anim2 = ns_anim::moveTo(m_dPtr->card_2, p, 300);

//            anim1->setEasingCurve(QEasingCurve::InSine);
//            anim2->setEasingCurve(QEasingCurve::InSine);

//            anim1->start(QAbstractAnimation::DeleteWhenStopped);
//            anim2->start(QAbstractAnimation::DeleteWhenStopped);

//            connect(anim2, &QPropertyAnimation::finished, this, [this, pos1, pos2]{
//                setCardsCount(0);
//                setOpacity(0.55);
//                m_dPtr->card_1->move(pos1);
//                m_dPtr->card_2->move(pos2);
//            });
//        }
//        else
//        {
//            setOpacity(0.55);
//        }
//    }
//    if (pl->isSitOut()) {
//        setOpacity(0.35);
//    }
//    if (pl->isInGame()) {
//        setOpacity(1.0);
//    }
//    if (pl->isCurrent()) {
//        setOpacity(1.0);
//    }
//    setCurrentIndex(totalMoney().isPositive() ? 1 : 2);
//}
//void Userbox::playerRemoveEvent(const TablePlayer *pl)
//{
//    Q_ASSERT(pl == m_dPtr->m_player);
//    disconnect(pl);
//    pl->disconnect(this);
//    free();
//}


QByteArray Userbox::modelKey() const {
    return QString(sm_modelKey[0]).arg(m_seatIndex).toLocal8Bit();
}

void Userbox::updateData(const QAbstractItemModel *model)
{
//    QPropertyAnimation* eff = new QPropertyAnimation(this, "money", this);
//    eff->setStartValue(stack());
//    eff->setEndValue(index(model).data(UserType::OriginDataRole));
//    eff->setDuration(400);
//    eff->setEasingCurve(QEasingCurve::Linear);
//    m_animQueue.enqueue(eff);
//    connect(eff, &QPropertyAnimation::finished, this, [this]{
//        m_animQueue.dequeue()->deleteLater();
//        if (!m_animQueue.isEmpty()) {
//           m_animQueue.head()->start();
//        }
//    });
//    if (m_animQueue.head()->state() != QAbstractAnimation::Running) {
//        m_animQueue.head()->start();
    //    }
}



void Userbox::enqueueAnimation(QObject* target
                             , const QByteArray& property
                             , int duration
                             , const QVariant& startValue
                             , const QVariant& endValue
                             , QEasingCurve::Type ease
                             , const QByteArray& queueName
                             , bool run)
{
    QPropertyAnimation* eff = new QPropertyAnimation(target, property, this);
    eff->setStartValue(startValue);
    eff->setEndValue(endValue);
    eff->setDuration(duration);
    eff->setEasingCurve(ease);
    auto& queue = m_animQueue[queueName];
    queue.enqueue(eff);
    connect(eff, &QPropertyAnimation::finished, this, [&]{
        queue.dequeue()->deleteLater();
        if (!queue.isEmpty()) {
            queue.head()->start();
        }
    });

    if (run && queue.head()->state() != QAbstractAnimation::Running) {
        queue.head()->start();
    }
}
