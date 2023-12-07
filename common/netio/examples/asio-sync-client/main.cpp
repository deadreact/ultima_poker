
#include <netio/worker.hpp>
#include <netio/socket.hpp>

#include <boost/asio/io_service.hpp>

#include <iostream>
#include <cstring>

int main() {
    gbp::net::worker worker;

    gbp::net::socket_type sock = gbp::net::create_socket(worker);

    int ec{};
    std::string emsg;
    sock->connect("127.0.0.1", 1234, &ec, &emsg);
    if (ec) {
        std::cerr << "connection error(" << ec << "): " << emsg << std::endl;
        return 1;
    }

    static const char str[] = "some message string....";
    std::size_t wr = sock->write(str, sizeof(str)-1, &ec, &emsg);
    if (ec) {
        std::cerr << "write error(" << ec << "): " << emsg << std::endl;
        return 1;
    }

    char buf[1024];
    std::size_t rd = sock->read(buf, sizeof(str)-1, &ec, &emsg);
    if (ec) {
        std::cerr << "read error(" << ec << "): " << emsg << std::endl;
        return 1;
    }

    if ( wr != rd || std::strcmp(str, buf) != 0 ) {
        std::cerr << "strings is not equal" << std::endl;
        return 1;
    } else {
        std::cout << "finished successfully!" << std::endl;
    };

    return 0;
}
