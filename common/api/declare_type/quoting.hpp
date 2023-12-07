#ifndef _gbp__declare_type__quoting_hpp
#define _gbp__declare_type__quoting_hpp

#include <ostream>
#include <string>
#include <type_traits>

/***************************************************************************/

namespace std {

template<typename T>
struct _is_string: std::integral_constant<bool, std::is_same<T, std::string>::value>
{};

template<typename T>
struct _is_bool: std::integral_constant<bool, std::is_same<T, bool>::value>
{};

template<typename T>
struct _is_char: std::integral_constant<bool, (std::is_same<T, char>::value || std::is_same<T, unsigned char>::value || std::is_same<T, signed char>::value)>
{};

template<typename T>
struct _is_not_string_bool_char: std::integral_constant<
	  bool
	 ,!(_is_string<T>::value || _is_bool<T>::value || _is_char<T>::value)
>
{};

// for string
template<typename T>
void quoting(std::ostream& s, const T &o, typename std::enable_if<_is_string<T>::value>::type* = 0) {
	s << '\"' << o << '\"';
}
// for bool
template<typename T>
void quoting(std::ostream& s, const T &o, typename std::enable_if<_is_bool<T>::value>::type* = 0) {
	 s << (o ? "true" : "false");
}
// for char
template<typename T>
void quoting(std::ostream& s, const T &o, typename std::enable_if<_is_char<T>::value>::type* = 0) {
	 if ( std::is_signed<T>::value )
		  s << static_cast<int>(o);
	 else
		  s << static_cast<unsigned int>(o);
}
// for other types
template<typename T>
void quoting(std::ostream& s, const T &o, typename std::enable_if<_is_not_string_bool_char<T>::value>::type* = 0) {
	s << o;
}

} // ns std

/***************************************************************************/

#endif // _gbp__declare_type__quoting_hpp
