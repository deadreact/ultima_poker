#include <precompiled.h>
#include "GLWidget.h"
#include <QMouseEvent>
#include <basewidgets/types.h>
#include "render/geometry.h"

GLWidget::GLWidget(QWidget *parent)
    : GLWidgetParent(parent)
    , QGLFunctions()
    , m_timer()
#ifndef NO_OPENGL
    , m_fbo(nullptr)
#endif // !NO_OPENGL
    , m_camera(this)
    , m_projection()
    , m_textureMap()
    , m_renderedObjects()
    , m_mouseOffset()
    , m_angleX(0.f)
    , m_angleY(0.f)
    , m_resourcePathes()
    , m_needUpdate(true)
    , m_renderToFrameBuffer(true)
{
//    m_camera.setViewportRect(QRectF(QPointF(0.f, 0.f), this->size()));
#ifndef NO_OPENGL
    static const int fps = 1000/60; // 60 fps is enough
    m_timer.start(fps, Qt::PreciseTimer, this);
#else
    setStyleSheet("QWidget {background-color: transparent; }");
    setScaledContents(true);
    setPixmap(QPixmap(":/textures/game/bg.png"));
#endif // !NO_OPENGL
}

GLWidget::~GLWidget()
{
    for (QMap<QString, std::pair<gbp_u16, uint> >::const_iterator it = m_textureMap.begin(); it != m_textureMap.end(); it++)
    {
//        deleteTexture(it.value());
    }
#ifndef NO_OPENGL
    for (RenderedObject* ro : m_renderedObjects) {
        glDeleteBuffers(2, ro->buffers());
    }
    if (m_fbo != nullptr)
    {
        delete m_fbo;
        m_fbo = nullptr;
    }
#endif // !NO_OPENGL
}

qreal GLWidget::fov() const   { return m_camera.fov(); }
qreal GLWidget::nearPlane() const { return m_camera.nearPlane();}
qreal GLWidget::farPlane() const  { return m_camera.farPlane();}

bool GLWidget::isRenderToFrameBuffer() const
{
    return m_renderToFrameBuffer;
}

void GLWidget::setResourcePathes(const QStringList& path)
{
    if (m_resourcePathes == path)
        return;

    m_resourcePathes = path;
    emit resourcePathesChanged(path);
}

void GLWidget::setFov(qreal fov)
{
    if (qFuzzyCompare((qreal)m_camera.fov(), fov))
        return;

    m_camera.setFov(fov);
    updateProjection(float(width())/height());
    updateRendering();
}

void GLWidget::setNearPlane(qreal nearPlane)
{
    if (qFuzzyCompare((qreal)m_camera.nearPlane(), nearPlane))
        return;

   m_camera.setNearPlane(nearPlane);
   updateProjection(float(width())/height());
   updateRendering();
}

void GLWidget::setFarPlane(qreal farPlane)
{
    if (qFuzzyCompare((qreal)m_camera.farPlane(), farPlane))
        return;

    m_camera.setFarPlane(farPlane);
    updateProjection(float(width())/height());
    updateRendering();
}

void GLWidget::setCameraRotation(const QQuaternion &quat)
{
    m_camera.setRotation(quat);
    updateProjection(float(width())/height());
    updateRendering();
}

void GLWidget::setCameraTranslation(const QVector3D &vec)
{
    m_camera.setPosition(vec);
    updateProjection(float(width())/height());
    updateRendering();
}

void GLWidget::glEnableDepthTest(bool enable)
{
#ifndef NO_OPENGL
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    updateGL();
#endif // !NO_OPENGL
}

void GLWidget::glEnableCullFace(bool enable)
{
#ifndef NO_OPENGL
    if (enable)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    updateGL();
#endif // !NO_OPENGL
}

void GLWidget::setRenderToFrameBuffer(bool toFbo)
{
    m_renderToFrameBuffer = toFbo;
}

