
#include <netio/distributor.hpp>

#include <iostream>

struct noncopyable {
    noncopyable() {}
    virtual ~noncopyable() {}

    noncopyable(const noncopyable &) = delete;
    noncopyable& operator= (const noncopyable &) = delete;

    template<typename Ar>
    void serialize(Ar &) {}

    int v;
};

bool m0_called=false;
bool m1_called=false;
int m1_v=0;
bool m2_called=false;
int m2_v=0;
int m2_vv=0;

struct type {
    void m0() {m0_called=true;}
    void m1(int v) {m1_called=true; m1_v=v;}
    void m2(int v, const noncopyable &n) {m2_called=true;m2_v=v;m2_vv=n.v;}
};

int main() {
    gbp::net::distributor distr;
    gbp::net::object_registry_ptr reg = distr.create_registry("ns::cls");

    type t;
    reg->on("m0", &t, &type::m0);
    reg->on("m1", &t, &type::m1);
    reg->on("m2", &t, &type::m2);

    distr.local_invoke("ns::cls", "m0");
    assert(m0_called);

    distr.local_invoke("ns::cls", "m1", 33);
    assert(m1_called && m1_v == 33);

    noncopyable nc;
    nc.v = 55;
    distr.local_invoke("ns::cls", "m2", 44, nc);
    assert(m2_called && m2_v == 44 && m2_vv == 55);
}
