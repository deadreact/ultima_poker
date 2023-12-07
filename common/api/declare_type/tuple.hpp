
#ifndef _gbp__declare_type__tuple_hpp
#define _gbp__declare_type__tuple_hpp

#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <typeinfo>
#include <cstring>
#include <cstdlib>

#include <ostream>
#include <tuple>

#include <api/declare_type/decorators.hpp>
#include <api/declare_type/quoting.hpp>

/***************************************************************************/

namespace  {

template<typename T>
const char* demangle()  {
	static char str[1024] = { 0 };
#ifndef _MSC_VER
	int stat;
	char* ptr = abi::__cxa_demangle(typeid(T).name(), 0, 0, &stat);
	if ( !ptr ) return "NULL";
	std::strncpy(str, ptr, sizeof(str));
	::free(ptr);
#else
	std::strncpy(str, typeid(T).name(), sizeof(str));
#endif
	return str;
}

/***************************************************************************/

namespace types {
struct zero {};
struct one {};
struct some {};
} // ns types

/***************************************************************************/

template<std::size_t idx, typename... Args>
void print_item(std::ostream& s, const std::tuple<Args...> &o, std::true_type) {
	s << '\"' << demangle<typename std::tuple_element<idx, std::tuple<Args...>>::type>() << "\":";
	quoting(s, std::get<idx>(o));
}

template<std::size_t idx, typename... Args>
void print_item(std::ostream& s, const std::tuple<Args...> &o, std::false_type) {
	s << '\"' << demangle<typename std::tuple_element<idx, std::tuple<Args...>>::type>() << "\":";
	quoting(s, std::get<idx>(o));
	s << gbp::decorators::default_delimiter;
	print_item<idx + 1>(s, o, std::integral_constant<bool, idx + 1 == sizeof...(Args) - 1>());
}

template<typename ... Args>
void print_tuple(std::ostream&, const std::tuple<Args...>&, types::zero) {
}

template<typename ... Args>
void print_tuple(std::ostream& s, const std::tuple<Args...>& o, types::one) {
	quoting(s, std::get<0>(o));
}

template<typename ... Args>
void print_tuple(std::ostream& s, const std::tuple<Args...>& o, types::some) {
	print_item<0>(s, o, std::false_type());
}

} // ns

/***************************************************************************/

namespace std {
//std::integral_constant<bool, 1 == sizeof...(Args)>()

template<typename... Args>
ostream& operator<<(ostream& s, const tuple<Args...>& o) {
	using tuple_size = typename std::conditional<
		 sizeof...(Args) == 0
		,types::zero
		,typename std::conditional<sizeof...(Args) == 1,types::one,types::some>::type
	>::type;

	s << gbp::decorators::object_open_symbol;
	print_tuple(s, o, tuple_size());
	return s << gbp::decorators::object_close_symbol;
}

} // ns std

/***************************************************************************/

#endif // _gbp__declare_type__tuple_hpp