#ifndef NO_OPENGL
void GLWidget::initializeGL()
{
    initializeGLFunctions();
    qglClearColor(Qt::black);
    //    m_data.init();
    initChildren();
    initShaders();
    initTextures();
    initBufferObjects();

    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glEnable (GL_CULL_FACE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
#endif // !NO_OPENGL

void GLWidget::initBufferObjects()
{
#ifndef NO_OPENGL
    for (RenderedObject* ro : m_renderedObjects) {
        initRenderObject(ro);
    }
    QGLFramebufferObjectFormat fmt;
    fmt.setInternalTextureFormat(GL_RGBA);
    m_fbo = new QGLFramebufferObject(size(), fmt);
#endif // !NO_OPENGL
}

void GLWidget::initShaders()
{
#ifndef NO_OPENGL

    for (RenderedObject* ro : m_renderedObjects) {
        ro->initShaders();
    }
#endif // !NO_OPENGL
}



void GLWidget::initTextures()
{
#ifndef NO_OPENGL

    glEnable(GL_TEXTURE_2D);
    for (RenderedObject* sprite : m_renderedObjects)
    {
        QString path = (sprite->texturePath().isEmpty()) ? (":/textures/Common/templates/empty.png") : (sprite->texturePath());
        updateTexture(path, sprite->program());
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif // !NO_OPENGL
}

void GLWidget::initChildren()
{
#ifndef NO_OPENGL

    for (QObject* child : children())
    {
        if (SpriteBuilder* spriteForm = qobject_cast<SpriteBuilder*>(child))
        {
            RenderedObject* ro = spriteForm->takeRenderedObject(this);
            m_renderedObjects << ro;
            ro->init();
            if (QFileInfo(ro->texturePath()).exists())
            {
                m_resourcePathes << ro->texturePath();
            }
            spriteForm->deleteLater();
        }
    }
#endif // !NO_OPENGL
}

#ifdef NO_OPENGL
void GLWidget::updateTexture(const QString& path) {
#else
void GLWidget::updateTexture(const QString& path, QGLShaderProgram *shaderProgram) {

    if (!m_textureMap.contains(path))
    {
        QImage image(path);
        if (image.isNull())
            return;

        gbp_u16 index = m_textureMap.size();
        glActiveTexture(GL_TEXTURE0 + index);
        m_textureMap.insert(path, std::make_pair(index, bindTexture(image, GL_TEXTURE_2D, GL_RGBA, QGLContext::DefaultBindOption)));
    }
    else
    {
        glActiveTexture(GL_TEXTURE0 + m_textureMap[path].first);
        glBindTexture(GL_TEXTURE_2D, m_textureMap[path].second);
    }

    shaderProgram->setUniformValue("texture", m_textureMap[path].first);
#endif // !NO_OPENGL
}

void GLWidget::initRenderObject(RenderedObject* rObject)
{
#ifndef NO_OPENGL
    rObject->program()->bind();
    glGenBuffers(2, rObject->buffers());

    glBindBuffer(GL_ARRAY_BUFFER, rObject->vbo());
    glBufferData(GL_ARRAY_BUFFER, rObject->vertexBufferSize(), rObject->vertexBufferData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rObject->ibo());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rObject->indexBufferSize(), rObject->indexBufferData(), GL_STATIC_DRAW);

    for (QObject* child : rObject->children()) {
        if (RenderedObject* ro = qobject_cast<RenderedObject*>(child)) {
            initRenderObject(ro);
        }
    }
#endif // !NO_OPENGL
}

void GLWidget::doRender(const RenderedObject* rObject)
{
#ifndef NO_OPENGL
    if (rObject->program()->bind())
    {
        rObject->program()->setUniformValue("mvp_matrix", m_projection * rObject->transform());
        this->glBindBuffer(GL_ARRAY_BUFFER, rObject->vbo());
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rObject->ibo());
        updateTexture(rObject->texturePath(), rObject->program());
        quintptr offset = 0;

        int vertexLocation = rObject->program()->attributeLocation("a_position");
        rObject->program()->enableAttributeArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3fuv), (const void *)offset);

        offset += 3*sizeof(GLfloat);

        int texcoordLocation = rObject->program()->attributeLocation("a_texcoord");
        rObject->program()->enableAttributeArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3fuv), (const void *)offset);

        rObject->program()->setUniformValue("color_matrix", rObject->colorTransform());
        rObject->program()->setUniformValue("color_offset", rObject->colorOffset());


        glDrawElements(GL_TRIANGLES, rObject->mesh()->indicesCount(), GL_UNSIGNED_SHORT, 0);

        for (RenderedObject* child : rObject->renderedChildren()) {
            doRender(child);
        }
    }
#endif // !NO_OPENGL
}
#ifndef NO_OPENGL

void GLWidget::paintGL() {
    if (m_renderToFrameBuffer) {
        m_fbo->bind();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const RenderedObject* ro: m_renderedObjects) {
        doRender(ro);
    }

    if (m_renderToFrameBuffer)
    {
        m_fbo->release();

        QImage img = m_fbo->toImage();
        emit renderedToImage(img);
    }
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjection(float(w) / std::max(h, 1));
}
#endif // !NO_OPENGL

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void GLWidget::timerEvent(QTimerEvent *)
{
    for (RenderedObject* ro : m_renderedObjects)
    {
        if (ro->isNeedUpdate()) {
            ro->updateTransform();
            m_needUpdate = true;
        }
    }
    if (m_needUpdate)
    {
        updateRendering();
        m_needUpdate = false;
    }
}

void GLWidget::updateContent()
{
    for (auto&& sprite : m_renderedObjects) {
        sprite->updateTransform();
    }
}

const QStringList& GLWidget::resourcePathes() const
{
    return m_resourcePathes;
}

void GLWidget::updateRendering()
{
#ifndef NO_OPENGL
    updateGL();
#else
    if (m_renderToFrameBuffer)
    {
        static const QImage img(":/textures/game/bg.png");
        emit renderedToImage(img);
    }
    update();
#endif // !NO_OPENGL
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    GLWidgetParent::mouseReleaseEvent(event);
}

void GLWidget::enterEvent(QEvent *event)
{
    GLWidgetParent::enterEvent(event);
}

void GLWidget::leaveEvent(QEvent *event)
{
    GLWidgetParent::leaveEvent(event);
}

void GLWidget::updateProjection(float aspect)
{
    m_camera.setAspectRatio(aspect);
    m_projection = m_camera.projViewMatrix();
#ifndef NO_OPENGL
    glDepthRange(m_camera.nearPlane(), m_camera.farPlane());
#endif // !NO_OPENGL

}


void GLWidget::paintEvent(QPaintEvent *event)
{
    GLWidgetParent::paintEvent(event);
}
