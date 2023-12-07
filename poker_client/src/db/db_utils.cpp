#include <precompiled.h>
#include "db_utils.h"

#ifdef DB_ENCRYPTION_ENABLED
    // #include <api/crypto/crypto.hpp>
#endif

#ifdef VM_PROTECT
//    #include <VMProtectSDK.h>
#endif

#include <QString>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QApplication>

#ifdef Q_OS_MAC 
#define HOST_NAME_MAX 2048
#endif

#if defined(Q_OS_WIN)
#include <windows.h>
    #include <lmcons.h>
    #ifdef _MSC_VER
        #include <intrin.h>
    #else
        #include <cpuid.h>
    #endif
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    #include <cpuid.h>
    #include <unistd.h>
    #include <limits.h>
    #include <pwd.h>
#else
    #error Bad OS type
#endif

#include <iostream>

/**************************************************************************/

QString getResource(const QString &path) {
#ifdef RESOURCES_IN_QT_RESOURCES_FILE
     return "qrc:/" + path;
#else
#if defined(Q_OS_MAC)
     return QApplication::applicationDirPath() + "/../Resources/" + path;
#elif defined(Q_OS_WIN) || defined(Q_OS_LINUX)
     return QFileInfo(path).absoluteFilePath();
#endif
#endif
}

/**************************************************************************/

QString get_cpu_info() {
    QString res;
    gbp_u32 eax0=0, ebx0=0, ecx0=0, edx0=0;
#if defined(Q_OS_MAC)
    __get_cpuid(0, &eax0, &ebx0, &ecx0, &edx0);
#elif defined(Q_OS_WIN)
    // WINDOWS
    #ifdef _MSC_VER
        int hw[4] = {0};
        __cpuid(hw, 0);
        eax0=hw[0];ebx0=hw[1];ecx0=hw[2];edx0=hw[3];
    #else
        __get_cpuid(0, &eax0, &ebx0, &ecx0, &edx0);
    #endif
#elif defined(Q_OS_LINUX)
    // LINUX
    __cpuid(0, eax0, ebx0, ecx0, edx0);
#else
    #error Bad OS type
#endif
    res.append(QByteArray((const char*)&ebx0, sizeof(gbp_u32)));
    res.append(QByteArray((const char*)&edx0, sizeof(gbp_u32)));
    res.append(QByteArray((const char*)&ecx0, sizeof(gbp_u32)));

    return res;
}

QString make_pass_string() {
    QString res;
#if defined(Q_OS_WIN)
    // WINDOWS
    char comp_name_buf[MAX_COMPUTERNAME_LENGTH+1] = {0};
    char user_name_buf[UNLEN+1] = {0};
    DWORD buf_size = MAX_COMPUTERNAME_LENGTH;

    GetComputerNameA(comp_name_buf, &buf_size);
    buf_size = UNLEN;
    GetUserNameA(user_name_buf, &buf_size);

    res = comp_name_buf;
    res += "-";
    res += user_name_buf;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    // LINUX OR MAC
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);

    res = hostname;
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if ( pw ) {
        res += "-";
        res += pw->pw_name;
    }
#else
    #error Bad OS type
#endif

    res += "-";
    res += get_cpu_info();

    return res;
}

/**************************************************************************/

QString get_db_name() {
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    return "resources.db";
#elif defined(Q_OS_MAC)
    return QApplication::applicationDirPath() + "/resources.db";
#endif
}

/**************************************************************************/

QString qstring_encrypt(const QString &str) {
#ifdef DB_ENCRYPTION_ENABLED
#ifdef VM_PROTECT
    VMProtectBegin("dbkey_enc");
#endif

    QByteArray plain = str.toUtf8();
    QByteArray cipher(plain.size(), 0);

    QByteArray pass = make_pass_string().toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(pass);
    pass = hash.result().toHex();

    BF_KEY bfkey;
    gbp::crypto::bf_set_key(&bfkey, pass.data());
    gbp::crypto::bf_encrypt(cipher.data(), plain.data(), plain.size(), bfkey);

#ifdef VM_PROTECT
    VMProtectEnd();
#endif

    return cipher.toBase64();
#else // !DB_ENCRYPTION_ENABLED
    return str;
#endif // DB_ENCRYPTION_ENABLED
}

QString qstring_decrypt(const QString &str) {
#ifdef DB_ENCRYPTION_ENABLED
#ifdef VM_PROTECT
    VMProtectBegin("dbkey_dec");
#endif

    QByteArray cipher = QByteArray::fromBase64(str.toUtf8());
    QByteArray plain(cipher.size(), 0);

    QByteArray pass = make_pass_string().toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(pass);
    pass = hash.result().toHex();

    BF_KEY bfkey;
    gbp::crypto::bf_set_key(&bfkey, pass.data());
    gbp::crypto::bf_decrypt(plain.data(), cipher.data(), cipher.size(), bfkey);

#ifdef VM_PROTECT
    VMProtectEnd();
#endif

    return plain;
#else // !DB_ENCRYPTION_ENABLED
    return str;
#endif // DB_ENCRYPTION_ENABLED
}

/**************************************************************************/
