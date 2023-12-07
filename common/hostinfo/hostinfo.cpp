
#include "hostinfo.h"

#include <sstream>

namespace gbp {

/**************************************************************************/

hostinfo::hostinfo()
    :osname{}
    ,username{}
    ,hostname{}
    ,drives{}
    ,cpu_name{}
    ,mb_name{}
    ,mac_addr{}
    ,ip_addr{}
{}

hostinfo::~hostinfo()
{}


/**************************************************************************/

std::ostream& operator<< (std::ostream &os, const hostinfo &hi) {
    os
    << "os:" << std::endl
    << "  " << hi.osname << std::endl;

    os
    << "user:" << std::endl
    << "  " << hi.username << std::endl;

    os
    << "host:" << std::endl
    << "  " << hi.hostname << std::endl;

    os
    << "drives:" << std::endl;
    for ( const auto &it: hi.drives ) {
        os << "  " << it << std::endl;
    }

    os
    << "cpu:" << std::endl
    << "  " << hi.cpu_name << std::endl;

    os
    << "MB:" << std::endl
    << "  " << hi.mb_name << std::endl;

    os
    << "MAC:" << std::endl
    << "  " << hi.mac_addr << std::endl;

//    os
//    << "IP:" << std::endl
//    << "  " << hi.ip_addr << std::endl;

    return os;
}

std::string hostinfo::to_string() const {
    std::ostringstream os;

    os << *this;

    return os.str();
}

/**************************************************************************/

std::string get_osname();
std::string get_username();
std::string get_hostname();
std::vector<std::string>
get_disk_drives();
std::string get_cpu();
std::string get_mb();
std::string get_mac_addr();
std::string get_ip();

/**************************************************************************/

} // ns gbp

/**************************************************************************/

#if defined(_WIN32) || defined(_WIN64)
#   include "src/win.cpp"
#elif defined(__ANDROID__)
#   include "src/android.cpp"
#elif defined(__linux__)
#       include "src/nix.cpp"
#elif defined(__APPLE__)
#   include "TargetConditionals.h"
#   if defined(TARGET_OS_IPHONE)
#       include "src/ios.cpp"
#   else
#       include "src/osx.cpp"
#   endif // defined(TARGET_OS_IPHONE)
#endif

/**************************************************************************/

namespace gbp {

hostinfo get_host_info() {
    hostinfo res;

    res.osname = get_osname();
    res.username = get_username();
    res.hostname = get_hostname();
    res.drives = get_disk_drives();
    res.cpu_name = get_cpu();
    res.mb_name = get_mb();
    res.mac_addr = get_mac_addr();
    res.ip_addr = get_ip();

    return res;
}

} // ns gbp

/**************************************************************************/
