#pragma once
/*
 *  Copyright (C) 2011 Aldebaran Robotics
 */


#ifndef _QI_API_H_
#define _QI_API_H_

// Deprecated
#if defined(__GNUC__)
#  define QI_API_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#  define QI_API_DEPRECATED __declspec(deprecated)
#else
#  define QI_API_DEPRECATED
#endif

// For shared library
#if defined _WIN32 || defined __CYGWIN__
#  define QI_EXPORT_API __declspec(dllexport)
#  if defined _WINDLL
#    define QI_IMPORT_API __declspec(dllimport)
#  else
#    define QI_IMPORT_API
#  endif
#elif __GNUC__ >= 4
#  define QI_EXPORT_API __attribute__ ((visibility("default")))
#  define QI_IMPORT_API __attribute__ ((visibility("default")))
#else
#  define QI_EXPORT_API
#  define QI_IMPORT_API
#endif

// qi_EXPORTS controls which symbols are exported when libqi
// is compiled as a SHARED lib.
// DO NOT USE OUTSIDE LIBQI
#ifdef qi_EXPORTS
# define QI_API QI_EXPORT_API
#else
# define QI_API QI_IMPORT_API
#endif

// Macros adapted from opencv2.2
#if defined(_MSC_VER)
  #define QI_DO_PRAGMA(x) __pragma(x)
  #define __ALSTR2__(x) #x
  #define __ALSTR1__(x) __ALSTR2__(x)
  #define _ALMSVCLOC_ __FILE__ "("__ALSTR1__(__LINE__)") : "
  #define QI_MSG_PRAGMA(_msg) QI_DO_PRAGMA(message (_ALMSVCLOC_ _msg))
#elif defined(__GNUC__)
  #define QI_DO_PRAGMA(x) _Pragma (#x)
  #define QI_MSG_PRAGMA(_msg) QI_DO_PRAGMA(message (_msg))
#else
  #define QI_DO_PRAGMA(x)
  #define QI_MSG_PRAGMA(_msg)
#endif

// Use this macro to generate compiler warnings.
#define QI_COMPILER_WARNING(x) QI_MSG_PRAGMA("Warning: " #x)


/** \brief Deprecate a header, add a message to explain what user should do
 */
#define QI_DEPRECATED_HEADER(x) QI_MSG_PRAGMA("\
This file includes at least one deprecated or antiquated ALDEBARAN header \
which may be removed without further notice in the next version. \
Please consult the changelog for details. " #x)


#endif  // _QI_API_H_

