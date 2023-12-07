#include <precompiled.h>
#include "dialog_gametable.h"
//#include "MainWindow.h"
#include "ui_dialog_GameTable.h"
#include <widgets/CardWidget.h>
#include <QGraphicsColorizeEffect>
#include <QFileDialog>
#include <QTimer>
#include <widgets/GLWidget.h>
#include <utils/CommonPainter.h>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
#endif //Q_MOC_RUN

#include <game/userbox.h>
#include <models/model_Chat.h>
#include <QTextBrowser>
#include <widgets/StatisticTableView.h>
#include <db/SharedDataService.h>
#include <db/GameTableSettingsData.h>
#include <descriptors/descriptor_GameTable.h>
#include <game/tableItems/potitem.h>
#include <animation/animation.h>

#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/table/rpc_table.h>
#endif //Q_MOC_RUN

#include <db/modeldatabase.h>
#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <tools/item_reference.hpp>
#include <game/actions/single_actions.h>

namespace
{
    rpc_table* getRpc() { return rpc_object<rpc_table>(); }
}

GameTable::GameTable(const GameId &id, QWidget *parent)
    : InitableDialog(parent)
    , GameIdObject(id)
    , m_ui(new Ui::GameTable)
    , m_gameID(id)
    , m_isNowResetting(false)
    , m_needClose(false)
    , m_editorOpenedByDefault(false)
{
    setWindowFlags(Qt::Window);
    m_ui->setupUi(this);

    m_withoutEditorSize = m_ui->gameSceneContainer->size() + QSize(4, 4);
    m_withEditorSize = maximumSize() + QSize(4, 4);
//    connect(m_ui->layers, SIGNAL(signalBttnClicked(QAbstractButton*)), this, SIGNAL(signalBttnClicked(QAbstractButton*)));

    setFixedSize(m_ui->editor_1->isVisible() ? m_withEditorSize : m_withoutEditorSize);

}

void GameTable::updateTitle()
{
    ItemRefCash itemRef(modelsDB().getCashStatistic()->item(id().m_tableID).ref());

    QString tableTitle = QString::fromStdString(itemRef._title)
            % QStringLiteral(" ")
            % ns_meta::money_type::display(itemRef._big_blind)
            % QStringLiteral("/")
            % ns_meta::money_type::display(itemRef._big_blind/2)
            % QStringLiteral(" ")
            % ns_util::toString(itemRef._limit)
            % QStringLiteral(" ")
            % ns_util::toString(itemRef._type);
    setWindowTitle(tableTitle);
}

GameTable::~GameTable()
{
    //qDebug() << "~GameTable";
    delete m_ui;
}

TableOver* GameTable::layers() const {
    return m_ui->layers;
}

UserControls* GameTable::hud() const {
    return (m_ui->layers) ? m_ui->layers->hud() : nullptr;
}

void GameTable::setEditorVisible(bool visible)
{
    m_ui->editor_1->setVisible(visible);
    m_ui->editor_2->setVisible(visible);
    m_ui->line_1->setVisible(visible);

    setFixedSize(visible ? m_withEditorSize : m_withoutEditorSize);
}

void GameTable::updateCurrentItemPosition(const QVector3D &pos)
{
    if (!isNowResetting()) {
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData())) {
            ro->setPosition(pos);
        }
    }
}

void GameTable::updateCurrentItemScale(const QVector3D &scale)
{
    if (!isNowResetting()) {
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData())) {
            ro->setScale(scale);
        }
    }
}

void GameTable::updateCurrentItemRotation(const QQuaternion &quat)
{
    if (!isNowResetting()) {
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData())) {
            ro->setRotation(quat);
        }
    }
}

void GameTable::updateCurrentItemOpacity(qreal opacity)
{
    if (!isNowResetting()) {
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData())) {
            ro->setOpacity(opacity);
        }
    }
}

void GameTable::chatPlayersMsg(const QString &name, const QString &msg)
{
    QTextBrowser* chatBrowser = m_ui->layers->chatBrowser();
    QString pattern = chatBrowser->property("msgPattern").toString();
    QString html = chatBrowser->toHtml().replace("</body></html>", pattern.arg(name, msg, "</body></html>"));
    chatBrowser->setHtml(html);
    chatBrowser->moveCursor(QTextCursor::End);
}

