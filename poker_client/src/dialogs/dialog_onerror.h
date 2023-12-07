#pragma once
#include <QDialog>

namespace Ui {
class OnError;
}

class OnError : public QDialog
{
    Q_OBJECT
private:
    Ui::OnError *m_ui;
    int m_direction;
    int m_timerID;
    static const int sm_range     = 800;
    static const int sm_minOffset = sm_range/33;
public:
    explicit OnError(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::Dialog | Qt::WindowStaysOnTopHint);
    virtual ~OnError();

private slots:
    void quitApp();
protected:
    virtual void timerEvent(QTimerEvent *e) override;
public slots:
    virtual void accept() override;
    virtual void reject() override;
};
