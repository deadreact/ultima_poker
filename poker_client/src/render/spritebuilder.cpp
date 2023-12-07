#include <precompiled.h>
#include "spritebuilder.h"

#include <QUrl>



SpriteBuilder::SpriteBuilder(QWidget *parent)
    : QWidget(parent)
    , m_renderedObject(nullptr)
{
    QObject* spriteParent = parent;
    if (SpriteBuilder* spriteForm = qobject_cast<SpriteBuilder*>(parentWidget())) {
        spriteParent = spriteForm->renderedObject();
    }
    m_renderedObject = new RenderedObject(spriteParent);
    connect(this, SIGNAL(moved(QVector3D)),            m_renderedObject, SLOT(setPosition(QVector3D)));
    connect(this, SIGNAL(scaled(QVector3D)),           m_renderedObject, SLOT(setScale(QVector3D)));
    connect(this, SIGNAL(rotated(QQuaternion)),        m_renderedObject, SLOT(setRotation(QQuaternion)));
    connect(this, SIGNAL(transformed(QMatrix4x4)),     m_renderedObject, SLOT(setTransform(QMatrix4x4)));
    connect(this, SIGNAL(opacityChanged(qreal)),       m_renderedObject, SLOT(setOpacity(qreal)));
    connect(this, SIGNAL(textureUrlChanged(QUrl)), m_renderedObject, SLOT(setTextureUrl(QUrl)));
}

SpriteBuilder::~SpriteBuilder() {}

RenderedObject *SpriteBuilder::renderedObject() const { return m_renderedObject; }
RenderedObject *SpriteBuilder::takeRenderedObject(QObject *parent)
{
    RenderedObject* ro = m_renderedObject;
    if (parent != nullptr)
        ro->setParent(parent);

    setupGeometry(ro);

    if (parentWidget()) {
//        float z = 0;//qvariant_cast<float>(property("z"));
        ro->setPosition(pos().x() - parentWidget()->width()/2 + width()/2, parentWidget()->height()/2 - pos().y() - height()/2, zPos());
    }
    ro->setObjectName(QString("ro_") + this->objectName());
    for (QObject* child : children())
    {
        if (SpriteBuilder *builder = qobject_cast<SpriteBuilder*>(child)) {
            builder->takeRenderedObject(ro);
        }
    }
    m_renderedObject = nullptr;
    return ro;
}


void SpriteBuilder::setupGeometry(RenderedObject *ro)
{
    if (m_meshTypeName == "Cube") {
        ro->setMesh(new Cube());
    } else if (m_meshTypeName == "Circle") {
        ro->setMesh(new Circle(QVector2D(width(), height()).length()/2));
    } else if (m_meshTypeName == "RoundedRect") {
        ro->setMesh(new RoundedRect(size(), property("borderRadius").toInt()));
    } else {
        ro->setMesh( Quad::create(size()) );
    }
}

const QVector3D &SpriteBuilder::scale() const                           { return m_renderedObject->scale(); }
const QVector3D &SpriteBuilder::position() const                        { return m_renderedObject->position(); }
const QQuaternion &SpriteBuilder::rotation() const                      { return m_renderedObject->rotation(); }
const QMatrix4x4 &SpriteBuilder::transform() const                      { return m_renderedObject->transform(); }
const QRect &SpriteBuilder::clippingBox() const                         { return m_renderedObject->clippingBox(); }

const QMatrix4x4 &SpriteBuilder::colorTransform() const                 { return m_renderedObject->colorTransform(); }
qreal SpriteBuilder::opacity() const                                    { return m_renderedObject->opacity(); }
const QUrl &SpriteBuilder::textureUrl() const                           { return m_renderedObject->textureUrl(); }
#ifndef NO_OPENGL
const QUrl &SpriteBuilder::vertexShaderUrl() const                      { return m_renderedObject->vertexShaderUrl(); }
const QUrl &SpriteBuilder::fragmentShaderUrl() const                    { return m_renderedObject->fragmentShaderUrl(); }
#endif // !NO_OPENGL

int SpriteBuilder::order() const                                        { return m_renderedObject->order(); }

void SpriteBuilder::setScale(const QVector3D &scale)                    { m_renderedObject->setScale(scale); }
void SpriteBuilder::setPosition(const QVector3D &position)              { m_renderedObject->setPosition(position); }
void SpriteBuilder::setRotation(const QQuaternion &rotation)            { m_renderedObject->setRotation(rotation); }
void SpriteBuilder::setTransform(const QMatrix4x4 &transform)           { m_renderedObject->setTransform(transform); }
void SpriteBuilder::setClippingBox(const QRect &clippingBox)            { m_renderedObject->setClippingBox(clippingBox); }

void SpriteBuilder::setColorTransform(const QMatrix4x4 &colorTransform) { m_renderedObject->setColorTransform(colorTransform); }
void SpriteBuilder::setOpacity(qreal opacity)                           { m_renderedObject->setOpacity(opacity); }
void SpriteBuilder::setTextureUrl(const QUrl &url)                      { m_renderedObject->setTextureUrl(url); }
#ifndef NO_OPENGL
void SpriteBuilder::setVertexShaderUrl(const QUrl &vertexShaderUrl)     { m_renderedObject->setVertexShaderUrl(vertexShaderUrl); }
void SpriteBuilder::setFragmentShaderUrl(const QUrl &fragmentShaderUrl) { m_renderedObject->setFragmentShaderUrl(fragmentShaderUrl); }
#endif // !NO_OPENGL

void SpriteBuilder::setOrder(int order)                                 { m_renderedObject->setOrder(order); }
void SpriteBuilder::resetScale()                                        { m_renderedObject->setScale(QVector3D(1, 1, 1)); }
void SpriteBuilder::resetPosition()                                     { m_renderedObject->setPosition(QVector3D(0, 0, 0)); }
void SpriteBuilder::resetRotation()                                     { m_renderedObject->setRotation(0, 0, 0); }
void SpriteBuilder::resetTransform()                                    { m_renderedObject->setTransform(QMatrix4x4()); }
void SpriteBuilder::unsetClippingBox()                                  { m_renderedObject->unsetClippingBox(); }

void SpriteBuilder::resetColor()
{
    m_renderedObject->setColorTransform(QMatrix4x4());
    m_renderedObject->setColorOffset(QVector4D(0, 0, 0, 0));
}
void SpriteBuilder::resetOpacity()                                      { m_renderedObject->setOpacity(1.0); }
void SpriteBuilder::resetTextureUrl()                                   { m_renderedObject->setTextureUrl(QUrl()); }
#ifndef NO_OPENGL
void SpriteBuilder::resetVertexShaderUrl()                              { m_renderedObject->setVertexShaderUrl(QUrl("")); }
void SpriteBuilder::resetFragmentShaderUrl()                            { m_renderedObject->setFragmentShaderUrl(QUrl("")); }
#endif // !NO_OPENGL
