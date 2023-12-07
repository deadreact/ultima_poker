
#ifndef __gbp__net__error_hpp
#define __gbp__net__error_hpp

#include <ostream>
#include <string>

namespace gbp {
namespace net {

/*************************************************************************************************/

struct errorstack {
    errorstack(const errorstack &r) = delete;
    errorstack &operator=(const errorstack &r) = delete;

    errorstack();
    errorstack(const char *fl, int ec, const char *emsg);
    errorstack(errorstack &&r);
    virtual ~errorstack();

    void push(const char *fl, int ec, const char *emsg);
    void pop();

    std::size_t size() const;
    bool empty() const;
    void clear();

    const char* top_fl() const;
    int top_ec() const;
    const char* top_emsg() const;

    friend std::ostream &operator<<(std::ostream &os, const errorstack &err);
    std::string to_string() const;

private:
    struct impl;
    impl *pimpl;
};

/*************************************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__error_hpp
