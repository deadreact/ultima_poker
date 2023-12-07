#include "transform3dmapper.h"
#include "transform3dmapperplugin.h"

#include <QtUiPlugin>

Transform3DMapperPlugin::Transform3DMapperPlugin(QObject *parent)
    : ConvertTypeMapperPlugin(parent)
{}

void Transform3DMapperPlugin::initialize(QDesignerFormEditorInterface */*core*/)
{
    if (isInitialized())
        return;

    // Add extension registrations, etc. here

    setInitialized();
}
QWidget *Transform3DMapperPlugin::createWidget(QWidget *parent)
{
    return new Transform3DMapper(parent);
}
