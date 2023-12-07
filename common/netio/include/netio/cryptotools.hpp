
#ifndef __gbp__net__cryptotools_hpp
#define __gbp__net__cryptotools_hpp

#include <cstdint>
#include <array>
#include <memory>

#include <sodium/crypto_box.h>
#include <sodium/crypto_secretbox.h>
#include <sodium/crypto_generichash.h>

/**************************************************************************/

namespace gbp {
namespace net {

/**************************************************************************/

std::ostream& hexdump(std::ostream &os, const void *ptr, std::size_t size);
std::string   hexdump(const void *ptr, std::size_t size);
std::ostream& hexstring(std::ostream &os, const void *ptr, std::size_t size);
std::string   hexstring(const void *ptr, std::size_t size);

/**************************************************************************/

struct cryptotools {
    enum: std::uint32_t {
         k_public_key_size = crypto_box_PUBLICKEYBYTES
        ,k_secret_key_size = crypto_box_SECRETKEYBYTES
        ,k_nonce_size = crypto_secretbox_NONCEBYTES
        ,k_nonce_counter_size = sizeof(std::uint32_t)
        ,k_shared_key_size = crypto_generichash_BYTES
        ,k_compress_if_greater = 100
        ,k_header_storage_size = sizeof(std::uint32_t)
        ,k_hmac_size = crypto_secretbox_MACBYTES
    };

    enum side_type {
         client_side
        ,server_side
    };

    using public_key_storage    = std::array<std::uint8_t, k_public_key_size>;
    using secret_key_storage    = std::array<std::uint8_t, k_secret_key_size>;
    using shared_key_storage    = std::array<std::uint8_t, k_shared_key_size>;
    using pk_and_nonce_storage  = std::array<std::uint8_t, k_public_key_size*2>;
    using nonce_storage         = std::array<std::uint8_t, k_nonce_size>;

    cryptotools(const cryptotools &) = delete;
    cryptotools &operator=(const cryptotools &) = delete;

    cryptotools(std::size_t compress_if_greater = k_compress_if_greater);
    cryptotools(cryptotools &&r);
    virtual ~cryptotools();

    void generate_own_piblic_key_and_nonce();
    void generate_nonce();
    void generate_shared_key(side_type side);
    pk_and_nonce_storage get_own_piblic_key_and_nonce() const;
    void set_oppo_public_key_and_nonce(const pk_and_nonce_storage &pkn);

    void pack_packet(std::shared_ptr<char> *dptr, std::size_t *dsize, const char *sptr, std::size_t ssize);
    void unpack_packet(std::shared_ptr<char> *dptr, std::size_t *dsize, const char *sptr, std::size_t ssize);

    std::size_t get_compress_if_greater()      const;
    void set_compress_if_greater(std::size_t size);

    const std::uint8_t* get_secret_key()       const;
    std::size_t         secret_key_size()      const;

    const std::uint8_t* get_public_key()       const;
    std::size_t         public_key_size()      const;

    const std::uint8_t* get_oppo_public_key()  const;
    std::size_t         oppo_public_key_size() const;

    const std::uint8_t* get_shared_key()       const;
    void                set_shared_key(const std::uint8_t *key);
    std::size_t         shared_key_size()      const;

    const std::uint8_t* get_nonce()            const;
    std::size_t         nonce_size()           const;

    const std::uint8_t* get_oppo_nonce()       const;
    std::size_t         oppo_nonce_size()      const;

    std::ostream &dump(std::ostream &os) const;

private:
    struct impl;
    impl *pimpl;
};

/**************************************************************************/

} // ns net
} // ns gbp

#endif // __gbp__net__cryptotools_hpp
