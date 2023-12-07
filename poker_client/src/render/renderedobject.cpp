#include <precompiled.h>
#include "renderedobject.h"
#include <QUrl>
#include <cmath>
#include <qregularexpression.h>
#include <basewidgets/util.h>

#ifndef NO_OPENGL
#include <QGLShaderProgram>
#endif // !NO_OPENGL

struct SpriteData
{
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    QMatrix4x4 m_transform;

    QMatrix4x4 m_colorTransform;
    QVector4D m_colorOffset;

    Geometry* m_mesh;
    QRect m_clippingBox;
    qreal m_opacity;
    qreal m_scrOpacity;
    int m_surfaceCount;
    bool m_isNeedUpdate;

    BufferObject m_buffer;
#ifndef NO_OPENGL
    QGLShaderProgram* m_program;
    QUrl m_vertexShaderUrl;
    QUrl m_fragmentShaderUrl;
#endif // !NO_OPENGL

    QUrl m_textureUrl;
    QSize m_size; // in pixels
    int m_order;

    SpriteData() 
    : m_position(0.f, 0.f, 0.f)
    , m_scale(1.f, 1.f, 1.f)
    , m_rotation()
    , m_transform()
    , m_colorTransform()
    , m_colorOffset(0, 0, 0, 0)
    , m_mesh(nullptr)
    , m_clippingBox(QRect())
    , m_opacity(1.0)
    , m_scrOpacity(1.0)
    , m_surfaceCount(1)
    , m_isNeedUpdate(true)
    , m_buffer()
#ifndef NO_OPENGL
    , m_program(nullptr)
    , m_vertexShaderUrl("")
    , m_fragmentShaderUrl("")
#endif // !NO_OPENGL
    , m_textureUrl("")
    , m_size(10, 10)
    , m_order(0)
    {
        m_transform.setToIdentity();
        m_colorTransform.setToIdentity();
    }
};

RenderedObject::RenderedObject(RenderedObjectParent *parent)
    : RenderedObjectParent(parent)
    , m_dataPtr(new SpriteData)
{
    m_dataPtr->m_transform.setToIdentity();
}

RenderedObject::~RenderedObject()
{
    if (m_dataPtr->m_mesh)
        delete m_dataPtr->m_mesh;
    delete m_dataPtr;
}

RenderedObject *RenderedObject::parentRenderedObject() const {
    return qobject_cast<RenderedObject*>(parent());
}

void RenderedObject::updateRenderedParent()
{
    m_parent = qobject_cast<RenderedObject*>(parent());
}

void RenderedObject::updateRenderedChildren()
{
    m_children.resize(0);
    for (QObject* child : children()) {
        if (RenderedObject* ro = qobject_cast<RenderedObject*>(child)) {
            m_children.push_back(ro);
        }
    }
    std::stable_sort(m_children.begin(), m_children.end(), [](RenderedObject* ro1, RenderedObject* ro2) { return ro1->order() < ro2->order(); });
}
void RenderedObject::init()
{
    if (m_dataPtr->m_mesh == nullptr)
    {
        Surface* surface = Surface::create(QSize(4, 4), QSize(200, 200));
        m_dataPtr->m_mesh = surface;
    }
    for (RenderedObject* ro : renderedChildren()) {
        ro->init();
    }
}
#ifndef NO_OPENGL
void RenderedObject::initShaders()
{
    if (QGLShaderProgram* program = initShaderProgram(vertexShaderUrl(), fragmentShaderUrl())) {
        setShaderProgram(program);
    } else {
        setShaderProgram(initShaderProgram(QUrl::fromLocalFile(":/shaders/vertex.glsl"), QUrl::fromLocalFile(":/shaders/fragment.glsl")));
    }
    for (QObject* child : children()) {
        if (RenderedObject* ro = qobject_cast<RenderedObject*>(child)) {
            ro->initShaders();
        }
    }
}

QGLShaderProgram * RenderedObject::initShaderProgram(const QUrl &vert, const QUrl &frag)
{
    QGLShaderProgram *program = new QGLShaderProgram();
    QString str;

    str = vert.toString().replace("qrc:/", ":/").replace(QRegularExpression("file:(///)?"), "");

    if (!program->addShaderFromSourceFile(QGLShader::Vertex, str )) {
        delete program;
        return nullptr;
    }
    str = frag.toString().replace("qrc:/", ":/").replace(QRegularExpression("file:(///)?"), "");

    if (!program->addShaderFromSourceFile(QGLShader::Fragment, str)) {
        delete program;
        return nullptr;
    }
    if (!program->link()) {
        delete program;
        return nullptr;
    }
    if (!program->bind()) {
        delete program;
        return nullptr;
    }
    return program;
}
QGLShaderProgram *RenderedObject::program() const {
    return m_dataPtr->m_program;
}

void RenderedObject::setShaderProgram(QGLShaderProgram *program) {
    m_dataPtr->m_program = program;
}
void RenderedObject::setVertexShaderUrl(const QUrl& vertexShaderUrl) {
    m_dataPtr->m_vertexShaderUrl = vertexShaderUrl;
}