void GameTable::applyCurrentGameSettings()
{
//    SharedDataService& sharedDataService = pApp->db().sharedDataService();
//    quintptr dataId = sharedDataService.singleIdForType<GameTableSettingsData>();
//    GameTableSettingsData* data = sharedDataService.getData<GameTableSettingsData>(dataId);
//    GameTableSettingsData::Selection selection = id().isCash() ? data->cashSetting(tableId()) : data->tournamentSetting(tableId());
//    GameTableSettingsData::Item item = data->selectItem(selection);

//    if (RenderedObject* bg = findChild<RenderedObject*>("ro_bg")) {
//        float r = item.background.redF();
//        float g = item.background.greenF();
//        float b = item.background.blueF();
//        float a = item.background.alphaF();
//        QMatrix4x4 colorTransform( r, 0, 0, 0
//                                 , 0, g, 0, 0
//                                 , 0, 0, b, 0
//                                 , 0, 0, 0, a);
//        bg->setColorTransform(colorTransform);
//        if (qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData()) == bg) {
//            m_ui->obj_red->setValue(item.background.red());
//            m_ui->obj_green->setValue(item.background.green());
//            m_ui->obj_blue->setValue(item.background.blue());
//            m_ui->obj_alpha->setValue(item.background.alpha());
//        }
//    }
//    if (RenderedObject* table = findChild<RenderedObject*>("ro_under")) {
//        float r = item.feltColor.redF();
//        float g = item.feltColor.greenF();
//        float b = item.feltColor.blueF();
//        float a = item.feltColor.alphaF();
//        QMatrix4x4 colorTransform( r, 0, 0, 0
//                                 , 0, g, 0, 0
//                                 , 0, 0, b, 0
//                                 , 0, 0, 0, a);
////        colorTransform.
//        table->setColorTransform(colorTransform);
//        if (qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData()) == table) {
//            m_ui->obj_red->setValue(item.feltColor.red());
//            m_ui->obj_green->setValue(item.feltColor.green());
//            m_ui->obj_blue->setValue(item.feltColor.blue());
//            m_ui->obj_alpha->setValue(item.feltColor.alpha());
//        }
//    }

}

void GameTable::setupRenderedObjects(SpriteBuilder *builder)
{
    int index = m_ui->comboBox->count();
    RenderedObject* ro = builder->renderedObject();
    ro->setProperty("indexInComboBox", index);
    m_ui->comboBox->addItem(ro->objectName(), QVariant::fromValue(ro));
    m_ui->comboBox->setItemIcon(index, QIcon(ro->texturePath()));

    connect(builder->renderedObject(), &RenderedObject::objectNameChanged, this, [this](const QString& objectName)
    {
        m_ui->comboBox->setItemText(sender()->property("indexInComboBox").toInt(), objectName);
    });

    for (QObject* child : builder->children()) {
        if (SpriteBuilder* sbuilder  = qobject_cast<SpriteBuilder*>(child)) {
            setupRenderedObjects(sbuilder);
        }
    }
}

void GameTable::updateRTTObject(const QPixmap &pixmap)
{
    m_glRTTObject = pixmap;
    emit glRenderedToTexture(m_glRTTObject);
}

double GameTable::sumOrMul(double a, double b)
{
    if (m_ui->bttngroup_anim_type->checkedButton()->objectName() == "bttn_scale") {
        return a * b;
    }
    return a + b;
}

double GameTable::subOrDiv(double a, double b)
{
    if (m_ui->bttngroup_anim_type->checkedButton()->objectName() == "bttn_scale") {
        return a / b;
    }
    return a - b;
}
//0937439828


