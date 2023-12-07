#include "floatdoublemapper.h"
#include "floatdoublemapperplugin.h"

#include <QtPlugin>

FloatDoubleMapperPlugin::FloatDoubleMapperPlugin(QObject *parent)
    : ConvertTypeMapperPlugin(parent)
{}

void FloatDoubleMapperPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (isInitialized())
        return;

    // Add extension registrations, etc. here
    setInitialized();
}

QWidget *FloatDoubleMapperPlugin::createWidget(QWidget *parent)
{
    return new FloatDoubleMapper(parent);
}
