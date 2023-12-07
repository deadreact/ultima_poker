#pragma once

//-------------------------------------------------------------//
//*********************  BufferObject  ************************//
//-------------------------------------------------------------//



struct BufferObject {
    unsigned int vbo;
    unsigned int ibo;
    unsigned int* operator&() { return &vbo; }
};

//-------------------------------------------------------------//
//*****************  GenericIndexPattern  *********************//
//-------------------------------------------------------------//
template <typename T, int n>
struct GenericIndexPattern
{
    typedef GenericIndexPattern<T, n> TIndices;
    T m_indices[n];

    GenericIndexPattern() { memset(m_indices, 0, sizeof(T)*n); }
    GenericIndexPattern(const T arr[]) { memcpy(m_indices, arr, sizeof(T)*n); }

    T* operator&() { return m_indices; }
    const T* operator &() const { return m_indices; }
    T& operator[](int idx) { return m_indices[idx];}
    const T& operator[](int idx) const { return m_indices[idx];}


    TIndices operator+(int offset) const
    {
        TIndices indexPattern;
        for ( int i = 0; i < n; i++ )
        {
            indexPattern.m_indices[i] = m_indices[i] + offset;
        }
        return indexPattern;
    }
};


//-------------------------------------------------------------//
//***********************  Vertex3DUV  ************************//
//-------------------------------------------------------------//

struct Vertex3DUV {
    float x, y, z;
    float u, v;
    Vertex3DUV(): x(0), y(0), z(0), u(0), v(0) {}
    Vertex3DUV(float x, float y, float z, float u, float v)
        : x(x), y(y), z(z), u(u), v(v) {}

    float* operator&() { return &x; }
};

class VertexIterator2D
{
private:
    Vertex3DUV& m_vertex;
    //unsigned int m_textureNumber;
public:
    VertexIterator2D(Vertex3DUV& v) : m_vertex(v) {}

    Vertex3DUV& current() const
    {
        return m_vertex;
    }
    Vertex3DUV& operator()(float x, float y, unsigned int texN=0)
    {
        m_vertex.x = x;
        m_vertex.y = y;
        m_vertex.u = texN;
        m_vertex.v = 0;
        return m_vertex;
    }
    Vertex3DUV& left(float offset)
    {
        m_vertex.x -= offset;
        m_vertex.u -= 1.f;
        return m_vertex;
    }
    Vertex3DUV& right(float offset)
    {
        m_vertex.x += offset;
        m_vertex.u += 1.f;
        return m_vertex;
    }
    Vertex3DUV& up(float offset)
    {
        m_vertex.y -= offset;
        m_vertex.v -= 1.f;
        return m_vertex;
    }
    Vertex3DUV& down(float offset)
    {
        m_vertex.y += offset;
        m_vertex.v += 1.f;
        return m_vertex;
    }
};

template <typename VertexType>
struct Surface_old
{
    VertexType* m_vertices;
    unsigned short* m_indices;

    unsigned int m_verticesCount;
    unsigned int m_indicesCount;

    Surface_old(): m_vertices(nullptr), m_indices(nullptr), m_verticesCount(0), m_indicesCount(0) {}
    Surface_old(unsigned int verticesCount, unsigned int indicesCount)
        : m_vertices(new VertexType[verticesCount])
        , m_indices(new unsigned short[indicesCount])
        , m_verticesCount(verticesCount)
        , m_indicesCount(indicesCount)
    {
    }

    ~Surface_old()
    {
        if (m_vertices != nullptr) {
            delete[] m_vertices;
        }
        if (m_indices != nullptr) {
            delete[] m_indices;
        }
    }
};

