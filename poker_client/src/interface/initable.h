#pragma once

#include <QDialog>
//#include <tools/validated.h>

class DebugPainter;

class Initable /*: public Validated<Initable>*/
{
public:
    virtual void init()=0;
    virtual ~Initable();
    inline bool isInitialized() const { return m_isInitialized; }
protected:
    virtual void onInitialized() = 0;
    bool m_isInitialized = false;
};

class Linkable
{
protected:
    Linkable* m_next;
    Linkable* m_prev;

    static void createLinkedConnection(Linkable* first, Linkable* second)
    {
        first->setNext(second);
        second->setPrev(first);
    }

public:
    virtual ~Linkable();
    Linkable(Linkable* prev = nullptr)
        : m_next(nullptr)
        , m_prev(prev)
    {
        if (prev) {
            prev->setNext(this);
        }
    }
    virtual void setPrev(Linkable* prev);
    virtual void setNext(Linkable* next);
    virtual bool hasPrev() const;
    virtual bool hasNext() const;
    virtual Linkable* getNext() const;
    virtual Linkable* getPrev() const;
    const Linkable *getFirst() const;
    Linkable *getFirst();

    QWidget* prevWidget() const;
    QWidget* nextWidget() const;

    void printChain(QString& text) const {
        if (Linkable* prev = getPrev()) {
            prev->printChain(text);

        } else {
            text = "ROOT ";
        }
        if (const QObject* obj = dynamic_cast<const QObject*>(this)) {
            text += QString("[") + obj->objectName() + "]-->";
        } else {
            text += "[?]-->";
        }
    }

    virtual bool stepForward()  = 0;
    virtual bool stepBackward() = 0;
};

class InitableDialog : public QDialog, public Initable, public Linkable
{
    Q_OBJECT

    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY initialized)
public:
    explicit InitableDialog(QWidget* parent=0, Qt::WindowFlags flags = 0);
    InitableDialog(InitableDialog* linkedFrom, Qt::WindowFlags flags = 0);
    virtual ~InitableDialog();
signals:
    void initialized();
    void canceled();
protected:
    virtual void onInitialized() override final;

    // Linkable interface
public slots:
    virtual void cancel();
    virtual bool stepForward() override;
    virtual bool stepBackward() override;
};

class InitableWidget : public QWidget, public Initable
{
    Q_OBJECT

    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY initialized)
public:
    explicit InitableWidget(QWidget* parent=0, Qt::WindowFlags flags = 0);
    virtual ~InitableWidget();
signals:
    void initialized();
protected:
    virtual void onInitialized() override final;

};
