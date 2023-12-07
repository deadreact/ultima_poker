
//#include <iostream>

#include <string>
#include <vector>
#include <fstream>

#include <boost/filesystem/operations.hpp>
#include <boost/asio.hpp>

#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>

namespace gbp {

#define HOSTINFO_OUT_FNAME "/tmp/__34#1:@163__.txt"

/**************************************************************************/

std::string get_osname() {
    static const char *cmd = "python -c \"import platform;print(platform.platform())\" > " HOSTINFO_OUT_FNAME;

    boost::system::error_code ec;
    boost::filesystem::remove(HOSTINFO_OUT_FNAME, ec);
    ec.clear();

    ::system(cmd);

    if ( boost::filesystem::exists(HOSTINFO_OUT_FNAME, ec) ) {
        std::ifstream file(HOSTINFO_OUT_FNAME);
        std::string line;
        if ( file ) {
            std::getline(file, line);
        }

        boost::filesystem::remove(HOSTINFO_OUT_FNAME, ec);

        return line;
    }

    return std::string("");
}

/**************************************************************************/

std::string get_username() {
    ::uid_t uid = ::geteuid();
    struct ::passwd *pwd = ::getpwuid(uid);
    if ( pwd ) {
        return pwd->pw_name;
    }

    return "NULL";
}

/**************************************************************************/

std::string get_hostname() {
    char hostname[HOST_NAME_MAX] = "\0";
    ::gethostname(hostname, HOST_NAME_MAX);

    return hostname;
}

/**************************************************************************/

std::vector<std::string>
get_disk_drives() {
    std::vector<std::string> devs;

    static const char pref[] = "ata-";
    enum { pref_len = sizeof(pref)-1 };
    static const char suff[] = "-part"; // N
    enum { suff_len = sizeof(suff)-1 };

    static const char cpath[] = "/dev/disk/by-id/";
    boost::filesystem::directory_iterator cur(cpath), end;
    for ( ; cur != end; ++cur ) {
        std::string p = cur->path().filename().string();
//		std::cout << "p1=" << p << std::endl;

        if ( p.compare(0, pref_len, pref) != 0 )
            continue;

        p.erase(0, pref_len);
//		std::cout << "p2=" << p << std::endl;

        char last = p.back();
        p.pop_back();

        if ( p.compare(p.length()-suff_len, suff_len, suff) == 0 )
            continue;
        p.push_back(last);
        devs.push_back(p);
    }

    return devs;
}

/**************************************************************************/

std::string get_cpu() {
    std::string res{};

    static const char pref[] = "model name";
    enum { pref_len = sizeof(pref)-1 };

    std::ifstream file("/proc/cpuinfo");
    if ( !file ) return res;

    std::string line;
    while ( std::getline(file, line) ) {
        if ( line.compare(0, pref_len, pref) != 0 )
            continue;

        auto pos = line.find(':', 0);
        if ( pos == std::string::npos )
            continue;

        line.erase(0, pos+1);
//		std::cout << "line1=" << line << std::endl;
        pos = line.find_first_not_of(' ');
        line.erase(0, pos);
//		std::cout << "line2=" << line << std::endl;

        res = line;
    }

    return res;
}

/**************************************************************************/

std::string get_mb() {
    std::ifstream vfile("/sys/devices/virtual/dmi/id/board_vendor");
    if ( !vfile )
        return "unknown";

    std::ifstream mfile("/sys/devices/virtual/dmi/id/board_name");
    if ( !mfile )
        return "unknown";

    std::string vendor, model;
    std::getline(vfile, vendor);
    std::getline(mfile, model);

    return vendor+"-"+model;
}

/**************************************************************************/

std::string get_mac_addr() {
    std::ifstream file("/sys/class/net/eth0/address");
    if ( !file ) {
        static const char *cmd = "cat /sys/class/net/$(ip route show default | awk '/default/ {print $5}')/address > " HOSTINFO_OUT_FNAME;

        boost::system::error_code ec;
        boost::filesystem::remove(cmd, ec);
        ec.clear();

        ::system(cmd);
        if ( boost::filesystem::exists(HOSTINFO_OUT_FNAME, ec) ) {
            std::ifstream file(HOSTINFO_OUT_FNAME);
            std::string line;
            if ( file ) {
                std::getline(file, line);
            }

            boost::filesystem::remove(HOSTINFO_OUT_FNAME, ec);

            return line;
        }

        return "unknown";
    }

    std::string res;
    std::getline(file, res);

    return res;
}

/**************************************************************************/

std::string get_ip() {
    boost::asio::ip::tcp::iostream stream;
    stream.expires_from_now(boost::posix_time::seconds(2));
    stream.connect("ipinfo.io", "http");
    stream << "GET /ip HTTP/1.0\r\n";
    stream << "Host: ipinfo.io\r\n";
    stream << "Accept: */*\r\n";
    stream << "Connection: close\r\n\r\n";
    stream.flush();

    std::string res;
    while( std::getline(stream, res) ) {
        if ( !res.empty() && std::isdigit(res[0]) )
            break;
    }

    return res;
}

/**************************************************************************/

} // ns gbp
