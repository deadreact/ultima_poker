#pragma once

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class ConvertTypeMapperPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)


public:
    ConvertTypeMapperPlugin(QObject *parent = 0);

    virtual bool isContainer() const override;
    virtual bool isInitialized() const override;
    virtual QIcon icon() const override;
    virtual QString domXml() const override;
    virtual QString group() const override;
    virtual QString includeFile() const override;
    virtual QString name() const override;
    virtual QString toolTip() const override;
    virtual QString whatsThis() const override;
    virtual QWidget *createWidget(QWidget *parent) override;
    virtual void initialize(QDesignerFormEditorInterface *core) override;
protected:
    void setInitialized();
private:
    bool m_initialized;
};
