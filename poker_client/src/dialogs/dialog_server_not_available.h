#pragma once

#include <QDialog>

namespace Ui {
class DialogServerNotAvailableUi;
}

struct DialogServerNotAvailable : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogServerNotAvailableUi* m_ui;
public:
    explicit DialogServerNotAvailable(QWidget* parent = nullptr);
    virtual ~DialogServerNotAvailable();
};
