
#ifndef __gbp__net__preallocated_handler_invoker_hpp
#define __gbp__net__preallocated_handler_invoker_hpp

#include <boost/asio/handler_invoke_hook.hpp>
#include <memory>

namespace gbp {
namespace net {

/***************************************************************************/

template<typename Allocator, typename H>
struct preallocated_handler_invoker {
    typedef preallocated_handler_invoker<Allocator, H> this_type;

    preallocated_handler_invoker(Allocator &allocator, H h)
        :allocator(std::addressof(allocator))
        ,handler(std::move(h))
    {}

    friend void *asio_handler_allocate(std::size_t size, this_type *ctx) {
        return ctx->allocator->allocate(size);
    }

    friend void asio_handler_deallocate(void *ptr, std::size_t, this_type *ctx) {
        ctx->allocator->deallocate(ptr);
    }

    template<typename F>
    friend void asio_handler_invoke(F &function, this_type *context) {
        using boost::asio::asio_handler_invoke;
        asio_handler_invoke(function, std::addressof(context->handler));
    }

    template<typename... Args>
    void operator()(Args &&... args) {
        handler(std::forward<Args>(args)...);
    }

private:
    Allocator *allocator;
    H handler;
};

/***************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__preallocated_handler_invoker_hpp
