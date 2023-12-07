
#include <netio/worker.hpp>
#include <netio/socket.hpp>

#include <boost/asio/io_service.hpp>

#include <iostream>
#include <cstring>

static const char msg[] = "some message string...";

struct client {
    client(gbp::net::worker &w)
        :sock(gbp::net::create_socket(w))
    {}

    void start(gbp::net::impl_holder holder = gbp::net::impl_holder()) {
        sock->async_connect(
            "127.0.0.1", 1234,
            [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
            { on_connected(std::move(holder), fl, ec, emsg); }
            ,std::move(holder)
        );
    }

    void on_connected(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg) {
        if (ec) {
            std::cerr << "connect error(" << fl << ": ec=" << ec << "): " << emsg << std::endl;
            return;
        }

        start_write(std::move(holder));
    }

    void start_write(gbp::net::impl_holder holder) {
        sock->async_write(
            msg, sizeof(msg)-1,
            [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size)
            { on_writen(std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }
    void on_writen(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size) {
        (void)buf;
        (void)size;

        if (ec) {
            std::cerr << "write error(" << fl << ": ec=" << ec << "): " << emsg << std::endl;
            return;
        }

        sock->async_read(
            m_buf, sizeof(msg)-1,
            [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size)
            { on_readed(std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }
    void on_readed(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, char *buf, std::size_t size) {
        (void)holder;

        if (ec) {
            std::cerr << "read error(" << fl << ": ec=" << ec << "): " << emsg << std::endl;
            return;
        }

        if ( sizeof(msg)-1 != size || std::strcmp(msg, buf) ) {
            std::cerr << "strings is not equal" << std::endl;
        } else {
            std::cout << "finished successfully!" << std::endl;
        }
    }

    gbp::net::socket_type sock;
    char m_buf[1024];
};

int main() {
    gbp::net::worker worker;

    client cli(worker);
    cli.start();

    worker.run();

    return 0;
}
