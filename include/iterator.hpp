/**@file iterator.hpp
 *
 * This is part of uboost, a lightweight tools/helper functions and classes
 * implementation that supposed  to help bring c++11 feature/experience to
 * embedded software development. This library is an implementation
 * subset/streamlined version of c+11 boost/std library so to fit and
 * compatible with embedded compiler/environment(given the compiler front-end
 * compliance with c++11 specification).
 *
 * @author Oky Firmansyah <mail@okyfirmansyah.net>.
 *
 * @date Created      : Thu Jan 28 15:18:45 2016 okyfirmansyah
 */

#include "config.hpp"

#ifndef UBOOST_ITERATOR_HPP_
#define UBOOST_ITERATOR_HPP_

#include "uboost.hpp"

namespace uboost
{

/**
 *  @brief struct tag to define that an iterator
 *  has bidirectional characteristic
 */
struct bidirectional_iterator_tag {};

/**
 *  @brief struct tag to define that an iterator
 *  has one-way/forward characteristic
 */
struct forward_iterator_tag {};

/**
 *  @brief struct tag to define that an iterator
 *  has input characteristic
 */
struct input_iterator_tag {};

/**
 *  @brief struct tag to define that an iterator
 *  has output characteristic
 */

struct output_iterator_tag {};

/**
 *  @brief struct tag to define that an iterator
 *  has random-accessed characteristic
 */
struct random_access_iterator_tag {};

/**
 *  @brief defines suitable integer type to contains
 *  difference between two pointer of object
 */
typedef int16_t ptrdiff_t;

/**
 *  @brief Base class of all iterator. This struct
 *  makes an iterator inspectable using iterator_traits.
 *  (for e.g to compile-check an iterator assignment)
 */
template<class Cat,
         class T,
		 class Dist=ptrdiff_t,
         class Ptr=T*,
		 class Ref=T&>
struct iterator {
	typedef Cat  iterator_category;
	typedef T    value_type;
	typedef Dist difference_type;
	typedef Ptr  pointer;
	typedef Ref  reference;
};

/**
 *  @brief Helper class to inspect iterator traits
 */
template<class Iter>
struct iterator_traits
{
	typedef typename Iter::iterator_category iterator_category;
	typedef typename Iter::value_type value_type;
	typedef typename Iter::difference_type difference_type;
	typedef typename Iter::pointer pointer;
	typedef typename Iter::reference reference;
};

/**
 *  @brief Helper class to inspect iterator traits(pointer specialization)
 */
template<class T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T *pointer;
	typedef T& reference;
};

/**
 *  @brief Helper class to inspect iterator traits(const pointer
 *  specialization)
 */
template<class T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef const T *pointer;
	typedef const T& reference;
};

/**
 *  @brief Predefined iterator: back_insert_iterator
 *  (please refer to C++11 iterator reference)
 */
template <class Container>
class back_insert_iterator:public iterator<output_iterator_tag,
                                           void,
							    		   void,
										   void,
										   void>
{
	protected:
	Container* container;

	public:
	typedef Container container_type;

	explicit back_insert_iterator (Container& x):container(x){};

	back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
	{
		container->push_back(value);
		return *this;
	};

	back_insert_iterator<Container>& operator=(typename Container::value_type&& value)
	{
		container->push_back(move(value));
		return *this;
	};

	back_insert_iterator<Container>& operator*()
	{
		return *this;
	};

	back_insert_iterator<Container>& operator++()
	{
		// no operation actually
		return *this;
	};

	back_insert_iterator<Container> operator++(int)
	{
		// no operation actually
		return *this;
	};
};

/**
 *  @brief Predefined iterator: front_insert_iterator
 *  (please refer to C++11 iterator reference)
 */
template <class Container>
class front_insert_iterator:public iterator<output_iterator_tag,
                                             void,
											 void,
											 void,
											 void>
{
	protected:
	Container* container;

	public:
	typedef Container container_type;

	explicit front_insert_iterator(Container& x):container(&x){};

	front_insert_iterator<Container>& operator=(const typename Container::value_type& value)
	{
		container->push_front(value);
		return *this;
	};

	front_insert_iterator<Container>& operator=(typename Container::value_type&& value)
	{
		container->push_front(move(value));
		return *this;
	};

	front_insert_iterator<Container>& operator*()
	{
		return *this;
	};

	front_insert_iterator<Container>& operator++()
	{
		// no operation actually
		return *this;
	};

	front_insert_iterator<Container> operator++(int)
	{
		// no operation actually
		return *this;
	};
};

/**
 *  @brief Predefined iterator: insert_iterator
 *  (please refer to C++11 iterator reference)
 */
template <class Container>
class insert_iterator: public iterator<output_iterator_tag,
                                       void,
									   void,
									   void,
									   void>
{
	protected:
	Container* container;
	typename Container::iterator iter;

	public:
	typedef Container container_type;

	explicit insert_iterator(Container& x, typename Container::iterator i)
	:container(&x),iter(i){};

	insert_iterator<Container>& operator=(const typename Container::value_type& value)
	{
		iter=container->insert(iter,value);
		++iter;
		return *this;
	};

	insert_iterator<Container>& operator=(typename Container::value_type&& value)
	{
		iter=container->insert(iter,move(value));
		++iter;
		return *this;
	};

	insert_iterator<Container>& operator*()
	{
		return *this;
	};
	insert_iterator<Container>& operator++()
	{
		// no operation actually
		return *this;
	};
	insert_iterator<Container> operator++(int)
	{
		// no operation actually
		return *this;
	};
};

