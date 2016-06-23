
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
class list;

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
       // Underlying content destruction should be done on list class
       // level
    }

    //empty
	uint16_t free() const
	{
		return _cap-_sz;
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

	const _internal::listitem<T>& operator[](uint16_t idx) const
    {
		return _list[idx];
	};

	_internal::listitem<T>& operator[](uint16_t idx)
    {
		return _list[idx];
	};

	void free(uint16_t idx)
	{
        if(_startIdx!=-1)
	        _list[_startIdx].prevIdx=idx;
	    _list[idx].nextIdx=_startIdx;
	    _list[idx].prevIdx=-1;
	    _startIdx=idx;
	};

	uint16_t claim()
	{
	    if(!_startIdx)
	        return -1;
        else
        {
            uint16_t idx=_startIdx;
            _startIdx=_list[_startIdx].nextIdx;
            if(_startIdx!=-1)
                _list[_startIdx].prevIdx=-1;
            return idx;
        }
	};

    private:
    _internal::listitem<T>* _list;
    uint16_t _sz;
    uint16_t _cap;

    // use index instead pointer, null is marked by Idx=MAX(uint16_t)
    uint16_t _startIdx;
	friend class list<T>;
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
    :_listpool(&listpool),_sz(0),_firstIdx(-1),_lastIdx(-1)
    {
    }

    //copy constructor
    list (const list& x)=delete;
    //move constructor
    list (list&& x)=delete;

    //!!!TODO: support constructor variants:
    //explicit list (listpool<T>& listpool, size_type n);
    //explicit list (listpool<T>& listpool, size_type n, const value_type& val);
    //template <class InputIterator>
    //list (listpool<T>& listpool, InputIterator first, InputIterator last);
    //list (listpool<T>& listpool, initializer_list<value_type> il);


    //destructor
    ~list()
    {
        resize(0);
    }

    //copy operation
    list& operator= (const list& x)
    {
        assign(x.cbegin(), x.cend());
    };
    //move operation
    list& operator= (list&& x)
    {
        assign(move_iterator<decltype(x.begin())>(x.begin()),
	           move_iterator<decltype(x.end())>(x.end()));
        x.clear();
    };
    //TODO: support initializer_list
    //list& operator= (initializer_list<value_type> il);

    private:
    template <bool is_const=false>
	struct iterator_base
	{
	    public:
		typedef random_access_iterator_tag  iterator_category;
		typedef T                           value_type;
		typedef ptrdiff_t                   difference_type;
		typedef typename conditional<is_const,const T*,T*>::type  pointer;
		typedef typename conditional<is_const,const T&,T&>::type  reference;
		typedef typename conditional<is_const,const list*,list*>::type container_ptr_type;

        iterator_base(typename conditional<is_const,const list&,list&>::type x,
	                  const ptrdiff_t &_pos):container(&x),end(false)
        {
             //determines idx from pos here
             idx=container->_firstIdx;
             int i=0;
             while((idx!=-1) && i<_pos)
             {
                 idx=container->_listpool->_list[idx].nextIdx;
                 i++;
             }
             if(idx==-1)end=true;
        };
		
		//template <bool _is_const>
		iterator_base(const iterator_base<true> & it) //cross copy constructor
		{
			container=static_cast<iterator_base<is_const>::container_ptr_type>(it.container);
			end=it.end;
			idx=it.idx;
		}
		
		//template <bool _is_const>
		iterator_base& operator= (const iterator_base<true>& it)//cross assign operator
		{
			container=static_cast<iterator_base<is_const>::container_ptr_type>(it.container);
			end=it.end;
			idx=it.idx;
			return *this;
		};
		


        iterator_base& operator++()
		{
		    if(!end)
			    idx=container._listpool[idx].nextIdx;
            end=(idx==-1);
			return *this;
		};

		iterator_base operator++(int)
		{
			iterator_base temp = *this;
			if(!end)
			    idx=container._listpool[idx].nextIdx;
            end=(idx==-1);
			return temp;
		};

		iterator_base& operator--()
		{
		    if(end)
		    {
		        idx=container->_lastidx;
		        end=(idx==-1);
            }else
			    idx=container->_listpool->_list[idx].prevIdx;
			return *this;
		};

		iterator_base operator--(int)
		{
			iterator_base temp = *this;
			if(end)
		    {
		        idx=container->_lastIdx;
		        end=(idx==-1);
            }else
			    idx=container->_listpool->_list[idx].prevIdx;
			return temp;
		}

		template<bool _B>
		bool operator==(const iterator_base<_B>& C) const
		{
			return (idx==C.idx)&&(end==C.end);
		};

		template<bool _B>
		bool operator!=(const iterator_base<_B>&C) const
		{
			return (idx!=C.idx)||(end!=C.end);
		};


        //!!!TODO: do we really have to implement these?
		//template<bool _B>
		//bool operator<(const iterator_base<_B>&C) const;

		//template<bool _B>
		//bool operator>(const iterator_base<_B>&C) const;

		//template<bool _B>
		//bool operator<=(const iterator_base<_B>&C) const;

		//template<bool _B>
		//bool operator>=(const iterator_base<_B>&C) const;


        reference operator*() const
		{
			return container._listpool[idx];
		};

        pointer operator->() const
		{
			return &(container._listpool[idx]);
		};



        reference operator[](difference_type pos) const
		{
             uint16_t k=idx;
             int i=0;
             while((k!=-1) && i<pos)
             {
                 k=container._listpool[k].nextIdx;
                 i++;
             }
             return &(container._listpool[k]);
		};

		iterator_base operator+(difference_type n)const
		{
			 iterator_base temp=*this;

             for(int i=0;i<n;i++)temp++;

			return temp;
		};

		iterator_base operator-(difference_type n)const
		{
			iterator_base temp=*this;

            for(int i=0;i<n;i++)temp--;

			return temp;
		};

		iterator_base& operator+=(difference_type n)
		{
			for(int i=0;i<n;i++)(*this)++;
			return *this;
		};

		iterator_base& operator-= (difference_type n)
		{
			for(int i=0;i<n;i++)(*this)--;
			return *this;
		}

		uint16_t get_idx()const{return idx;};

        private:
        container_ptr_type  container;
		uint16_t idx;
		bool end;
		friend class list<T>;
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

    //!!!max_size
	//size_type max_size() const
	//{
	//	return _cap;
	//};


    reference front()
    {
        return _listpool[_firstIdx];
    }

    const_reference front() const
    {
        return _listpool[_firstIdx];
    };

    reference back()
    {
        return _listpool[_lastIdx];
    };

    const_reference back() const
    {
        return _listpool[_lastIdx];
    };

    template <class InputIterator>
      void assign (InputIterator first, InputIterator last)
    {
        //!!!TODO: need to optimize this
        resize(0);
        for(InputIterator it=first;
            it!=last ;
            it++)
        {
            push_front(*it);
        }
    };

    void assign (size_type n, const value_type& val)
    {
        //!!!TODO: need to optimize this
        resize(0);
        for(size_type i=0;i<n;i++)
            push_back(val);
    };
    // TODO: support initializer_list
    //void assign (initializer_list<value_type> il);


    template <class... Args>
      void emplace_front (Args&&... args)
    {
        uint16_t idx=_listpool.claim();
        _listpool._list[idx].nextIdx=_firstIdx;
        _listpool._list[idx].prevIdx=-1;
        if(_firstIdx!=-1)
            _listpool._list[_firstIdx].prevIdx=idx;
        _firstIdx=idx;
        _sz++;

        new(&_listpool._list[_firstIdx].buf) value_type(uboost::forward<Args>(args)...);
    };


    void push_front (const value_type& val)
    {
       emplace_front(val);
    };

    void push_front (value_type&& val)
    {
        emplace_front(uboost::move(val));
    };

    void pop_front()
    {
        // call destructor manually
        ( (T*)_listpool._list[_firstIdx].buf)->~T();

        uint16_t idx=_listpool._list[_firstIdx].nextIdx;
        if(idx!=-1)
            _listpool._list[idx].prevIdx=-1;
        _listpool.free(_firstIdx);
        _firstIdx=idx;
    };

    template <class... Args>
      void emplace_back (Args&&... args)
    {
        uint16_t idx=_listpool->claim();
        _listpool->_list[idx].nextIdx=-1;
        _listpool->_list[idx].prevIdx=_lastIdx;
        if(_lastIdx!=-1)
            _listpool->_list[_lastIdx].nextIdx=idx;
        _lastIdx=idx;
        _sz++;

        new(&_listpool->_list[_lastIdx].buf) value_type(uboost::forward<Args>(args)...);
    };

    void push_back (const value_type& val)
    {
        emplace_back(val);
    };

    void push_back (value_type&& val)
    {
        emplace_back(uboost::move(val));
    };

    void pop_back()
    {
        // call destructor manually
        ( (T*)_listpool._list[_lastIdx].buf)->~T();

        uint16_t idx=_listpool._list[_lastIdx].prevIdx;
        if(idx!=-1)
            _listpool._list[idx].nextIdx=-1;
        _listpool.free(_lastIdx);
        _lastIdx=idx;
    };

    template <class... Args>
      iterator emplace (const_iterator position, Args&&... args)
    {
        uint16_t dstIdx=position.get_idx();

        uint16_t idx=_listpool.claim();
        _listpool._list[idx].nextIdx=dstIdx;



        if(dstIdx!=-1)
        {
            _listpool._list[idx].prevIdx=_listpool._list[dstIdx].prevIdx;
            _listpool._list[dstIdx].prevIdx=idx;
            if(_listpool._list[idx].prevIdx!=-1)//not empty, middle
                _listpool._list[_listpool._list[idx].prevIdx].nextIdx=idx;
            else// //notempty, begin
                _firstIdx=idx;
        }else
        {
            // //notempty, begin
            //notempty, end
            if(_sz)
            {
                _listpool._list[idx].prevIdx=_lastIdx;
                _lastIdx=idx;
                _listpool._list[_listpool._list[idx].prevIdx].nextIdx=idx;
            }
            //empty, begin/end
            else
            {
                _listpool._list[idx].prevIdx=-1;
                _lastIdx=idx;
                _firstIdx=idx;
            }
        }

        _sz++;

        new(&_listpool._list[idx].buf) value_type(uboost::forward<Args>(args)...);

        return position--;
    };


    iterator insert (const_iterator position, const value_type& val)
    {
        return emplace(position, val);
    };

    iterator insert (const_iterator position, size_type n, const value_type& val)
    {
        iterator it=position;
        for(int i=0;i<n;i++)
            it=emplace(position, val);
        return it;
    };

    template <class InputIterator>
      iterator insert (const_iterator position, InputIterator first, InputIterator last)
    {
        iterator ptr=end();
        for(InputIterator it=first;
            it!=last ;
            it++)
        {
            if(ptr.end)
                ptr=emplace(position,it);
            else
                emplace(position,it);
        }
        if(ptr.end)
        return position;
        else return ptr;
    };

    iterator insert (const_iterator position, value_type&& val)
    {
        return emplace(position, uboost::move(val));
    }
    //TODO: support initializer_list
    //iterator insert (const_iterator position, initializer_list<value_type> il);

    private:
    void cut(const_iterator position)
    {
        uint16_t idx=position.idx;
        if(idx!=-1)
        {
            if(_listpool[idx].nextIdx!=-1)
                 //notempty,middle
                _listpool[_listpool[idx].nextIdx].prevIdx=_listpool[idx].prevIdx;
            else //notempty, prior to end
                _lastIdx=_listpool[idx].prevIdx;
            if(_listpool[idx].prevIdx!=-1)
                 //notempty, middle
                _listpool[_listpool[idx].prevIdx].nextIdx=_listpool[idx].nextIdx;
            else //notempty, begin
                _firstIdx=_listpool[idx].nextIdx;
            _sz--;
        }else //empty begin/end, notempty end
            ;//return end();
    };

    public:
    iterator erase (const_iterator position)
    {
        if(position.idx==-1)
          return end();

        cut(position);

        uint16_t idx=position.idx;
        position++;
        ( (T*)_listpool->_list[idx].buf)->~T();
        _listpool->free(idx);

        return position;
    }

    iterator erase (const_iterator first, const_iterator last)
    {
        const_iterator it=first;
        while(it!=last)
        {
            it=erase(it);
        };
        return it;
    };

    //TODO: support this
    //void swap (list& x)
    //{
    //    assert(false);
    //}

    void resize (size_type n)
    {
        if(n>_sz)
        {
            while(n++<_sz)
                emplace_back();
        }else
        {
            const_iterator it=cend()-(_sz-n);
            erase(it, cend());
        }
    };

    void resize (size_type n, const value_type& val)
    {
        if(n>_sz)
        {
            while(n++<_sz)
                push_back(val);
        }else
        {
            iterator it=end()-(_sz-n);
            erase(it, end());
        }
    }

    void clear()
    {
        resize(0);
    }

    void splice (const_iterator position, list& x)
    //void splice (const_iterator position, list&& x);
    {
        uint16_t dstIdx=position.get_idx();//can be -1(empty) or -1(end)
        if(!x._sz)return; // do nothing if source x is empty

        _listpool._list[x._lastIdx].nextIdx=dstIdx;

        if(dstIdx!=-1)
        {
            //this this list not empty and not end
            _listpool._list[dstIdx].prevIdx=x._lastIdx;

            //position is not first
            if(_listpool._list[dstIdx].prevIdx!=-1)
            {
                _listpool._list[_listpool._list[dstIdx].prevIdx].nextIdx=x._firstIdx;
                _listpool._list[x._firstIdx].prevIdx=_listpool._list[_listpool._list[dstIdx].prevIdx];
            }
            else
                _firstIdx=x._firstIdx;
        }else
        {

            //not empty, end
            if(_sz)
            {
                _listpool._list[_lastIdx].nextIdx=x._firstIdx;
                _listpool._list[x.firstIdx].prevIdx=_lastIdx;
                _lastIdx=x._lastIdx;
            }
            //empty, begin/end
            else
            {
                _firstIdx=x.firstIdx;
                _lastIdx=x.lastIdx;
            }
        }
        _sz+=x._sz;

        //leave clear state to the source list:
        x._firstIdx=-1;
        x._lastIdx=-1;
        x._sz=0;
    };


    void splice (const_iterator position, list& x, const_iterator i)
    //void splice (const_iterator position, list&& x, const_iterator i);
    {
        //TODO: the logic is similiar with emplace.
        //we should manage to share the code somehow
        uint16_t dstIdx=position.get_idx();

        uint16_t idx=i.get_idx;

        if(idx==-1)return;
        x.cut(i);

        _listpool._list[idx].nextIdx=dstIdx;

        if(dstIdx!=-1)
        {
            _listpool._list[idx].prevIdx=_listpool._list[dstIdx].prevIdx;
            _listpool._list[dstIdx].prevIdx=idx;
            if(_listpool._list[idx].prevIdx!=-1)//not empty, middle
                _listpool._list[_listpool._list[idx].prevIdx].nextIdx=idx;
            else// //notempty, begin
                _firstIdx=idx;
        }else
        {
            // //notempty, begin
            //notempty, end
            if(_sz)
            {
                _listpool._list[idx].prevIdx=_lastIdx;
                _lastIdx=idx;
                _listpool._list[_listpool._list[idx].prevIdx].nextIdx=idx;
            }
            //empty, begin/end
            else
            {
                _listpool._list[idx].prevIdx=-1;
                _lastIdx=idx;
                _firstIdx=idx;
            }
        }

        _sz++;

    };

    void splice (const_iterator position, list& x,
                 const_iterator first, const_iterator last)
    {
        const_iterator it=first;
        while(it!=last)
        {
            const_iterator next_it=it++;
            splice(position--, x, it);
            it=next_it;
        };
    }

   // void splice (const_iterator position, list&& x,
   //              const_iterator first, const_iterator last);

  void merge (list& x);
  void merge (list&& x);
  template <class Compare>
    void merge (list& x, Compare comp);
  template <class Compare>
    void merge (list&& x, Compare comp);

  //TODO: support these algo-modification
  //void sort();
  //template <class Compare>
  //  void sort (Compare comp);

  //void reverse();

  //void remove (const value_type& val);
  //  template <class Predicate>
  //    void remove_if (Predicate pred);

  //  void unique();
  //  template <class BinaryPredicate>
  //    void unique (BinaryPredicate binary_pred);

  //TODO: support relational operators

  private:
  listpool<T>* _listpool;
  uint16_t _sz;
  //uint16_t _cap;

  // use index instead pointer, null is marked by Idx=MAX(uint16_t)
  uint16_t _firstIdx;
  uint16_t _lastIdx;
};

namespace declare
{

template <class T, uint16_t cap>
class listpool:public uboost::listpool<T>
{
    public:
    listpool()
    :uboost::listpool<T>(_buf, cap){};
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
