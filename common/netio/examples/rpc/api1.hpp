
#pragma once

#include <cstdint>

namespace gbp {
namespace api1 {
struct request {
    virtual void send_idx(std::uint32_t idx) = 0;

    virtual ~request() {}
};

struct reply {
    virtual void recv_idx(std::uint32_t idx) = 0;

    virtual ~reply() {}
};
} // na api1
} // ns gbp
