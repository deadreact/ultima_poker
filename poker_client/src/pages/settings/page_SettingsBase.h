#pragma once

#include <interface/initable.h>

namespace ns_page
{
    class SettingsBase : public InitableWidget
    {
        Q_OBJECT

        Q_PROPERTY(bool modified READ isModified MEMBER m_modified)
    protected:
        bool m_modified;
    signals:
        void applied();
        void rollbacked();
    public:
        explicit SettingsBase(QWidget *parent = nullptr);
        virtual ~SettingsBase();

        inline bool isModified() const { return m_modified; }
    protected:
    	virtual void updateTexts() = 0;
        virtual void changeEvent(QEvent *e) override;
    protected slots:
        virtual void load() = 0;
        virtual void updateIsModified() = 0;
        virtual void apply() = 0;
        virtual void rollback() { emit rollbacked(); }

    };

} //namespace ns_page
