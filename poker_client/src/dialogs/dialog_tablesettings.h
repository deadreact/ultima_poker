#pragma once
#include <QDialog>
#include <interface/initable.h>
#include <QTableView>
#include <models/model_TableSettings.h>

//class TableView: public QTableView {
//    Q_OBJECT
//public:
//    TableView(QWidget* parent = nullptr);
//signals:
//    void currentIndexChanged(const QModelIndex &current, const QModelIndex &previous);
//protected slots:
//    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
//};

namespace Ui {
class TableSettings;
}
class QComboBox;
class GameTableSettingsData;

namespace ns_dialog
{
class TableSettings : public InitableDialog
{
    Q_OBJECT

private:
    Ui::TableSettings *m_ui;
    quintptr m_sharedDataId;
    QAction* m_actionOnSettingsApplied;
//    GameTableSettingsData& m_data;
signals:
    void settingsApplied();
public:
    explicit TableSettings(QWidget *parent = nullptr);
    virtual ~TableSettings();

    virtual void init() override;
public slots:
    virtual void accept() override;
protected:
//    virtual void showEvent(QShowEvent* e) override;
    virtual void actionEvent(QActionEvent *e) override;
};

} //namespace ns_dialog
