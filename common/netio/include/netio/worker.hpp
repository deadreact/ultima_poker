
#ifndef __gbp__net__worker_hpp
#define __gbp__net__worker_hpp

#include <functional>

namespace gbp {
namespace net {

/**************************************************************************/

struct worker {
    worker(const worker &r) = delete;
    worker& operator= (const worker &r) = delete;

    worker(worker &&r) = default;
    worker& operator= (worker &&r) = default;

    worker(void *impl_data = nullptr);
    virtual ~worker();

    void run();
    void post(std::function<void()> f);

    // возвращает указатель на io_service.
    void* get_handle();

private:
    struct impl;
    impl *pimpl;
};

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__worker_hpp