void RenderedObject::setFragmentShaderUrl(const QUrl &fragmentShaderUrl) {
    m_dataPtr->m_fragmentShaderUrl = fragmentShaderUrl;
}


const QUrl& RenderedObject::vertexShaderUrl() const {
    return m_dataPtr->m_vertexShaderUrl;
}

const QUrl& RenderedObject::fragmentShaderUrl() const {
    return m_dataPtr->m_fragmentShaderUrl;
}
#endif // !NO_OPENGL
bool RenderedObject::event(QEvent *e) {
    return RenderedObjectParent::event(e);
}

void RenderedObject::updateTransform(const QMatrix4x4& parentTransform, qreal parentScrOpacity)
{
    updateRenderedParent();
    updateRenderedChildren();

    m_dataPtr->m_scrOpacity = parentScrOpacity * m_dataPtr->m_opacity;
//    if ( m_dataPtr->m_isNeedUpdate )
    {
        m_dataPtr->m_transform = parentTransform;
        m_dataPtr->m_transform.translate(m_dataPtr->m_position);
        m_dataPtr->m_transform.rotate(m_dataPtr->m_rotation);
        m_dataPtr->m_transform.scale(m_dataPtr->m_scale);
//        m_dataPtr->m_transform *= parentTransform;

        for (RenderedObject* ro : renderedChildren()) {
            ro->updateTransform(m_dataPtr->m_transform, m_dataPtr->m_scrOpacity);
        }
        m_dataPtr->m_isNeedUpdate = false;
    }
}


void RenderedObject::setScale(const QVector2D &scale)
{
    m_dataPtr->m_scale.setX(scale.x());
    m_dataPtr->m_scale.setY(scale.y());
    requestUpdate();
}

void RenderedObject::setScale(const QVector3D &scale)
{
    if (qFuzzyCompare(m_dataPtr->m_scale, scale))
        return;

    m_dataPtr->m_scale = scale;
    requestUpdate();
    emit scaled(scale);
}

void RenderedObject::setPosition(const QVector3D &position)
{
    if (qFuzzyCompare(m_dataPtr->m_position, position))
        return;

    m_dataPtr->m_position = position;
    requestUpdate();
    emit moved(position);
}

void RenderedObject::setRotation(const QQuaternion &rotation)
{
    if (m_dataPtr->m_rotation == rotation)
        return;

    m_dataPtr->m_rotation = rotation;
    requestUpdate();
    emit rotated(rotation);
}

void RenderedObject::setScale(float x, float y)
{
    setScale(QVector3D(x, y, scale().z()));
}

void RenderedObject::setPosition(float x, float y, float z)
{
    setPosition(QVector3D(x, y, z));
}

void RenderedObject::setRotation(float pitch, float yaw, float roll)
{
    setRotation(QQuaternion::fromEulerAngles(pitch, yaw, roll));
}

void RenderedObject::setTransform(const QMatrix4x4 &transform)
{
    m_dataPtr->m_transform = transform;
}

void RenderedObject::setColorTransform(const QMatrix4x4 &transform)
{
    if (m_dataPtr->m_colorTransform != transform) {
        m_dataPtr->m_colorTransform = transform;
        requestUpdate();
        emit colorTransformChanged(transform);
    }
}

void RenderedObject::setColorOffset(const QVector4D &offset)
{
    if (m_dataPtr->m_colorOffset != offset) {
        m_dataPtr->m_colorOffset = offset;
        requestUpdate();
        emit colorOffsetChanged(offset);
    }
}

Geometry* RenderedObject::mesh() const { return m_dataPtr->m_mesh; }

void RenderedObject::setMesh(Geometry *mesh)
{
    m_dataPtr->m_mesh = mesh;
    requestUpdate();
}

const QMatrix4x4 &RenderedObject::transform() const { return m_dataPtr->m_transform; }
const QVector3D &RenderedObject::scale()      const { return m_dataPtr->m_scale; }
const QVector3D &RenderedObject::position()   const { return m_dataPtr->m_position; }
const QQuaternion &RenderedObject::rotation() const { return m_dataPtr->m_rotation; }

const QRect& RenderedObject::clippingBox() const {
    return m_dataPtr->m_clippingBox;
}

bool RenderedObject::isNeedUpdate() const {
    return m_dataPtr->m_isNeedUpdate;
}

const QMatrix4x4 &RenderedObject::colorTransform() const
{
    return m_dataPtr->m_colorTransform;
}

const QVector4D &RenderedObject::colorOffset() const
{
    return m_dataPtr->m_colorOffset;
}

qreal RenderedObject::opacity() const
{
    return m_dataPtr->m_opacity;
}

qreal RenderedObject::scrOpacity() const
{
    return m_dataPtr->m_scrOpacity;
}

