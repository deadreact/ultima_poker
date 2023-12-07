
#ifndef __gbp__hostinfo_h__
#define __gbp__hostinfo_h__

#include <string>
#include <vector>
#include <ostream>

namespace gbp {

/**************************************************************************/

struct hostinfo {
    hostinfo();
    ~hostinfo();

    std::string osname;
    std::string username;
    std::string hostname;
    std::vector<std::string> drives;
    std::string cpu_name;
    std::string mb_name;
    std::string mac_addr;
    std::string ip_addr;

    friend std::ostream& operator<< (std::ostream &os, const hostinfo &hi);
    std::string to_string() const;
};

/**************************************************************************/

hostinfo get_host_info();

/**************************************************************************/

} // ns gbp

#endif // __gbp__hostinfo_h__
