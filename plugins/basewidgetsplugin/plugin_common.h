#pragma once

#include "../plugins_base/pluginhelper.h"
#include <basewidgets/framedentity.h>
#include <basewidgets/framedbutton.h>
#include <basewidgets/textwidget.h>
#include <basewidgets/infomessage.h>
#include <basewidgets/validatedLineEdit.h>
#include <basewidgets/timezonebox.h>
#include <basewidgets/easingCurveComboBox.h>
#include <basewidgets/clockwidget.h>
#include <basewidgets/svgwidget.h>
#include <animation/rotatedwidget.h>

static const char* groupName = "Common widgets";

template <class T>
class CommonPluginContainerInterface : public CommonPluginInterface {
public:
    CommonPluginContainerInterface()
        : CommonPluginInterface(&T::staticMetaObject, groupName)
    {}
    virtual bool isContainer() const override { return true; }
};

class EntityWidgetPlugin : public QObject, public CommonPluginContainerInterface<EntityWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    EntityWidgetPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;

    virtual void initialize(QDesignerFormEditorInterface *core) override;
    virtual QWidget *createWidget(QWidget *parent) override;
};

class FramedEntityPlugin : public QObject, public CommonPluginContainerInterface<FramedEntity>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    FramedEntityPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
};

class FramedButtonPlugin : public QObject, public CommonPluginContainerInterface<FramedButton>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    FramedButtonPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
};


class TextWidgetPlugin : public QObject, public CommonPluginContainerInterface<TextWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    TextWidgetPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
    virtual bool isContainer() const override { return false; }
};

class InfoMessagePlugin : public QObject, public CommonPluginContainerInterface<InfoMessage>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    InfoMessagePlugin(QObject *parent = 0);
    virtual QSize defaultSizeHint() const override;
    virtual bool isContainer() const override { return false; }
};

class ValidatedLineEditPlugin : public QObject, public CommonPluginContainerInterface<ValidatedLineEdit>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    ValidatedLineEditPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
    virtual QIcon icon() const override;
    virtual QString domXml() const override;
};

class TimeZoneBoxPlugin : public QObject, public CommonPluginContainerInterface<TimeZoneBox>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    TimeZoneBoxPlugin(QObject *parent = 0);
    virtual ~TimeZoneBoxPlugin() {}
    virtual QSize defaultSizeHint() const override;
public:
    virtual QWidget *createWidget(QWidget *parent) override;
};

class EasingCurveComboBoxPlugin : public QObject, public CommonPluginContainerInterface<EasingCurveComboBox>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    EasingCurveComboBoxPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
    virtual bool isContainer() const override { return false; }
};

class ClockWidgetPlugin : public QObject, public CommonPluginContainerInterface<ClockWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    ClockWidgetPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
};

class SvgWidgetPlugin : public QObject, public CommonPluginContainerInterface<SvgWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    SvgWidgetPlugin(QObject *parent = nullptr);
    virtual QIcon icon() const override;
};

class RotatedWidgetPlugin : public QObject, public CommonPluginContainerInterface<RotatedWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    RotatedWidgetPlugin(QObject *parent = nullptr);
    virtual QSize defaultSizeHint() const override;
};
