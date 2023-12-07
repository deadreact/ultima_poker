#include <precompiled.h>
#include <sodium.h>
#include <hostinfo/hostinfo.h>
#include "logger.h"
#include "crypt.h"

namespace {
    // генерит ключь шифрования из строки
    std::vector<gbp_u8> generate_hash(const std::string &hwinfo) {
        std::uint8_t hash[crypto_generichash_BYTES];
        ::sodium_memzero(hash, sizeof(hash));

        int ec = ::crypto_generichash(
             hash
            ,sizeof(hash)
            ,reinterpret_cast<const std::uint8_t *>(hwinfo.c_str())
            ,hwinfo.size()
            ,nullptr
            ,0
        );
        if ( ec ) {
            qFatal("can'e create crypto-key");
        }

        std::vector<std::uint8_t> res(hash, hash+sizeof(hash));

        return res;
    }
}

/*************************************************************************************************/
namespace ns_crypt
{
    // шифрует, возвращает строку содержащую base64
    QByteArray string_encrypt(const QByteArray &src, const std::string &hwinfo) {
        if (src.isNull() || src.isEmpty()) {
            Logger::warning(QString("can't encrypt an empty bytearray"), __FILELINE__);
            return src;
        }
        const std::vector<std::uint8_t> hash = generate_hash(hwinfo);
        QByteArray arr = src;
        QByteArray dst(crypto_secretbox_MACBYTES+arr.size(), 0);

        static const std::uint8_t nonce[crypto_secretbox_NONCEBYTES] = {0};
        int ec = ::crypto_secretbox_easy(
             reinterpret_cast<std::uint8_t*>(dst.data())
            ,reinterpret_cast<std::uint8_t*>(arr.data())
            ,arr.size()
            ,nonce
            ,hash.data()
        );
        if ( ec ) {
            Logger::error(QString("can't encrypt message %0").arg(QString::fromLatin1(src)), __FILELINE__);
            return QByteArray();
        }

        return dst.toBase64();
    }

    /*************************************************************************************************/

    // дешифрует, принимает строку в base64
    QByteArray string_decrypt(const QByteArray &src, const std::string &hwinfo) {
        if (src.isNull() || src.isEmpty()) {
            Logger::warning(QString("can't decrypt an empty bytearray"), __FILELINE__);
            return src;
        }
        const std::vector<std::uint8_t> hash = generate_hash(hwinfo);
        QByteArray arr = QByteArray::fromBase64(src);
        QByteArray dst(arr.size()-crypto_secretbox_MACBYTES, 0);

        static const std::uint8_t nonce[crypto_secretbox_NONCEBYTES] = {0};
        int ec = ::crypto_secretbox_open_easy(
             reinterpret_cast<std::uint8_t*>(dst.data())
            ,reinterpret_cast<const std::uint8_t*>(arr.data())
            ,arr.size()
            ,nonce
            ,hash.data()
        );
        if ( ec ) {
            Logger::error(QString("can't decrypt message %0").arg(QString::fromLatin1(src)), __FILELINE__);
            return QByteArray();
        }

        return dst;
    }
} // namespace ns_crypt

/*************************************************************************************************/

//int example() {
//    const gbp::hostinfo info = gbp::get_host_info();
//    const std::string sinfo = info.to_string();
//    std::cout << sinfo << std::endl;

//    QString src = "some string";
//    QString enc = string_encrypt(src, sinfo);
//    QString dec = string_decrypt(enc, sinfo);
//    Q_ASSERT(src == dec);
//}

/*************************************************************************************************/