void GameTable::init()
{
    if (isInitialized()) {
        return;
    }
    if (id().isCash()) {
        ItemRefCash itemRef(modelsDB().getCashStatistic()->item(id().m_tableID).ref());
        m_ui->layers->setSeatCount(itemRef._players_count.second);
    }

    addAction(m_ui->actionShowEditor);
    addAction(m_ui->actionUpdateGameSettings);

    m_ui->layers->init();
    m_ui->layers->addAction(m_ui->actionUpdateGameSettings);
//    connect(m_ui.)

    m_ui->actionShowEditor->setChecked(m_editorOpenedByDefault);

    for (QObject* child : m_ui->glwidget->children()) {
        if (SpriteBuilder* sbuilder = qobject_cast<SpriteBuilder*>(child)) {
            setupRenderedObjects(sbuilder);
        }
    }
    connect(m_ui->comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &GameTable::onComboBoxIndexChanged);

    connect(m_ui->colorTransformEditor, &ColorMatrixEditor::changed, this, [this](const ColorTransform& mat){
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData())) {
            ro->setColorTransform(mat.matrix()/*.transposed()*/);
            ro->setColorOffset(mat.offset());
        }
    });

    connect(m_ui->anim_0, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_6->setValue(sumOrMul(m_ui->anim_0->value(), m_ui->anim_3->value()));});
    connect(m_ui->anim_3, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_6->setValue(sumOrMul(m_ui->anim_0->value(), m_ui->anim_3->value()));});
    connect(m_ui->anim_1, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_7->setValue(sumOrMul(m_ui->anim_1->value(), m_ui->anim_4->value()));});
    connect(m_ui->anim_4, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_7->setValue(sumOrMul(m_ui->anim_1->value(), m_ui->anim_4->value()));});
    connect(m_ui->anim_2, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_8->setValue(sumOrMul(m_ui->anim_2->value(), m_ui->anim_5->value()));});
    connect(m_ui->anim_5, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_8->setValue(sumOrMul(m_ui->anim_2->value(), m_ui->anim_5->value()));});
    connect(m_ui->anim_6, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_3->setValue(subOrDiv(m_ui->anim_6->value(), m_ui->anim_0->value()));});
    connect(m_ui->anim_7, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_4->setValue(subOrDiv(m_ui->anim_7->value(), m_ui->anim_1->value()));});
    connect(m_ui->anim_8, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this]{m_ui->anim_5->setValue(subOrDiv(m_ui->anim_8->value(), m_ui->anim_2->value()));});

    connect(m_ui->bttn_playanim, &QPushButton::clicked, this, [this]
    {
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData()))
        {
            QString animName = m_ui->bttngroup_anim_type->checkedButton()->objectName();
            QPropertyAnimation* anim = nullptr;
            if (animName == "bttn_move") {
                anim = ro->moveTo(QVector3D(m_ui->anim_6->value(), m_ui->anim_7->value(), m_ui->anim_8->value()), m_ui->duration->value());
            } else if (animName == "bttn_rotate") {
                anim = ro->rotateTo(QQuaternion::fromEulerAngles(m_ui->anim_6->value(), m_ui->anim_7->value(), m_ui->anim_8->value()), m_ui->duration->value());
            } else if (animName == "bttn_scale") {
                anim = ro->scaleTo(QVector3D(m_ui->anim_6->value(), m_ui->anim_7->value(), m_ui->anim_8->value()), m_ui->duration->value());
            }/* else if (animName == "bttn_colorize") {
//                anim = ro->colorizeTo(QColor((int)m_ui->anim_6->value(), (int)m_ui->anim_7->value(), (int)m_ui->anim_8->value()), m_ui->duration->value());
            }*/ else {
                anim = ro->fadeTo(m_ui->anim_6->value(), m_ui->duration->value());
            }
            anim->setEasingCurve(QEasingCurve::Type(m_ui->obj_EasingCurveComboBox->currentIndex()));
            anim->start();
            connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
        }
    });

    connect(m_ui->bttngroup_anim_type, (void(QButtonGroup::*)(QAbstractButton*))&QButtonGroup::buttonClicked, this, [this](QAbstractButton* bttn)
    {
        if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData()))
        {
            QString animName = bttn->objectName();
            if (animName == "bttn_move") {
                m_ui->anim_0->setValue(ro->position().x());
                m_ui->anim_1->setValue(ro->position().y());
                m_ui->anim_2->setValue(ro->position().z());
            } else if (animName == "bttn_rotate") {
                QVector3D angles = ro->rotation().toEulerAngles();
                m_ui->anim_0->setValue(angles.x());
                m_ui->anim_1->setValue(angles.y());
                m_ui->anim_2->setValue(angles.z());
            } else if (animName == "bttn_colorize") {
//                m_ui->anim_0->setValue(ro->color().red());
//                m_ui->anim_1->setValue(ro->color().green());
//                m_ui->anim_2->setValue(ro->color().blue());
            } else if (animName == "bttn_scale") {
                m_ui->anim_0->setValue(ro->scale().x());
                m_ui->anim_1->setValue(ro->scale().y());
                m_ui->anim_2->setValue(ro->scale().z());
            } else {
                m_ui->anim_0->setValue(ro->opacity());
            }
            if (animName == "bttn_scale")
            {
                m_ui->plus_0->setText("*");
                m_ui->plus_1->setText("*");
                m_ui->plus_2->setText("*");
            }
            else
            {
                m_ui->plus_0->setText("+");
                m_ui->plus_1->setText("+");
                m_ui->plus_2->setText("+");
            }
        }
    });

    m_ui->glwidget->setCameraTranslation(QVector3D(0, 0, -750));
    connect(m_ui->glwidget, &GLWidget::renderedToImage, m_ui->layers, &TableOver::setBackground);

    applyCurrentGameSettings();
    updateTitle();

    onInitialized();
}

void GameTable::setSitCount(int seatCount) {
    m_ui->layers->setSeatCount(seatCount);
}



