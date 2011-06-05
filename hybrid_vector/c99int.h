/* hybrid_vector/c99int.h - C99 int types
 *
 * Author: Andrey Vul
 * Version: r4
 *
 * This file is public domain.
 */
#ifndef HYBRID_VECTOR_C99INT_H
#define HYBRID_VECTOR_C99INT_H
/*!
 * \brief Support for C99 int types
 *
 * Adds support for the extended integer types in \c \<stdint.h\>
 * and specifiers in \c \<inttypes.h\> .
 */
#ifdef __CXX0X__
#include <cstdint>
#include <cinttypes>
#else
extern "C" {
#include <stdint.h>
#include <inttypes.h>
}
#endif
#endif
