#pragma once

#include "converttypemapperplugin.h"

class IntUIntMapperPlugin : public ConvertTypeMapperPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    IntUIntMapperPlugin(QObject *parent = 0);

    virtual QWidget *createWidget(QWidget *parent) override;
    virtual void initialize(QDesignerFormEditorInterface *core) override;
};
