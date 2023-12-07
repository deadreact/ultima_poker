
#include <netio/profdata.hpp>

#include <cinttypes>
#include <cstdio>

#include <sstream>

namespace gbp {
namespace net {

/**************************************************************************/

std::string profdata::dump(const profdata *data) const {
    (void)data;
    const auto d = data ? data : this;
    const auto t = std::chrono::duration_cast<std::chrono::nanoseconds>(d->m_time);
    const auto h = std::chrono::duration_cast<std::chrono::hours>(t).count();
    const auto m = std::chrono::duration_cast<std::chrono::minutes>(t % std::chrono::hours(1)).count();
    const auto s = std::chrono::duration_cast<std::chrono::seconds>(t % std::chrono::minutes(1)).count();
    const auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(t % std::chrono::seconds(1)).count();
    const auto micro = std::chrono::duration_cast<std::chrono::microseconds>(t % std::chrono::milliseconds(1)).count();

    char timebuf[128] = {0};
    snprintf(timebuf, sizeof(timebuf), "%02" PRId64 "h:%02" PRId64 "m:%02" PRId64 "s-%03" PRId64 "ms.%03" PRId64 "us", h, m, s, milli, micro);

    std::ostringstream ss;
    ss  << "called=" << d->m_called
        << ", data=" << d->m_data_size
        << ", avg-data=" << d->m_data_size/(d->m_called+1)
        << ", avg-time=" << (t.count()/1000.0)/(double)(d->m_called+1) << "us"
        << ", total-time=" << timebuf;
    ;

    return ss.str();
}

/**************************************************************************/

} // ns net
} // ns gbp
