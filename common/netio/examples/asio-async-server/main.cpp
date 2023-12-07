
#include <netio/worker.hpp>
#include <netio/socket.hpp>
#include <netio/acceptor.hpp>

#include <boost/asio/io_service.hpp>

#include <iostream>

struct session: std::enable_shared_from_this<session> {
    session(gbp::net::socket_type sock)
        :sock{std::move(sock)}
    {}

    void start() {
        auto holder = shared_from_this();
        start_read(std::move(holder));
    }

    void start_read(gbp::net::impl_holder holder) {
        sock->async_read_some(
            buf, sizeof(buf),
            [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size)
            { on_readed(std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }
    void on_readed(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size) {
        if (ec) {
            std::cerr << "read error(" << fl << ": " << ec << "): " << emsg << std::endl;
            return;
        }

        sock->async_write(
             buf
            ,size
            ,[this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size)
             { on_writen(std::move(holder), fl, ec, emsg, buf, size); }
            ,std::move(holder)
        );
    }
    void on_writen(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, const char *buf, std::size_t size) {
        (void)buf;
        (void)size;

        if (ec) {
            std::cerr << "write error(" << fl << ": " << ec << "): " << emsg << std::endl;
            return;
        }

        start_read(std::move(holder));
    }

private:
    gbp::net::socket_type sock;
    char buf[1024];
};

struct server {
    server(gbp::net::worker &w)
        :acceptor{gbp::net::create_acceptor(w, "127.0.0.1", 1234)}
    {}

    void start(gbp::net::impl_holder holder = gbp::net::impl_holder()) {
        acceptor->async_accept(
            [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, gbp::net::socket_type sock)
            { on_accepted(std::move(holder), fl, ec, emsg, std::move(sock)); }
            ,std::move(holder)
        );
    }

private:
    void on_accepted(gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg, gbp::net::socket_type sock) {
        if (ec) {
            std::cerr << "accept error(" << fl << ": " <<  ec << "): " << emsg << std::endl;
            return;
        }

        auto sesp = std::make_shared<session>(std::move(sock));
        sesp->start();

        start(std::move(holder));
    }

    gbp::net::acceptor_type acceptor;
};

int main() {
    gbp::net::worker worker;

    server serv(worker);
    serv.start();

    worker.run();

    return 0;
}
