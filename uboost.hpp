
/*!
 * \file uboost.hpp
 *
 * \author Oky Firmansyah(mail@okyfirmansyah.net)
 * \date
 *
 * \brief boost/c++11-styled tools/helper that can be used for embedded development environment
 *
 * This is lightweight tools/helper functions and classes implementation that supposed to help
 * bring c++11 feature/experience to embedded software development. This library is supposed to implement
 * subset of streamlined version of boost library and c++11's stdlib library so to fit and compatible with
 * embedded c++ compiler(given the compiler comply with c++11 specification)
 * 
 */

#ifndef UBOOST_HPP_
#define UBOOST_HPP_


/*!
 * Use USE_STATIC_ALLOCATION definition to force all helper class/function to use static
 * allocation (if possible). Internally all class/function will revert to stack to allocate
 * object so no dynamic allocation environment required(for e.g bare c++ w/o OS).
 * The allocated space will be predetermined(not really efficient) and we will losing
 * some object move semantic performance advantage as consequence
 */
#define USE_STATIC_ALLOCATION



namespace uboost
{



/*! @brief metafunction to remove any reference(& or &&) semantics
 * and return the base type instead
 * @tparam T class typename that can be any reference type
 */
template <class T> struct remove_reference     {typedef T type;};
template <class T> struct remove_reference<T&> {typedef T type;};
template <class T> struct remove_reference<T&&>{typedef T type;};




/*! @brief metafunction to forward correct reference semantic
 *  to particular function in templated scope
 *  @tparam T class typename that can be any reference type
 */	
template<class T>
T&& forward(typename remove_reference<T>::type& t)noexcept
{
	return static_cast<T&&>(t);
}

template<class T>
T&& forward(typename remove_reference<T>::type&& t)noexcept
{
	return static_cast<T&&>(t);
}

/*! @brief metafunction to convert any lvalue or rvalue input
 *  as rvalue reference type
 *  @tparam T class typename that can be any reference type
 */	
template< class T >
typename remove_reference<T>::type&& move( T&& t )
{
	return static_cast<typename remove_reference<T>::type &&>(t);
}

/*! @brief metafunction to test whether typename T and U are 
 *  exactly same typename(with same reference type)
 *  @tparam T typename to be compared
 *  @tparam U typename to be compared
 */	
template <typename T, typename U>
struct is_same
{
	static const bool value=false;
};

// specialized implementation in case same typename
template <typename T>
struct is_same<T,T>
{
	static const bool value=true;
};

namespace _internal
{

/*! @brief change maximum_ptr_per_function_object to set
 *  maximum number of captured pointer in uboost::function()
 *  if USE_STATIC_ALLOCATION is used.
*/
const int maximum_ptr_per_function_object=3;
const int maximum_functionhandle_overhead=sizeof(void*)*2;
const int maximum_function_object_size_word=((maximum_ptr_per_function_object+1)*sizeof(void*)+
                                             maximum_functionhandle_overhead)/4;

// base clase for function implementation
class functionImplBase
{
	public:
#ifdef USE_STATIC_ALLOCATION
	virtual functionImplBase* Clone(void* buffr)const=0;
#else
	virtual functionImplBase* Clone()const=0;
#endif
};

// Generalized functor filter class
template <typename R,typename... TArgs>
class functionImpl;

// functor filter class for 0 params
template <typename R>
class functionImpl<R>:public functionImplBase
{
	public:
	typedef R ResultType;
	typedef void* Parm1;
	typedef void* Parm2;
	typedef void* Parm3;
	typedef void* Parm4;
	typedef void* Parm5;
	typedef void* Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()() = 0;
};

// functor filter class for 1 params
template <typename R, typename T1>
class functionImpl<R, T1>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef void* Parm2;
	typedef void* Parm3;
	typedef void* Parm4;
	typedef void* Parm5;
	typedef void* Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1) = 0;
};

// functor filter class for 2 params
template <typename R, typename T1, typename T2>
class functionImpl<R, T1, T2>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef void* Parm3;
	typedef void* Parm4;
	typedef void* Parm5;
	typedef void* Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1, Parm2) = 0;
};

// functor filter class for 3 params
template <typename R, typename T1, typename T2, typename T3>
class functionImpl<R, T1, T2, T3>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef T3 Parm3;
	typedef void* Parm4;
	typedef void* Parm5;
	typedef void* Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1, Parm2, Parm3) = 0;
};

// functor filter class for 4 params
template <typename R, typename T1, typename T2, typename T3, typename T4>
class functionImpl<R, T1, T2, T3, T4>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef T3 Parm3;
	typedef T4 Parm4;
	typedef void* Parm5;
	typedef void* Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1, Parm2, Parm3, Parm4) = 0;
};

