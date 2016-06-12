
/**@file vector.hpp
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
 * @date Created      : Thu Jan 28 11:18:45 2016 okyfirmansyah
 */

#include "config.hpp"

#ifndef UBOOST_VECTOR_HPP_
#define UBOOST_VECTOR_HPP_

#include "uboost.hpp"
#include "iterator.hpp"

namespace uboost
{

namespace internal{
template <bool have_default_const>
struct resize_dispatch
{
    template<class vector>
    static void emplace_narg(vector& v)
    {
        v.emplace_back();
    }
};

template <>
struct resize_dispatch<false>
{
    template<class vector>
    static void emplace_narg(vector& v)
    {
        assert(false);
    }
};

};//internal
template <class T>
class vector
{
	public:
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	protected:
	/*struct T_container
	{
		uint8_t buf[sizeof(T)];
		reference get()
		{
			return *((T*)buf);
		}
		const_reference get()const
		{
			return *((T*)buf);
		}
	};*/

	// constructors
	protected:
	vector(T* __buff, size_type __cap)
	:_buf(__buff),_capacity(__cap),_sz(0)
	{};
	public:
	vector()=delete;
	vector(const vector& x)=delete;
	vector(vector&& x)=delete;
	//vector(initializer_list<T> il);

	//destructor
	virtual ~vector()
	{
		resize(0);
	};

	//at
	reference at(size_type n)
	{
		rangecheck(n);
		assert(n<_sz);
		return _buf[n];
	};

	const_reference at(size_type n)const
	{
		rangecheck(n);
		assert(n<_sz);
		return _buf[n];
	};

	//back
	reference back()
	{
		return at(_sz-1);
	};

	const_reference back() const
	{
		return at(_sz-1);
	};

	//capacity
	size_type capacity() const
	{
		return _capacity;
	};

	//clear
	void clear()
	{
		resize(0);
	};

	//data
	T* data()
	{
		return _buf;
	};

	const T* data() const
	{
		return _buf;
	};

	//emplace_back
	template <class... Args>
	void emplace_back (Args&&... args)
	{
		rangecheck(_sz);
		new(&_buf[_sz++]) T(args...);
	};

	//empty
	bool empty() const
	{
		return (_sz==0);
	};

	//front
	reference front()
	{
		return at(0);
	};

	const_reference front() const
	{
		return at(0);
	};


	//max_size
	size_type max_size() const
	{
		return _capacity;
	};

	//copy/move assign operator
	vector& operator=(const vector& x)
	{
        this->assign(x.cbegin(), x.cend());
        return *this;
	};

	vector& operator=(vector&& x)
	{
	    this->assign(move_iterator <vector::iterator>(x.begin()),
	                 move_iterator <vector::iterator>(x.end()));
        x.clear();
	};
	//vector& operator= (initializer_list<value_type> il);

	//element access operator
	reference operator[](size_type n)
	{
		return at(n);
	};

	const_reference operator[] (size_type n) const
	{
		return at(n);
	};

	//pop_back
	void pop_back()
	{
		assert(_sz--);
		at(_sz).~T();
	};

	//push_back
    template <typename Tt>
	void push_back(Tt&& val)
	{
		rangecheck(_sz);
		new(&_buf[_sz++]) T(forward<Tt>(val));
	}

	//reserve
	void reserve(size_type n)
	{
		assert(n<=_capacity);
	}

	//resize
	void resize(size_type n, const T& val)
	{
        while(n>_sz)
            push_back(val);
	    while(n<_sz)
            pop_back();
	};

	void resize(size_type n)
	{
		//!!!TODO: need workaround to allow vector
		//containing object with no default constructor
        //typedef internal::resize_dispatch<true> dispatcher;
        while(n>_sz)
            emplace_back();
            //dispatcher::emplace_narg(*this);
	    while(n<_sz)
            pop_back();
	};

	//shrink_to_fit
	void shrink_to_fit()
	{
        // do nothing
	}

