/*
 * Simple Header that can detect the platform and the compiler.
 * Based on Boost.Config selection headers:
 * - https://github.com/boostorg/config/blob/boost-1.64.0/include/boost/config/select_platform_config.hpp
 * - https://github.com/boostorg/config/blob/boost-1.64.0/include/boost/config/select_compiler_config.
 *
 * (C) Copyright John Maddock 2001 - 2002.
 * (C) Copyright Jens Maurer 2001.
 * (C) Copyright John Maddock 2001 - 2003.
 * (C) Copyright Martin Wille 2003.
 * (C) Copyright Guillaume Melquiond 2003.
 * (C) Copyright SaeruHikari 20.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SAKURA_PLATFORM_FLAG_H
#define SAKURA_PLATFORM_FLAG_H

/*****************************************************************************/
/***** Detect Compiler *******************************************************/
/*****************************************************************************/

#if defined(__GCCXML__)
// GCC-XML emulates other compilers, it has to appear first here!
#   define SAKURA_PLATFORM_FLAG_COMPILER_GCC_XML

#elif defined(_CRAYC)
// EDG based Cray compiler:
#   define SAKURA_PLATFORM_FLAG_COMPILER_CRAY

#elif defined __COMO__
//  Comeau C++
#   define SAKURA_PLATFORM_FLAG_COMPILER_COMEAU

#elif defined(__PATHSCALE__) && (__PATHCC__ >= 4)
// PathScale EKOPath compiler (has to come before clang and gcc)
#   define SAKURA_PLATFORM_FLAG_COMPILER_PATHSCALE

#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
//  Intel
#   define SAKURA_PLATFORM_FLAG_COMPILER_INTEL

#elif defined __clang__ && !defined(__CUDACC__) && !defined(__ibmxl__)
// when using clang and cuda at same time, you want to appear as gcc
//  Clang C++ emulates GCC, so it has to appear early.
#   define SAKURA_PLATFORM_FLAG_COMPILER_CLANG

#elif defined __DMC__
//  Digital Mars C++
#   define SAKURA_PLATFORM_FLAG_COMPILER_DIGITAL_MARS

# elif defined(__GNUC__) && !defined(__ibmxl__)
//  GNU C++:
#   define SAKURA_PLATFORM_FLAG_COMPILER_GCC

#elif defined __KCC
//  Kai C++
#   define SAKURA_PLATFORM_FLAG_COMPILER_KAI

#elif defined __sgi
//  SGI MIPSpro C++
#   define SAKURA_PLATFORM_FLAG_COMPILER_SGI_MIPSPRO

#elif defined __DECCXX
//  Compaq Tru64 Unix cxx
#   define SAKURA_PLATFORM_FLAG_COMPILER_COMPAQ_CXX

#elif defined __ghs
//  Greenhills C++
#   define SAKURA_PLATFORM_FLAG_COMPILER_GREENHILLS

#elif defined __CODEGEARC__
//  CodeGear - must be checked for before Borland
#   define SAKURA_PLATFORM_FLAG_COMPILER_CODEGEAR

#elif defined __BORLANDC__
//  Borland
#   define SAKURA_PLATFORM_FLAG_COMPILER_BORLAND

#elif defined  __MWERKS__
//  Metrowerks CodeWarrior
#   define SAKURA_PLATFORM_FLAG_COMPILER_METROWERKS

#elif defined  __SUNPRO_CC
//  Sun Workshop Compiler C++
#   define SAKURA_PLATFORM_FLAG_COMPILER_SUNPRO_CC

#elif defined __HP_aCC
//  HP aCC
#   define SAKURA_PLATFORM_FLAG_COMPILER_HP_ACC

#elif defined(__MRC__) || defined(__SC__)
//  MPW MrCpp or SCpp
#   define SAKURA_PLATFORM_FLAG_COMPILER_MPW

#elif defined(__ibmxl__)
// IBM XL C/C++ for Linux (Little Endian)
#   define SAKURA_PLATFORM_FLAG_COMPILER_XLCPP

#elif defined(__IBMCPP__)
//  IBM Visual Age or IBM XL C/C++ for Linux (Big Endian)
#   define SAKURA_PLATFORM_FLAG_COMPILER_VACPP

#elif defined(__PGI)
//  Portland Group Inc.
#   define SAKURA_PLATFORM_FLAG_COMPILER_PGI

#elif defined _MSC_VER
//  Microsoft Visual C++
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER
#   define SAKURA_PLATFORM_FLAG_COMPILER_MSVC

#else
// this must come last - generate an error if we don't
// recognise the compiler:
#  error "Unknown compiler"

#endif

/*****************************************************************************/
/***** Detect Platform *******************************************************/
/*****************************************************************************/

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
// linux, also other platforms (Hurd etc) that use GLIBC
#  define SAKURA_PLATFORM_FLAG_PLATFORM_LINUX

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_BSD

#elif defined(sun) || defined(__sun)
// solaris:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_SOLARIS

#elif defined(__sgi)
// SGI Irix:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_SGI_IRIX

#elif defined(__hpux)
// hp unix:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_HPUX

#elif defined(__CYGWIN__)
// cygwin is not win32:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_CYGWIN

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_WIN32

#elif defined(__HAIKU__)
// Haiku
#  define SAKURA_PLATFORM_FLAG_PLATFORM_HAIKU

#elif defined(__BEOS__)
// BeOS
#  define SAKURA_PLATFORM_FLAG_PLATFORM_BEOS

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define SAKURA_PLATFORM_FLAG_PLATFORM_MACOS

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM
#  define SAKURA_PLATFORM_FLAG_PLATFORM_IBM_AIX

#elif defined(__amigaos__)
// AmigaOS
#  define SAKURA_PLATFORM_FLAG_PLATFORM_AMIGAOS

#elif defined(__QNXNTO__)
// QNX:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_QNX

#elif defined(__VXWORKS__)
// vxWorks:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_VXWORKS

#elif defined(__SYMBIAN32__) 
// Symbian: 
#  define SAKURA_PLATFORM_FLAG_PLATFORM_SYMBIAN

#elif defined(_CRAYC)
// Cray:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_CRAY

#elif defined(__VMS) 
// VMS:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_VMW

#elif defined(__CloudABI__)
// Nuxi CloudABI:
#  define SAKURA_PLATFORM_FLAG_PLATFORM_CLOUDABI
#else

#  if defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
   // generic unix platform:
#     define SAKURA_PLATFORM_FLAG_PLATFORM_GENERIC_UNIX
#  else
#     error "Unknown platform"
#  endif

#endif

#endif // SAKURA_PLATFORM_FLAG_H
