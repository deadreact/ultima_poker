
#ifndef __gbp__net__object_registry_hpp
#define __gbp__net__object_registry_hpp

#include <netio/invokers.hpp>
#include <netio/profdata.hpp>

#include <memory>

namespace gbp {
namespace net {

/***************************************************************************/

struct object_registry {
private:
    template<typename T>
    struct is_callable_impl {
        struct Fallback { void operator()(); };
        struct Derived: T, Fallback {};
        template<typename U, U> struct Check;

        typedef char(&yes)[1];
        typedef char(&no)[2];
        template<typename> static yes test(...);
        template<typename C> static no test(Check<void (Fallback::*)(), &C::operator()> *);

        static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
    };

    template<typename T>
    struct is_callable: std::conditional<
            std::is_class<T>::value,
            is_callable_impl<T>,
            std::false_type
        >::type
    {};

    template<typename T>
    struct function_traits: function_traits<decltype(&T::operator())> {};
    template<typename F, typename... Args>
    struct function_traits<void (F::*)(Args...) const> {
        using args = std::tuple<
            typename std::remove_cv<
                typename std::remove_reference<Args>::type
            >::type...
        >;
    };

public:
    object_registry(const object_registry &) = delete;
    object_registry &operator=(const object_registry &) = delete;

    object_registry(const std::string &object);
    virtual ~object_registry();

    const std::string &object() const;

    bool registered(const std::string &method) const;

    detail::i_invoker_ptr get_invoker(const std::string &proc) const;

    template<typename Obj, typename... Args>
    void on(std::string mf, Obj *object, void(Obj::*func)(Args...)) {
        if (registered(mf)) {
            const std::string msg = "member function \"" + mf + "\" already registered";
            throw std::runtime_error(msg);
        }

        insert(std::move(mf), detail::i_invoker_ptr(new detail::method_invoker<Obj, Args...>(object, func)));
    }

    template<typename F>
    typename std::enable_if<is_callable<F>::value, void>::type
    on(std::string mf, F f) {
        static_assert(is_callable<F>::value, "F is not callable type");
        if (registered(mf)) {
            const std::string msg = "callable \"" + mf + "\" already registered";
            throw std::runtime_error(msg);
        }

        auto ptr = detail::i_invoker_ptr(
            new detail::callable_invoker<
                 F
                ,typename function_traits<decltype(f)>::args
            >(std::move(f))
        );
        insert(std::move(mf), std::move(ptr));
    }

    using get_profdata_cb = std::function<void(const std::string &proc, const profdata &data)>;
    void get_profdata(const get_profdata_cb &f) const;

private:
    void insert(std::string mf, detail::i_invoker_ptr ptr);

    struct impl;
    std::unique_ptr<impl> m_pimpl;
};

typedef std::shared_ptr<object_registry> object_registry_ptr;

/***************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__object_registry_hpp
