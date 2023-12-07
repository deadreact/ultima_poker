#include "pluginhelper.h"
#include <QWidget>
#include <QMetaMethod>
#include <QDebug>

CommonPluginInterface::CommonPluginInterface(const QMetaObject* metaObject, const QString group)
    : m_metaObject(metaObject)
    , m_group(group)
    , m_initialized(false)
{
    qDebug() << "m_metaObject" << m_metaObject->className();
}

QString CommonPluginInterface::name() const
{
    return m_metaObject->className();
}

QString CommonPluginInterface::group() const {
    return m_group;
}

QString CommonPluginInterface::toolTip() const {
    return QString();
}

QString CommonPluginInterface::whatsThis() const {
    return QString();
}

QString CommonPluginInterface::includeFile() const {
    return QLatin1String("plugins.h");
}

QIcon CommonPluginInterface::icon() const
{
    QIcon ico(QString(":/icons/%0.png").arg(name().toLower()));
    return !ico.isNull() ? ico : QIcon(QString(":/icons/%0.png").arg(group().toLower()));
}

bool CommonPluginInterface::isContainer() const {
    return false;
}

bool CommonPluginInterface::isInitialized() const
{
    return m_initialized;
}


QWidget *CommonPluginInterface::createWidget(QWidget *parent)
{
    return qobject_cast<QWidget*>(m_metaObject->newInstance(Q_ARG(QWidget*, parent)));
}

void CommonPluginInterface::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

QString CommonPluginInterface::domXml() const
{
    QString geometryStr = !defaultSizeHint().isValid()
                          ? ""
                          : QString("  <property name=\"geometry\">\n"
                                    "   <rect>\n"
                                    "    <x>0</x>\n"
                                    "    <y>0</y>\n"
                                    "    <width>%2</width>\n"
                                    "    <height>%3</height>\n"
                                    "   </rect>\n"
                                    "  </property>\n").arg(defaultSizeHint().width()).arg(defaultSizeHint().height());
    return QString("<widget class=\"%0\" name=\"%1\">\n"
                   "%2</widget>\n").arg(name()).arg(name().toLower()).arg(geometryStr);
}

QSize CommonPluginInterface::defaultSizeHint() const {
    return QSize();
}
