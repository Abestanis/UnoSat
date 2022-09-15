#pragma once

#ifdef __GNUC__
#    define PRINTF_LIKE(formatIndex, argsIndex) \
        __attribute__((format(printf, formatIndex, argsIndex)))
#else
#    define PRINTF_LIKE(formatIndex, argsIndex)
#endif /* __GNUC__ */

/** A log message sender. */
class Logger {
public:

    /**
     * Sends the formatted info message.
     *
     * @param format The format string.
     * @param ... Format values.
     */
    static void info(const char* format, ...) PRINTF_LIKE(1, 2);

    /**
     * Sends the formatted debug message only if [enableDebugMessages] is `true`.
     *
     * @param format The format string.
     * @param ... Format values.
     */
    static void debug(const char* format, ...) PRINTF_LIKE(1, 2);

    /**
     * Writes the formatted error message.
     *
     * @param format The format string.
     * @param ... Format values.
     */
    static void error(const char* format, ...) PRINTF_LIKE(1, 2);

    /**
     * Whether debug messages should be sent.
     */
    static bool enableDebugMessages;
};
