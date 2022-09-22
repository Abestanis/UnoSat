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
     * Send a info message.
     *
     * @param message The message to send.
     */
    static void info(const char* message);

    /**
     * Send a formatted info message.
     *
     * @param format The format string.
     * @param ... Format values.
     */
    static void infoFormatted(const char* format, ...) PRINTF_LIKE(1, 2);

    /**
     * Send a debug message.
     *
     * @param message The message to send.
     */
    static void debug(const char* message);

    /**
     * Send a formatted debug message only if enabled via [sendDebugMessages].
     *
     * @param format The format string.
     * @param ... Format values.
     */
    static void debugFormatted(const char* format, ...) PRINTF_LIKE(1, 2);

    /**
     * Send a error message.
     *
     * @param message The message to send.
     */
    static void error(const char* message);

    /**
     * Send a formatted error message.
     *
     * @param format The format string.
     * @param ... Format values.
     */
    static void errorFormatted(const char* format, ...) PRINTF_LIKE(1, 2);

    /**
     * Change if debug messages are sent.
     *
     * @param enabled Whether debug messages should be sent.
     */
    static void sendDebugMessages(bool enabled);

private:
    Logger() = default;
};
