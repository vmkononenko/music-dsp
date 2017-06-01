/*
 * logger.h
 */

#pragma once


typedef enum {
	LL_ERROR,
	LL_WARNING,
	LL_INFO,
	LL_DEBUG,
	LL_VERBOSE,
	LL_MIN = LL_ERROR,
	LL_MAX = LL_VERBOSE
} log_level_t;

#define LOG_ENTRY_MAX_LEN 1024


/**
 * The library does not produce any log output because it is not
 * aware where to produce output to. It fully depends on the client
 * application. If a client application wants to make the library's
 * output a part of its own log it should provide a corresponding
 * log function and set verbosity level
 */
class Logger {

private:
	static log_level_t __mLogLevel;

	static void (*__printFunc)(const char *, const char *);

	Logger() {}

public:
	/**
	 * Interface for the client application to specify log function
	 * (see class description)
	 */
    static void setLogFunc(void (*printFunc)(const char *, const char *));

    /**
     * Interface to set log verbosity level
     */
    static void setLogLevel(log_level_t ll);

    /**
     * Wrapper for __print_func which takes into account specified log level
     */
	static void log(log_level_t ll, const char *tag, const char *fmt, ...);
};

#define LOGMSG_E(tag, ...) Logger::log(LL_ERROR,	tag, __VA_ARGS__)
#define LOGMSG_W(tag, ...) Logger::log(LL_WARNING,	tag, __VA_ARGS__)
#define LOGMSG_I(tag, ...) Logger::log(LL_INFO, 	tag, __VA_ARGS__)
#define LOGMSG_D(tag, ...) Logger::log(LL_DEBUG, 	tag, __VA_ARGS__)
#define LOGMSG_V(tag, ...) Logger::log(LL_VERBOSE, 	tag, __VA_ARGS__)
