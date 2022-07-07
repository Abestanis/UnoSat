// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //
#pragma once

#include <stddef.h>
#include "generated/util.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct BufferAccessor {
    size_t (* availableBytes)(const struct BufferAccessor* accessor);

    bool (* peek)(const struct BufferAccessor* accessor,
                  void* buffer, size_t size, size_t at);

    void (* advance)(struct BufferAccessor* accessor, size_t size);

    void* data;
} BufferAccessor;

typedef struct {
    const void* buffer;
    size_t size;
    size_t readOffset;
} SimpleBufferAccessorData;


extern size_t simpleBufferAccessorGetAvailableBytes(const BufferAccessor* accessor);

extern bool simpleBufferAccessorPeek(
        const struct BufferAccessor* accessor, void* buffer, size_t size, size_t at);

extern void simpleBufferAccessorAdvance(struct BufferAccessor* accessor, size_t size);

#define CONSTRUCT_SIMPLE_ACCESSOR_FROM_BUFFER(name, sourceBuffer, size) \
    SimpleBufferAccessorData name ## _data = {                          \
         .buffer = (sourceBuffer),                                      \
         .size = (size),                                                \
         .readOffset = 0                                                \
    };                                                                  \
    BufferAccessor name = {                                             \
        .availableBytes = simpleBufferAccessorGetAvailableBytes,        \
        .peek = simpleBufferAccessorPeek,                               \
        .advance = simpleBufferAccessorAdvance,                         \
        .data = &(name ## _data)                                        \
    }


#ifdef __cplusplus
}
#endif
