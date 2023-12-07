#include <precompiled.h>
#include "playerinfoheaderview.h"
#include <api_models/model_user_info_t.hpp>

PlayerInfoHeaderView::PlayerInfoHeaderView(QObject *parent)
    : QObject(parent)
    , m_model(nullptr)
    , m_avatar(nullptr)
    , m_nickname(nullptr)
    , m_amount(nullptr)
    , m_vipStatus(nullptr)
{}

void PlayerInfoHeaderView::setModel(ns_model::user_info_t *model)
{
    if (m_model != model)
    {
        if (m_model != nullptr) {
            disconnectFromModel();
        }
        m_model = model;
        if (model != nullptr) {
            connectToModel();
        }
        emit modelChanged(model);
    }
}

ns_model::user_info_t *PlayerInfoHeaderView::model() const {
    return m_model;
}

void PlayerInfoHeaderView::setAvatarWidget(QWidget* wgt) {
	if (m_avatar != wgt) {
		if (m_model != nullptr && m_avatar != nullptr) {

		}		

		m_avatar = wgt;


	}
}

void PlayerInfoHeaderView::setNicknameWidget(QWidget* wgt) {
	if (m_nickname != wgt) {
		QWidget* old = m_nickname;
		m_nickname = wgt;
	}
}
void PlayerInfoHeaderView::setAmountWidget(QWidget* wgt) {
	if (m_amount != wgt) {
		QWidget* old = m_amount;
		m_amount = wgt;
	}
}
void PlayerInfoHeaderView::setVipStatusWidget(QWidget* wgt) {
	if (m_vipStatus != wgt) {
		QWidget* old = m_vipStatus;
		m_vipStatus = wgt;
	}
}

QWidget* PlayerInfoHeaderView::avatarWidget() const {
	return m_avatar;
}
QWidget* PlayerInfoHeaderView::nicknameWidget() const {
	return m_nickname;
}
QWidget* PlayerInfoHeaderView::amountWidget() const {
	return m_amount;
}
QWidget* PlayerInfoHeaderView::vipStatusWidget() const {
    return m_vipStatus;
}

void PlayerInfoHeaderView::updateAvatar(const QVector<int> &roles)
{
    Q_ASSERT(m_model);
    if (m_avatar) {
        if (roles.isEmpty() || roles.contains(Qt::DecorationRole)) {
            QIcon icon = m_model->icon(ns_model::user_info_t::field::_hash_avatar);
            m_avatar->setProperty("icon", icon);
//            m_avatar->setProperty("pixmap", icon.pixmap(m_avatar->size()));
        }
    }
}

void PlayerInfoHeaderView::updateNickname(const QVector<int> &roles)
{
    Q_ASSERT(m_model);
    if (m_nickname) {
        if (roles.isEmpty() || roles.contains(Qt::DisplayRole)) {
            m_nickname->setProperty("text", m_model->text(ns_model::user_info_t::field::_nickname));
        }
    }
}

void PlayerInfoHeaderView::updateAmount(const QVector<int> &roles)
{
    Q_ASSERT(m_model);
    if (m_amount) {
        if (roles.isEmpty() || roles.contains(Qt::DisplayRole)) {
            m_amount->setProperty("text", m_model->text(ns_model::user_info_t::field::_money));
        }
//        if (roles.isEmpty() || roles.contains(Qt::DecorationRole)) {
//            m_amount->setProperty("icon", m_model->icon(ns_model::user_info_t::field::_money));
//        }
    }
}

void PlayerInfoHeaderView::updateVipStatus(const QVector<int> &roles)
{
    Q_ASSERT(m_model);
    if (m_vipStatus) {
        if (roles.isEmpty() || roles.contains(Qt::DisplayRole)) {
            m_vipStatus->setProperty("text", m_model->text(ns_model::user_info_t::field::_vip_status));
        }
//        if (roles.isEmpty() || roles.contains(Qt::DecorationRole)) {
//            m_vipStatus->setProperty("icon", m_model->icon(ns_model::user_info_t::field::_vip_status));
//        }
    }
}

void PlayerInfoHeaderView::connectToModel()
{
    connect(m_model, &ns_model::user_info_t::modelAboutToBeReset, this, &PlayerInfoHeaderView::onModelAboutToReset);
    connect(m_model, &ns_model::user_info_t::modelReset, this, &PlayerInfoHeaderView::onModelReset);
    connect(m_model, &ns_model::user_info_t::dataChanged, this, &PlayerInfoHeaderView::onModelDataChanged);
}

void PlayerInfoHeaderView::disconnectFromModel()
{
    disconnect(m_model, &ns_model::user_info_t::modelAboutToBeReset, this, &PlayerInfoHeaderView::onModelAboutToReset);
    disconnect(m_model, &ns_model::user_info_t::modelReset, this, &PlayerInfoHeaderView::onModelReset);
    disconnect(m_model, &ns_model::user_info_t::dataChanged, this, &PlayerInfoHeaderView::onModelDataChanged);
}

void PlayerInfoHeaderView::onModelAboutToReset()
{
    if (m_avatar) {
        m_avatar->setProperty("icon", QIcon());
//        m_avatar->setProperty("pixmap", QPixmap());
    }
    if (m_nickname) {
        m_nickname->setProperty("text", QString());
    }
    if (m_amount) {
        m_amount->setProperty("text", QString());
//        m_amount->setProperty("icon", QIcon());
    }
    if (m_vipStatus) {
        m_vipStatus->setProperty("text", QString());
//        m_vipStatus->setProperty("icon", QIcon());
    }
}
void PlayerInfoHeaderView::onModelReset()
{
    updateAvatar();
    updateNickname();
    updateAmount();
    updateVipStatus();
}
void PlayerInfoHeaderView::onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    for (int i = topLeft.row(); i <= bottomRight.row(); i++) {
        for (int j = topLeft.column(); j <= bottomRight.column(); j++) {
            QModelIndex idx = m_model->index(i, j);
            qDebug() << "data changed" << idx;
            qDebug() << m_model->itemData(idx);
            switch (m_model->fieldOf(idx)) {
            case ns_model::user_info_t::field::_hash_avatar: updateAvatar(roles);    break;
            case ns_model::user_info_t::field::_nickname:    updateNickname(roles);  break;
            case ns_model::user_info_t::field::_money:       updateAmount(roles);    break;
            case ns_model::user_info_t::field::_vip_status:  updateVipStatus(roles); break;
            default:
                break;
            }
        }
    }
}
