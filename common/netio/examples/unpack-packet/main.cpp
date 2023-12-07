
#include <netio/cryptotools.hpp>

#include <sodium/utils.h>

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

int main(int argc, char **argv) {
    if ( argc != 3 ) {
        std::cout << "usage:\n  unpack-packet <file name> <shared key>" << std::endl;

        return EXIT_FAILURE;
    }
    std::string fname = argv[1];
    std::string sharedkey = argv[2];
    if ( sharedkey.size() != 64 ) {
        std::cout << "bad key size, expected key size is 64 bytes" << std::endl;

        return EXIT_FAILURE;
    }

    // shared key
    std::uint8_t binkey[gbp::net::cryptotools::k_shared_key_size];
    int ec = ::sodium_hex2bin(
         binkey
        ,sizeof(binkey)
        ,sharedkey.c_str()
        ,sharedkey.size()
        ,nullptr
        ,nullptr
        ,nullptr
    );
    if ( ec ) {
        std::cerr << "sodium_hex2bin() error: " << ec << std::endl;
        return EXIT_FAILURE;
    }

    // input file
    std::ifstream file(fname, std::ios::in|std::ios::binary);
    if ( !file ) {
        std::cerr << "can't open input file." << std::endl;
        return EXIT_FAILURE;
    }
    std::size_t fsize=0;
    {
        std::ifstream file(fname, std::ios::in|std::ios::ate);
        fsize = file.tellg();
    }
    std::string filebuf;
    filebuf.resize(fsize, 0);
    file.read(&filebuf[0], fsize);

    // decrypting
    gbp::net::cryptotools crypto;
    crypto.set_shared_key(binkey);

    std::shared_ptr<char> packbuf;
    std::size_t packsize=0;
    try {
        crypto.unpack_packet(&packbuf, &packsize, filebuf.c_str(), filebuf.size());
    } catch (const std::exception &ex) {
        std::cerr << "std::exception when unpack_packet(): " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "packet dump:\n";
    gbp::net::hexdump(std::cout, packbuf.get(), packsize);

    return EXIT_SUCCESS;
}
