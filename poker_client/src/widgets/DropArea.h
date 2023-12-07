#pragma once

#include <QWidget>
#include <QMimeData>

class DropArea : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QStringList acceptedFormats READ acceptedFormats WRITE setAcceptedFormats)

    QStringList m_acceptedFormats;

public:
    explicit DropArea(QWidget *parent = nullptr);

    const QStringList& acceptedFormats() const;

signals:
    void objectDropped(const QMimeData*);
public slots:

    // QWidget interface
    void setAcceptedFormats(const QStringList& acceptedFormats);
    void addAcceptedFormat(const QString& acceptedFormat);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

