
#ifndef _gbp__declare_type__map_hpp
#define _gbp__declare_type__map_hpp

#include <ostream>
#include <map>

#include <api/declare_type/decorators.hpp>
#include <api/declare_type/quoting.hpp>

/***************************************************************************/

namespace std {

template<typename Key, typename T, typename Comp, typename Allocator>
std::ostream& operator<< (std::ostream &s, const std::map<Key, T, Comp, Allocator> &o) {
	s << gbp::decorators::object_open_symbol;
	for ( auto cur = o.begin(), end = o.end(); cur != end; ++cur ) 
    {
        if ( std::is_integral<Key>::value )
            s << "\"";
        quoting(s, cur->first);
        if ( std::is_integral<Key>::value )
            s << "\"";
        s << ":";

        quoting(s, cur->second);
        if ( std::next(cur) != end )
            s << gbp::decorators::default_delimiter;
	}
	return s << gbp::decorators::object_close_symbol;
}

} // ns std

/***************************************************************************/

#endif // _gbp__declare_type__map_hpp
