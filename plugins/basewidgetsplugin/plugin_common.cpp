#include "plugin_common.h"


EntityWidgetPlugin::EntityWidgetPlugin(QObject *parent)
    : QObject(parent)
{}
FramedEntityPlugin::FramedEntityPlugin(QObject *parent)
    : QObject(parent)
{}
FramedButtonPlugin::FramedButtonPlugin(QObject *parent)
    : QObject(parent)
{}
TextWidgetPlugin::TextWidgetPlugin(QObject *parent)
    : QObject(parent)
{}

InfoMessagePlugin::InfoMessagePlugin(QObject *parent)
    : QObject(parent)
{}
ValidatedLineEditPlugin::ValidatedLineEditPlugin(QObject *parent)
    : QObject(parent)
{}
TimeZoneBoxPlugin::TimeZoneBoxPlugin(QObject *parent)
    : QObject(parent)
{}
EasingCurveComboBoxPlugin::EasingCurveComboBoxPlugin(QObject *parent)
    : QObject(parent)
{}
ClockWidgetPlugin::ClockWidgetPlugin(QObject *parent)
    : QObject(parent)
{}
SvgWidgetPlugin::SvgWidgetPlugin(QObject *parent)
    : QObject(parent)
{}
RotatedWidgetPlugin::RotatedWidgetPlugin(QObject *parent)
    : QObject(parent)
{}

QSize EntityWidgetPlugin::defaultSizeHint() const {
    return QSize(128, 128);
}
QSize FramedEntityPlugin::defaultSizeHint() const {
    return QSize(60, 60);
}
QSize FramedButtonPlugin::defaultSizeHint() const {
    return QSize(60, 20);
}
QSize TextWidgetPlugin::defaultSizeHint() const {
    return QSize(60, 60);
}
QSize InfoMessagePlugin::defaultSizeHint() const {
    return QSize(220, 18);
}
QSize ValidatedLineEditPlugin::defaultSizeHint() const {
    return QSize(148, 26);
}
QSize TimeZoneBoxPlugin::defaultSizeHint() const {
    return QSize(220, 20);
}
QSize EasingCurveComboBoxPlugin::defaultSizeHint() const {
    return QSize(220, 20);
}
QSize ClockWidgetPlugin::defaultSizeHint() const {
    return QSize(120, 32);
}
QSize RotatedWidgetPlugin::defaultSizeHint() const {
    return QSize(128, 128);
}
QIcon SvgWidgetPlugin::icon() const {
    return QIcon(":/icons/svgwidget.png");
}


void EntityWidgetPlugin::initialize(QDesignerFormEditorInterface */*core*/)
{
    if (m_initialized) {
        return;
    }



    m_initialized = true;
}


QWidget *EntityWidgetPlugin::createWidget(QWidget *parent)
{
    EntityWidget* entity = new EntityWidget(parent);
    entity->setProperty("animations", QVariantMap());

//    connect(entity, &EntityWidget::doubleClicked, [entity]{
//        AnimationEditor editor(entity);
//        editor.setTarget(entity);
//        editor.exec();
//    }/*, Qt::UniqueConnection*/);

    return entity;
}



QIcon ValidatedLineEditPlugin::icon() const
{
    return QIcon(":/icons/validator.png");
}

QString ValidatedLineEditPlugin::domXml() const
{
    return QString("<ui language=\"c++\"> displayname=\"ValidatedLineEdit\">"
                "     <widget class=\"ValidatedLineEdit\" name=\"validated_lineEdit\"/>"
                "     <customwidgets>"
                "         <customwidget>"
                "             <class>ValidatedLineEdit</class>"

                "             <propertyspecifications>"
                "                 <stringpropertyspecification name=\"testString1\" notr=\"true\" type=\"singleline\"/>"
                "                 <stringpropertyspecification name=\"testString2\" notr=\"true\" type=\"richtext\"/>"
                "                 <stringpropertyspecification name=\"testString3\" notr=\"true\" type=\"url\"/>"
                "             </propertyspecifications>"
                "         </customwidget>"
                "     </customwidgets>"
                " </ui>");
}


QWidget *TimeZoneBoxPlugin::createWidget(QWidget *parent)
{
    TimeZoneBox* b = new TimeZoneBox(parent);
    static ns_model::Timezone* m = new ns_model::Timezone(this);
    b->setModel(m);
    return b;
}