	//size
	size_type size() const
	{
		return _sz;
	}

	//swap
	void swap (vector& x)
	{
	    //TODO: support this
        assert(false);
	}

	private:
	void rangecheck(int n)
	{
		if((n<0)||(n>=_capacity))assert(false);
	};
	protected:

	T* _buf;
	size_type _capacity;
	size_type _sz;

	template <bool is_const=false>
	struct iterator_base/*:public
						 uboost::iterator<random_access_iterator_tag,
	                                      T,
										  ptrdiff_t,
										  typename conditional<is_const,const T*,T*>::type,
										  typename conditional<is_const,const T&,T&>::type>*/
	{
		public:

		typedef random_access_iterator_tag  iterator_category;
		typedef T                           value_type;
		typedef ptrdiff_t                   difference_type;
		typedef typename conditional<is_const,const T*,T*>::type  pointer;
		typedef typename conditional<is_const,const T&,T&>::type  reference;


	    iterator_base(typename conditional<is_const,const vector&,vector&>::type x,
	                  const ptrdiff_t &_pos):container(&x),pos(_pos){};

		iterator_base& operator++()
		{
			pos++;
			return *this;
		};

		iterator_base operator++(int)
		{
			iterator_base temp = *this;
			pos++;
			return temp;
		};

		iterator_base& operator--()
		{
			pos--;
			return *this;
		};

		iterator_base operator--(int)
		{
			iterator_base temp = *this;
			pos--;
			return temp;
		};

		template<bool _B>
		bool operator==(const iterator_base<_B>& C) const
		{
			return (pos==C.pos);
		};

		template<bool _B>
		bool operator!=(const iterator_base<_B>&C) const
		{
			return (pos!=C->pos);
		};

		template<bool _B>
		bool operator<(const iterator_base<_B>&C) const
		{
			return (pos<C.pos);
		};

		template<bool _B>
		bool operator>(const iterator_base<_B>&C) const
		{
			return (pos>C.pos);
		};

		template<bool _B>
		bool operator<=(const iterator_base<_B>&C) const
		{
			return (pos<=C.pos);
		};

		template<bool _B>
		bool operator>=(const iterator_base<_B>&C) const
		{
			return (pos>=C.pos);
		};

        reference operator*() const
		{
			return container->_buf[pos];
		};

        pointer operator->() const
		{
			return &(container->_buf[pos]);
		};

        reference operator[](difference_type i) const
		{
			return container->_buf[pos+i];
		};

		iterator_base operator+(difference_type n)const
		{
			iterator_base temp=*this;
			temp.pos+=n;
			return temp;
		};

		iterator_base operator-(difference_type n)const
		{
			iterator_base temp=*this;
			temp.pos-=n;
			return temp;
		};

		iterator_base& operator+=(difference_type n)
		{
			pos+=n;
			return *this;
		};

		iterator_base& operator-= (difference_type n)
		{
			pos-=n;
			return *this;
		}

		private:
		typename conditional<is_const,const vector*,vector*>::type  container;
		ptrdiff_t pos;
	};


	public:
	typedef iterator_base<false>       iterator;
	typedef iterator_base<true>        const_iterator;
	typedef uboost::reverse_iterator<iterator> reverse_iterator;
	typedef uboost::reverse_iterator<const_iterator> const_reverse_iterator;

	private:
	// this is internal function and not should be called alone as
	// this function temporarily leave list in undefined state
	void open_gap(iterator first, size_type n)
	{
	    if(!n)return;
        assert((_sz+n)<=_capacity);
        _sz+=n;
        reverse_iterator rfirst(rend());
        reverse_iterator rlast(first);
        int i=0;
        for(;rfirst!=rlast;rfirst++)
        {
			if((i++)<n)
			    new(&rfirst[0]) T(move(rfirst[n]));
			else
                *rfirst=move(rfirst[n]);
        };
		rfirst[n].~T();
	};

