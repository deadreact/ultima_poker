
#ifndef __gbp__net__make_preallocated_handler_hpp
#define __gbp__net__make_preallocated_handler_hpp

#include <netio/preallocated_handler_invoker.hpp>

namespace gbp {
namespace net {

/***************************************************************************/

template<typename Allocator, typename F>
preallocated_handler_invoker <Allocator, F>
make_preallocated_handler(Allocator &allocator, F f) {
    return preallocated_handler_invoker<Allocator, F>(allocator, std::move(f));
}

/***************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__make_preallocated_handler_hpp
