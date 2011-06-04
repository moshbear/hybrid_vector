/* hybrid_vector/iterator.h - iterator template class
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

#ifndef HYBRID_VECTOR_ITERATOR_H
#define HYBRID_VECTOR_ITERATOR_H

#include <iterator>
#include <boost/assert.hpp>

#include <hybrid_vector/fwd.h>

template <typename T, typename rv, typename dv>
class hybrid_vector_iterator
{
	typedef hybrid_vector_iterator<T, rv, dv> this_type;
	friend class hybrid_vector_const_iterator<T, rv, dv>;
public:
	typedef hybrid_vector_const_iterator<T, rv, dv> const_iterator;
	typedef this_type iterator;
	typedef hybrid_vector<T, rv, dv> vector_type;
	friend class hybrid_vector<T, rv, dv>;

	typedef std::random_access_iterator_tag iterator_category;
	typedef typename vector_type::size_type size_type;
	typedef typename vector_type::difference_type difference_type;
	typedef typename vector_type::value_type value_type;
	typedef typename vector_type::reference reference;
	typedef typename vector_type::const_reference const_reference;
	typedef typename vector_type::pointer pointer;
	typedef typename vector_type::const_pointer const_pointer;

protected:
	vector_type* parent;
	size_type off;

private:
	hybrid_vector_iterator(vector_type* parent_, size_type off_) :
		parent(parent_), off(off_) { }
public:
	hybrid_vector_iterator() :
		parent(0), off(0) { }
	hybrid_vector_iterator(const iterator& it) :
		parent(it.parent), off(it.off) { }

	difference_type operator-(const iterator& it) const {
		return off - it.off;
	}
	difference_type operator-(const const_iterator& it) const {
		return off - it.off;
	}

	this_type operator-(size_type n) const {
		return this_type(parent, off - n);
	}

	this_type operator+(size_type n) const {
		return this_type(parent, off + n);
	}

	this_type& operator-=(size_type n) {
		off -= n;
		return *this;
	}

	this_type& operator+=(size_type n) {
		off += n;
		return *this;
	}

	reference operator*() {
		return (*parent)[off];
	}
	const_reference operator*() const {
		return (*parent)[off];
	}

	pointer operator->() {
		return &((*parent)[off]);
	}
	const_pointer operator->() const {
		return &((*parent)[off]);
	}

	reference operator[](size_type n) {
		return (*parent)[off + n];
	}
	const_reference operator[](size_type n) const {
		return (*parent)[off + n];
	}

	this_type& operator++() {
		++off;
		return *this;
	}
	this_type& operator++(int) {
		this_type tmp = *this;
		++off;
		return tmp;
	}

	this_type& operator--() {
		--off;
		return *this;
	}
	this_type& operator--(int) {
		this_type tmp = *this;
		--off;
		return tmp;
	}

	bool operator==(const iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off == it.off;
	}
	bool operator==(const const_iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off == it.off;
	}

	bool operator!=(const iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off != it.off;
	}
	bool operator!=(const const_iterator& it) const {
		BOOSST_ASSERT(parent == it.parent);
		return off != it.off;
	}

	bool operator<(const iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off < it.off;
	}
	bool operator<(const const_iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off < it.off;
	}

	bool operator<=(const iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off <= it.off;
	}
	bool operator<=(const const_iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off <= it.off;
	}

	bool operator>(const iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off > it.off;
	}
	bool operator>(const const_iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off > it.off;
	}

	bool operator>=(const iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off >= it.off;
	}
	bool operator>=(const const_iterator& it) const {
		BOOST_ASSERT(parent == it.parent);
		return off >= it.off;
	}
};

#endif

