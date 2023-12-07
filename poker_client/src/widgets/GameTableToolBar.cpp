#include <precompiled.h>
#include <qabstractbutton.h>
#include <qevent.h>
#include "GameTableToolBar.h"
#include <dialogs/dialog_tablesettings.h>
#include <QAction>
#include <QEvent>

GameTableToolBar::GameTableToolBar(QWidget *parent)
    : QFrame(parent)
    , m_buttonGroup(this)
    , m_settingsDialog(nullptr)
    , m_actionOnSettingsApplied(nullptr)
{

}

GameTableToolBar::~GameTableToolBar()
{
    if (m_settingsDialog != nullptr) {
        delete m_settingsDialog;
    }
}

QAbstractButton *GameTableToolBar::button(int index) const
{
    if (index >= 0 && index < m_buttonGroup.buttons().size()) {
        return m_buttonGroup.button(index);
    }
    return nullptr;
}

QAbstractButton *GameTableToolBar::button(const QString &name) const
{
    for (QAbstractButton* bttn: m_buttonGroup.buttons()) {
        if (bttn->objectName() == name) {
            return bttn;
        }
    }
    return nullptr;
}

void GameTableToolBar::addButton(QAbstractButton *bttn)
{
    m_buttonGroup.addButton(bttn, m_buttonGroup.buttons().size());
}

void GameTableToolBar::init()
{
    QList<QAbstractButton*> buttons = findChildren<QAbstractButton*>(QRegExp("bttn_\\w+"));

    for (QAbstractButton* bttn: buttons) {
        addButton(bttn);
    }

    if (QAbstractButton* bttn = button("bttn_settings")) {
        connect(bttn, SIGNAL(clicked(bool)), this, SLOT(onGameSettingsClicked()));
    }
    if (QAbstractButton* bttn = button("bttn_wndMaximize")) {
        connect(bttn, SIGNAL(clicked(bool)), this, SLOT(onWndMaximizeClicked()));
    }
}

void GameTableToolBar::onGameSettingsClicked()
{
    if (m_settingsDialog == nullptr)
    {
        m_settingsDialog = new ns_dialog::TableSettings(this->window());
        m_settingsDialog->addAction(m_actionOnSettingsApplied);
        m_settingsDialog->init();
        connect(m_settingsDialog, SIGNAL(settingsApplied()), this, SLOT(onGameSettingsApplied()));
    }
    m_settingsDialog->open();
}

void GameTableToolBar::onWndMaximizeClicked()
{
    this->window()->showMaximized();
}

void GameTableToolBar::onGameSettingsApplied()
{
    if (m_actionOnSettingsApplied) {
        m_actionOnSettingsApplied->trigger();
    }
}


void GameTableToolBar::actionEvent(QActionEvent *e)
{
    if (e->type() == QEvent::ActionAdded)
    {
        if (e->action()->text() == "UpdateGameSettings") {
            m_actionOnSettingsApplied = e->action();
        }
    }
    else if (e->type() == QEvent::ActionRemoved)
    {
        m_actionOnSettingsApplied = nullptr;
    }
}
