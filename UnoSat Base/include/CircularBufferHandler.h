#pragma once

#include <CircularBuffer.h>
#include "generated/bufferAccessor.h"


template<typename T, size_t S>
class CircularBufferHandler {
public:
    CircularBufferHandler() = default;

    ~CircularBufferHandler() = default;

    void copyFrom(Stream& stream) {
        int byteRead;
        while ((byteRead = stream.read()) != -1) {
            buffer.push((uint8_t) byteRead);
        }
    }

    size_t availableBytes() const {
        return buffer.size();
    }

    bool peek(void* peekBuffer, size_t size, size_t at) const {
        if (at + size > buffer.size()) {
            return false;
        }
        for (size_t index = 0; index < size; index++) {
            ((uint8_t*) peekBuffer)[index] = buffer[at + index];
        }
        return true;
    }

    void advance(size_t size) {
        if (size > buffer.size()) {
            size = buffer.size();
        }
        while (size-- > 0) {
            buffer.shift();
        }
    }

    BufferAccessor* operator*() {
        return &accessor;
    }

private:
    CircularBuffer<T, S> buffer = {};
    BufferAccessor accessor {
            .availableBytes = [](const BufferAccessor* accessorInstance) {
                return ((const CircularBufferHandler<T, S>*) accessorInstance->data)
                        ->availableBytes();
            },
            .peek = [](const BufferAccessor* accessorInstance, void* peekBuffer, size_t size,
                       size_t at) {
                return ((const CircularBufferHandler<T, S>*) accessorInstance->data)
                        ->peek(peekBuffer, size, at);
            },
            .advance = [](BufferAccessor* accessorInstance, size_t size) {
                return ((CircularBufferHandler<T, S>*) accessorInstance->data)->advance(size);
            },
            .data = this,
    };
};
