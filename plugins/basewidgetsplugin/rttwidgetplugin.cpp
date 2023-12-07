#include "rttwidget.h"
#include "rttwidgetplugin.h"

#include <QtPlugin>

RTTWidgetPlugin::RTTWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void RTTWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool RTTWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *RTTWidgetPlugin::createWidget(QWidget *parent)
{
    return new RTTWidget(parent);
}

QString RTTWidgetPlugin::name() const
{
    return QLatin1String("RTTWidget");
}

QString RTTWidgetPlugin::group() const
{
    return QLatin1String("CustomWidgets");
}

QIcon RTTWidgetPlugin::icon() const
{
    return QIcon(QLatin1String(":/button.png"));
}

QString RTTWidgetPlugin::toolTip() const
{
    return QLatin1String("");
}

QString RTTWidgetPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool RTTWidgetPlugin::isContainer() const
{
    return true;
}

QString RTTWidgetPlugin::domXml() const
{
    return QLatin1String("<widget class=\"RTTWidget\" name=\"bttn_framed\">\n"
                         "  <property name=\"geometry\">\n"
                         "   <rect>\n"
                         "    <x>0</x>\n"
                         "    <y>0</y>\n"
                         "    <width>60</width>\n"
                         "    <height>20</height>\n"
                         "   </rect>\n"
                         "  </property>\n"
                         "</widget>\n");
}

QString RTTWidgetPlugin::includeFile() const
{
    return QLatin1String("plugins.h");
}

