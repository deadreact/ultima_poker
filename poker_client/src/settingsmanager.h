#pragma once

#include <QObject>

class NotesCompactView;

class SettingsMgr : public QObject
{
    Q_OBJECT
    struct PrivateData;
    PrivateData* m_dPtr;
public:
    enum GuiType : quint32
    {
        Dialog      = 0x01,
        Page        = 0x02,
        Notes       = 0x04,
        NotesDialog = Notes | Dialog,
        NotesPage   = Notes | Page
    };
    Q_FLAG(GuiType)
    Q_DECLARE_FLAGS(SettingsGuiFlag, GuiType)

public:
    explicit SettingsMgr(QObject *parent = 0);
    virtual ~SettingsMgr();

    template <typename TWidget>
    bool init(TWidget* obj) const;

    template <typename TWidget>
    TWidget* create(QWidget *parent = nullptr) const {
        TWidget* w = new TWidget(parent);
        if (init(w)) {
            return w;
        }
        delete w;
        return nullptr;
    }
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SettingsMgr::SettingsGuiFlag)
