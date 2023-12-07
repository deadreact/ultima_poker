#include "converttypemapperplugin.h"
#include "intuintmapperplugin.h"
#include "floatdoublemapperplugin.h"
#include "transform2dmapperplugin.h"
#include "transform3dmapperplugin.h"
#include "mappers.h"

Mappers::Mappers(QObject *parent)
    : QObject(parent)
{
//    m_widgets.append(new ConvertTypeMapperPlugin(this));
    m_widgets.append(new IntUIntMapperPlugin(this));
    m_widgets.append(new FloatDoubleMapperPlugin(this));
    m_widgets.append(new Transform2DMapperPlugin(this));
    m_widgets.append(new Transform3DMapperPlugin(this));

}

QList<QDesignerCustomWidgetInterface*> Mappers::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mappersplugin, Mappers)
#endif // QT_VERSION < 0x050000
