#pragma once
#include <QString>
#include "../../common/api/gbp_int.hpp"

namespace ns_crypt
{
    // шифрует, возвращает строку содержащую base64
    QByteArray string_encrypt(const QByteArray &src, const std::string &hwinfo);
    // дешифрует, принимает строку в base64
    QByteArray string_decrypt(const QByteArray &src, const std::string &hwinfo);
}
