#pragma once

#include <string>
#include <type_traits>

#include <QString>

/**************************************************************************/

template<
	 typename From
	,bool ok = std::is_same<From, const char*>::value||std::is_same<From, char*>::value
>
QString qutf8(From str, typename std::enable_if<ok>::type* = 0)
	{ return QString::fromUtf8(str); }
inline QString qutf8(const std::string &str) { return QString::fromUtf8(str.c_str()); }
inline std::string qutf8(const QString &str) { return str.toUtf8().data(); }

/**************************************************************************/

QString getResource(const QString &path);

/**************************************************************************/

QString get_cpu_info();
QString make_pass_string();

/**************************************************************************/

QString get_db_name();

/**************************************************************************/

QString qstring_encrypt(const QString &str);
QString qstring_decrypt(const QString &str);

/**************************************************************************/
