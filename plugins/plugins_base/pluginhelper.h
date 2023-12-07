#pragma once

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class CommonPluginInterface : public QDesignerCustomWidgetInterface
{
protected:
    const QMetaObject* m_metaObject;
    const QString m_group;
    bool m_initialized;
public:
    CommonPluginInterface(const QMetaObject* metaObject, const QString group = QString());
    virtual QString name() const override;
    virtual QString group() const override;
    virtual QString toolTip() const override;
    virtual QString whatsThis() const override;
    virtual QString includeFile() const override;
    virtual QIcon icon() const override;
    virtual bool isContainer() const override;
    virtual bool isInitialized() const override;
    virtual void initialize(QDesignerFormEditorInterface * /* core */) override;
    virtual QWidget *createWidget(QWidget *parent) override;
    virtual QString domXml() const override;

    virtual QSize defaultSizeHint() const;
};
