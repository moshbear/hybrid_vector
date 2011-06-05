/* hybrid_vector/hybrid_vector.h - a template hybrid vector class
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

#ifndef HYBRID_VECTOR_HYBRID_VECTOR_H
#define HYBRID_VECTOR_HYBRID_VECTOR_H

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>
#include <stxxl/vector>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/current_function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/once.hpp>

#include <hybrid_vector/c99int.h>
#include <hybrid_vector/fwd.h>
#include <hybrid_vector/iterator.h>
#include <hybrid_vector/const_iterator.h>
#include <hybrid_vector/pmf.h>

#define HYBRID_VECTOR_PP_CONCAT_2(x,y) x##y
#define HYBRID_VECTOR_PP_CONCAT(x,y) HYBRID_VECTOR_PP_CONCAT_2(x,y)

template <typename T,
	  typename rv = std::vector<T>,
	  typename dv = stxxl::vector<T> >
class hybrid_vector
{
public:
	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	// i'm going with the larger of the two (size|difference)_type(s)
	typedef uint64_t size_type;
	typedef int64_t difference_type;
	typedef const value_type* const_pointer;

	friend class hybrid_vector_iterator<T, rv, dv>;
	typedef hybrid_vector_iterator<T, rv, dv> iterator;
	friend class hybrid_vector_const_iterator<T, rv, dv>;
	typedef hybrid_vector_iterator<T, rv, dv> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
	typedef hybrid_vector_pmf<T, rv, dv> pmf;
	size_type size_;

	// The size which triggers swap_container
	size_type swap_size;

	// Forces the use of one container exclusively
	bool force_ram;
	bool force_disk;

	boost::scoped_ptr<rv> p_rv;
	boost::scoped_ptr<dv> p_dv;

protected:
	enum selector {
		uninit = 0,
		ram = 1,
		disk = 2,
	} state;
public:
	hybrid_vector(size_type n = 0, size_type swap_size_ = 128<<20 /* 128 MB */,
	              bool force_ram_ = 0, bool force_disk_ = 0) :
			size_(n),
			swap_size(swap_size_),
			force_ram(force_ram_),
			force_disk(force_disk_) {
		__ctor_init(n);
	}

	template <typename InIt>
	hybrid_vector(InIt _Start, InIt _End, size_type swap_size_ = 128<<20,
			bool force_ram_ = 0, bool force_disk_ = 0) :
			size_(std::distance(_Start, _End)),
			swap_size(swap_size_),
			force_ram(force_ram_),
			force_disk(force_disk_) {
		__ctor_init(0);
		assign(_Start, _End);
	}

	hybrid_vector(const hybrid_vector& vec) :
			size_(vec.size_),
			swap_size(vec.swap_size),
			force_ram(vec.force_ram),
			force_disk(vec.force_disk),
			state(vec.state) {
		vec.check_consistency();
		switch (state) {
		case ram:
			p_rv.reset(new rv(*vec.p_rv));
			break;
		case disk:
			p_dv.reset(new dv(*vec.p_dv));
			break;
		default:
			;
		}
	}

	hybrid_vector& operator=(const hybrid_vector& vec) {
		if (this != &vec) {
			hybrid_vector tmp(vec);
			swap(tmp);
		}
		return *this;
	}

	void swap(hybrid_vector& v);

	bool empty() const {
		return !size_;
	}

	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator begin() const {
		return const_iterator(this, 0);
	}
	iterator end() {
		return iterator(this, size_);
	}
	const_iterator end() const {
		return const_iterator(this, size_);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}
	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}

