#include "tablestatedemo.h"
#include "ui_tablestatedemo.h"

#include "../api_models/model_table_state_t.hpp"
#include "../api_models/model_table_player_info_t.hpp"

#include <api/table/types.h>
#include <QActionEvent>
#include <api_util.hpp>
#include <api_models.hpp>
#include <qtextstream.h>
#include <qdebug.h>
#include <api/tools/random.hpp>
#include <QDir>
#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <itemtype_concept.hpp>
#include <qpainter.h>
#include <iostream>
#include <QSvgRenderer>
#include <model_user_info_t.hpp>
#include <model_cash_stat_t.hpp>
#include <model_tournament_stat_t.hpp>

class SvgDelegate : public QStyledItemDelegate
{
private:
    QSvgRenderer* m_renderer;
public:
    SvgDelegate(QWidget* parent = nullptr)
        : QStyledItemDelegate(parent)
        , m_renderer(new QSvgRenderer(this))
    {}
    virtual ~SvgDelegate() {}
public:
    virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override {
//        QString text = index.data(Qt::DisplayRole).toString();
        qint8 id = index.data(Qt::EditRole).value<qint8>();
        int type = 4;
        if (id >= 0 && id < 52)
        {
            qint8 suit = id / 13;
            qint8 card = id % 13;
            QStringList suit_tmp;
            QStringList card_tmp;
            suit_tmp << "%0_clubs.svg";
            suit_tmp << "%0_spades.svg";
            suit_tmp << "%0_diamonds.svg";
            suit_tmp << "%0_hearts.svg";
            card_tmp << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "J" << "Q" << "K" << "A";

            p->drawRect(opt.rect);

            QString filepath = QString("../../poker_client/data/textures/svg/cards_0%0/").arg(type) +  suit_tmp.at(suit).arg(card_tmp.at(card));
            qDebug() << QFileInfo(filepath).exists();
            m_renderer->load(filepath);
        }
        else
        {
            QString path = QString("../../poker_client/data/textures/svg/shirts/0%0.svg").arg(type);
            m_renderer->load(path);
        }
        emit m_renderer->repaintNeeded();
        m_renderer->render(p, opt.rect);
    }
};


class Delegate : public QItemDelegate
{
private:
public:
    Delegate(QWidget* parent = nullptr)
        : QItemDelegate(parent)
    {}
    virtual ~Delegate() {}
protected:
    virtual void drawDisplay(QPainter *p, const QStyleOptionViewItem &/*opt*/, const QRect &rc, const QString &text) const override {

        QString avaName;
        QDir dir = QDir::current();
        dir.cd("../../poker_client/data/textures/avatars");
        QStringList lst = dir.entryList();
        QMap<QString, QMap<int, QByteArray>> namedHashes;
        for (const QString &str: lst) {
            QStringList texts = QRegularExpression("(\\w+)_(\\d+)_(\\w+)").match(str).capturedTexts();
            if (texts.size() > 2) {
                if (texts.at(3) == text) {
                    avaName = texts.at(1);
                }
                namedHashes[texts.at(1)].insert(texts.at(2).toInt(), texts.at(3).toLatin1());
            }
        }

        QIcon icon;
        for (int i = 0; i < 4; i++) {
            int s = namedHashes.value(avaName).keys().at(i);
            QString filename = QString("../../poker_client/data/textures/avatars/%0_%1_%2.png").arg(avaName).arg(s).arg(namedHashes.value(avaName).value(s).data());
//            qDebug() << QFileInfo(filename).exists();
            icon.addFile(filename, QSize(s, s));
        }
        QRect quad(rc.topLeft(), QSize(qMin(rc.width(), rc.height()), qMin(rc.width(), rc.height())));
        icon.paint(p, quad);
//        p->drawPixmap(quad, ns_meta::img_type::display(text.toStdString()));
    }
    virtual void drawDecoration(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QPixmap &pixmap) const override {
        QItemDelegate::drawDecoration(p, opt, rc, pixmap);
    }

