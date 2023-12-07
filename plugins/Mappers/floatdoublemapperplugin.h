#pragma once

#include "converttypemapperplugin.h"

class FloatDoubleMapperPlugin : public ConvertTypeMapperPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    FloatDoubleMapperPlugin(QObject *parent = 0);

    virtual QWidget *createWidget(QWidget *parent) override;
    virtual void initialize(QDesignerFormEditorInterface *core) override;
};
