#pragma once

#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QWidget>
#include <QMetaMethod>
#include <QUrl>
#include <QPropertyAnimation>
#include "geometry.h"

struct SpriteData;
#ifndef NO_OPENGL
class QGLShaderProgram;
#endif // !NO_OPENGL

typedef QObject RenderedObjectParent;

class RenderedObject : public RenderedObjectParent
{
    Q_OBJECT

    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY moved RESET resetPosition)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaled RESET resetScale)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotated RESET resetRotation)
    Q_PROPERTY(QMatrix4x4 transform READ transform WRITE setTransform)
    Q_PROPERTY(QMatrix4x4 colorTransform READ colorTransform WRITE setColorTransform NOTIFY colorTransformChanged)
    Q_PROPERTY(QVector4D colorOffset READ colorOffset WRITE setColorOffset NOTIFY colorOffsetChanged)
    Q_PROPERTY(QRect clippingBox READ clippingBox WRITE setClippingBox RESET unsetClippingBox)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged RESET resetOpacity)
    Q_PROPERTY(qreal scrOpacity READ scrOpacity)
    Q_PROPERTY(QUrl textureUrl READ textureUrl WRITE setTextureUrl NOTIFY textureUrlChanged)
    Q_PROPERTY(bool isNeedUpdate READ isNeedUpdate)
#ifndef NO_OPENGL
    Q_PROPERTY(QUrl vertexShaderUrl READ vertexShaderUrl WRITE setVertexShaderUrl)
    Q_PROPERTY(QUrl fragmentShaderUrl READ fragmentShaderUrl WRITE setFragmentShaderUrl)
#endif // !NO_OPENGL

    Q_PROPERTY(int order READ order WRITE setOrder)

    Q_PROPERTY(qreal xPos READ xPos WRITE setXPos)
    Q_PROPERTY(qreal yPos READ yPos WRITE setYPos)
    Q_PROPERTY(qreal zPos READ zPos WRITE setZPos)
private:
    SpriteData *m_dataPtr;
    RenderedObject* m_parent;
    std::vector<RenderedObject*> m_children;

private slots:
    void updateRenderedParent();
    void updateRenderedChildren();
signals:
    void moved(const QVector3D&);
    void scaled(const QVector3D&);
    void rotated(const QQuaternion&);
    void transformed(const QMatrix4x4&);
    void colorTransformChanged(const QMatrix4x4&);
    void colorOffsetChanged(const QVector4D&);
    void opacityChanged(qreal opacity);

    void textureUrlChanged(const QUrl&);

public:
    RenderedObject(RenderedObjectParent *parent = nullptr);
    virtual ~RenderedObject();

    RenderedObject* parentRenderedObject() const;

    virtual bool event(QEvent *e) override;
    void init();
    void updateTransform(const QMatrix4x4 &parentTransform = QMatrix4x4(), qreal parentScrOpacity=1.0);

#ifndef NO_OPENGL
    void initShaders();
    static QGLShaderProgram* initShaderProgram(const QUrl &vert, const QUrl &frag);
    QGLShaderProgram* program() const;
    void setShaderProgram(QGLShaderProgram* program);
    const QUrl& vertexShaderUrl() const;
    const QUrl& fragmentShaderUrl() const;
    Q_SLOT void setVertexShaderUrl(const QUrl& vertexShaderUrl);
    Q_SLOT void setFragmentShaderUrl(const QUrl& fragmentShaderUrl);
#endif // !NO_OPENGL

    Geometry* mesh() const;
    void setMesh(Geometry* );
    const QVector3D &scale() const;
    const QVector3D& position() const;
    const QQuaternion& rotation() const;
    const QRect &clippingBox() const;

    const QMatrix4x4& transform() const;

    const QMatrix4x4& colorTransform() const;
    const QVector4D& colorOffset() const;

    qreal opacity() const;
    qreal scrOpacity() const;


    bool isNeedUpdate() const;

    virtual size_t vertexBufferSize() const;
    virtual size_t indexBufferSize() const;
    virtual const void *vertexBufferData() const;
    virtual const void* indexBufferData() const;
    unsigned int* buffers();
    unsigned int vbo() const;
    unsigned int ibo() const;

    int surfaceCount() const;

    const QUrl& textureUrl() const;
    QString texturePath() const;
    RenderedObject* renderedParent() const;
    const std::vector<RenderedObject *>& renderedChildren() const;
public slots:
    void setScale(const QVector3D &scale);
    void setScale(const QVector2D &scale);
    void setScale(float x, float y);
    void setPosition(const QVector3D& position);
    void setPosition(float x, float y, float z=-5.f);

    void setRotation(const QQuaternion& rotation);
    void setRotation(float pitch, float yaw, float roll);
    void setClippingBox(const QRect &clippingBox);
    void setTransform(const QMatrix4x4& transform);

    void setColorTransform(const QMatrix4x4& transform);
    void setColorOffset(const QVector4D& offset);

    void setOpacity(qreal opacity);
    void setSurfaceCount(int surfaceCount);

    void resetPosition();
    void resetScale();
    void resetRotation();
    void unsetClippingBox();
    void resetOpacity();
    void setTextureUrl(const QUrl &url);


    void setOrder(int order);
protected:
    void requestUpdate();
public slots:
    QPropertyAnimation* move(const QVector3D& distance, int msecs = 500);
    QPropertyAnimation* moveTo(const QVector3D& newPos, int msecs = 500);
    QPropertyAnimation* scale(const QVector3D& scaling, int msecs = 500);
    QPropertyAnimation* scaleTo(const QVector3D& newScale, int msecs = 500);
    QPropertyAnimation* rotate(const QQuaternion& quat, int msecs = 500);
    QPropertyAnimation* rotateTo(const QQuaternion& newRotation, int msecs = 500);

    QPropertyAnimation* fadeTo(float newOpacity, int msecs = 500);

    inline void setXPos(qreal _x) { setPosition(QVector3D(_x, yPos(), zPos())); }
    inline void setYPos(qreal _y) { setPosition(QVector3D(xPos(), _y, zPos())); }
    inline void setZPos(qreal _z) { setPosition(QVector3D(xPos(), yPos(), _z)); }
public:
    inline qreal xPos() const { return position().x(); }
    inline qreal yPos() const { return position().y(); }
    inline qreal zPos() const { return position().z(); }
    int order() const;
};
