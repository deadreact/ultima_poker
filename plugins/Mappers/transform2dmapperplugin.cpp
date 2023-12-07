#include "transform2dmapper.h"
#include "transform2dmapperplugin.h"

#include <QtPlugin>

Transform2DMapperPlugin::Transform2DMapperPlugin(QObject *parent)
    : ConvertTypeMapperPlugin(parent)
{}

void Transform2DMapperPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (isInitialized())
        return;

    // Add extension registrations, etc. here

    setInitialized();
}
QWidget *Transform2DMapperPlugin::createWidget(QWidget *parent)
{
    return new Transform2DMapper(parent);
}
