#pragma once

#include <CircularBuffer.h>
#include "generated/bufferAccessor.h"


/**
 * An ECom buffer accessor that uses a CircularBuffer.
 *
 * @tparam S The size of the circular buffer in bytes.
 */
template<size_t S>
class CircularBufferHandler {
public:
    /**
     * Construct an empty buffer handler.
     */
    CircularBufferHandler() = default;

    ~CircularBufferHandler() = default;

    /**
     * Copy data from the stream into the buffer at the current write position.
     * Data will be read until the stream is empty.
     *
     * @param stream The stream to read from.
     */
    void copyFrom(Stream& stream) {
        int byteRead;
        while ((byteRead = stream.read()) != -1) {
            buffer.push((uint8_t) byteRead);
        }
    }

    /**
     * Copy data from the given buffer into the circular buffer at the current write position.
     *
     * @param otherBuffer The buffer to copy from.
     * @param size The amount of bytes to copy from the buffer.
     */
    void copyFrom(const void* otherBuffer, size_t size) {
        for (size_t i = 0; i < size; i++) {
            buffer.push(static_cast<const uint8_t*>(otherBuffer)[i]);
        }
    }

    /**
     * @return The number of bytes that can be read.
     */
    size_t availableBytes() const {
        return buffer.size();
    }

    /**
     * Read from the buffer at the current read position plus the given at offset without advancing the read position.
     *
     * @param peekBuffer The buffer to read into.
     * @param size The number of bytes to read.
     * @param at An offset from the current read position to start reading from.
     * @return Whether the buffer has the requested number of bytes.
     */
    bool peek(void* peekBuffer, size_t size, size_t at) const {
        if (at + size > buffer.size()) {
            return false;
        }
        for (size_t index = 0; index < size; index++) {
            ((uint8_t*) peekBuffer)[index] = buffer[at + index];
        }
        return true;
    }

    /**
     * Advance the read index by the given number of bytes.
     * This will not go past the write index.
     *
     * @param size The number of bytes to advance the read index.
     */
    void advance(size_t size) {
        if (size > buffer.size()) {
            size = buffer.size();
        }
        while (size-- > 0) {
            buffer.shift();
        }
    }

    /**
     * @return An ECom BufferAccessor of this CircularBufferAccessor.
     */
    BufferAccessor* operator*() {
        return &accessor;
    }

private:
    /** The underlying circular buffer. */
    CircularBuffer<uint8_t, S> buffer = {};

    /** An ECom BufferAccessor that forwards to this CircularBufferAccessor. */
    BufferAccessor accessor = {
            .availableBytes = [](const BufferAccessor* accessorInstance) {
                return ((const CircularBufferHandler<S>*) accessorInstance->data)->availableBytes();
            },
            .peek = [](const BufferAccessor* accessorInstance, void* peekBuffer, size_t size, size_t at) {
                return ((const CircularBufferHandler<S>*) accessorInstance->data)->peek(peekBuffer, size, at);
            },
            .advance = [](BufferAccessor* accessorInstance, size_t size) {
                return ((CircularBufferHandler<S>*) accessorInstance->data)->advance(size);
            },
            .data = this,
    };
};
