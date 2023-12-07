
#include <netio/worker.hpp>
#include <netio/socket.hpp>
#include <netio/acceptor.hpp>

#include <iostream>

void serve(gbp::net::socket_type sock) {
    char buf[1024];
    int ec{};
    std::string emsg;

    std::size_t rd = sock->read_some(buf, sizeof(buf), &ec, &emsg);
    if (ec) {
        std::cerr << "read error(" << ec  << "): " << emsg << std::endl;
        return;
    }

    buf[rd] = 0;

    sock->write(buf, rd, &ec, &emsg);
    if (ec) {
        std::cerr << "write error(" << ec  << "): " << emsg << std::endl;
        return;
    }
}

int main() {
    gbp::net::worker worker;
    gbp::net::acceptor_type acc = gbp::net::create_acceptor(worker, "127.0.0.1", 1234);

    while (1) {
        int ec{};
        std::string emsg{};
        auto sock = acc->accept(&ec, &emsg);
        serve(std::move(sock));
    }

    return 0;
}
