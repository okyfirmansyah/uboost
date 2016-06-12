
/**@file config.hpp
 *
 * This is part of uboost library, a lightweight tools/helper functions and classes implementation that supposed
 * to help bring c++11 feature/experience to embedded software development. This library is supposed to implement
 * subset of streamlined version of boost library and c++11's stdlib library so to fit and compatible with
 * embedded c++ compiler(given the compiler comply with c++11 specification)
 *
 * @author Oky Firmansyah <mail@okyfirmansyah.net>.
 *
 * @date Created      : Thu Jan 11 13:00:10 2016 okyfirmansyah
 */

#ifndef UBOOST_CONFIG_HPP_
#define UBOOST_CONFIG_HPP_



#ifndef uint32_t
#include <stdint.h>
#endif

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

/**
 * Use USE_STATIC_ALLOCATION definition to force all helper class/function to use static
 * allocation (if possible). Internally all class/function will revert to stack to allocate
 * object so no dynamic allocation environment required(for e.g bare c++ w/o OS).
 * The allocated space will be predetermined(not really efficient) and we will losing
 * some object move semantic performance advantage as consequence
 */
#define USE_STATIC_ALLOCATION

#define COMPILER_AVR_GCC     1
#define COMPILER_AVR_IAR     2
#define COMPILER_AVR32_GCC   3
#define COMPILER_ARM_GCC     4
#define COMPILER_X86_GCC     5
// ...

#define USE_COMPILER_MODE COMPILER_X86_GCC


#if USE_COMPILER_MODE==COMPILER_AVR_GCC
#include "port/avr-gcc/port.hpp"
#elif USE_COMPILER_MODE==COMPILER_AVR_IAR
#include "port/iar-gcc/port.hpp"
#elif USE_COMPILER_MODE==COMPILER_X86_GCC
#define UBOOST_MOST_STRICT_ALIGNMENT uint32_t
#endif

#endif /* UBOOST_H_ */
