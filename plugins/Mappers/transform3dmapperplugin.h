#pragma once

#include "converttypemapperplugin.h"

class Transform3DMapperPlugin : public ConvertTypeMapperPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    Transform3DMapperPlugin(QObject *parent = 0);

    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);
};
