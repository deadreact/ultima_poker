#pragma once
#include <basewidgets/types.h>
#include <basewidgets/correctinput.h>
#include "../plugins_base/pluginhelper.h"
#include <QAbstractButton>

#define DECLARE_CORRECT_INPUT_PLUGIN(classname)                                   \
class classname##Plugin : public QObject, public CommonPluginInterface {          \
    Q_OBJECT                                                                      \
    Q_INTERFACES(QDesignerCustomWidgetInterface)                                  \
public:                                                                           \
    Q_INVOKABLE classname##Plugin(QObject* parent = nullptr)                      \
        : QObject(parent)                                                         \
        , CommonPluginInterface(&classname::staticMetaObject, "Correct Input") {} \
    virtual QIcon icon() const override { return QIcon(":/icon_input.png"); }     \
    virtual QSize defaultSizeHint() const override { return QSize(100, 23); }     \
}

DECLARE_CORRECT_INPUT_PLUGIN(NotEmptyLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(PasswordLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(ConfirmationLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(CountryComboBox);
DECLARE_CORRECT_INPUT_PLUGIN(PhoneNumberLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(EmailLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(UsernameLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(PostIndexLineEdit);
DECLARE_CORRECT_INPUT_PLUGIN(CheckedCheckBox);
DECLARE_CORRECT_INPUT_PLUGIN(CheckedComboBox);
DECLARE_CORRECT_INPUT_PLUGIN(NotEmptyPlainTextEdit);
