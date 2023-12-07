#include "converttypemapper.h"
#include "converttypemapperplugin.h"

#include <QtPlugin>

ConvertTypeMapperPlugin::ConvertTypeMapperPlugin(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{}

void ConvertTypeMapperPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (isInitialized())
        return;

    // Add extension registrations, etc. here

    setInitialized();
}

bool ConvertTypeMapperPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ConvertTypeMapperPlugin::createWidget(QWidget *parent)
{
    return new ConvertTypeMapper(parent);
}

QString ConvertTypeMapperPlugin::name() const
{
    return QString(metaObject()->className()).replace("Plugin", "");
}

QString ConvertTypeMapperPlugin::group() const
{
    return QLatin1String("Mappers");
}

QIcon ConvertTypeMapperPlugin::icon() const
{
    return QIcon(":/mapper.png");
}

QString ConvertTypeMapperPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ConvertTypeMapperPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ConvertTypeMapperPlugin::isContainer() const
{
    return false;
}

QString ConvertTypeMapperPlugin::domXml() const
{
    return QString("<widget class=\"%0\" name=\"mapper\">\n</widget>\n").arg(name());
}

QString ConvertTypeMapperPlugin::includeFile() const
{
    return QString("%0.h").arg(name().toLower());
}

void ConvertTypeMapperPlugin::setInitialized()
{
    m_initialized = true;
}
