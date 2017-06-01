/*
 * logger.cpp
 *
 * Logger implementation
 */

#include <cstdarg>
#include <stdexcept>

#include "lmlogger.h"


log_level_t Logger::__mLogLevel = LL_ERROR;
void (*Logger::__printFunc)(const char *, const char *) = nullptr;

/**
 * Interface for the client application to specify log function
 * (see class description)
 */
void Logger::setLogFunc(void (*printFunc)(const char *, const char *))
{
	if (__printFunc != nullptr)
	__printFunc = printFunc;
}

/**
 * Interface to set log verbosity level
 */
void Logger::setLogLevel(log_level_t ll)
{
	if ((ll < LL_MIN) || (ll > LL_MAX)) {
		throw std::invalid_argument("Invalid log level");
	}
	__mLogLevel = ll;
}

/**
 * Wrapper for __print_func which takes into account specified log level
 */
void Logger::log(log_level_t ll, const char *tag, const char *fmt, ...)
{
	if ((__printFunc == nullptr) || (ll > __mLogLevel)) {
		return;
	}

	char buf[LOG_ENTRY_MAX_LEN];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	__printFunc(tag, buf);
}
