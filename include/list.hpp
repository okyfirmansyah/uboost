
/**@file list.hpp
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
 * @date Created      : Thu April 28 10:08:45 2016 okyfirmansyah
 */

#include "config.hpp"

#ifndef UBOOST_LIST_HPP_
#define UBOOST_LIST_HPP_

#include "uboost.hpp"
#include "iterator.hpp"


namespace uboost
{

namespace _internal
{

template <class T>
struct listitem
{
    uint8_t buf[sizeof(T)];

    // use index instead pointer, null is marked by Idx=MAX(uint16_t)
    uint16_t nextIdx;
    uint16_t prevIdx;
};

}; // namespace internal

template <class T>
class listpool
{
    public:
    listpool(_internal::listitem<T>* __list, uint16_t __cap)
    :_sz(0),_cap(__cap),_list(__list),_startIdx(-1)
    {
        uint16_t  i;
        // initialize all list item
        for(i=0;i<_cap;i++)
        {
            _list[i].nextIdx=i+1;
            _list[i].prevIdx=i-1;
        };
        if(i)
        {
            _list[i-1].nextIdx=-1;
        };
        _startIdx=0;
    };

    listpool(const listpool&)=delete;
    listpool(listpool&&)=delete;
    listpool& operator=(const listpool&)=delete;
    listpool& operator=(listpool&&)=delete;

    ~listpool()
    {
       // do nothing. list pool only handles
       // list items as container.
       // Underlying content destuction should be done on list class
       // level
    }

    //empty
	bool available() const
	{
		return _sz<_cap;
	};

    //size
	size_type size() const
	{
		return _sz;
	}

    //max_size
	size_type max_size() const
	{
		return _cap;
	};

    private:
    _internal::listitem<T>* _list;
    uint16_t _sz;
    uint16_t _cap;

    // use index instead pointer, null is marked by Idx=MAX(uint16_t)
    uint16_t _startIdx;
};

template <class T>
class list
{
    public:
    typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    explicit list (listpool<T>& listpool)
    {
       !!!  here here
    }

    explicit list (listpool<T>& listpool, size_type n)
    :list(listpool)
    {
    };

    explicit list (listpool<T>& listpool, size_type n, const value_type& val)
    :list(listpool)
    {
    };

    template <class InputIterator>
    list (listpool<T>& listpool, InputIterator first, InputIterator last)
    :list(listpool)
    {
    };

    //copy constructor
    list (const list& x)=delete;
    //move constructor
    list (list&& x)=delete;

    //TODO: support initializer list
    //list (initializer_list<value_type> il,
    //   const allocator_type& alloc = allocator_type());

    //destructor
    ~list();

    //copy operation
    list& operator= (const list& x);
    //move operation
    list& operator= (list&& x);
    //TODO: support initializer_list
    //list& operator= (initializer_list<value_type> il);

    private:
    template <bool is_const=false>
	struct iterator_base
	{
	};
	public:
    typedef iterator_base<false>       iterator;
	typedef iterator_base<true>        const_iterator;
	typedef uboost::reverse_iterator<iterator> reverse_iterator;
	typedef uboost::reverse_iterator<const_iterator> const_reverse_iterator;

    //begin
	iterator begin()
	{
		return iterator(*this, 0);
	};

	const_iterator begin() const
	{
		return const_iterator(*this, 0);
	};

	//end
	iterator end()
	{
		return iterator(*this, _sz);
	};

	const_iterator end() const
	{
		return const_iterator(*this, _sz);
	};

	//cbegin
	const_iterator cbegin() const
	{
		return const_iterator(*this, 0);
	};

	//cend
	const_iterator cend() const
	{
		return const_iterator(*this, _sz);
	};

	//constant reverse iterators
	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(const_iterator(*this, 0));
	};

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(const_iterator(*this, _sz));
	};

	//rbegin
	reverse_iterator rbegin()
	{
		return reverse_iterator(iterator(*this, 0));
	};

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(const_iterator(*this, 0));
	};

	//rend
	reverse_iterator rend()
	{
		return reverse_iterator(iterator(*this, _sz));
	};

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(const_iterator(*this, _sz));
	};

    //empty
	bool empty() const
	{
		return (_sz==0);
	};

    //size
	size_type size() const
	{
		return _sz;
	}

    //max_size
	size_type max_size() const
	{
		return _capacity;
	};

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    template <class InputIterator>
      void assign (InputIterator first, InputIterator last);
    void assign (size_type n, const value_type& val);
    // TODO: support initializer_list
    //void assign (initializer_list<value_type> il);

    template <class... Args>
      void emplace_front (Args&&... args);

    void push_front (const value_type& val);
    void push_front (value_type&& val);

    void pop_front();

    template <class... Args>
      void emplace_back (Args&&... args);

    void push_back (const value_type& val);
    void push_back (value_type&& val);

    void pop_back();

    template <class... Args>
      iterator emplace (const_iterator position, Args&&... args);



    iterator insert (const_iterator position, const value_type& val);
    iterator insert (const_iterator position, size_type n, const value_type& val);
    template <class InputIterator>
      iterator insert (const_iterator position, InputIterator first, InputIterator last);
    iterator insert (const_iterator position, value_type&& val);
    //TODO: support initializer_list
    //iterator insert (const_iterator position, initializer_list<value_type> il);

    iterator erase (const_iterator position);
    iterator erase (const_iterator first, const_iterator last);

    void swap (list& x)
    {
        //TODO: support this
        assert(FALSE);
    }

    void resize (size_type n);
    void resize (size_type n, const value_type& val);

    void clear();

    void splice (const_iterator position, list& x);
    void splice (const_iterator position, list&& x);
    void splice (const_iterator position, list& x, const_iterator i);
    void splice (const_iterator position, list&& x, const_iterator i);
    void splice (const_iterator position, list& x,
                 const_iterator first, const_iterator last);
    void splice (const_iterator position, list&& x,
                 const_iterator first, const_iterator last);

    void remove (const value_type& val);
    template <class Predicate>
      void remove_if (Predicate pred);

    void unique();
    template <class BinaryPredicate>
      void unique (BinaryPredicate binary_pred);

  void merge (list& x);
  void merge (list&& x);
  template <class Compare>
    void merge (list& x, Compare comp);
  template <class Compare>
    void merge (list&& x, Compare comp);

  void sort();
  template <class Compare>
    void sort (Compare comp);

  void reverse();

  //TODO: support relational operators
};

namespace declare
{

template <class T, uint16_t cap>
class listpool:public uboost::listpool<T>
{
    public:
    listpool()
    :boost::listpool<T>(_buf, cap);
    listpool(const listpool&)=delete;
    listpool(listpool&&)=delete;
    listpool& operator=(const listpool&)=delete;
    listpool& operator=(listpool&&)=delete;

    ~listpool();

    private:
    _internal::listitem<T> _buf[cap];
};

};//namespace declare

};//namespace boost
#endif
