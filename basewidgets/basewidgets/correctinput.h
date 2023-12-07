#pragma once

#include "basewidgets_global.h"
#include "types.h"
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <functional>

// ----------------------------------------------------------------------------
class BASEWIDGETS_DLL CorrectInput
{
private:
protected:
    E::ForceCorrectState m_forceCorrect;
    CorrectInput(): m_forceCorrect(E::ForceCorrectState::None) {}

public:
    virtual ~CorrectInput() {}
    virtual bool isCorrect() const = 0;
    inline bool IsIncorrect() const { return !isCorrect(); }
    void forceSetCorrect(E::ForceCorrectState state) { m_forceCorrect = state; }
};
// ----------------------------------------------------------------------------


#define DECLARE_CORRECT_INPUT_CLASS(classname, qtUiClassname)       \
    class BASEWIDGETS_DLL classname : public qtUiClassname, public CorrectInput     \
    {                                                               \
        Q_OBJECT                                                    \
        Q_PROPERTY(E::ForceCorrectState forceCorrect MEMBER m_forceCorrect WRITE forceSetCorrect) \
        Q_PROPERTY(bool isCorrect READ isCorrect STORED false)      \
    Q_SIGNALS:                                                      \
        Q_SIGNAL void correctStateUpdated(bool);                    \
        Q_SIGNAL void focusToggled(bool);                           \
        Q_SIGNAL void focused();                                    \
        Q_SIGNAL void unfocused();                                  \
    public:                                                         \
        Q_INVOKABLE classname(QWidget* parent = 0);                             \
        virtual bool isCorrect() const override;                    \
    public slots:                                                   \
        virtual void updateCorrectState() {                         \
            emit correctStateUpdated(isCorrect());                  \
        }                                                           \
    virtual void focusInEvent(QFocusEvent *e) override;             \
    virtual void focusOutEvent(QFocusEvent *e) override;            \
    }

DECLARE_CORRECT_INPUT_CLASS(EmailLineEdit,          QLineEdit);
DECLARE_CORRECT_INPUT_CLASS(UsernameLineEdit,       QLineEdit);
DECLARE_CORRECT_INPUT_CLASS(NotEmptyLineEdit,       QLineEdit);
DECLARE_CORRECT_INPUT_CLASS(PostIndexLineEdit,      QLineEdit);
DECLARE_CORRECT_INPUT_CLASS(CheckedCheckBox,        QCheckBox);
DECLARE_CORRECT_INPUT_CLASS(CheckedComboBox,        QComboBox);
DECLARE_CORRECT_INPUT_CLASS(NotEmptyPlainTextEdit,  QPlainTextEdit);

//class EmailLineEdit : public QLineEdit, public CorrectInput {
//    DEFINE_CORRECT_INPUT_CLASS(EmailLineEdit)
//signals:
//    void focused();
//    void unfocused();
//    void correctStateUpdated(bool);
//};


class BASEWIDGETS_DLL PasswordLineEdit : public QLineEdit, public CorrectInput
{
    Q_OBJECT
    Q_PROPERTY(E::ForceCorrectState forceCorrect MEMBER m_forceCorrect WRITE forceSetCorrect)
    Q_PROPERTY(bool isCorrect READ isCorrect STORED false)
public:
    Q_INVOKABLE PasswordLineEdit(QWidget* parent = 0);
    virtual bool isCorrect() const override;
signals:
    void correctStateUpdated(bool);
    void passwordDifficultyChanged(qreal v) const;
    void focusToggled(bool);
    void focused();
    void unfocused();
private slots:
    void analyzePassword(const QString&);
public slots:
    void onRequested();
    virtual void updateCorrectState() {
        emit correctStateUpdated(isCorrect());
    }
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
};

class BASEWIDGETS_DLL ConfirmationLineEdit : public QLineEdit, public CorrectInput
{
    Q_OBJECT
    Q_PROPERTY(E::ForceCorrectState forceCorrect MEMBER m_forceCorrect WRITE forceSetCorrect)
    Q_PROPERTY(bool isCorrect READ isCorrect STORED false)
private:
    const PasswordLineEdit* m_passLineEdit;
public:
    Q_INVOKABLE ConfirmationLineEdit(QWidget* parent=nullptr);
    virtual bool isCorrect() const override;
signals:
    void correctStateUpdated(bool);
    void requestPasswordLineEdit() const;
    void focusToggled(bool);
    void focused();
    void unfocused();
public slots:
    void setPasswordLineEdit(const PasswordLineEdit* pass);
    virtual void updateCorrectState() {
        emit correctStateUpdated(isCorrect());
    }
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
};

