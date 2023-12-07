#pragma once

#include <QAbstractItemView>
#include <game/tableItems/betitem.h>
#include <interface/initable.h>
#include <interface/identifiable.h>
#include <qabstractbutton.h>
#include <qbuttongroup.h>

#include <db/GameTableSettingsData.h>

namespace ns_model {
class CardsDeck;
} // ns_model

class Userbox;
class UserControls;

class QTextBrowser;
class FramedEntity;
class QLabel;
class PotItem;
class CardWidget;
class QListView;

class EntityWidget;
class NotesCompactView;

class TableOver : public InitableWidget, public GameIdRefObject
{
    Q_OBJECT

    Q_PROPERTY(QImage background READ background WRITE setBackground NOTIFY backgroundChanged)
//    Q_PROPERTY(Player* playerController READ playerController WRITE setPlayerController)
public:
    enum class Layer
    {
        Background = 0, /*Rendered content from OpenGL framebuffer*/
        GameTable,      /* Table cards, pots, bets, chips */
        MiddleSpace,    /* Profilers, Ingame menus */
        HudControls     /* Player Controller */
    };

private:
    struct TableOverData;
    QScopedPointer<TableOverData> m_dPtr;
    typedef const void* Handle;
//    QScopedPointer<NotesCompactView> m_addNoteDialog;
public:
    TableOver(QWidget *parent);
    TableOver(const GameId& id = GameId::invalidID(), QWidget *parent = nullptr);
    virtual ~TableOver();

    virtual void init() override;

    UserControls* hud() const;

    void setSeatCount(int n);
    int seatCount() const;

    const ns_model::CardsDeck* deckModel() const;

    template <int seatCount>
    Handle initWithSeatCount();
    void updateUserboxes();

    gbp_i8 playerPos(gbp_i64 playerID) const;

    const QImage& background() const;

    const QList<Userbox*>& userboxes() const;
    Userbox* userbox(gbp_i8 pos) const;
    Userbox* userboxByPlayerId(gbp_i64 playerID) const;
    void arrangeItems();

    const QList<BetItem*>& betItems() const;
    BetItem *betItem(int index) const;
    void clearBetItems();

    void setPotsCount(int count);
    PotItem *tablePot(int index = 0/*common pot*/) const;
    void clearPots();

    QTextBrowser *chatBrowser() const;
    CardWidget* tableCard(int index) const;
    CardWidget* findCard(gbp_i8 cardId, bool onlyTableCards = false) const;

    void clearTableCards();
    void setDealerPos(gbp_i8 pos);

    QWidget *layer(Layer layerType) const;
    void setModelKeys();
signals:
    void signalBttnClicked(QAbstractButton *bttn);
    void backgroundChanged(const QImage& background);

public slots:
    void setBackground(const QImage& background);
//    void setDistributionNumber(gbp_i64 number);

    void onSettingsChanged(QAction *settings);
private slots:
protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void actionEvent(QActionEvent* e) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void paintEvent(QPaintEvent *e) override;
};
