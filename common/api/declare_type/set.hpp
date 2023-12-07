
#ifndef _gbp__declare_type__set_hpp
#define _gbp__declare_type__set_hpp

#include <ostream>
#include <set>

#include <api/declare_type/decorators.hpp>
#include <api/declare_type/quoting.hpp>

/***************************************************************************/

namespace std {

template<typename Key, typename Comp, typename Allocator>
std::ostream& operator<< (std::ostream &s, const std::set<Key, Comp, Allocator> &o) {
	s << gbp::decorators::array_open_symbol;
	for ( auto cur = o.begin(), end = o.end(); cur != end; ++cur ) {
		quoting(s, *cur);
		if ( std::next(cur) != end )
			s << gbp::decorators::default_delimiter;
	}
	return s << gbp::decorators::array_close_symbol;
}

} // ns std

/***************************************************************************/

#endif // _gbp__declare_type__set_hpp
