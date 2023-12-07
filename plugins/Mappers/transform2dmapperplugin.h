#pragma once

#include "converttypemapperplugin.h"

class Transform2DMapperPlugin : public ConvertTypeMapperPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    Transform2DMapperPlugin(QObject *parent = 0);

    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);
};
