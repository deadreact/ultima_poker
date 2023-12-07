#pragma once

#include "renderedobject.h"
#include <QUrl>

typedef QWidget SpriteBuilderParent;

class SpriteBuilder : public SpriteBuilderParent
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY moved RESET resetPosition)
    Q_PROPERTY(qreal xPos READ xPos WRITE setXPos)
    Q_PROPERTY(qreal yPos READ yPos WRITE setYPos)
    Q_PROPERTY(qreal zPos READ zPos WRITE setZPos)


    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaled RESET resetScale)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotated RESET resetRotation)
    Q_PROPERTY(QMatrix4x4 transform READ transform WRITE setTransform RESET resetTransform)
    Q_PROPERTY(QRect clippingBox READ clippingBox WRITE setClippingBox RESET unsetClippingBox)
    Q_PROPERTY(QMatrix4x4 colorTransform READ colorTransform WRITE setColorTransform NOTIFY colorTransformChanged RESET resetColor)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged RESET resetOpacity)
    Q_PROPERTY(QUrl textureUrl READ textureUrl WRITE setTextureUrl NOTIFY textureUrlChanged RESET resetTextureUrl)
#ifndef NO_OPENGL
    Q_PROPERTY(QUrl vertexShaderUrl READ vertexShaderUrl WRITE setVertexShaderUrl RESET resetVertexShaderUrl)
    Q_PROPERTY(QUrl fragmentShaderUrl READ fragmentShaderUrl WRITE setFragmentShaderUrl RESET resetFragmentShaderUrl)
#endif // !NO_OPENGL

    Q_PROPERTY(int order READ order WRITE setOrder)
    Q_PROPERTY(QString meshTypeName MEMBER m_meshTypeName)
private:
    RenderedObject* m_renderedObject;
    QString m_meshTypeName;

    void setupGeometry(RenderedObject* ro);
signals:
    void moved(const QVector3D&);
    void scaled(const QVector3D&);
    void rotated(const QQuaternion&);
    void transformed(const QMatrix4x4&);
    void colorTransformChanged(const QMatrix4x4&);
    void opacityChanged(qreal opacity);
    void textureUrlChanged(const QUrl&);

public:
    SpriteBuilder(QWidget* parent = nullptr);

    virtual ~SpriteBuilder();

    RenderedObject* renderedObject() const;
    RenderedObject* takeRenderedObject(QObject* parent = nullptr);

    const QVector3D& scale() const;
    const QVector3D& position() const;
    const QQuaternion& rotation() const;
    const QMatrix4x4& transform() const;
    const QRect& clippingBox() const;
    const QMatrix4x4& colorTransform() const;
    qreal opacity() const;
    const QUrl& textureUrl() const;
#ifndef NO_OPENGL
    const QUrl& vertexShaderUrl() const;
    const QUrl& fragmentShaderUrl() const;
    Q_SLOT void setVertexShaderUrl(const QUrl& vertexShaderUrl);
    Q_SLOT void setFragmentShaderUrl(const QUrl& fragmentShaderUrl);
    Q_SLOT void resetVertexShaderUrl();
    Q_SLOT void resetFragmentShaderUrl();
#endif // !NO_OPENGL

    int order() const;
public slots:
    void setScale(const QVector3D &scale);
    void setPosition(const QVector3D& position);
    void setRotation(const QQuaternion& rotation);
    void setTransform(const QMatrix4x4& transform);
    void setClippingBox(const QRect &clippingBox);
    void setColorTransform(const QMatrix4x4& colorTransform);
    void setOpacity(qreal opacity);

    void setTextureUrl(const QUrl& url);

    void setOrder(int order);

    void resetScale();
    void resetPosition();
    void resetRotation();
    void resetTransform();
    void unsetClippingBox();
    void resetColor();
    void resetOpacity();
    void resetTextureUrl();

    inline void setXPos(qreal _x) { setPosition(QVector3D(_x, yPos(), zPos())); }
    inline void setYPos(qreal _y) { setPosition(QVector3D(xPos(), _y, zPos())); }
    inline void setZPos(qreal _z) { setPosition(QVector3D(xPos(), yPos(), _z)); }
public:
    inline qreal xPos() const { return position().x(); }
    inline qreal yPos() const { return position().y(); }
    inline qreal zPos() const { return position().z(); }
};