void GameTable::displayEndOfRound(gbp_i8 /*roundNumber*/)
{
    QPropertyAnimation* anim = nullptr;

    for (BetItem* betItem : m_ui->layers->betItems()) {
//        betItem->setDisplayChipsOnly(true);
        betItem->setProperty("lastPos", betItem->pos());
//        PotItem
        anim = ns_anim::moveTo(betItem, m_ui->layers->tablePot()->pos(), 150);

        connect(anim, &QPropertyAnimation::finished, this
        , [this, betItem] {
            betItem->clear();
            betItem->move(betItem->property("lastPos").toPoint());
//            betItem->setDisplayChipsOnly(false);
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void GameTable::displayEndOfDistribution()
{
    // FIXME: implement me
}

void GameTable::displayEndOfGame()
{
    QLabel* label = new QLabel(tr("@msg_gameOver"), m_ui->gameSceneContainer);
    label->setStyleSheet("QLabel{ font: 500 30px \"Calibri\";"
                         "        color: #cc79df;}");
    label->setGeometry(200, 200, 500, 250);
    label->show();
    label->raise();
}

void GameTable::closeTable()
{
    m_needClose = true;
    InitableDialog::reject();
}

void GameTable::displayWinningAt(gbp_i8 pos
                                , gbp_i64 total
                                , gbp_i64 gain
                                , gbp_i8 potNumber
                                , gbp::ns_api::ns_table::e_cards_combination combination
                                , const std::vector<gbp_i8> &cards
                                , bool showCombination)
{
    if (Userbox* usrbox = m_ui->layers->userbox(pos)) {
        usrbox->setWinnings(gain);
        if (showCombination) {
            QList<CardWidget*> lst;
            QPropertyAnimation* anim = nullptr;

            for (int i = 0; i < 5; i++) {
                m_ui->layers->tableCard(i)->setOpacity(0.5);
            }

            for (gbp_i8 cardId: cards) {
                if (CardWidget* card = m_ui->layers->findCard(cardId))
                {
                    anim = ns_anim::moveTo(card, card->pos() + QPoint(0, -7), 400);
                    anim->setEasingCurve(QEasingCurve::InOutBounce);
                    anim->start(QAbstractAnimation::DeleteWhenStopped);
//                    card->setBlending(E::Normal);
                    card->setOpacity(1.0);
                    lst << card;
                }
            }
            connect(anim, &QPropertyAnimation::finished, this, [this, lst]{
                for (CardWidget* card: lst) {
                    QPropertyAnimation* anim = ns_anim::moveTo(card, card->pos() + QPoint(0, 7), 400);
                    anim->setEasingCurve(QEasingCurve::InOutBounce);
                    anim->start(QAbstractAnimation::DeleteWhenStopped);
                    connect(anim, &QPropertyAnimation::finished, this, [this]{
                        for (int i = 0; i < 5; i++) {
//                            m_ui->layers->tableCard(i)->setBlending(E::Normal);
                            m_ui->layers->tableCard(i)->setOpacity(1.0);
                        }
                    });
                }
            });
        }
    }
}

void GameTable::onComboBoxIndexChanged(int /*index*/)
{
    beginReset();
    if (RenderedObject* ro = qvariant_cast<RenderedObject*>(m_ui->comboBox->currentData()))
    {
        float pitch, yaw, roll;
        ro->rotation().getEulerAngles(&pitch, &yaw, &roll);
        m_ui->obj_pitch->setValue(static_cast<int>(pitch));
        m_ui->obj_yaw->setValue(static_cast<int>(yaw));
        m_ui->obj_roll->setValue(static_cast<int>(roll));

        QVector3D pos = ro->position();
        m_ui->obj_x->setValue(static_cast<int>(pos.x()));
        m_ui->obj_y->setValue(static_cast<int>(pos.y()));
        m_ui->obj_z->setValue(static_cast<int>(pos.z()));

        QVector3D scale = ro->scale();
        m_ui->obj_sx->setValue(scale.x());
        m_ui->obj_sy->setValue(scale.y());
        m_ui->obj_sz->setValue(scale.z());

//        QColor color = ro->color();
//        m_ui->obj_red->setValue(color.red());
//        m_ui->obj_green->setValue(color.green());
//        m_ui->obj_blue->setValue(color.blue());
//        m_ui->obj_alpha->setValue(color.alpha());

        m_ui->obj_opacity->setValue(ro->opacity());

        ColorTransform transform(ro->colorTransform()/*.transposed()*/, ro->colorOffset());

        m_ui->colorTransformEditor->setColorTransform(transform);
    }
    endReset();
}

void GameTable::closeEvent(QCloseEvent *e)
{
    //qDebug() << "closeEvent" << id().toString();
    if (m_needClose) {
        //qDebug() << "accepted";
        e->accept();
    } else {
        //qDebug() << "wait...";
        e->ignore();
        ns_action::UserClosingTable* action = new ns_action::UserClosingTable(id());
        action->execute();
    }
    InitableDialog::closeEvent(e);
}

void GameTable::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange) {
        m_ui->retranslateUi(this);
    }
    InitableDialog::changeEvent(e);
}

void GameTable::actionEvent(QActionEvent *e)
{
    InitableDialog::actionEvent(e);
}


void GameTable::accept()
{
    close();
}

void GameTable::reject()
{
    close();
}
