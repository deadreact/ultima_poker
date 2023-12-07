
#ifndef __gbp__net__invokers_hpp
#define __gbp__net__invokers_hpp

#include <netio/profdata.hpp>

#include <yas/mem_streams.hpp>
#include <yas/binary_iarchive.hpp>
#include <yas/serializers/std_types_serializers.hpp>

#include <tuple>
#include <functional>
#include <type_traits>

//#include <iostream>

namespace gbp {
namespace net {
namespace detail {

/***************************************************************************/

template<std::size_t... idxs>
struct index_list {
    typedef index_list<idxs..., sizeof...(idxs)> next;
};

template<std::size_t idx>
struct make_index_list {
    typedef typename make_index_list<idx - 1>::type::next type;
};

template<>
struct make_index_list<0> {
    typedef index_list<> type;
};

/***************************************************************************/

struct i_invoker {
    i_invoker()
        :m_profdata{}
    {}

    virtual void invoke(const char *ptr, std::size_t size) = 0;
    virtual void local_invoke(const void *args) = 0;

    void update_profdata(std::size_t data_size, profdata::time_type time)
    { m_profdata.update(data_size, time); }

    const profdata& get_profdata() const
    { return m_profdata; }

private:
    profdata m_profdata;
};

using i_invoker_ptr = std::shared_ptr<detail::i_invoker>;

/***************************************************************************/

template<typename... Args>
struct args_pack {
    using args = std::tuple<
        typename std::remove_cv<
            typename std::remove_reference<Args>::type
        >::type...
    >;
};
template<typename... Args>
struct args_pack<std::tuple<Args...>> {
    using args = std::tuple<
        typename std::remove_cv<
            typename std::remove_reference<Args>::type
        >::type...
    >;
};

template<typename Derived, typename... Args>
struct invoker_traits : i_invoker {
    using args_t         = typename args_pack<Args...>::args;
    using indexes_pack_t = typename detail::make_index_list<
        std::tuple_size<args_t>::value
    >::type;

    invoker_traits(Derived *derived)
        :i_invoker()
        ,derived(derived)
    {}

    virtual void invoke(const char *ptr, std::size_t size) override {
        args_t args;

        if ( std::tuple_size<typename args_pack<Args...>::args>::value ) {
            /** deserializing */
            yas::mem_istream is(ptr, size);
            yas::binary_iarchive<yas::mem_istream> ia(is, yas::no_header);
            ia & args;
        } else {
            assert(size == 0);
        }

        /** invoke */
        derived->invoke(args, indexes_pack_t());
    }

    virtual void local_invoke(const void *args) override {
        const std::tuple<const Args&...> *targs = static_cast<const std::tuple<const Args&...>*>(args);

        derived->local_invoke(*targs, indexes_pack_t());
    }

    Derived *derived;
};

template<typename Obj, typename... Args>
struct method_invoker : invoker_traits<method_invoker<Obj, Args...>, Args...> {
    method_invoker(Obj *obj, void(Obj::*func)(Args...))
        :invoker_traits<method_invoker<Obj, Args...>, Args...>(this)
        ,obj(obj)
        ,func(func)
    {}

    template<typename args_pack, std::size_t... indices>
    void invoke(const args_pack &args, detail::index_list<indices...>) {
        (obj->*func)(std::get<indices>(args)...);
    }

    template<typename Tuple, std::size_t... indices>
    void local_invoke(const Tuple &targs, detail::index_list<indices...>) {
        (obj->*func)(std::get<indices>(targs)...);
    }

    Obj *obj;
    void (Obj::*func)(Args...);
};

/***************************************************************************/

template<typename F, typename Tuple>
struct callable_invoker : invoker_traits<callable_invoker<F, Tuple>, Tuple> {
    callable_invoker(F f)
        :invoker_traits<callable_invoker<F, Tuple>, Tuple>(this)
        ,func(std::move(f))
    {}

    template<typename Args, std::size_t... indices>
    void invoke(const Args &args, detail::index_list<indices...>) {
        func(std::get<indices>(args)...);
    }

    template<typename CTuple, std::size_t... indices>
    void local_invoke(const CTuple &targs, detail::index_list<indices...>) {
        func(std::get<indices>(targs)...);
    }

    F func;
};

/***************************************************************************/

} // ns detail
} // ns net
} // ns gbp

#endif // __gbp__net__invokers_hpp
