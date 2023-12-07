
#ifndef __gbp__net__handler_allocator_hpp
#define __gbp__net__handler_allocator_hpp

#include <memory>
#include <type_traits>

namespace gbp {
namespace net {

/***************************************************************************/

template<std::size_t alloc_size>
struct handler_allocator {
    handler_allocator(const handler_allocator &) = delete;
    handler_allocator &operator=(const handler_allocator &) = delete;

    handler_allocator()
        :in_use(false)
    {}

    void *allocate(std::size_t size) {
        if (!in_use && size <= alloc_size) {
            in_use = true;
            return std::addressof(storage);
        }

        return ::operator new(size);
    }

    void deallocate(void *pointer) {
        if (std::addressof(storage) == pointer) {
            in_use = false;
        } else {
            ::operator delete(pointer);
        }
    }

private:
    typename std::aligned_storage<alloc_size>::type storage;
    bool in_use;
};

/***************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__handler_allocator_hpp