// functor filter class for 5 params
template <typename R, typename T1, typename T2, typename T3, typename T4,
typename T5>
class functionImpl<R, T1, T2, T3, T4, T5>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef T3 Parm3;
	typedef T4 Parm4;
	typedef T5 Parm5;
	typedef void* Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5) = 0;
};

// functor filter class for 6 params
template <typename R, typename T1, typename T2, typename T3, typename T4,
typename T5, typename T6>
class functionImpl<R, T1, T2, T3, T4, T5, T6>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef T3 Parm3;
	typedef T4 Parm4;
	typedef T5 Parm5;
	typedef T6 Parm6;
	typedef void* Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6) = 0;
};

// functor filter class for 7 params
template <typename R, typename T1, typename T2, typename T3, typename T4,
typename T5, typename T6, typename T7>
class functionImpl<R, T1, T2, T3, T4, T5, T6, T7>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef T3 Parm3;
	typedef T4 Parm4;
	typedef T5 Parm5;
	typedef T6 Parm6;
	typedef T7 Parm7;
	typedef void* Parm8;
	virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, Parm7) = 0;
};

// functor filter class for 8 params
template <typename R, typename T1, typename T2, typename T3, typename T4,
typename T5, typename T6, typename T7, typename T8>
class functionImpl<R, T1, T2, T3, T4, T5, T6, T7, T8>:public functionImplBase

{
	public:
	typedef R ResultType;
	typedef T1 Parm1;
	typedef T2 Parm2;
	typedef T3 Parm3;
	typedef T4 Parm4;
	typedef T5 Parm5;
	typedef T6 Parm6;
	typedef T7 Parm7;
	typedef T8 Parm8;
	virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, Parm7, Parm8) = 0;
};

// template class to handle/wrap all kind of function pointer/function object
// or lambda function reference
template <class Parentfunctor, 
          typename fun, 
#ifdef USE_STATIC_ALLOCATION
		  // in case of static allocation, we must check whether incoming function
		  // object to be wrapped is fit with our predetermined space
		  bool oversize=!(is_same<typename remove_reference<Parentfunctor>::type,
		                          typename remove_reference<fun>::type>::value)&&
		                 (sizeof(fun)+maximum_functionhandle_overhead>(maximum_function_object_size_word*4))>
#else
          // dynamic allocation doesnt have predetermined space issue
          bool oversize=false>
#endif
class functionHandle;

// main implementation
template <class Parentfunctor, typename fun>
class functionHandle<Parentfunctor, fun, false>
: public Parentfunctor::Impl
{
	typedef typename Parentfunctor::Impl Base;
	public:
	typedef typename Base::ResultType ResultType;
	typedef typename Base::Parm1 Parm1;
	typedef typename Base::Parm2 Parm2;
	typedef typename Base::Parm3 Parm3;
	typedef typename Base::Parm4 Parm4;
	typedef typename Base::Parm5 Parm5;
	typedef typename Base::Parm6 Parm6;
	typedef typename Base::Parm7 Parm7;
	typedef typename Base::Parm8 Parm8;
#ifdef USE_STATIC_ALLOCATION
	virtual functionImplBase* Clone(void* buffr)const
	{
		return new(buffr) functionHandle(*this);
	}
#else
	virtual functionImplBase* Clone()const
	{
		return new functionHandle(*this);
	}
#endif
	
	functionHandle(const fun& _fun)
	:f_(_fun)
	{
	}
	
	functionHandle(fun&& _fun)
	:f_(move(_fun))
	{
	}
	
	functionHandle()=delete;
	
	// operator() implementations for up to 8 arguments
	
	ResultType operator()()
	{ return f_(); }

	ResultType operator()(Parm1 p1)
	{ return f_(p1); }
	
	ResultType operator()(Parm1 p1, Parm2 p2)
	{ return f_(p1, p2); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3)
	{ return f_(p1, p2, p3); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
	{ return f_(p1, p2, p3, p4); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5)
	{ return f_(p1, p2, p3, p4, p5); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6)
	{ return f_(p1, p2, p3, p4, p5, p6); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7)
	{ return f_(p1, p2, p3, p4, p5, p6, p7); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8)
	{ return f_(p1, p2, p3, p4, p5, p6, p7, p8); }
	
	private:
	fun f_;
};

}; // namespace _internal

template <typename... T>
class function;

/*! @brief implement universal wrapper for
 *  all callable object or pointer.
 *  
 *  class uboost::function, like boost::function or std::function can be used
 *  as universal wrapper for callable object and function pointer.
 *  As per C++11 this wrapper should support lambda function with capturing
 *  as well. We can pass the object across local variables or function calls
 *  as this class implements copy and move semantivs. For embedded environment
 *  without dynamic allocation support, uboost::function can be implemented
 *  with full stack-based allocator via macro USE_STATIC_ALLOCATION.
 *  
 *  Unlike std::function or boost::function, this uboost version still
 *  not support class member function wrapping.
 *
 *  @tparam R return type of function object/pointer
 *  @tparam TArgs type list of arguments
 */	
