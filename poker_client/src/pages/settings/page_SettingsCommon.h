#pragma once

#include "page_SettingsBase.h"

namespace Ui {
class SettingsCommon;
} // Ui

class QButtonGroup;

namespace ns_model {
class Timezone;
}
class Localizer;
class QActionGroup;

namespace ns_page
{
    struct SettingsCommonPageData
    {
        QString lang;
        QByteArray timezoneID;
        union {
            gbp_u32 commonFlags;
            struct {
                gbp_u32 soundEnabled   : 1;  // on/off
                gbp_u32 volume         : 10; // 0 - 1000
                gbp_u32 sfxEnable      : 11; // 11 x on/off
                gbp_u32 memLogin       : 1;  // remember/do not save
                gbp_u32 memPassword    : 1;  // remember/do not save
                gbp_u32 useTimeout     : 1;  // use/do not use
                gbp_u32 timeoutMinutes : 7;  // 0-99
            };
        };

        SettingsCommonPageData();
        bool operator==(const SettingsCommonPageData& d) const;
        inline bool operator!=(const SettingsCommonPageData& d) const { return !operator ==(d); }
    };

    class SettingsCommon : public SettingsBase
    {
        Q_OBJECT
    private:
        Ui::SettingsCommon* m_ui;
        QButtonGroup* m_groupPlaySound;

        SettingsCommonPageData m_data;
        bool m_soundEnabled;
        int m_volume;
        QByteArray m_timezoneId;
        Localizer* m_localizer;

        static QStringList m_sounds;
    public:
        explicit SettingsCommon(QWidget *parent = nullptr);
        virtual ~SettingsCommon();

        virtual void init() override;
//        QModelIndex indexOfTimezone(const QByteArray& id) const;
        QString restoreLanguage() const;
        // ex: "en", "uk", "ru"
        int indexOfLang(const QString& lang) const;
        int indexOfLang(QAction* lang) const;
        QAction* languageAction(const QString& lang) const;
    protected:
        void createTimezoneMenu(ns_model::Timezone *model);
        void createLanguageMenu(Localizer *localizer);
    	virtual void updateTexts() override;
        virtual void updateIsModified() override;
        void importData(const SettingsCommonPageData& d);
        void exportData(SettingsCommonPageData& d);
    protected slots:
        virtual void load() override;
        virtual void apply() override;
    };

} //namespace ns_page


QDataStream& operator<<(QDataStream& os, const ns_page::SettingsCommonPageData&  data);
QDataStream& operator>>(QDataStream& is, ns_page::SettingsCommonPageData&  data);
