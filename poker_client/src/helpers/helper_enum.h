#pragma once

#include <tools/types_fwd.hpp>

template <typename enumerator>
struct status_enum {
    using value_type = typename std::enable_if<std::is_enum<enumerator>::value, typename std::underlying_type<enumerator>::type>::type;
    static const value_type ok;
    static const value_type unknown_error;

    static QByteArray to_string(enumerator e);
};

namespace ns_helpers
{
    template <typename enumerator> constexpr enumerator enum_ok()            { return enumerator(status_enum<enumerator>::ok); }
    template <typename enumerator> constexpr enumerator enum_unknown_error() { return enumerator(status_enum<enumerator>::unknown_error); }

    template <typename enumerator> constexpr bool isOk(enumerator enumValue) { return enumValue == enum_ok<enumerator>(); }
    template <typename enumerator> QByteArray status_string(enumerator e) { return status_enum<enumerator>::to_string(e); }

    template <typename enumerator> bool check_status(enumerator enumValue, const char* name) {
        return status_enum<enumerator>::to_string(enumValue) == QByteArray(name);
    }
} //namespace ns_helpers