class QSortFilterProxyModel;

class BASEWIDGETS_DLL CountryComboBox : public CheckedComboBox
{
    Q_OBJECT
    Q_PROPERTY(E::ForceCorrectState forceCorrect MEMBER m_forceCorrect WRITE forceSetCorrect)
    Q_PROPERTY(bool isCorrect READ isCorrect STORED false)
    Q_PROPERTY(QString filterRegExp READ filterRegExp WRITE setFilterRegExp NOTIFY filterRegExpChanged)
    Q_PROPERTY(QString excludeRegExp READ excludeRegExp WRITE setExcludeRegExp NOTIFY excludeRegExpChanged)
    Q_PROPERTY(Flags displayFlag READ displayFlag WRITE setDisplayFlag)
    Q_FLAGS(Flags)
public:
    enum Display
    {
        Nothing   = 0,
        Id        = 0x01,
        Icon      = 0x02,
        Country   = 0x04,
        PhoneCode = 0x08
    };
    Q_FLAG(Display)
    Q_DECLARE_FLAGS(Flags, Display)
private:
    Flags m_displayFlag;
    QString m_filterRegExp;
    QString m_excludeRegExp;
    QSortFilterProxyModel* m_proxyModel;

public:
    Q_INVOKABLE CountryComboBox(QWidget* parent=0);

    Flags displayFlag() const { return m_displayFlag; }
    void setDisplayFlag(Flags f);
    QString filterRegExp() const
    {
        return m_filterRegExp;
    }

    QString excludeRegExp() const
    {
        return m_excludeRegExp;
    }

public slots:
    void setFilterRegExp(QString filterRegExp)
    {
        if (m_filterRegExp == filterRegExp)
            return;

        m_filterRegExp = filterRegExp;
        emit filterRegExpChanged(filterRegExp);
    }

    void setExcludeRegExp(QString excludeRegExp)
    {
        if (m_excludeRegExp == excludeRegExp)
            return;

        m_excludeRegExp = excludeRegExp;
        emit excludeRegExpChanged(excludeRegExp);
    }

signals:
    void currentCountryInfoChanged(const CountryInfo&);
    void focusToggled(bool);
    void focused();
    void unfocused();
    void filterRegExpChanged(QString filterRegExp);
    void excludeRegExpChanged(QString excludeRegExp);

private slots:
    void onCurrentIndexChanged(int index);
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
};

class BASEWIDGETS_DLL PhoneNumberLineEdit : public QLineEdit, public CorrectInput
{
    Q_OBJECT
    Q_PROPERTY(E::ForceCorrectState forceCorrect MEMBER m_forceCorrect WRITE forceSetCorrect)
    Q_PROPERTY(bool isCorrect READ isCorrect STORED false)

    Q_PROPERTY(QString inputMaskRegExp READ inputMaskRegExp WRITE setInputMaskRegExp NOTIFY inputMaskRegExpChanged)
    Q_PROPERTY(QString correctValueRegExp READ correctValueRegExp WRITE setCorrectValueRegExp NOTIFY correctValueRegExpChanged)
private:
    QString m_lastText;
    CountryInfo m_info;
    QString m_inputMaskRegExp;
    QString m_correctValueRegExp;

public:
    Q_INVOKABLE PhoneNumberLineEdit(QWidget* parent=0);
    virtual bool isCorrect() const override;

    QString inputMaskRegExp() const;
    QString correctValueRegExp() const;

public slots:
//    void setLocalNumber(gbp_u64 n);
    void setCodeFromCountryInfo(const CountryInfo& info);
    void updateCorrectState();
    void setInputMaskRegExp(const QString &inputMaskRegExp);
    void setCorrectValueRegExp(const QString& correctValueRegExp);

signals:
    void correctStateUpdated(bool);
    void focusToggled(bool);
    void focused();
    void unfocused();
    void inputMaskRegExpChanged(QString inputMaskRegExp);

    void correctValueRegExpChanged(QString correctValueRegExp);

protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CountryComboBox::Flags)
