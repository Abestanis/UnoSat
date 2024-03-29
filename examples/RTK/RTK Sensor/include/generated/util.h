/** Utility macros and types. */
// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Indicate that this type holds the specified enum, but is represented as the base type.
 *
 * @param enumType: The enum that this type represents.
 * @param baseType: The type that represents the enum.
 */
#define TYPED_ENUM(enumType, baseType) baseType

#ifndef __cplusplus
#    include "stdbool.h"
#endif

#if !defined(__cplusplus) && !defined(static_assert)
/**
 * Assert that the condition is true at compile-time, otherwise fail the compilation.
 *
 * @note If the condition can not be evaluated at compile-time this static assert will be ignored.
 * @param condition A condition that can be evaluated at compile time and must be true,
 *                  otherwise compilation will fail.
 * @param message A message to describe what is asserted.
 */
#    define static_assert(condition, message) ((void) sizeof(char[1 - 2 * !(condition)]))
#endif

#ifdef __unused
#    define MAYBE_UNUSED __unused
#else
#    define MAYBE_UNUSED __attribute__((__unused__))
#endif

#ifdef __packed
#    define PACKED __packed
#else
#    define PACKED __attribute__((__packed__))
#endif

#ifdef __weak
#    define WEAK __weak
#else
#    define WEAK __attribute__((weak))
#endif

#ifdef __cplusplus
}
#endif
