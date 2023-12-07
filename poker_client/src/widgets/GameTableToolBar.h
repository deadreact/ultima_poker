#pragma once

#include <QFrame>
#include <qbuttongroup.h>

#include <db/GameTableSettingsData.h>

namespace ns_dialog {
    class TableSettings;
}

class GameTableToolBar : public QFrame
{
    Q_OBJECT
private:
    QButtonGroup m_buttonGroup;
    ns_dialog::TableSettings* m_settingsDialog;
    QAction* m_actionOnSettingsApplied;
public:
    explicit GameTableToolBar(QWidget *parent = 0);
    virtual ~GameTableToolBar();
    QAbstractButton* button(int index) const;
    QAbstractButton* button(const QString &name) const;
    void addButton(QAbstractButton* bttn);

    void init();
private slots:
    void onGameSettingsClicked();
    void onWndMaximizeClicked();
    void onGameSettingsApplied();

    // QWidget interface
protected:
    virtual void actionEvent(QActionEvent *e) override;
};
