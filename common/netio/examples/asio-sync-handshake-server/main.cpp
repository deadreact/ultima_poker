
#include <netio/cryptotools.hpp>
#include <netio/acceptor.hpp>
#include <netio/socket.hpp>

#define NETIO_DEBUG
#include <netio/netio.hpp>

#include <iostream>

int main() {
    gbp::net::worker worker;

    auto error_handler = [](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg){
        (void)holder;

        std::cerr << "netio(" << fl << "): ec=" << ec << ", emsg=" << emsg << std::endl;
    };

    gbp::net::acceptor_type acc = gbp::net::create_acceptor(worker, "127.0.0.1", 1234);

    while (1) {
        int ec{};
        std::string emsg;

        gbp::net::socket_type sock = acc->accept(&ec, &emsg);
        if (ec) {
            std::cerr << "accept error(" << ec << "): " << emsg << std::endl;
            return 1;
        }

        gbp::net::netio netio(
        	 sock
        	,gbp::net::netio::server
        	,10
        	,gbp::net::netio::k_max_body_size
        	,100
        	,error_handler
        );
        netio.handshake(&ec, &emsg);
        if (ec) {
            std::cerr << "handshake error(" << ec << "): " << emsg << std::endl;
            return 1;
        }

        std::cout << "shared_key:" << std::endl;
        gbp::net::hexstring(std::cout, netio.shared_key(), netio.shared_key_size()) << std::endl;
    }

    return 0;
}