	public:
	//assign
	template <class InputIterator>
	void assign (InputIterator first, InputIterator last)
	{
        iterator thisIt=begin();
        size_type i=0;
        for(InputIterator it=first;
            it<last ;
            it++)
        {
            assert(i<_capacity);
            if((thisIt+i)<end())
            {
                //T temp(*it);
				//this:
                //thisIt[i].~T();
                //new(&thisIt[i++]) T(move(temp));
				//or, use move instead:
				//thisIt[i++]=move(temp);
				thisIt[i++]=*it;
            }else
            {
                //push_back(*it);
                new(&thisIt[i++]) T(*it);
            }
        }
        if(!i &&_sz)
           resize(0);
        else
           _sz=i;
	};

	void assign (size_type n, const T& val)
	{
	    resize(0);
        for(size_type i=0;i<n;i++)
            push_back(val);
	}
	//void assign (initializer_list<value_type> il);

	//begin
	iterator begin()
	{
		return iterator(*this, 0);
	};

	const_iterator begin() const
	{
		return const_iterator(*this, 0);
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
	}

	//emplace
	template <class... Args>
	iterator emplace (const_iterator position, Args&&... args)
	{
	    rangecheck(_sz);
	    open_gap(position, 1);
	    new(&position[0]) T(args...);
	    return position;
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

	//erase
	iterator erase (const_iterator position)
	{
        return erase(position, position+1);
    };

	iterator erase (const_iterator first, const_iterator last)
	{
	    const_iterator saved=first;
	    size_type i=0;
        while(first<last)
        {
            if(first<cend())
            {
                first->~T();
            }break;
            first++;
            i++;
        };
        iterator deleted=saved;
        while(first<cend())
        {
            *deleted++=move(*first);
            first++->~T();
        }
        _sz-=i;
        return saved;
	};

	//insert
	iterator insert (const_iterator position, size_type n, const value_type& val)
	{
	    if(n){
	    open_gap(position, n);
        while(n)
        {
            new (&position[--n]) T(val);
        };
        }
        return iterator(position);
	};

	iterator insert (const_iterator position, const value_type& val)
	{
	    return insert(position, 1, val);
	};
	template <class InputIterator>
	iterator insert (const_iterator position, InputIterator first, InputIterator last)
	{
	    // !!!TODO: support this
	    assert(false);
	};

	iterator insert (const_iterator position, value_type&& val)
	{
	    open_gap(position, 1);
        new (&position[0]) T(move(val));
        return iterator(position);
	};
	//iterator insert (const_iterator position, initializer_list<value_type> il);

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

	//TODO: support relational operators
};

namespace declare
{
template <class T, uint16_t Size>
class vector:public uboost::vector<T>
{
	public:
	// constructors
	vector()
	:uboost::vector<T>((T*)_physBuf, Size){};

	vector(const uboost::vector<T>& x)
	:vector()
	{
	    assign(x.cbegin(), x.cend());
	};
	vector(uboost::vector<T>&& x)
	:vector()
	{
	    assign(move_iterator<typename uboost::vector<T>::iterator>(x.begin()),
	           move_iterator<typename uboost::vector<T>::iterator>(x.end()));
        x.clear();
	};
	//vector(initializer_list<T> il);

    vector& operator=(const uboost::vector<T>& x)
	{
        assign(x.cbegin(), x.cend());
	};

	vector& operator=(uboost::vector<T>&& x)
	{
	    assign(move_iterator<typename uboost::vector<T>::iterator>(x.begin()),
	           move_iterator<typename uboost::vector<T>::iterator>(x.end()));
        x.clear();
	};

	virtual ~vector()
	{
	    // base::~vector() will do contents destruction
	};

	private:
	//typedef typename uboost::vector<T>::T_container T_container;
	uint8_t _physBuf[sizeof(T)*Size];
};

}// declare


}// uboost

#endif /* UBOOST_VECTOR_H_ */