template <typename R, typename... TArgs>
class function<R (TArgs...)>
{
	public:
	// Handy type definitions for the body type
	typedef _internal::functionImpl<R, TArgs...> Impl;
	typedef R ResultType;
	typedef typename Impl::Parm1 Parm1;
	typedef typename Impl::Parm2 Parm2;
	typedef typename Impl::Parm3 Parm3;
	typedef typename Impl::Parm4 Parm4;
	typedef typename Impl::Parm5 Parm5;
	typedef typename Impl::Parm6 Parm6;
	typedef typename Impl::Parm7 Parm7;
	typedef typename Impl::Parm8 Parm8;
	
	
	// Member functions

	function() : spImpl_(nullptr)
	{}
	
	~function()
	{
		if(spImpl_)
#ifdef USE_STATIC_ALLOCATION
           spImpl_->~Impl();
#else
           delete spImpl_;
#endif
	}
	
	// move to the more generalized version(below)
	/*
	template <typename fun>
	function(const fun& _fun)
#ifdef USE_STATIC_ALLOCATION	
	: spImpl_(new((void*)buffr) _internal::functionHandle<function, fun>(_fun))
#else
	: spImpl_(new _internal::functionHandle <function, fun>(_fun))
#endif
	{
		
	}*/
	
	template <typename fun>
	function(fun&& _fun)
	#ifdef USE_STATIC_ALLOCATION
	: spImpl_(new((void*)buffr) 
	_internal::functionHandle<function, 
	                          typename remove_reference<fun>::type >
							  (forward<fun>(_fun) ))
	#else
	: spImpl_(new 
	          _internal::functionHandle <function, 
			                             typename remove_reference<fun>::type >
										 (forward<fun>(_fun) ))
	#endif
	{
	}
	
	
	function(const function& rhs)
	#ifdef USE_STATIC_ALLOCATION
	:spImpl_(static_cast<Impl*>( rhs.spImpl_->Clone((void*)buffr))  )
	#else
	:spImpl_(static_cast<Impl*>( rhs.spImpl_->Clone()  ))
	#endif
	{
	}
	
	
	function(function&& rhs)
	#ifdef USE_STATIC_ALLOCATION
	:spImpl_(static_cast<Impl*>( rhs.spImpl_->Clone((void*)buffr))  )
	#else
	:spImpl_(static_cast<Impl*>(rhs.spImpl_))
	#endif
	{
		rhs.spImpl_=nullptr;
	}
	

	function& operator=(const function& rhs)
	{
		if(spImpl_)
#ifdef USE_STATIC_ALLOCATION		    
			spImpl_->~Impl();
		spImpl_=static_cast<Impl*>(rhs.spImpl_->Clone((void*)buffr));
#else
			delete spImpl_;
		spImpl_=static_cast<Impl*>(rhs.spImpl_->Clone());
#endif

		return *this;
	}

    function& operator=(function&& rhs)
    {
	    if(spImpl_)
		#ifdef USE_STATIC_ALLOCATION		
		    spImpl_->~Impl();
		#else
	        delete spImpl_;
		#endif
	    
	    #ifdef USE_STATIC_ALLOCATION		
		spImpl_=static_cast<Impl*>(rhs.spImpl_->Clone((void*)buffr));
		#else
		spImpl_=static_cast<Impl*>(rhs.spImpl_);
		#endif
		
		rhs.spImpl_=nullptr;
	    
	    return *this;
    }
	
	ResultType operator()() const
	{ return (*spImpl_)(); }

	ResultType operator()(Parm1 p1) const
	{ return (*spImpl_)(p1); }
	
	ResultType operator()(Parm1 p1, Parm2 p2) const
	{ return (*spImpl_)(p1, p2); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
	{ return (*spImpl_)(p1, p2, p3); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
	{ return (*spImpl_)(p1, p2, p3, p4); }
	
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
	                      Parm5 p5) const
	{ return (*spImpl_)(p1, p2, p3, p4, p5); }
		
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
	                      Parm5 p5, Parm6 p6) const
	{ return (*spImpl_)(p1, p2, p3, p4, p5, p6); }
		
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
	                      Parm5 p5, Parm6 p6, Parm7 p7) const
	{ return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7); }
		
	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
	                      Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8) const
	{ return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8); }
	
	private:
#ifdef USE_STATIC_ALLOCATION		  
	uint32_t buffr[_internal::maximum_function_object_size_word];
#endif
	Impl *spImpl_;
};



}; // namespace uboost



#endif /* UBOOST_H_ */