#include <precompiled.h>
#include "geometry.h"
#include <QVector2D>
#include <cmath>

Triangle::Triangle()
    : m_a(0, 0)
    , m_b(0, 0)
    , m_c(0, 0)
{}

Triangle::Triangle(QPoint a, QPoint b, QPoint c)
    : m_a(a)
    , m_b(b)
    , m_c(c)
{}

Vertex2f::Vertex2f(float x, float y)
    : Vertex_tmp()
{
    this->x() = x;
    this->y() = y;
}

Vertex2f::Vertex2f(): Vertex_tmp() {}

Vertex3f::Vertex3f(): Vertex_tmp() {}
Vertex3f::Vertex3f(const Vertex3f &vertex)
    : Vertex_tmp()
{
    this->x() = vertex.x();
    this->y() = vertex.y();
    this->z() = vertex.z();
}
Vertex3f::Vertex3f(const float *arr)
    : Vertex_tmp(arr)
{}
Vertex3f::Vertex3f(float x, float y, float z)
    : Vertex_tmp()
{
    this->x() = x;
    this->y() = y;
    this->z() = z;
}
Vertex2f Vertex3f::toVertex2f() const { return Vertex2f(x(), y()); }

TexCoord::TexCoord(): Vertex_tmp() {}
TexCoord::TexCoord(const TexCoord &texcoord)
    : Vertex_tmp()
{
    this->u() = texcoord.u();
    this->v() = texcoord.v();
}
TexCoord::TexCoord(float u, float v)
    : Vertex_tmp()
{
    this->u() = u;
    this->v() = v;
}

Vertex3fuv::Vertex3fuv() : Vertex_tmp() {}
Vertex3fuv::Vertex3fuv(const Vertex3f &vertex, const TexCoord &texcoord)
    : Vertex_tmp()
{
    this->x() = vertex.x();
    this->y() = vertex.y();
    this->z() = vertex.z();
    this->u() = texcoord.u();
    this->v() = texcoord.v();
}
Vertex3fuv::Vertex3fuv(float x, float y, float z, float u, float v)
    : Vertex_tmp()
{
    this->x() = x;
    this->y() = y;
    this->z() = z;
    this->u() = u;
    this->v() = v;
}
Vertex3f &Vertex3fuv::asVertex3f() { return reinterpret_cast<Vertex3f&>(x()); }
TexCoord &Vertex3fuv::asTexCoord() { return reinterpret_cast<TexCoord&>(u()); }
const Vertex3f &Vertex3fuv::asVertex3f() const { return reinterpret_cast<const Vertex3f&>(*m_); }
const TexCoord &Vertex3fuv::asTexCoord() const { return reinterpret_cast<const TexCoord&>(*(m_+3)); }

// ------------------------------- Geometry -------------------------------------
Geometry::Geometry(int dimension): m_dimention(dimension) {}
Geometry::~Geometry() {}
const std::vector<Vertex3fuv> &Geometry::vertices() const { return m_vertices; }
const std::vector<gbp_u16> &Geometry::indices() const { return m_indices; }
Vertex3f &Geometry::vertex(int idx) { return m_vertices[idx].asVertex3f(); }
TexCoord &Geometry::texcoord(int idx) { return m_vertices[idx].asTexCoord(); }
gbp_u16 &Geometry::index(int idx) { return m_indices[idx]; }
const Vertex3f &Geometry::vertex(int idx) const { return m_vertices[idx].asVertex3f(); }
const TexCoord &Geometry::texcoord(int idx) const { return m_vertices[idx].asTexCoord(); }
gbp_u16 Geometry::index(int idx) const { return m_indices[idx]; }
int Geometry::verticesCount() const { return m_vertices.size(); }
int Geometry::indicesCount() const { return m_indices.size(); }



// ------------------------------- Quad -------------------------------------
Quad::Quad(int w, int h, float hx, float hy)
    : Geometry()
{
    m_vertices.push_back(Vertex3fuv(w*(0.f-hx), h*(0.f-hy), 0.f, 0.f, 0.f));
    m_vertices.push_back(Vertex3fuv(w*(1.f-hx), h*(0.f-hy), 0.f, 1.f, 0.f));
    m_vertices.push_back(Vertex3fuv(w*(1.f-hx), h*(1.f-hy), 0.f, 1.f, 1.f));
    m_vertices.push_back(Vertex3fuv(w*(0.f-hx), h*(1.f-hy), 0.f, 0.f, 1.f));

    m_indices << 0 << 1 << 2;//     0________1
    m_indices << 0 << 2 << 3;//     |        |   4______5
                             //     |        |   |      |
                             //     |        |   |      |
                             //     |________|   |______|
                             //     3        2   7      6
}