// HYBRID_VECTOR Vector's Member Function CALL
// Note: abuse of variadic arguments here causes warnings to be generated at maximum level
// E.g. g++-4.4 -Wall -ansi pedantic
#define HYBRID_VECTOR_VMF_CALL(__Func, ... /* return, etc */) \
	do { \
		switch (state) { \
		case ram: \
			__VA_ARGS__ HYBRID_VECTOR_PP_CONCAT(rv_, __Func); \
			break; \
		case disk: \
			__VA_ARGS__ HYBRID_VECTOR_PP_CONCAT(dv_, __Func); \
			break; \
		default: \
			throw std::runtime_error(BOOST_CURRENT_FUNCTION + std::string(": bad state")); \
		}\
	} while(0)

	reference operator [] (size_type n) {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(operator_subscript(n), return);
	}
	const_reference operator [] (size_type n) const {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(operator_subscript(n), return);
	}

	size_type capacity() const {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(capacity(), return);
	}

	size_type size() const {
		return size_;
	}

	void reserve(size_type n) {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(reserve(n));
	}

	void resize(size_type n) {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(resize(n));
		size_ = n;
	}

	void clear() {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(clear);
		swap_containers(-1);
		size_ = 0;
	}

	void push_back(const_reference obj) {
		check_consistency();
		if (real_size(++size_) >= swap_size)
			use_disk();
		HYBRID_VECTOR_VMF_CALL(push_back(obj));
	}

	void pop_back() {
		check_consistency();
		HYBRID_VECTOR_VMF_CALL(pop_back());
		if (real_size(--size_) < swap_size)
			use_ram();
	}

	inline reference back() {
		return (*this)[size_ - 1];
	}

	inline reference front() {
		return (*this)[0];
	}

	inline const_reference back() const {
		return (*this)[size_ - 1];
	}

	inline const_reference front() const {
		return (*this)[0];
	}

	template <typename InIt>
	void assign(InIt _Start, InIt _End) {
		check_consistency();
		typename pmf::dv_size_type n_ = std::distance(_Start, _End);
		size_type n = real_size(n_);
		if (n > swap_size)
			use_disk(); // safe: does nothing if force_ram is set
		else
			use_ram(); // safe: does nothing if force_disk is set
		HYBRID_VECTOR_VMF_CALL(assign(_Start, _End));
		size_ = n;
	}

	// hybrid_vector-specific member function
	template <typename InIt>
	void append(InIt _Start, InIt _End) {
		check_consistency();
		size_type n = real_size(std::distance(_Start, _End) + size_);
		if (n > swap_size)
			use_disk(); // safe: does nothing if force_ram is set
		else
			use_ram(); // safe: does nothing if force_disk is set
		HYBRID_VECTOR_VMF_CALL(bulk_append(_Start, _End));
	}

#undef HYBRID_VECTOR_VMF_CALL

	~hybrid_vector() {
		switch (state) {
		case ram:
			p_rv.reset();
			break;
		case disk:
			p_dv.reset();
			break;
		default:
			;
		}
	}

protected:
	// no overloads => nice and clean :)
	typename pmf::rv_size_type rv_capacity() const {
		return p_rv->capacity();
	}
	typename pmf::dv_size_type dv_capacity() const {
		return p_dv->capacity();
	}

	typename pmf::rv_size_type rv_size() const {
		return p_rv->size();
	}
	typename pmf::dv_size_type dv_size() const {
		return p_dv->size();
	}
	
	void rv_reserve(typename pmf::rv_size_type _1) {
		p_rv->reserve(_1);
	}
	void dv_reserve(typename pmf::dv_size_type _1) {
		p_dv->reserve(_1);
	}

	void rv_resize(typename pmf::rv_size_type _1) {
		p_rv->resize(_1);
	}
	void dv_resize(typename pmf::dv_size_type _1) {
		p_dv->resize(_1);
	}

	void rv_clear() {
		p_rv->clear();
	}
	void dv_clear() {
		p_dv->clear();
	}

	void rv_push_back(const T& _1) {
		p_rv->push_back(_1);
	}
	void dv_push_back(const T& _1) {
		p_dv->push_back(_1);
	}

	void rv_pop_back() {
		p_rv->pop_back();
	}
	void dv_pop_back() {
		p_dv->pop_back();
	}

	// overloads: get correct pmf via typedef then do call
	// a bit messier than the "clean ones"
	reference rv_operator_subscript(typename pmf::rv_size_type _1) {
		static const typename pmf::rv_get_ref p(&rv::operator[]);
		return (p_rv->*p)(_1);
	}
	reference dv_operator_subscript(typename pmf::dv_size_type _1) {
		static const typename pmf::dv_get_ref p(&dv::operator[]);
		return (p_dv->*p)(_1);
	}
	
	const_reference rv_operator_subscript(typename pmf::rv_size_type _1) const {
		static const typename pmf::rv_get_cref p(&rv::operator[]);
		return (p_rv->*p)(_1);
	}
	const_reference dv_operator_subscript(typename pmf::dv_size_type _1) const {
		static const typename pmf::dv_get_cref p(&dv::operator[]);
		return (p_dv->*p)(_1);
	}

	template <typename InIt>
	void rv_assign(InIt _Start, InIt _End) {
		rv_clear();
		(*p_rv).template assign(_Start, _End);
	}
	template <typename InIt>
	void dv_assign(InIt _Start, InIt _End) {
		dv_clear();
		(*p_dv).template set_content(_Start, _End, std::distance(_Start, _End));
	}
	
	template <typename InIt>
	void rv_bulk_append(InIt _Start, InIt  _End) {
		rv_reserve(rv_size() + std::distance(_Start, _End));
		(*p_rv).template insert(p_rv->end(), _Start, _End);
	}
	template <typename InIt>
	void dv_bulk_append(InIt _Start, InIt _End) {
		dv_reserve(dv_size() + std::distance(_Start, _End));
		std::for_each(_Start, _End, boost::bind(&dv_push_back, _1));
	}

	void use_ram(bool and_stay_there = 0) {
		force_disk = force_ram = 0;
		swap_containers(-1);
		force_ram = and_stay_there;
	}
	void use_disk(bool and_stay_there = 0) {
		force_disk = force_ram = 0;
		swap_containers(1);
		force_disk = and_stay_there;
	}

