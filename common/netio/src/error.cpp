
#include <netio/error.hpp>

#include <array>
#include <string>
#include <sstream>

#include <cassert>

namespace gbp {
namespace net {

/*************************************************************************************************/

struct item {
    item(const item &r) = delete;
    item& operator=(const item &r) = delete;

    item(item &&r) = default;
    item& operator=(item &&) = default;

    item()
        :m_fl{nullptr}
        ,m_ec{0}
        ,m_emsg(1024u, '\0')
    {}

    void clear() {
        m_fl = nullptr;
        m_ec = 0;
        m_emsg.clear();
    }

    friend std::ostream& operator<< (std::ostream &os, const item &i) {
        return os << "fl=\"" << i.m_fl << "\", ec=" << i.m_ec << ", emsg=\"" << i.m_emsg << "\"";
    }

    const char *m_fl;
    int m_ec;
    std::string m_emsg;
};

/*************************************************************************************************/

template<std::size_t N>
struct static_size_stack {
    static_size_stack(const static_size_stack &) = delete;
    static_size_stack& operator=(const static_size_stack &) = delete;
    static_size_stack(static_size_stack &&) = delete;
    static_size_stack& operator=(static_size_stack &&) = delete;

    using container_type = std::array<item, N>;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    static_size_stack()
        :m_storage{}
        ,m_pos{}
    {}
    virtual ~static_size_stack() {}

    void push(const char *fl, int ec, const char *emsg) {
        assert(m_pos < N);

        auto p = std::addressof(m_storage[m_pos]);
        p->m_fl = fl;
        p->m_ec = ec;
        p->m_emsg = emsg;

        ++m_pos;
    }
    void pop() { assert(m_pos > 0); m_storage[--m_pos].clear(); }

    std::size_t size() const { return m_pos; }
    bool empty() const { return 0 == size(); }

    void clear() { m_pos = 0; }

    iterator begin() { return m_storage.begin(); }
    iterator end() { return std::addressof(m_storage[m_pos]); }

    const_iterator begin() const { return m_storage.begin(); }
    const_iterator end() const { return std::addressof(m_storage[m_pos]); }

private:
    container_type m_storage;
    std::size_t m_pos;
};

/*************************************************************************************************/

struct errorstack::impl {
    enum { default_stack_size = 10 };

    impl()
        :m_stack{}
    {}

    void push(const char *fl, int ec, const char *emsg) { m_stack.push(fl, ec, emsg); }
    void pop() { m_stack.pop(); }

    bool empty() const { return m_stack.empty(); }
    std::size_t size() const { return m_stack.size(); }

    void clear() {
        for ( auto &it: m_stack ) {
            it.clear();
        }
        m_stack.clear();
    }

    const char* top_fl() const { return empty() ? nullptr : m_stack.begin()->m_fl; }
    int top_ec() const { return empty() ? -1 : m_stack.begin()->m_ec; }
    const char* top_emsg() const { return empty() ? nullptr : m_stack.begin()->m_emsg.c_str(); }

    static_size_stack<default_stack_size> m_stack;
};

/*************************************************************************************************/

errorstack::errorstack()
    :pimpl(new impl)
{}

errorstack::errorstack(const char *fl, int ec, const char *emsg)
    :pimpl(new impl)
{ push(fl, ec, emsg); }

errorstack::errorstack(errorstack &&r)
    :pimpl(r.pimpl)
{ r.pimpl = nullptr; }

errorstack::~errorstack()
{ delete pimpl; }

/*************************************************************************************************/

void errorstack::push(const char *fl, int ec, const char *emsg) { pimpl->push(fl, ec, emsg); }

void errorstack::pop() { pimpl->pop(); }

std::size_t errorstack::size() const { return pimpl->size(); }

bool errorstack::empty() const { return pimpl->empty(); }

void errorstack::clear() { pimpl->clear(); }

const char* errorstack::top_fl() const { return pimpl->top_fl(); }
int errorstack::top_ec() const { return pimpl->top_ec(); }
const char* errorstack::top_emsg() const { return pimpl->top_emsg(); }

/*************************************************************************************************/

std::ostream& operator<< (std::ostream &os, const errorstack &err) {
    for ( auto it = err.pimpl->m_stack.begin(); it != err.pimpl->m_stack.end(); ++it ) {
        os << "#" << std::distance(err.pimpl->m_stack.begin(), it) << ": " << *it << std::endl;
    }

    return os;
}

std::string errorstack::to_string() const {
    std::ostringstream os;

    os << *this;

    return os.str();
}

/*************************************************************************************************/

} // ns net
} // ns gbp
