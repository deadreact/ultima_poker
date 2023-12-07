#pragma once

#include <QWidget>

namespace Ui {
class VipStatusItem;
}
class QAbstractTableModel;

namespace ns_page
{
    class VipStatusItem : public QWidget
    {
        Q_OBJECT
    private:
        Ui::VipStatusItem *m_ui;
        QAbstractTableModel* m_model;
        int m_displayedRow;
    public:
        explicit VipStatusItem(QWidget *parent = nullptr);
        virtual ~VipStatusItem();

        virtual void init();
    public slots:
        void setModel(QAbstractTableModel *model, int displayRow = 0);
        void setCurrentIndex(const QModelIndex& modelIndex);

    protected:
        void displayModelData(int row);
        virtual void changeEvent(QEvent* e) override;
    };

} //namespace ns_page
