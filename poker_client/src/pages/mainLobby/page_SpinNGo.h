#pragma once
#include <QFrame>

namespace Ui {
class SpinNGo;
}

namespace ns_page
{

    class SpinNGo : public QFrame
    {
        Q_OBJECT
    private:
        Ui::SpinNGo *m_ui;
    public:
        explicit SpinNGo(QWidget *parent = nullptr);
        virtual ~SpinNGo();

        virtual void init();
    };

} //namespace ns_page
