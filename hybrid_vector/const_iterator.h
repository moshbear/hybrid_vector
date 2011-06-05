/* hybrid_vector/const_iterator.h - const_iterator template class
 *
 * Author: Andrey Vul
 * Version: r4
 *
 * DO NOT INCLUDE THIS HEADER DIRECTLY!
 *
** Copyright (C) 2011, Andrey Vul <andrey@moshbear.net>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef HYBRID_VECTOR_CONST_ITERATOR_H
#define HYBRID_VECTOR_CONST_ITERATOR_H

#include <iterator>
#include <boost/assert.hpp>

#include <hybrid_vector/fwd.h>

template <typename T, typename rv, typename dv>
class hybrid_vector_const_iterator
{
	typedef hybrid_vector_const_iterator<T, rv, dv> this_type;
	friend class hybrid_vector_iterator<T, rv, dv>;
	typedef hybrid_vector_iterator<T, rv, dv> non_const_iterator;
public:
	typedef non_const_iterator iterator;
	typedef this_type const_iterator;
	typedef hybrid_vector<T, rv, dv> vector_type;
	friend class hybrid_vector<T, rv, dv>;

	typedef std::random_access_iterator_tag iterator_category;
	typedef typename vector_type::size_type size_type;
	typedef typename vector_type::difference_type difference_type;
	typedef typename vector_type::value_type value_type;
	typedef typename vector_type::const_reference reference;
	typedef typename vector_type::const_reference const_reference;
	typedef typename vector_type::const_pointer pointer;
	typedef typename vector_type::const_pointer const_pointer;

protected:
	const vector_type* parent;
	size_type off;

private:
	hybrid_vector_const_iterator(vector_type* parent_, size_type off_) :
		parent(parent_), off(off_) { }
public:
	hybrid_vector_const_iterator() :
			parent(0), off(0) { }
	hybrid_vector_const_iterator(const iterator& it) :
			parent(it.parent), off(it.off) { }
	hybrid_vector_const_iterator(const const_iterator& it) :
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

	const_reference operator*() const {
		return (*parent)[off];
	}

	const_pointer operator->() {
		return &((*parent)[off]);
	}

	const_reference operator[](size_type n) {
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
		iterator tmp = *this;
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
