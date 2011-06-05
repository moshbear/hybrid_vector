/* hybrid_vector/pmf.h - typedefs for pmfs in hybrid_vector
 * 
 * Author: Andrey Vul
 * Version: r4
 *
 * DO NOT INCLUDE THIS HEADER DIRECTLY!
 *
** Copyright (C) 2011, Andrey Vul <andrey@moshbear.net>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef HYBRID_VECTOR_PMF_H
#define HYBRID_VECTOR_PMF_H

template <typename T,
	  typename dv,
	  typename rv>
struct hybrid_vector_pmf {
	// size_t
	typedef typename rv::size_type rv_size_type;
	typedef typename dv::size_type dv_size_type;
	// reference
	typedef typename rv::reference rv_reference;
	typedef typename dv::reference dv_reference;
	// const_reference
	typedef typename rv::const_reference rv_const_reference;
	typedef typename dv::const_reference dv_const_reference;
	// iterator
	typedef typename rv::iterator rv_iterator;
	typedef typename dv::iterator dv_iterator;
	// const_iterator
	typedef typename rv::const_iterator rv_const_iterator;
	typedef typename dv::const_iterator dv_const_iterator;
	// And now, the pmf typedefs themselves
	// vector::size_type (vector::*)() const
	typedef rv_size_type (rv::*rv_get_size)() const;
	typedef dv_size_type (dv::*dv_get_size)() const;
	// void (vector::*)(vector::size_type)
	typedef void (rv::*rv_set_size)(rv_size_type);
	typedef void (dv::*dv_set_size)(dv_size_type);
	// void (vector::*)()
	typedef void (rv::*rv_reset)();
	typedef void (dv::*dv_reset)();
	// reference (vector::*)(vector::size_type)
	typedef rv_reference (rv::*rv_get_ref)(rv_size_type);
	typedef dv_reference (dv::*dv_get_ref)(dv_size_type);
	// const_reference (vector::*)(vector::size_type) const
	typedef rv_const_reference (rv::*rv_get_cref)(rv_size_type) const;
	typedef dv_const_reference (dv::*dv_get_cref)(dv_size_type) const;
	// void (vector::*)(const_reference) const
	typedef void (rv::*rv_push)(rv_const_reference);
	typedef void (dv::*dv_push)(dv_const_reference);
};

#endif