size_t RenderedObject::vertexBufferSize() const
{
    return (m_dataPtr->m_mesh == nullptr) ? 0 : m_dataPtr->m_mesh->vertexBufferSize();
}
size_t RenderedObject::indexBufferSize() const
{
    return (m_dataPtr->m_mesh == nullptr) ? 0 : m_dataPtr->m_mesh->indexBufferSize();
}

const void *RenderedObject::vertexBufferData() const
{
    return (m_dataPtr->m_mesh == nullptr) ? nullptr : m_dataPtr->m_mesh->vertices().data();
}
const void *RenderedObject::indexBufferData() const
{
    return (m_dataPtr->m_mesh == nullptr) ? nullptr : m_dataPtr->m_mesh->indices().data();
}

unsigned int *RenderedObject::buffers()
{
    return &m_dataPtr->m_buffer;
}

unsigned int RenderedObject::vbo() const {
    return m_dataPtr->m_buffer.vbo;
}

unsigned int RenderedObject::ibo() const {
    return m_dataPtr->m_buffer.ibo;
}

int RenderedObject::surfaceCount() const
{
    return m_dataPtr->m_surfaceCount;
}

const QUrl &RenderedObject::textureUrl() const
{
    return m_dataPtr->m_textureUrl;
}

QString RenderedObject::texturePath() const {
    return  m_dataPtr->m_textureUrl.toString().replace("qrc:/", ":/");
}

RenderedObject* RenderedObject::renderedParent() const
{
    return m_parent;
}

const std::vector<RenderedObject*>& RenderedObject::renderedChildren() const
{
    return m_children;
}

void RenderedObject::setClippingBox(const QRect& clippingBox)
{
    m_dataPtr->m_clippingBox = clippingBox;
}

void RenderedObject::setOpacity(qreal opacity)
{
    opacity = qBound(0.0, opacity, 1.0);
    if (qFuzzyCompare(m_dataPtr->m_opacity, opacity))
        return;

    m_dataPtr->m_opacity = opacity;
    requestUpdate();
    emit opacityChanged(opacity);
}

void RenderedObject::setSurfaceCount(int surfaceCount)
{
    m_dataPtr->m_surfaceCount = surfaceCount;
}

void RenderedObject::resetPosition() {
    setPosition(0.f, 0.f);
}

void RenderedObject::resetScale() {
    setScale(QVector3D(1.f, 1.f, 1.f));
}

void RenderedObject::resetRotation() {
    setRotation(0, 0, 0);
}

void RenderedObject::unsetClippingBox() {
    setClippingBox(QRect());
}

void RenderedObject::resetOpacity() {
    setOpacity(1);
}

void RenderedObject::setTextureUrl(const QUrl& url)
{
    if (m_dataPtr->m_textureUrl == url)
        return;

    m_dataPtr->m_textureUrl = url;
    emit textureUrlChanged(url);
}

void RenderedObject::requestUpdate()
{
    if(!m_dataPtr->m_isNeedUpdate)
    {
        m_dataPtr->m_isNeedUpdate = true;
        if (RenderedObject* ro = renderedParent()) {
            ro->requestUpdate();
        }
    }
}

QPropertyAnimation* RenderedObject::move(const QVector3D& distance, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "position", this);
    effector->setStartValue(position());
    effector->setEndValue(position() + distance);
    effector->setDuration(msecs);
    return effector;
}

QPropertyAnimation* RenderedObject::moveTo(const QVector3D& newPos, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "position", this);
    effector->setStartValue(position());
    effector->setEndValue(newPos);
    effector->setDuration(msecs);
    return effector;
}

QPropertyAnimation* RenderedObject::scale(const QVector3D& scaling, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "scale", this);
    effector->setStartValue(scale());
    effector->setEndValue(scale() * scaling);
    effector->setDuration(msecs);
    return effector;
}

QPropertyAnimation* RenderedObject::scaleTo(const QVector3D& newScale, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "scale", this);
    effector->setStartValue(scale());
    effector->setEndValue(newScale);
    effector->setDuration(msecs);
    return effector;
}

QPropertyAnimation* RenderedObject::rotate(const QQuaternion& quat, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "rotation", this);
    effector->setStartValue(rotation());
    effector->setEndValue(rotation() + quat);
    effector->setDuration(msecs);
    return effector;
}

QPropertyAnimation* RenderedObject::rotateTo(const QQuaternion& newRotation, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "rotation", this);
    effector->setStartValue(rotation());
    effector->setEndValue(newRotation);
    effector->setDuration(msecs);
    return effector;
}

QPropertyAnimation *RenderedObject::fadeTo(float newOpacity, int msecs)
{
    QPropertyAnimation* effector = new QPropertyAnimation(this, "opacity", this);
    effector->setStartValue(opacity());
    effector->setEndValue(newOpacity);
    effector->setDuration(msecs);
    return effector;
}

void RenderedObject::setOrder(int order) {
    m_dataPtr->m_order = order;
}

int RenderedObject::order() const {
    return m_dataPtr->m_order;
}
