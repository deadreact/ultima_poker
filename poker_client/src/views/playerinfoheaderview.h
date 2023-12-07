#pragma once

#include <QWidget>

namespace ns_model
{
    class user_info_t;
} //ns_model

class PlayerInfoHeaderView : public QObject
{
    Q_OBJECT
public:
    enum class widgets : int {
        avatar = 0,
        nickname,
        amount,
        vip_status,

        count
    };
private:
    ns_model::user_info_t* m_model;
    QWidget* m_avatar;
    QWidget* m_nickname;
    QWidget* m_amount;
    QWidget* m_vipStatus;
public:
    explicit PlayerInfoHeaderView(QObject *parent = nullptr);
    void setModel(ns_model::user_info_t* model);
    ns_model::user_info_t* model() const;

    void setAvatarWidget(QWidget* wgt);
    void setNicknameWidget(QWidget* wgt);
    void setAmountWidget(QWidget* wgt);
    void setVipStatusWidget(QWidget* wgt);

    QWidget* avatarWidget() const;
    QWidget* nicknameWidget() const;
    QWidget* amountWidget() const;
    QWidget* vipStatusWidget() const;
protected:
    void updateAvatar(const QVector<int>& roles = QVector<int>());
    void updateNickname(const QVector<int>& roles = QVector<int>());
    void updateAmount(const QVector<int>& roles = QVector<int>());
    void updateVipStatus(const QVector<int>& roles = QVector<int>());
signals:
    void modelChanged(ns_model::user_info_t*);
private:
    void connectToModel();
    void disconnectFromModel();
private slots:
    void onModelAboutToReset();
    void onModelReset();
    void onModelDataChanged(const QModelIndex& topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

};

