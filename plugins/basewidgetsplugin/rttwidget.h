#pragma once

#include <QBasicTimer>
#include <QLabel>
#include <QUrl>
#include <basewidgets/util.h>

class QUiLoader;

namespace Ui {
class RTTWidget;
}

class RTTWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QUrl formUrl READ formUrl WRITE setFormUrl NOTIFY formUrlChanged RESET clearUrl)
    Q_PROPERTY(bool useLoader READ useLoader WRITE setUseLoader NOTIFY useLoaderChanged)
private:
    Ui::RTTWidget* m_ui;
    QUiLoader* m_uiLoader;
    QWidget* m_sourceWidget;
    QUrl m_formUrl;
    bool m_useLoader = true;

public:
    Q_INVOKABLE RTTWidget(QWidget *parent = 0);
    ~RTTWidget();

    const QUrl& formUrl() const;
    QWidget* sourceWidget() const;
    virtual void init();
    bool useLoader() const
    {
        return m_useLoader;
    }

public slots:
    void updateView();
    void clearUrl();
    void setFormUrl(const QUrl& formUrl);
    void setUseLoader(bool useLoader)
    {
        if (m_useLoader == useLoader)
            return;

        m_useLoader = useLoader;
        emit useLoaderChanged(useLoader);
    }

private slots:
    void loadCurrentForm();
    QWidget* loadCurrentFormByLoader(QIODevice *dev);
    QWidget* loadCurrentFormByBuilder(QIODevice *dev);
signals:
    void formUrlChanged(const QUrl& formUrl);
    void useLoaderChanged(bool useLoader);
};

