#pragma once

#include "tools_global.h"
#include <QTextBrowser>
#include <QFile>
#include <QDialog>

namespace Ui
{
    class StyleSheetEditor;
}
class QListWidgetItem;

namespace ns_tools
{
    void FUNCTION_TOOLS_DLL EditStyleSheet(QWidget* widget);

// ------ StyleSheetEditor ------
    class TOOLS_DLL StyleSheetEditor : public QDialog
    {
        Q_OBJECT

        static int sm_selfCounter;
    private:
        Q_PROPERTY(QFile* proxy READ proxy WRITE setProxy NOTIFY proxyChanged)
        Q_PROPERTY(QWidget* target READ target WRITE setTarget/* NOTIFY targetChanged*/)

        Ui::StyleSheetEditor* m_ui;
        QFile* m_proxy;
        QWidget* m_target;
        QTimer* m_timer;
        bool m_updated;
    public:
        StyleSheetEditor(QWidget* parent = nullptr);
        virtual ~StyleSheetEditor();


        QFile* proxy() const;
        QWidget* target() const;

        static int howMany() { return sm_selfCounter; }
    signals:
        void proxyChanged(QFile* proxy);
//        void targetChanged(QWidget* target);
    private slots:
        void writeToProxy();
        void updateTargetStyle();
        void updateTargetStyle(QFile* proxy);
        void on_bttn_search_clicked();

        void on_bttn_setTarget_clicked();        

        void on_listWidget_currentItemChanged(QListWidgetItem *item, QListWidgetItem *);

    public slots:
        virtual void init();
        void setTarget(QWidget* widget);
        void setProxy(QFile* file);
        void setText(const QString& text);

    };

} // namespace ns_tools