size_t Quad::vertexBufferSize() const {
    return m_vertices.size()*sizeof(Vertex3fuv);
}

size_t Quad::indexBufferSize() const {
    return m_indices.size()*sizeof(gbp_u16);
}


// ------------------------------- Cube -------------------------------------
Cube::Cube() : Geometry()
{
    m_vertices.push_back(Vertex3fuv(0.f, 0.f, 0.f, 0.f, 0.f));
    m_vertices.push_back(Vertex3fuv(1.f, 0.f, 0.f, 1.f, 0.f));
    m_vertices.push_back(Vertex3fuv(1.f, 1.f, 0.f, 1.f, 1.f));
    m_vertices.push_back(Vertex3fuv(0.f, 1.f, 0.f, 0.f, 1.f));

    m_vertices.push_back(Vertex3fuv(0.f, 0.f, 1.f, 1.f, 1.f));
    m_vertices.push_back(Vertex3fuv(1.f, 0.f, 1.f, 2.f, 1.f));
    m_vertices.push_back(Vertex3fuv(1.f, 1.f, 1.f, 2.f, 2.f));
    m_vertices.push_back(Vertex3fuv(0.f, 1.f, 1.f, 1.f, 2.f));

    m_indices << 0 << 1 << 2;
    m_indices << 0 << 2 << 3;

    m_indices << 0 << 3 << 7;
    m_indices << 0 << 7 << 4;

    m_indices << 0 << 4 << 5;
    m_indices << 0 << 5 << 1;

    m_indices << 6 << 7 << 4;
    m_indices << 6 << 4 << 5;

    m_indices << 6 << 2 << 3;
    m_indices << 6 << 3 << 7;

    m_indices << 6 << 2 << 1;
    m_indices << 6 << 1 << 5;
}

size_t Cube::vertexBufferSize() const {
    return m_vertices.size()*sizeof(Vertex3fuv);
}

size_t Cube::indexBufferSize() const {
    return m_indices.size()*sizeof(gbp_u16);
}



// ------------------------------- Pyramid -------------------------------------
Pyramid::Pyramid() : Geometry()
{
    m_vertices.push_back(Vertex3fuv(0.f, 0.f, 0.f, 0.f, 0.f));
    m_vertices.push_back(Vertex3fuv(1.f, 0.f, 0.f, 1.f, 0.f));
    m_vertices.push_back(Vertex3fuv(1.f, 1.f, 0.f, 1.f, 1.f));
    m_vertices.push_back(Vertex3fuv(0.f, 1.f, 0.f, 0.f, 1.f));

    m_vertices.push_back(Vertex3fuv(0.5f, 0.5f, 1.f, 0.5f, 0.5f));

    m_indices << 0 << 1 << 2;
    m_indices << 0 << 2 << 3;

    m_indices << 4 << 0 << 1;
    m_indices << 4 << 1 << 2;
    m_indices << 4 << 2 << 3;
    m_indices << 4 << 3 << 0;
}

size_t Pyramid::vertexBufferSize() const {
    return m_vertices.size()*sizeof(Vertex3fuv);
}

size_t Pyramid::indexBufferSize() const {
    return m_indices.size()*sizeof(gbp_u16);
}



// ------------------------------- Surface -------------------------------------
void Surface::pushQuadIndices(int quadNum)
{
    gbp_i16 idx0 = quadNum + quadNum/(m_cols-1);
    gbp_i16 idx1 = idx0 + 1;
    gbp_i16 idx2 = idx0 + m_cols;
    gbp_i16 idx3 = idx2 + 1;
    if (idx1 % m_cols != 0)
    {
        m_indices << idx0 << idx1 << idx3;
        m_indices << idx0 << idx3 << idx2;
    }
}

Surface::Surface(QSize gridSize, int w, int h, float hx, float hy)
    : Geometry()
    , m_rows(gridSize.height())
    , m_cols(gridSize.width())
{
    QPointF singleStep(float(w)/m_cols, float(h)/m_rows);
    QPointF offset(hx*w, hy*h);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            m_vertices.push_back(Vertex3fuv(j*singleStep.x() - offset.x(), i*singleStep.y() - offset.y(), 0, float(j)/(m_cols-1), float(i)/(m_rows-1)));
        }
    }
    int quadCount = (m_rows - 1) * (m_cols - 1);
