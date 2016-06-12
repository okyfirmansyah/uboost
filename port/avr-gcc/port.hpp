
/**@file port.hpp
 *
 * This is part of uboost library, a lightweight tools/helper functions and classes implementation that supposed 
 * to help bring c++11 feature/experience to embedded software development. This library is supposed to implement
 * subset of streamlined version of boost library and c++11's stdlib library so to fit and compatible with
 * embedded c++ compiler(given the compiler comply with c++11 specification)
 *
 * @author Oky Firmansyah <mail@okyfirmansyah.net>.
 *
 * @date Created      : Thu Jan 11 14:22:15 2016 okyfirmansyah
 */

#ifndef UBOOST_PORT_HPP_
#define UBOOST_PORT_HPP_

namespace uboost
{

namespace coroutine
{
	
namespace port
{
	
	enum coroutine_type
	{
		coroutine_pull_type,
		coroutine_push_type,
		coroutine_call_type,
		coroutine_yield_type,
	};
	
	struct coroutine_port_info
	{
        void* stackPtr;
        uint16_t stackSz;
        volatile void* volatile callerSP;
        //volatile void* volatile thisCoroutSP;
		
		coroutine_port_info(coroutine_type type)
		{
		};
    };
};

};

};
#endif /* UBOOST_H_ */