#pragma once
#include <QObject>
#include <QPoint>
#include <vector>
#include <cstring>
#include <QSize>
#include <QRect>

#include "basic.h"

// ***************************** operators ******************************* //
inline std::vector<gbp_u16>& operator<<(std::vector<gbp_u16>& indices, gbp_u16 index) {
    indices.push_back(index);
    return indices;
}
// **************************************************************************

class Triangle
{
private:
    QPoint m_a, m_b, m_c;
public:
    Triangle();
    Triangle(QPoint a, QPoint b, QPoint c);
};

template <typename T, int Dimension>
struct Vertex_tmp
{
    typedef T value_type;
    static const int value_count = Dimension;

    T m_[Dimension];

    Vertex_tmp() {
        memset(m_, 0, Dimension * sizeof(T));
    }
    Vertex_tmp(T value) {
        for (int i = 0; i < Dimension; ++i) {
            m_[i] = value;
        }
    }
    Vertex_tmp(const T array[]) {
        for (int i = 0; i < Dimension; ++i) {
            m_[i] = array[i];
        }
    }
    inline T operator[](int idx) const { return m_[idx]; }
    inline T& operator[](int idx) { return m_[idx]; }
    inline T* operator&() { return &m_; }
};

//**************** VERTEX_2F *******************
struct Vertex2f : public Vertex_tmp<float, 2>
{
public:
    Vertex2f();
    Vertex2f(float x, float y);
    inline float x() const;
    inline float& x();
    inline float y() const;
    inline float& y();
};

inline float Vertex2f::x() const { return *m_; }
inline float &Vertex2f::x() { return *m_; }
inline float Vertex2f::y() const { return *(m_ + 1); }
inline float &Vertex2f::y() { return *(m_ + 1); }

//**************** VERTEX_3F *******************
struct Vertex3f : public Vertex_tmp<float, 3>
{
public:
    Vertex3f();
    Vertex3f(const Vertex3f& vertex);
    Vertex3f(const float* arr);

    Vertex3f(float x, float y, float z);
    inline float x() const;
    inline float& x();
    inline float y() const;
    inline float& y();
    inline float z() const;
    inline float& z();

    Vertex2f toVertex2f() const;
};

inline float  Vertex3f::x() const { return *m_; }
inline float  Vertex3f::y() const { return *(m_ + 1); }
inline float  Vertex3f::z() const { return *(m_ + 2); }
inline float &Vertex3f::x()       { return *m_; }
inline float &Vertex3f::y()       { return *(m_ + 1); }
inline float &Vertex3f::z()       { return *(m_ + 2); }


struct TexCoord : public Vertex_tmp<float, 2>
{
    TexCoord();
    TexCoord(const TexCoord& texcoord);
    TexCoord(float u, float v);
    inline float u() const;
    inline float& u();
    inline float v() const;
    inline float& v();
};
//**************** TEXCOORD *******************

inline float TexCoord::u() const { return *m_; }
inline float &TexCoord::u() { return *m_; }
inline float TexCoord::v() const { return *(m_ + 1); }
inline float &TexCoord::v() { return *(m_ + 1); }

//**************** VERTEX_3FUV *******************
struct Vertex3fuv : public Vertex_tmp<float, 5> {
    Vertex3fuv();
    Vertex3fuv(const Vertex3f& vertex, const TexCoord& texcoord);
    Vertex3fuv(float x, float y, float z, float u, float v);
    inline float x() const;
    inline float y() const;
    inline float z() const;
    inline float u() const;
    inline float v() const;
    inline float& x();
    inline float& y();
    inline float& z();
    inline float& u();
    inline float& v();

    Vertex3f& asVertex3f();
    TexCoord& asTexCoord();
    const Vertex3f& asVertex3f() const;
    const TexCoord& asTexCoord() const;
};
inline float Vertex3fuv::x() const { return *m_; }
inline float Vertex3fuv::y() const { return *(m_ + 1); }
inline float Vertex3fuv::z() const { return *(m_ + 2); }
inline float Vertex3fuv::u() const { return *(m_ + 3); }
inline float Vertex3fuv::v() const { return *(m_ + 4); }
inline float &Vertex3fuv::x() { return *m_; }
inline float &Vertex3fuv::y() { return *(m_ + 1); }
inline float &Vertex3fuv::z() { return *(m_ + 2); }
inline float &Vertex3fuv::u() { return *(m_ + 3); }
inline float &Vertex3fuv::v() { return *(m_ + 4); }


// ------------------------------- Geometry -------------------------------------
class Geometry
{
    Q_GADGET
    Q_ENUMS(Shape)
public:
    enum Shape
    {
        Quad,
        Cube,
        Pyramid,
        Surface
    };
    Q_ENUM(Shape);

protected:
    const int m_dimention;
    std::vector<Vertex3fuv> m_vertices;
    std::vector<gbp_u16> m_indices;
public:
    Geometry(int dimension=3);
    virtual ~Geometry();
    const std::vector<Vertex3fuv>& vertices() const;
    const std::vector<gbp_u16>& indices() const;

    Vertex3f& vertex(int idx);
    TexCoord& texcoord(int idx);
    gbp_u16& index(int idx);

    const Vertex3f& vertex(int idx) const;
    const TexCoord& texcoord(int idx) const;
    gbp_u16 index(int idx) const;

    int verticesCount() const;
    int indicesCount() const;

    virtual size_t vertexBufferSize() const = 0;
    virtual size_t indexBufferSize() const = 0;
};


// ------------------------------- Quad -------------------------------------
class Quad : public Geometry
{
private:
    Quad(int w, int h, float hx, float hy);
public:

    inline static Quad* create();
    inline static Quad* create(QSize size, QPointF hotspot=QPointF(0.5, 0.5));

    virtual size_t vertexBufferSize() const override;
    virtual size_t indexBufferSize() const override;
};

inline Quad *Quad::create() {
    return Quad::create(QSize(100, 100));
}

inline Quad *Quad::create(QSize size, QPointF hotspot) {
    return new Quad(size.width(), size.height(), hotspot.x(), hotspot.y());
}


// ------------------------------- Cube -------------------------------------
class Cube : public Geometry
{
public:
    Cube();
    virtual size_t vertexBufferSize() const override;
    virtual size_t indexBufferSize() const override;
};


// ------------------------------- Pyramid -------------------------------------
class Pyramid : public Geometry
{
public:
    Pyramid();

    virtual size_t vertexBufferSize() const override;
    virtual size_t indexBufferSize() const override;
};


// ------------------------------- Surface -------------------------------------
class Surface : public Geometry
{
    int m_rows;
    int m_cols;
private:
    void pushQuadIndices(int quadNum);

    Surface(QSize gridSize, int w, int h, float hx, float hy);
public:

    static Surface* create(QSize gridSize, QSize size, QPointF hotspot=QPointF(0.5, 0.5));

    virtual size_t vertexBufferSize() const override;
    virtual size_t indexBufferSize() const override;

    Vertex3f& vertex(int row, int col);
    TexCoord& texcoord(int row, int col);
};

// -------------------------------------------- Circle ------------------------------------------

class Circle : public Geometry
{
private:

public:
    Circle(float radius=1.f, int verticesCount=64);

    virtual size_t vertexBufferSize() const override;
    virtual size_t indexBufferSize() const override;
};

// ----------------------------------------- Rounded Rect -----------------------------------------
class RoundedRect : public Geometry
{
private:

public:
    RoundedRect(QSize size, float radius=1.f, int verticesCount=128);

    virtual size_t vertexBufferSize() const override;
    virtual size_t indexBufferSize() const override;
};