//    int trianglesCount = quadCount * 2;
    // 0____1____2____3
    // |  I |  II| III|
    // 4____5____6____7
    // |  IV|  V |  VI|
    // 8____9___10____11
    // |VII |VIII|  IX|
    // |____|____|____|
    // 12   13   14   15

    for (int quad = 0; quad < quadCount; ++quad)
    {
        pushQuadIndices(quad);
    }
}

Surface *Surface::create(QSize gridSize, QSize size, QPointF hotspot)
{
    return new Surface(gridSize, size.width(), size.height(), hotspot.x(), hotspot.y());
}

size_t Surface::vertexBufferSize() const {
    return m_vertices.size()*sizeof(Vertex3fuv);
}

size_t Surface::indexBufferSize() const {
    return m_indices.size()*sizeof(gbp_u16);
}

Vertex3f &Surface::vertex(int row, int col)
{
    return Geometry::vertex(row*m_cols + col);
}

TexCoord &Surface::texcoord(int row, int col)
{
    return Geometry::texcoord(row*m_cols + col);
}

// ---------------------------------- Circle -----------------------------
Circle::Circle(float radius, int verticesCount)
    : Geometry()
{
    float singleStep = 2 * 3.1415926f / verticesCount;

    m_vertices.push_back(Vertex3fuv(0, 0, 0, 0.5f, 0.5f));

//    x^2 + y^2 = R^2;

    float sinValue, cosValue;
    QVector2D radiusVector;
    QVector2D normalizedRadiusVector;
    for (int i = 1; i < verticesCount; ++i)
    {
        ns_util::sincosf(singleStep*i, &sinValue, &cosValue);
        radiusVector.setX(radius*sinValue);
        radiusVector.setY(radius*cosValue);
        normalizedRadiusVector = radiusVector.normalized();
        m_vertices.push_back(Vertex3fuv(radiusVector.x(), radiusVector.y(), 0, normalizedRadiusVector.x(), normalizedRadiusVector.y()));

        m_indices << 0 << (i + 1) << i;
    }
    m_indices << 0 << 1 << verticesCount-1;
}

size_t Circle::vertexBufferSize() const {
    return m_vertices.size()*sizeof(Vertex3fuv);
}

size_t Circle::indexBufferSize() const {
    return m_indices.size()*sizeof(gbp_u16);
}



// ----------------------------------- Rounded Rect ---------------------------------------
RoundedRect::RoundedRect(QSize size, float radius, int verticesCount)
    : Geometry()
{
    static const QVector2D one(1, 1);
    verticesCount += 5;
    float singleStep = 2 * 3.1415926f / verticesCount;

    int quater = verticesCount / 4;

    m_vertices.push_back(Vertex3fuv(0, 0, 0, 0.5f, 0.5f));
    float sinValue, cosValue;
    QVector2D radiusVector;
    QVector2D normalizedRadiusVector;

    float xOffset(size.width()/2 - radius);
    float yOffset(size.height()/2 - radius);

    for (int i = 1; i < verticesCount; ++i)
    {
        ns_util::sincosf(singleStep*(i /*+ quater*part*/), &sinValue, &cosValue);
        radiusVector.setX(radius*sinValue + xOffset);
        radiusVector.setY(radius*cosValue + yOffset);
        normalizedRadiusVector = (radiusVector.normalized() + one) * 0.5f;
        m_vertices.push_back(Vertex3fuv(radiusVector.x(), radiusVector.y(), 0, (radiusVector.x() + (size.width()/2) )/size.width(), (radiusVector.y() + size.height()/2)/size.height()));

        m_indices << 0 << (i + 1) << i;

        if (i % quater == 0)
        {
            if (i/quater % 2 == 0) {
                xOffset = -xOffset;
            } else {
                yOffset = -yOffset;
            }
        }
    }
    m_indices << 0 << 1 << verticesCount-1;
}

size_t RoundedRect::vertexBufferSize() const {
    return m_vertices.size()*sizeof(Vertex3fuv);
}

size_t RoundedRect::indexBufferSize() const {
    return m_indices.size()*sizeof(gbp_u16);
}
