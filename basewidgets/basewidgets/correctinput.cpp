 //#ifdef NOT_A_PLUGIN
//    #include <precompiled.h>
//#endif // NOT_A_PLUGIN
#include "correctinput.h"
#include "util.h"

#include <QMetaMethod>
#include <QFileInfo>
#include <QPainter>
#include <QListView>
#include <qlabel.h>
#include <qsortfilterproxymodel.h>
#include <QStyledItemDelegate>

#include "country_combobox.inl"
#include "countryinfomodel.h"

#define IMPL_FOCUS_EVENTS(classname, super)         \
    void classname::focusInEvent(QFocusEvent *e) {  \
        super::focusInEvent(e);                     \
        emit focusToggled(true);                    \
        emit focused();                             \
    }                                               \
    void classname::focusOutEvent(QFocusEvent *e) { \
        super::focusOutEvent(e);                    \
        emit focusToggled(false);                   \
        emit unfocused();                           \
    }

namespace
{
//    int codelen(const QString& code)
//    {
//        int counter = 0;
//        for (int i=0; i < code.size(); ++i)
//        {
//            if (code[i] <= '9' && code[i] >= '0') ++counter;
//        }
//        return counter;
//    }
}


EmailLineEdit::EmailLineEdit(QWidget* parent): QLineEdit(parent) {}
bool EmailLineEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);
    static const QRegularExpression re("(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$)");
    return re.match(text()).hasMatch();
}
UsernameLineEdit::UsernameLineEdit(QWidget* parent): QLineEdit(parent) {}
bool UsernameLineEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);

    static const QRegularExpression re("(^[a-zA-Z][0-9a-zA-Z_\\-]+$)");
    return text().size() <= 12 && re.match(text()).hasMatch();
}

NotEmptyLineEdit::NotEmptyLineEdit(QWidget* parent): QLineEdit(parent) {}
bool NotEmptyLineEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);

    if (QString("NotEmptyLineEdit") == metaObject()->className()) {
        static const QRegularExpression re("[A-Za-z\\'\\`\\-]+");
        return text().size() <= 30 && re.match(text()).hasMatch();
    }

    return !text().isEmpty();
}

PasswordLineEdit::PasswordLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    setEchoMode(EchoMode::Password);
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(analyzePassword(QString)));
}
bool PasswordLineEdit::isCorrect() const
{
    static int last_result(0);
    int result = ns_util::getPasswordWeight(text().toStdString());

    if (result != last_result)
    {
        last_result = result;
        emit passwordDifficultyChanged(qreal(result)/135.0);
        return result > 50;
    }
    return text().size() >= 6;
}

void PasswordLineEdit::analyzePassword(const QString &/*pass*/)
{
    isCorrect();
}

void PasswordLineEdit::onRequested()
{
    if (ConfirmationLineEdit* sender_casted = qobject_cast<ConfirmationLineEdit*>(sender()))
        sender_casted->setPasswordLineEdit(this);
}

PostIndexLineEdit::PostIndexLineEdit(QWidget* parent): QLineEdit(parent) {}
bool PostIndexLineEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);
    static const QRegularExpression re("(^[A-Za-z0-9\\'\\`\\-]+$)");
    return text().size() <= 300 && re.match(text()).hasMatch();
}

PhoneNumberLineEdit::PhoneNumberLineEdit(QWidget* parent)
    : QLineEdit(parent)
    , m_lastText("")
    , m_info()
    , m_inputMaskRegExp("(^.*$)")
    , m_correctValueRegExp(".")
{
//    connect(this, &PhoneNumberLineEdit::textEdited, this, &PhoneNumberLineEdit::updatePhoneNumber);
//    m_internalBox->setDisplayFlag(CountryComboBox::Icon);

    connect(this, &PhoneNumberLineEdit::textEdited, this, [this](const QString& str) {
        if (QRegularExpression(m_inputMaskRegExp).match(str).hasMatch()) {
            m_lastText = text();
        } else {
            setText(m_lastText);
        }
    });
}

bool PhoneNumberLineEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);
    QRegExp re(QString("^\\d{8,12}$"));
    return re.exactMatch(text());
}

QString PhoneNumberLineEdit::inputMaskRegExp() const
{
    return m_inputMaskRegExp;
}

QString PhoneNumberLineEdit::correctValueRegExp() const
{
    return m_correctValueRegExp;
}

void PhoneNumberLineEdit::setCodeFromCountryInfo(const CountryInfo &info)
{
    if (m_info.id != info.id) {
        m_info = info;
        update();
    }
}

void PhoneNumberLineEdit::updateCorrectState() {
    emit correctStateUpdated(isCorrect());
}