/**
 *  @brief Predefined iterator: move_iterator
 *  (please refer to C++11 iterator reference)
 */
template <class Iterator>
class move_iterator
{
	Iterator cur;

	public:
	typedef Iterator iterator_type;
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type        value_type;
	typedef typename iterator_traits<Iterator>::difference_type   difference_type;
	typedef Iterator pointer;
	typedef value_type&& reference;

	move_iterator(){};
	explicit move_iterator(Iterator it)
	:cur(it){};
	template<class Iter> move_iterator(const move_iterator<Iter>& it):
	cur(it.cur){};
	template<class Iter> move_iterator& operator=(const move_iterator<Iter>& it)
	{
		cur=it.cur;
	};

	iterator_type base() const
	{
		return cur;
	};

	pointer operator->() const
	{
		return cur;
	};

	reference operator*() const
	{
		return move(*cur);
	};

	move_iterator& operator++()
	{
		++cur;
		return *this;
	};

	move_iterator& operator--(){
		--cur;
		return *this;
	};

	move_iterator operator++(int)
	{
		move_iterator temp = *this;
		++cur;
		return temp;
	};

	move_iterator operator--(int)
	{
		move_iterator temp = *this;
		--cur;
		return temp;
	};

	move_iterator operator+(difference_type n)const
	{
		return move_iterator(cur+n);
	};

	move_iterator operator-(difference_type n)const
	{
		return move_iterator(cur-n);
	};

	move_iterator& operator+=(difference_type n)
	{
		cur+=n;
		return *this;
	};

	move_iterator& operator-= (difference_type n)
	{
		cur-=n;
		return *this;
	}

	auto operator[](difference_type n) const ->decltype(move(cur[n]))
	{
		return move(cur[n]);
	}

	bool operator==(const move_iterator& C) const
    {
		return (cur==C.cur);
	};

	bool operator!=(const move_iterator&C) const
    {
		return !(operator==(C));
	};

	bool operator<(const move_iterator&C) const
	{
		return (cur<C.cur);
	};

	bool operator>(const move_iterator&C) const
	{
		return (cur>C.cur);
	};

	bool operator<=(const move_iterator&C) const
	{
		return (cur<=C.cur);
	};

    bool operator>=(const move_iterator&C) const
	{
		return (cur>=C.cur);
	};
};

/**
 *  @brief Predefined iterator: reverse_iterator
 *  (please refer to C++11 iterator reference)
 */
template <class Iterator>
class reverse_iterator
{
	Iterator cur;

	public:
	typedef Iterator iterator_type;
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type        value_type;
	typedef typename iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename iterator_traits<Iterator>::pointer           pointer;
	typedef typename iterator_traits<Iterator>::reference         reference;

	reverse_iterator(){};
	explicit reverse_iterator(Iterator it)
	:cur(it){};
	template <class Iter> reverse_iterator(const reverse_iterator<Iter>& it)
	:cur(it.cur){};
	template <class Iter> reverse_iterator& operator=(const reverse_iterator<Iter>& it)
	{
		cur = it.cur;
	};

	iterator_type base() const
	{
		return cur;
	};

	pointer operator->() const
	{
		return cur[-1];
	};

	reference operator*() const
	{
		return cur[-1];
	};

	reverse_iterator& operator++()
	{
		--cur;
		return *this;
	};

	reverse_iterator& operator--(){
		++cur;
		return *this;
	};

	reverse_iterator operator++(int)
	{
		reverse_iterator temp = *this;
		--cur;
		return temp;
	};

	reverse_iterator operator--(int)
	{
		reverse_iterator temp = *this;
		++cur;
		return temp;
	};

	reverse_iterator operator+(difference_type n)const
	{
		return reverse_iterator(cur-n);
	};

	reverse_iterator operator-(difference_type n)const
	{
		return reverse_iterator(cur+n);
	};

	reverse_iterator& operator+=(difference_type n)
	{
		cur-=n;
		return *this;
	};

	reverse_iterator& operator-= (difference_type n)
	{
		cur+=n;
		return *this;
	}

	auto operator[](difference_type n) const ->decltype(cur[n])
	{
		return cur[n-1];
	}


	bool operator==(const reverse_iterator& C) const
    {
		return (cur==C.cur);
	};

	bool operator!=(const reverse_iterator&C) const
    {
		return !(operator==(C));
	};

	bool operator<(const reverse_iterator&C) const
	{
		return (cur>C.cur);
	};

	bool operator>(const reverse_iterator&C) const
	{
		return (cur<C.cur);
	};

	bool operator<=(const reverse_iterator&C) const
	{
		return (cur>=C.cur);
	};

    bool operator>=(const reverse_iterator&C) const
	{
		return (cur<=C.cur);
	};
};

// TODO:
// istreambuf_iterator
// istream_iterator
// ostreambuf_iterator
// ostream_iterator

}// uboost

#endif /* UBOOST_ITERATOR_HPP_ */