    // QAbstractItemDelegate interface
public:
    virtual QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const override {
        int size = qMax(opt.rect.width(), opt.rect.height());
        return QSize(size, size);
    }
};

// ":/../../poker_client/data/textures/avatars/assassin_66_778d2a3d67ddb4b97146a6723beee8ef6d73573e.png";

namespace
{
    using player_type = decltype(std::declval<gbp::ns_api::ns_table::table_state_t>()._players)::value_type;
    void setRandomPlayers(gbp::ns_api::ns_table::table_state_t& d)
    {
        std::vector<player_type>& players = d._players;
        std::unordered_set<int64_t> excl_ids;
        QDir dir = QDir::current();
        dir.cd("../../poker_client/data/textures/avatars");
        QStringList lst = dir.entryList();
        QMap<QString, QMap<int, QByteArray>> namedHashes;
        for (const QString &str: lst) {
            QStringList texts = QRegularExpression("(\\w+)_(\\d+)_(\\w+)").match(str).capturedTexts();
            if (texts.size() > 2) {
                namedHashes[texts.at(1)].insert(texts.at(2).toInt(), texts.at(3).toLatin1());
            }
        }
//        qDebug() << lst;
        if (lst.isEmpty()) {
            return;
        }
        for (player_type& pl : players) {
            pl._id                       = gbp::randomgen().get_random_id(10, 10000, excl_ids);
            excl_ids.insert(pl._id);
            pl._is_user                  = rand() % 2;
            pl._pos                      = rand() % d._seats_count;
            pl._nickname                 = gbp::randomgen::get_random_string(rand() % 8 + 4);
            pl._country                  = rand() % 257;
            pl._money                    = rand() % 100000 + d._big_blind;
            pl._start_stack              = d._big_blind * ((rand() % 100) + 100);
            pl._bounty_progressive_money = (rand() % 3) * 100;
            pl._hash_avatar              = namedHashes.value(namedHashes.keys().at(rand()%namedHashes.keys().size())).value(244).toStdString();
            std::cout << pl._hash_avatar << std::endl;
            pl._is_dealer                = rand() % 2;
            pl._cards                    = {qint8(rand() % 52), qint8(rand() % 52)};
            pl._round_bet                = d._big_blind * (rand() % 20);
            pl._is_current               = d._current_user_pos == pl._pos;
        // pl._state                    = 0;
        }
    }

    gbp::ns_api::ns_table::table_state_t getRandomState()
    {
        QFile f("tmp.txt");
        if (!f.exists()) {
            f.open(QIODevice::WriteOnly);
            f.write(QString::number(rand()).toLatin1());
            f.close();
        }
        qDebug() << f.open(QIODevice::ReadWrite);
        QDataStream s(&f);
        QVector<quint32> vec;
        for (; !s.atEnd(); s >> vec);

        quint32 rnum = rand();
        for (;vec.contains(rnum); rnum = rand());

        s << rnum;
        f.close();
        qDebug() << rnum;

        gbp::ns_api::ns_table::table_state_t d;
        d._tournament_id = -1;
        d._game_id = gbp::randomgen().get_random_id(10, 10000, std::unordered_set<qint64>());
        d._big_blind = (rand() % 10000)*10;
        d._seats_count = qMin(2 + (rand() % 3)*4, 9);
        d._players.resize(rand() % d._seats_count + 1);

        setRandomPlayers(d);

        d._current_user_pos = rand() % d._seats_count;

        int round = rand() % 3 + 1;
        if (round > 0) {
            d._table_cards = {qint8(rand()%52), qint8(rand()%52), qint8(rand()%52)};
            if (round > 1) {
                d._table_cards.push_back(qint8(rand()%52));
                if (round > 2) {
                    d._table_cards.push_back(qint8(rand()%52));
                }
            }
        }
        d._all_pots.resize(rand() % 3);
        for (auto& pot: d._all_pots) {
            pot = d._big_blind * 2 * (rand() % 100) + rand() % 1000;
        }

        d._waiters_count = d._players.size() == d._seats_count ? rand() % 10 : 0;
        d._time_short = 2;
        d._time_main = 12;
        int additional_time = rand() % 30;
        d._time_action_passed = rand() % (d._time_main + additional_time);
        d._time_action_left = (d._time_short + d._time_main + additional_time) - d._time_action_passed;
        d._distribution_number = rand() % 1000;
        return d;
    }
}