void PhoneNumberLineEdit::setInputMaskRegExp(const QString& inputMaskRegExp)
{
    if (m_inputMaskRegExp == inputMaskRegExp)
        return;

    m_inputMaskRegExp = inputMaskRegExp;
    emit inputMaskRegExpChanged(inputMaskRegExp);
}

void PhoneNumberLineEdit::setCorrectValueRegExp(const QString& correctValueRegExp)
{
    if (m_correctValueRegExp == correctValueRegExp)
        return;

    m_correctValueRegExp = correctValueRegExp;
    emit correctValueRegExpChanged(correctValueRegExp);
}

CheckedCheckBox::CheckedCheckBox(QWidget* parent) : QCheckBox(parent) {}
bool CheckedCheckBox::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);
    return isChecked();
}

CheckedComboBox::CheckedComboBox(QWidget* parent)
    : QComboBox(parent)
{
    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
bool CheckedComboBox::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);
    return this->currentText() != "-";
}

NotEmptyPlainTextEdit::NotEmptyPlainTextEdit(QWidget* parent) : QPlainTextEdit(parent) {}
bool NotEmptyPlainTextEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);

    if (QString("NotEmptyPlainTextEdit") == metaObject()->className()) {
        static const QRegularExpression re("[A-Za-z0-9/\\'\\`\\-,\\.]{3,100}");
        return re.match(toPlainText()).hasMatch();
    }

    return !toPlainText().isEmpty();
}

CountryComboBox::CountryComboBox(QWidget *parent)
    : CheckedComboBox(parent)
    , m_displayFlag(Display::Country)
{
    QListView* view = qobject_cast<QListView*>(this->view());
    CountryInfoModel* model = new CountryInfoModel();
    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(model);
    view->setModel(m_proxyModel);
    setModel(m_proxyModel);

    connect(this, SIGNAL(filterRegExpChanged(QString)), m_proxyModel, SLOT(setFilterRegExp(QString)));
    connect(this, SIGNAL(excludeRegExpChanged(QString)), model, SLOT(setExcludeRegExp(QString)));

    model->initData();// initData();
    this->setItemDelegate(new QStyledItemDelegate);
    view->setItemDelegate(new QStyledItemDelegate);
//    view->setRowHidden(225, true); // United States
//    view->setRowHidden(226, true);
//#endif // DESIGNER
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
}

void CountryComboBox::setDisplayFlag(Flags f) {
    m_displayFlag = f;
    if (CountryInfoModel* m = qobject_cast<CountryInfoModel*>(m_proxyModel->sourceModel())) {
        m->setDisplayFlag(f);
    }
//    setModelColumn(f);
    update();
}

void CountryComboBox::onCurrentIndexChanged(int /*index*/)
{
    CountryInfo info = view()->currentIndex().data(UserType::RowDataRole).value<CountryInfo>();
    if (m_displayFlag & Display::Icon) {
        setItemIcon(view()->currentIndex().row(), QIcon(info.icon));
    }
    update();
    emit currentCountryInfoChanged(info);
}

ConfirmationLineEdit::ConfirmationLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_passLineEdit(qobject_cast<const PasswordLineEdit*>(parent))
{
    setEchoMode(EchoMode::Password);
    if (m_passLineEdit == nullptr)
        emit requestPasswordLineEdit();
}

void ConfirmationLineEdit::setPasswordLineEdit(const PasswordLineEdit *pass)
{
    m_passLineEdit = pass;
    if (m_passLineEdit == nullptr)
        emit requestPasswordLineEdit();
}

bool ConfirmationLineEdit::isCorrect() const
{
    if (m_forceCorrect != E::ForceCorrectState::None)
        return (m_forceCorrect == E::ForceCorrectState::Correct);

    if (m_passLineEdit != nullptr) {
        return m_passLineEdit->text() == text();
    }
    emit requestPasswordLineEdit();
    return false;
}

IMPL_FOCUS_EVENTS(EmailLineEdit,          QLineEdit)
IMPL_FOCUS_EVENTS(UsernameLineEdit,       QLineEdit)
IMPL_FOCUS_EVENTS(NotEmptyLineEdit,       QLineEdit)
IMPL_FOCUS_EVENTS(PostIndexLineEdit,      QLineEdit)
IMPL_FOCUS_EVENTS(CheckedCheckBox,        QCheckBox)
IMPL_FOCUS_EVENTS(CheckedComboBox,        QComboBox)
IMPL_FOCUS_EVENTS(NotEmptyPlainTextEdit,  QPlainTextEdit)
IMPL_FOCUS_EVENTS(PasswordLineEdit, QLineEdit)
IMPL_FOCUS_EVENTS(ConfirmationLineEdit, QLineEdit)
IMPL_FOCUS_EVENTS(CountryComboBox, QComboBox)
IMPL_FOCUS_EVENTS(PhoneNumberLineEdit, QLineEdit)

