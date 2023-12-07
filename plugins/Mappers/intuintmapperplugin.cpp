#include "intuintmapper.h"
#include "intuintmapperplugin.h"

#include <QtPlugin>

IntUIntMapperPlugin::IntUIntMapperPlugin(QObject *parent)
    : ConvertTypeMapperPlugin(parent)
{}

void IntUIntMapperPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (isInitialized())
        return;

    // Add extension registrations, etc. here
    setInitialized();
}

QWidget *IntUIntMapperPlugin::createWidget(QWidget *parent)
{
    return new IntUIntMapper(parent);
}
