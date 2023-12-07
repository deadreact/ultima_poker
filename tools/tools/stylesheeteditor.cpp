#include "stylesheeteditor.hpp"
#include "ui_stylesheeteditor.h"
#include <QDebug>
#include <QTimer>
#include <QDir>
//#include <PokerApplication.h>

namespace ns_tools
{
    int StyleSheetEditor::sm_selfCounter = 0;

    StyleSheetEditor::StyleSheetEditor(QWidget *parent)
        : QDialog(parent)
        , m_ui(new Ui::StyleSheetEditor)
        , m_proxy(nullptr)
        , m_target(nullptr)
        , m_timer(nullptr)
        , m_updated(false)
    {
        m_ui->setupUi(this);
        sm_selfCounter++;
    }

    StyleSheetEditor::~StyleSheetEditor() { sm_selfCounter--; delete m_ui; }

    void StyleSheetEditor::init()
    {
        if (m_target != nullptr)
        {
            if (m_proxy == nullptr)
            {
                static const char* path = "../resources/genereted";
                QDir::current().mkpath(path);
                m_proxy = new QFile(QString("%0/style_%1.qss").arg(path).arg(m_target->objectName()), this);
            }
            setText(m_target->styleSheet());
        }
        if (m_timer == nullptr)
        {
            m_timer = new QTimer(this);
            connect(m_ui->textEditor, SIGNAL(textChanged()), this, SLOT(writeToProxy()));
            connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTargetStyle()));

            m_ui->textEditor->setUndoRedoEnabled(true);
            m_ui->textEditor->setReadOnly(false);
        }
        m_ui->status->hide();
        m_ui->status_2->hide();
        m_ui->bttn_setTarget->hide();

        open();
        m_updated = true;

    }

    QFile *StyleSheetEditor::proxy() const
    {
        return m_proxy;
    }

    QWidget *StyleSheetEditor::target() const
    {
        return m_target;
    }

    void StyleSheetEditor::writeToProxy()
    {
        m_updated = true;
        if (m_proxy)
        {
            if ((!m_proxy->isOpen() && m_proxy->open(QFile::WriteOnly))/* || m_proxy->waitForReadyRead(2000)*/)
            {
                QString str = m_ui->textEditor->toPlainText();
                m_proxy->write(str.toLocal8Bit());
                m_proxy->close();
                emit proxyChanged(m_proxy);
                m_timer->stop();
            }
        }
        m_timer->start(100);
    }

    void StyleSheetEditor::updateTargetStyle()
    {
        if (!m_updated) {
            return;
        }
        m_target->setStyleSheet(m_ui->textEditor->toPlainText());
        m_updated = false;
    }

    void StyleSheetEditor::updateTargetStyle(QFile *proxy)
    {
        if (!m_updated) {
            return;
        }
        if (m_target && proxy)
        {
            if (!proxy->isOpen() && proxy->open(QIODevice::ReadOnly))
            {
                m_target->setStyleSheet(QString::fromLocal8Bit(proxy->readAll()));
                proxy->close();
            }
        }
        m_updated = false;
    }

    void StyleSheetEditor::setTarget(QWidget *widget)
    {
        if (m_target != widget) {
            m_target = widget;
            init();
        }
    }

    void StyleSheetEditor::setProxy(QFile *file)
    {
        m_proxy = file;
    }

    void StyleSheetEditor::setText(const QString &text)
    {
        m_ui->textEditor->setText(text);
    }

    void EditStyleSheet(QWidget *widget)
    {
        if (!widget || StyleSheetEditor::howMany() > 0) return;
        StyleSheetEditor* editor = new StyleSheetEditor;

        editor->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);
        editor->setAttribute(Qt::WA_DeleteOnClose);
        editor->setTarget(widget);
        editor->setText(widget->styleSheet());
        editor->init();
        editor->setWindowOpacity(0.9);
    }

} // namespace tools

void ns_tools::StyleSheetEditor::on_bttn_search_clicked()
{
    QString name = m_ui->lineEdit->text();
    if (name.isEmpty()) {
        return;
    }
    QRegularExpression re(name);

    m_ui->listWidget->clear();

    QListWidgetItem* item = nullptr;

    for (QWidget* w: QApplication::allWidgets()) {
        if (re.match(w->objectName()).hasMatch()) {
            QString itemText = QString("%0('%1')").arg(w->metaObject()->className()).arg(w->objectName());
            if (!w->isVisible()) {
                itemText += " [hidden]";
            }
            item = new QListWidgetItem(itemText, m_ui->listWidget);
            item->setData(Qt::UserRole, QVariant::fromValue(w));

            QColor textColor = (w->isVisible()) ? Qt::black : Qt::red;
            textColor.setAlpha(w->isEnabled() ? 255 : 100);

            item->setForeground(textColor);
        }
    }
    m_ui->status_2->setVisible(m_ui->listWidget->count() == 0);
}

void ns_tools::StyleSheetEditor::on_bttn_setTarget_clicked()
{
    QVariant w = m_ui->listWidget->currentItem()->data(Qt::UserRole);
    if (QWidget* wgt = qvariant_cast<QWidget*>(w)) {
        setTarget(wgt);
    }
}


void ns_tools::StyleSheetEditor::on_listWidget_currentItemChanged(QListWidgetItem *item, QListWidgetItem */*previous*/)
{
    if (!item) {
        return;
    }

    QString tmp = QString("<span style=\" font-size:10px; color:#800080;\">%0</span>(<span style=\" font-size:10px; color:#008000;\">'%1'</span>)");
    if (QWidget *w = qvariant_cast<QWidget*>(item->data(Qt::UserRole)))
    {
        QString fullStr = tmp.arg(w->metaObject()->className()).arg(w->objectName());

        for (QWidget *wgt = w->parentWidget(); wgt != nullptr; wgt = wgt->parentWidget()) {
            fullStr = tmp.arg(wgt->metaObject()->className()).arg(wgt->objectName()) + "->" + fullStr;
        }
        fullStr = QString("<html><head/><body><p>*->") + fullStr + "</p></body></html>";

        m_ui->detail->setText(fullStr);
        m_ui->detail->setToolTip(fullStr.replace("font-size:10px", "font-size:14px"));
    }
}
