#include "customwidgets.h"
#include "plugin_common.h"
#include "correctinputplugin.h"
#include "rttwidgetplugin.h"


CustomWidgets::CustomWidgets(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new FramedButtonPlugin(this));
    m_widgets.append(new EntityWidgetPlugin(this));
    m_widgets.append(new RotatedWidgetPlugin(this));
    m_widgets.append(new FramedEntityPlugin(this));
    m_widgets.append(new ClockWidgetPlugin(this));
    m_widgets.append(new RTTWidgetPlugin(this));
    m_widgets.append(new TextWidgetPlugin(this));
    m_widgets.append(new InfoMessagePlugin(this));
    m_widgets.append(new ValidatedLineEditPlugin(this));

    m_widgets.append(new NotEmptyLineEditPlugin(this));
    m_widgets.append(new PasswordLineEditPlugin(this));
    m_widgets.append(new ConfirmationLineEditPlugin(this));
    m_widgets.append(new CountryComboBoxPlugin(this));
    m_widgets.append(new PhoneNumberLineEditPlugin(this));
    m_widgets.append(new EmailLineEditPlugin(this));
    m_widgets.append(new UsernameLineEditPlugin(this));
    m_widgets.append(new PostIndexLineEditPlugin(this));
    m_widgets.append(new CheckedCheckBoxPlugin(this));
    m_widgets.append(new CheckedComboBoxPlugin(this));
    m_widgets.append(new NotEmptyPlainTextEditPlugin(this));
    m_widgets.append(new TimeZoneBoxPlugin(this));
    m_widgets.append(new SvgWidgetPlugin(this));
    m_widgets.append(new EasingCurveComboBoxPlugin(this));

}

QList<QDesignerCustomWidgetInterface*> CustomWidgets::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(customwidgetsplugin, CustomWidgets)
#endif // QT_VERSION < 0x050000