TableStateDemo::TableStateDemo(QWidget *parent)
    : QTabWidget(parent)
    , m_ui(new Ui::TableStateDemo)
    , m_table_state(new ns_model::table_state_t(this))
    , m_user_info(new ns_model::user_info_t(this))
    , m_cash_stat(new ns_model::cash_stat_t(this))
    , m_tournament_stat(new ns_model::tournament_stat_t(this))
{
    m_ui->setupUi(this);

    initPage3();
}

TableStateDemo::~TableStateDemo()
{
    delete m_ui;
}

void TableStateDemo::initPage3()
{
    m_table_state->update(getRandomState());
    m_user_info->resetData(ns_model::ns_test::createRandomUser());
    m_cash_stat->insertRows(0, 10);
    m_tournament_stat->insertRows(0, 10);

    m_ui->tree_1->setModel(m_table_state);
    m_ui->tree_2->setModel(m_user_info);
    m_ui->tree_3->setModel(m_cash_stat);
    m_ui->tree_4->setModel(m_tournament_stat);
}

void TableStateDemo::initPage2()
{
    ns_model::table_state_t* m = m_table_state;
    m->update(getRandomState());
    m_ui->_view->setModel(m);
    m_ui->view_cards->setModel(m);
    ns_model::bind(m_ui->table_card_1, m->index(0, 0, m->indexOf(ns_model::table_state_t::element::_table_cards)));
    ns_model::bind(m_ui->table_card_2, m->index(1, 0, m->indexOf(ns_model::table_state_t::element::_table_cards)));
    ns_model::bind(m_ui->table_card_3, m->index(2, 0, m->indexOf(ns_model::table_state_t::element::_table_cards)));
    ns_model::bind(m_ui->table_card_4, m->index(3, 0, m->indexOf(ns_model::table_state_t::element::_table_cards)));
    ns_model::bind(m_ui->table_card_5, m->index(4, 0, m->indexOf(ns_model::table_state_t::element::_table_cards)));

    ns_model::bind(m_ui->avatar, m->index(0, 0, m->indexOf(ns_model::table_state_t::element::_players)));

    ns_model::vec_table_player_info_t* tblPlayersModel = new ns_model::vec_table_player_info_t;
    tblPlayersModel->setSourceModel(m);

    m_ui->view_players->setModel(tblPlayersModel);
    m_ui->view_players->setItemDelegateForColumn(underlying_cast(ns_model::vec_table_player_info_t::field::_hash_avatar), new Delegate);

    m_ui->listView->setModel(m);
    m_ui->listView->setRootIndex(m->indexOf(ns_model::table_state_t::element::_table_cards));

    m_ui->listView->setItemDelegate(new SvgDelegate);
}


void TableStateDemo::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TableStateDemo::on__view_activated(const QModelIndex &index) {
    if (index.model()->hasChildren(index)) {
        m_ui->_view->setRootIndex(index);
        m_ui->_view->setCurrentIndex(m_ui->_view->model()->index(0, 0, index));
//        m_ui->index_name->setText(static_cast<ns_model::table_state_t*>(m_ui->_view->model())->displayPath(index));
    }
}

void TableStateDemo::on_bttn_back_clicked() {
    if (m_ui->_view->rootIndex().isValid()) {
        auto oldIndex = m_ui->_view->rootIndex();
        auto index = oldIndex.parent();
        m_ui->_view->setRootIndex(index);
        m_ui->_view->setCurrentIndex(oldIndex);
//        m_ui->index_name->setText(static_cast<ns_model::table_state_t*>(m_ui->_view->model())->displayPath(index));
    }
}

void TableStateDemo::on_bttn_reset_clicked() {
    m_table_state->update(getRandomState());
}
