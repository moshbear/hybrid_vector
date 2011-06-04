/* hybrid_vector/pmf.h - typedefs for pmfs in hybrid_vector
 * 
 * Author: Andrey Vul
 * Version: r3
 *
 * DO NOT INCLUDE THIS HEADER DIRECTLY!
 *
** Copyright (C) 2011, Andrey Vul <andrey@moshbear.net>
 * All rights reserved.
 *  
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  	* Redistributions of source code must retain the above copyright
 * 		notice, this list of conditions and the following disclaimer.
 *  	* Redistributions in binary form must reproduce the above copyright
 * 		notice, this list of conditions and the following disclaimer in the
 * 		documentation and/or other materials provided with the distribution.
 *       * The names of the contributors may not be used to endorse or promote products
 *        	derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANDREY VUL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
