#pragma once
#include "page_LobbyBase.h"

namespace Ui {
class LobbyCommonInfo;
}
namespace ns_model {
class LobbyCommonInfo;
}

namespace ns_page
{
    class LobbyCommonInfo : public LobbyBase
    {
        Q_OBJECT
    private:
        Ui::LobbyCommonInfo *m_ui;
        QScopedPointer<ns_model::LobbyCommonInfo> m_model;
    public:
        explicit LobbyCommonInfo(QWidget *parent = nullptr);
        virtual ~LobbyCommonInfo();


        virtual QAbstractItemView *view() const override;
        virtual QAbstractItemModel* model() const override;
        virtual QModelIndex rootIndex() const override;
    public slots:
        void popupQualifiers(bool show);
    protected:
        virtual void internalInit() override;

    };

} //namespace ns_page
