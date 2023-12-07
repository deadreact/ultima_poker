#pragma once

template <typename Type>
class Accessor
{
private:
    union {
        Type* m_obj;
        const Type* m_constObj;
    };
public:
    Accessor(): m_obj(nullptr) {}
    Accessor(Type* obj): m_obj(obj) {}
    Accessor(const Type* obj): m_constObj(obj) {}
    inline bool isValid() const { return m_obj != nullptr; }

    inline       Type   copy() const   { return *m_obj; }
    inline       Type&  ref()          { return *m_obj; }
    inline const Type&  ref()  const   { return *m_obj; }
    inline       Type*  ptr()          { return m_obj; }
    inline const Type*  ptr()  const   { return m_obj; }
    inline       Type*& origin()       { return m_obj; }
    inline const Type*& origin() const { return m_obj; }

    const Type* operator->() const {
        return m_constObj;
    }
};

template <typename T>
Accessor<T> accessor(const T& obj) {
    return Accessor<T>(&obj);
}
