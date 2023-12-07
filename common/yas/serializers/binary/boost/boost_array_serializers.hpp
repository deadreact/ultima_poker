
// Copyright (c) 2010-2016 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__binary__boost_array_serializers_hpp
#define _yas__binary__boost_array_serializers_hpp

#if defined(YAS_SERIALIZE_BOOST_TYPES)
#include <yas/serializers/serializer.hpp>
#include <yas/detail/type_traits/selector.hpp>
#include <yas/detail/io/serialization_exception.hpp>

#include <boost/array.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

template<typename T, size_t N>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	boost::array<T, N>
> {
	template<typename Archive>
	static Archive& save(Archive& ar, const boost::array<T, N>& array) {
		ar.write((std::uint32_t)N);
		if ( is_fundamental_and_sizeof_is<T, 1>::value ) {
			ar.write(&array[0], N);
		} else {
			for ( const auto &it: array ) {
				ar & it;
			}
		}
		return ar;
	}

	template<typename Archive>
	static Archive& load(Archive& ar, boost::array<T, N>& array) {
		std::uint32_t size = 0;
		ar.read(size);
		if ( size != N ) YAS_THROW_BAD_ARRAY_SIZE();
		if ( is_fundamental_and_sizeof_is<T, 1>::value ) {
			ar.read(&array[0], N);
		} else {
			for ( auto &it: array ) {
				ar & it;
			}
		}
		return ar;
	}
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // defined(YAS_SERIALIZE_BOOST_TYPES)

#endif // _yas__binary__boost_array_serializers_hpp
