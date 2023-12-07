
#ifndef _gbp__declare_type__pair_hpp
#define _gbp__declare_type__pair_hpp

#include <ostream>
#include <utility>

#include <api/declare_type/decorators.hpp>
#include <api/declare_type/quoting.hpp>

/***************************************************************************/

namespace std {

template<typename T0, typename T1>
std::ostream& operator<< (std::ostream &s, const std::pair<T0, T1> &o) {
	s << gbp::decorators::object_open_symbol << "\"first\":";
	quoting(s, o.first);
	s << gbp::decorators::default_delimiter << "\"second\":";
	quoting(s, o.second);
	return s << gbp::decorators::object_close_symbol;
}

} // ns std

/***************************************************************************/

#endif // _gbp__declare_type__pair_hpp
