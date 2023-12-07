
#include <netio/cryptotools.hpp>
#include <netio/ppdefs.hpp>

#include <cstring>
#include <ios>
#include <iomanip>
#include <sstream>

#include <sodium.h>
#include <zlib.h>

namespace gbp {
namespace net {

/**************************************************************************/

static const std::uint32_t crc32_table[] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

inline std::size_t chksum_crc32(const void *ptr, std::size_t length) {
    std::size_t crc = 0xFFFFFFFF;
    const std::uint8_t *block = static_cast<const std::uint8_t*>(ptr);
    for ( std::size_t idx = 0; idx < length; ++idx ) {
        crc = ((crc >> 8) & 0x00FFFFFF) ^ crc32_table[(crc ^ *block++) & 0xFF];
    }
    return (crc ^ 0xFFFFFFFF);
}

std::ostream& hexdump(std::ostream &os, const void *ptr, const std::size_t size) {
    if ( !ptr || !size ) return os;

    std::ios::fmtflags fmt(os.flags());

    const std::uint8_t *buffer = static_cast<const std::uint8_t*>(ptr);
    std::size_t addr = 0, n = 0, idx = 0, cnt2 = 0;
    for ( n = 0; n < size; ++n ) {
        if ( cnt2 == 0 ) {
            os << std::setw(7) << std::setfill('0') << addr << ": ";
            addr += 16;
        }
        cnt2 = (cnt2 + 1) % 18;
        if ( cnt2 <= 16 ) {
            os << std::hex << std::setw(2) << std::setfill('0') << (std::size_t)buffer[n] << " ";
        } else {
            os << "  ";
            os << std::setfill(' ');
            for ( idx = n - cnt2 + 1; idx < n; ++idx) {
                if ( buffer[idx] < 32 || 126 < buffer[idx] ) {
                    os << '.';
                } else {
                    os << buffer[idx];
                }
            }
            os << std::endl << std::dec;
            cnt2 = 0;
            n--;
        }
    }

    os << std::setfill(' ');

    for ( idx = cnt2+1; idx <= 16 ; ++idx ) {
        os << std::setw(2) << "-- ";
    }
    os << "  ";

    for ( idx = n-cnt2; cnt2 <= 16 && idx < n; ++idx ) {
        if ( buffer[idx] < 32 || 126 < buffer[idx] ) {
            os << '.';
        } else {
            os << buffer[idx];
        }
    }
    os << std::dec;

    os << std::endl
       << "LEN: " << size << " bytes, "
       << "CRC32: 0x" << std::hex << chksum_crc32(ptr, size)
       << std::endl;

    os.flags(fmt);

    return os;
}

std::string hexdump(const void *ptr, const std::size_t size) {
    std::ostringstream os;
    hexdump(os, ptr, size);

    return os.str();
}

/**************************************************************************/

std::string hexstring(const void *ptr, std::size_t size) {
    const std::uint8_t *uptr = static_cast<const std::uint8_t *>(ptr);

    std::unique_ptr<char[]> buf(new char[size*2+1]);

    return ::sodium_bin2hex(buf.get(), size*2+1, uptr, size);
}

std::ostream& hexstring(std::ostream &os, const void *ptr, std::size_t size) {
    const std::string hex = hexstring(ptr, size);

    return os << hex;
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

struct cryptotools::impl {
    impl(std::size_t compress_if_greater)
        :m_compress_if_greater(compress_if_greater)
        ,m_pk{{}}
        ,m_sk{{}}
        ,m_nonce{{}}
        ,m_oppo_pk{{}}
        ,m_oppo_nonce{{}}
        ,m_sharedkey{{}}
    {
        ::crypto_box_keypair(m_pk.data(), m_sk.data());
        generate_nonce();
    }

    void generate_own_piblic_key_and_nonce() {
        ::crypto_box_keypair(m_pk.data(), m_sk.data());
        generate_nonce();
    }

    void generate_nonce() {
        ::randombytes_buf(m_nonce.data(), m_nonce.size());
    }

    void generate_shared_key(cryptotools::side_type side) {
        std::uint8_t scalarmult_q[crypto_scalarmult_BYTES] = {0};
        const bool ok = ::crypto_scalarmult(
             scalarmult_q
            ,m_sk.data()
            ,m_oppo_pk.data()
        ) == 0;

        if ( !ok ) {
            throw std::runtime_error(
                __NETIO_MAKE_FILELINE() ": cryptotools::generate_shared_key::crypto_scalarmult()"
            );
        }

        const std::uint8_t *pk0 = (side == client_side ? m_pk.data() : m_oppo_pk.data());
        const std::uint8_t *pk1 = (side == client_side ? m_oppo_pk.data() : m_pk.data());

        ::crypto_generichash_state h;
        ::crypto_generichash_init  (&h, NULL, 0U, crypto_generichash_BYTES);
        ::crypto_generichash_update(&h, scalarmult_q, sizeof(scalarmult_q));
        ::crypto_generichash_update(&h, pk0, k_public_key_size);
        ::crypto_generichash_update(&h, pk1, k_public_key_size);
        ::crypto_generichash_final (&h, m_sharedkey.data(), m_sharedkey.size());
    }

    pk_and_nonce_storage get_own_piblic_key_and_nonce() const {
        pk_and_nonce_storage res;

        ::randombytes_buf(res.data(), res.size());
        std::memcpy(res.data(), m_pk.data(), m_pk.size());
        std::memcpy(res.data()+m_pk.size(), m_nonce.data(), m_nonce.size());

        return res;
    }

    void set_oppo_public_key_and_nonce(const pk_and_nonce_storage &pkn) {
        std::memcpy(m_oppo_pk.data(), pkn.data(), m_oppo_pk.size());
        std::memcpy(m_oppo_nonce.data(), pkn.data()+m_oppo_pk.size(), m_oppo_nonce.size());
    }

    void pack_packet(std::shared_ptr<char> *dptr, std::size_t *dsize, const char *ptr, std::size_t size) {
        const std::uint8_t *pptr = reinterpret_cast<const std::uint8_t *>(ptr);
        if ( size > m_compress_if_greater ) {
            std::size_t compressed_size = ::compressBound(size)+k_header_storage_size;
            std::unique_ptr<std::uint8_t[]> compressed_buf(new std::uint8_t[compressed_size]);

            *reinterpret_cast<std::uint32_t *>(compressed_buf.get()) = static_cast<std::uint32_t>(size);

            int res = ::compress2(
                compressed_buf.get()+k_header_storage_size // for unco size
                ,(uLongf*)&compressed_size
                ,pptr
                ,size
                ,Z_DEFAULT_COMPRESSION
            );
            if ( res != Z_OK ) {
                const std::string msg = __NETIO_MAKE_FILELINE() ": cryptotools::pack_packet::compress(): " + std::string(zError(res));
                throw std::runtime_error(msg);
            }

            const std::uint32_t packet_total_size =
                 k_header_storage_size // body size
                +k_nonce_size
                +k_header_storage_size // unco size
                +k_hmac_size
                +static_cast<std::uint32_t>(compressed_size)
            ;

            std::shared_ptr<char> packet_buf(new char[packet_total_size], &__deleter);
            std::uint8_t *pp = reinterpret_cast<std::uint8_t *>(packet_buf.get());

            const std::uint32_t body_size = packet_total_size-k_header_storage_size;
            std::memcpy(pp, &body_size, sizeof(body_size)); // body size
            pp += sizeof(body_size);

            std::memcpy(pp, m_nonce.data(), m_nonce.size()); // nonce
            pp += m_nonce.size();

            const bool ok = ::crypto_secretbox_easy(
                 pp
                ,reinterpret_cast<const std::uint8_t *>(compressed_buf.get())
                ,k_header_storage_size+compressed_size
                ,m_nonce.data()
                ,m_sharedkey.data()
            ) == 0;

            if ( !ok ) {
                throw std::runtime_error(
                    __NETIO_MAKE_FILELINE() ": cryptotools::pack_packet::crypto_secretbox_easy()"
                );
            }

            generate_nonce();

            *dptr = std::move(packet_buf);
            *dsize = packet_total_size;

            return;
        }

        const std::uint32_t packet_total_size =
             k_header_storage_size // body size
            +k_nonce_size
            +k_header_storage_size // unco size
            +k_hmac_size
            +static_cast<std::uint32_t>(size)
        ;

        std::shared_ptr<char> packet_buf(new char[packet_total_size], &__deleter);
        std::uint8_t *pp = reinterpret_cast<std::uint8_t *>(packet_buf.get());

        const std::uint32_t body_size = packet_total_size-k_header_storage_size;
        std::memcpy(pp, &body_size, sizeof(body_size)); // body size
        pp += sizeof(body_size);

        std::memcpy(pp, m_nonce.data(), m_nonce.size()); // nonce
        pp += m_nonce.size();

        std::unique_ptr<char[]> src_buf(new char[size+k_header_storage_size]);
        const std::uint32_t unco_size = 0; // unco size
        std::memcpy(src_buf.get(), &unco_size, sizeof(unco_size));
        std::memcpy(src_buf.get()+sizeof(unco_size), pptr, size);

        bool ok = ::crypto_secretbox_easy(
             pp
            ,reinterpret_cast<const std::uint8_t *>(src_buf.get())
            ,size+k_header_storage_size
            ,m_nonce.data()
            ,m_sharedkey.data()
        ) == 0;

        if ( !ok ) {
            throw std::runtime_error(
                __NETIO_MAKE_FILELINE() ": cryptotools::pack_packet::crypto_secretbox_easy()"
            );
        }

        generate_nonce();

        *dptr = std::move(packet_buf);
        *dsize = packet_total_size;
    }

    void unpack_packet(std::shared_ptr<char> *dptr, std::size_t *dsize, const char *ptr, std::size_t size) {
        const std::uint8_t *pp = reinterpret_cast<const std::uint8_t *>(ptr);

        nonce_storage nonce{{}};
        std::memcpy(nonce.data(), pp, nonce.size());
        pp += nonce.size();

        const std::size_t decrypted_size = size-nonce.size()-k_hmac_size;
        std::unique_ptr<char[]> decrypted(new char[decrypted_size]);
        std::uint8_t *dpp = reinterpret_cast<std::uint8_t *>(decrypted.get());

        const bool ok = ::crypto_secretbox_open_easy(
             dpp
            ,pp
            ,size-nonce.size()
            ,nonce.data()
            ,m_sharedkey.data()
        ) == 0;
        if ( !ok ) {
            throw std::runtime_error(
                __NETIO_MAKE_FILELINE() ": cryptotools::unpack_packet::crypto_secretbox_open_easy()"
            );
        }

        const std::uint32_t unco_size = *reinterpret_cast<std::uint32_t*>(dpp);
        dpp += sizeof(unco_size);

        if ( unco_size > 0 ) {
            std::shared_ptr<char> uncompressed(new char[unco_size], &__deleter);
            unsigned long _unco_size = unco_size;
            int res = ::uncompress(
                 reinterpret_cast<std::uint8_t *>(uncompressed.get())
                ,&_unco_size
                ,dpp
                ,size-nonce.size()-k_header_storage_size-k_hmac_size
            );
            if ( res != Z_OK ) {
                const std::string msg = __NETIO_MAKE_FILELINE() ": cryptotools::unpack_packet(): uncompress error: " + std::string(zError(res));
                throw std::runtime_error(msg);
            }
            if ( unco_size != _unco_size ) {
                const std::string msg = __NETIO_MAKE_FILELINE() ": cryptotools::unpack_packet(): uncompress error: " + std::string(zError(res));
                throw std::runtime_error(msg);
            }

            *dptr = std::move(uncompressed);
            *dsize = unco_size;

            return;
        }

        std::shared_ptr<char> res(new char[decrypted_size-sizeof(std::uint32_t)/*unco size*/], &__deleter);
        std::memcpy(res.get(), decrypted.get()+sizeof(std::uint32_t), decrypted_size-sizeof(std::uint32_t));

        *dptr = std::move(res);
        *dsize = decrypted_size-sizeof(std::uint32_t);
    }

    std::ostream& dump(std::ostream &os) {
        os
        << "self pk   =" << hexstring(m_pk.data(), m_pk.size()) << std::endl
        << "self sk   =" << hexstring(m_sk.data(), m_sk.size()) << std::endl
        << "self nonce=" << hexstring(m_nonce.data(), m_nonce.size()) << std::endl
        << "oppo pk   =" << hexstring(m_oppo_pk.data(), m_oppo_pk.size()) << std::endl
        << "oppo nonce=" << hexstring(m_oppo_nonce.data(), m_oppo_nonce.size()) << std::endl
        << "shared key=" << hexstring(m_sharedkey.data(), m_sharedkey.size()) << std::endl;

        return os;
    }

    std::size_t m_compress_if_greater;
    public_key_storage m_pk;
    secret_key_storage m_sk;
    nonce_storage      m_nonce;
    public_key_storage m_oppo_pk;
    nonce_storage      m_oppo_nonce;
    shared_key_storage m_sharedkey;

    static void __deleter(char *p) { delete[] p; }
};

/**************************************************************************/

cryptotools::cryptotools(std::size_t compress_if_greater)
    :pimpl(new impl(compress_if_greater))
{}

cryptotools::cryptotools(cryptotools &&r)
    :pimpl(r.pimpl)
{ r.pimpl = nullptr; }

cryptotools::~cryptotools()
{ delete pimpl; }

/**************************************************************************/

void cryptotools::generate_own_piblic_key_and_nonce() { pimpl->generate_own_piblic_key_and_nonce();}

void cryptotools::generate_nonce() { pimpl->generate_nonce(); }

void cryptotools::generate_shared_key(cryptotools::side_type side) { pimpl->generate_shared_key(side); }

cryptotools::pk_and_nonce_storage cryptotools::get_own_piblic_key_and_nonce() const { return pimpl->get_own_piblic_key_and_nonce(); }

void cryptotools::set_oppo_public_key_and_nonce(const cryptotools::pk_and_nonce_storage &pkn) { pimpl->set_oppo_public_key_and_nonce(pkn); }

void cryptotools::pack_packet(std::shared_ptr<char> *dptr, std::size_t *dsize, const char *sptr, std::size_t ssize) { pimpl->pack_packet(dptr, dsize, sptr, ssize); }

void cryptotools::unpack_packet(std::shared_ptr<char> *dptr, std::size_t *dsize, const char *sptr, std::size_t ssize) { pimpl->unpack_packet(dptr, dsize, sptr, ssize); }

std::size_t cryptotools::get_compress_if_greater() const { return pimpl->m_compress_if_greater; }

void cryptotools::set_compress_if_greater(std::size_t size) { pimpl->m_compress_if_greater = size; }

const uint8_t *cryptotools::get_secret_key() const { return pimpl->m_sk.data(); }

std::size_t cryptotools::secret_key_size() const { return pimpl->m_sk.size(); }

const uint8_t *cryptotools::get_public_key() const { return pimpl->m_pk.data(); }

std::size_t cryptotools::public_key_size() const { return pimpl->m_pk.size(); }

const uint8_t *cryptotools::get_oppo_public_key() const { return pimpl->m_oppo_pk.data(); }

std::size_t cryptotools::oppo_public_key_size() const { return pimpl->m_oppo_pk.size(); }

const uint8_t *cryptotools::get_shared_key() const { return pimpl->m_sharedkey.data(); }
void cryptotools::set_shared_key(const std::uint8_t *key) { std::memcpy(pimpl->m_sharedkey.data(), key, pimpl->m_sharedkey.size()); }

std::size_t cryptotools::shared_key_size() const { return pimpl->m_sharedkey.size(); }

const uint8_t *cryptotools::get_nonce() const { return pimpl->m_nonce.data(); }

std::size_t cryptotools::nonce_size() const { return pimpl->m_nonce.size(); }

const uint8_t *cryptotools::get_oppo_nonce() const { return pimpl->m_oppo_nonce.data(); }

std::size_t cryptotools::oppo_nonce_size() const { return pimpl->m_oppo_nonce.size(); }

std::ostream& cryptotools::dump(std::ostream &os) const { return pimpl->dump(os); }

/**************************************************************************/

} // ns net
} // ns gbp
