//#ifdef NOT_A_PLUGIN
//    #include <precompiled.h>
//#endif // NOT_A_PLUGIN
#include "rttwidget.h"
#include "ui_rttwidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QUiLoader>
#include <QMessageBox>
//#include <QFormBuilder>

RTTWidget::RTTWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::RTTWidget)
    , m_uiLoader(new QUiLoader(this))
    , m_sourceWidget(nullptr)
    , m_formUrl("")
{
    m_ui->setupUi(this);
    connect(this, &RTTWidget::formUrlChanged, this, &RTTWidget::loadCurrentForm);
}

RTTWidget::~RTTWidget()
{
    delete m_uiLoader;
}

const QUrl &RTTWidget::formUrl() const
{
    return m_formUrl;
}

QWidget *RTTWidget::sourceWidget() const
{
    return m_ui->target;
}

void RTTWidget::init()
{

}

void RTTWidget::updateView()
{

}

void RTTWidget::clearUrl()
{
    m_formUrl.clear();
    if (m_sourceWidget)
    {
        delete m_sourceWidget;
        m_sourceWidget = nullptr;
    }
}

void RTTWidget::setFormUrl(const QUrl &formUrl)
{
    if (m_formUrl == formUrl)
        return;

    if ( !formUrl.toLocalFile().endsWith(".ui"))
    {
        QMessageBox::warning(this, "Wrong file format", "Selected file has wrong type, loading aborted. Please, select another file which ends with '.ui'.");
        return;
    }

    m_formUrl = formUrl;
    emit formUrlChanged(formUrl);
}

void RTTWidget::loadCurrentForm()
{
    QString path = m_formUrl.toLocalFile();
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        if (m_sourceWidget) {
            delete m_sourceWidget;
        }
        if (useLoader())
            m_sourceWidget = loadCurrentFormByLoader(&file);
        else
            m_sourceWidget = loadCurrentFormByBuilder(&file);
        m_sourceWidget->setWindowFlags(Qt::Widget);
        layout()->addWidget(m_sourceWidget);
        //m_ui->target = qobject_cast<ns_page::MainScreen*>(w);
        file.close();
    }

}

QWidget *RTTWidget::loadCurrentFormByLoader(QIODevice *dev)
{
    return m_uiLoader->load(dev, this);
}

QWidget *RTTWidget::loadCurrentFormByBuilder(QIODevice* dev)
{
//    QFormBuilder builder;
//    return builder.load(dev, this);
    return loadCurrentFormByLoader(dev);
}
