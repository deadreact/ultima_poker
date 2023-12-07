
#ifndef __gbp__net__profdata_hpp
#define __gbp__net__profdata_hpp

#include <cstdint>

#include <chrono>
#include <string>
#include <functional>

namespace gbp {
namespace net {

/**************************************************************************/

struct profdata {
    using clock_type = std::chrono::high_resolution_clock;
    using time_type  = clock_type::duration;

    profdata()
        :m_called{}
        ,m_data_size{}
        ,m_time{}
    {}

    std::size_t called()    const { return m_called; }
    std::size_t data_size() const { return m_data_size; }
    time_type   time()      const { return m_time; }

    void update(std::size_t data_size, time_type time) {
        ++m_called;
        m_data_size += data_size;
        m_time += time;
    }
    void update(std::size_t called, std::size_t data_size, time_type time) {
        m_called += called;
        m_data_size += data_size;
        m_time += time;
    }

    std::string dump(const profdata *data = nullptr) const;

private:
    std::size_t m_called;
    std::size_t m_data_size;
    time_type m_time;
};

/**************************************************************************/

using get_profdata_cb = const std::function<
    void(const std::string &object, const std::string &proc, const profdata &data)
>;

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__profdata_hpp