private:
	void __ctor_init(size_type n) {
		if (force_ram && force_disk)
			throw std::invalid_argument("both force_ram and force_disk are enabled");
		size_type true_size = real_size(n);
		if (force_disk || true_size > swap_size) {
			state = disk;
			p_dv.reset(new dv(n));
		} else {
			state = ram;
			p_rv.reset(new rv(n));
		}
		check_consistency();
	}

	/* This is a sanity check to ensure that the invariants are correct.
	 * The following are verified:
	 * 	* only one container is active
	 * 	* state is correct
	 *
	 * Naturally, there is noticeable overhead, hence pass NDEBUG to the compiler when
	 * debugging is not required.
	 */
	void check_consistency();

	/* Swaps containers upon request.
	 * Use of @param direction allows us to avoid infinite loops
	 * of swapping.
	 * If >0, move to disk container _if not yet done_.
	 * If <0, move to ram container _if not yet done_.
	 */
	void swap_containers(signed char direction);

	static size_type real_size(size_type n) {
		return n * sizeof(T);
	}

};

template <typename T, typename rv, typename dv>
void hybrid_vector<T, rv, dv>::swap(hybrid_vector<T, rv, dv>& v)
{
	std::swap(size_, v.size_);
	std::swap(swap_size, v.swap_size);
	std::swap(force_ram, v.force_ram);
	std::swap(force_disk, v.force_disk);
	std::swap(state, v.state);
	p_rv.swap(v.p_rv);
	p_dv.swap(v.p_dv);
}

template <typename T, typename rv, typename dv>
void hybrid_vector<T, rv, dv>::check_consistency()
{
#ifndef NDEBUG
	BOOST_ASSERT((state == ram) ^ (state == disk));
	// get the state via slow method
	bool active_ram = p_rv.get() != 0;
	bool active_disk = p_dv.get() != 0;
	BOOST_ASSERT(active_ram ^ active_disk);
	// verify that curr points to the active object and that 'state' is correct
	if (active_ram) BOOST_ASSERT(state == ram);
	// XOR assertion passed, so 'else' not necessary
	if (active_disk) BOOST_ASSERT(state == disk);
#endif
}

template <typename T, typename rv, typename dv>
void hybrid_vector<T, rv, dv>::swap_containers(signed char direction)
{
	if (force_ram || force_disk) // should we even bother?
		return;
	check_consistency();
	if (state == ram && direction > 0) { // ram->disk
		p_dv.reset(new dv);
		dv_assign(p_rv->begin(), p_rv->end());
		p_rv.reset();
		state = disk;
	} else if (state == disk && direction < 0) { // disk->ram
		p_rv.reset(new rv);
		rv_assign(p_dv->begin(), p_dv->end());
		p_dv.reset();
		state = ram;
	}
}

#endif

