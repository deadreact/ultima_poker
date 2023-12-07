#pragma once

#include <QWidget>

namespace Ui {
class VipStatusDescription;
}
namespace ns_model {
class RakebackHistory;
class vip_system_info;
}

namespace ns_page
{
    class VipStatusDescription : public QWidget
    {
        Q_OBJECT
    private:
        Ui::VipStatusDescription *m_ui;
        QScopedPointer<ns_model::vip_system_info> m_sourceModel;
        QScopedPointer<ns_model::RakebackHistory> m_model;
    public:
        explicit VipStatusDescription(QWidget *parent = nullptr);
        virtual ~VipStatusDescription();

        virtual void init();
    public slots:
        void updateCalendarPage(const QDate& firstDayDate);
        void onLanguageChanged();
    protected:
        virtual void changeEvent(QEvent* e) override;
        void initShadows();
    private:
        void updateFilterBttnTexts();
    };

} //namespace ns_page
