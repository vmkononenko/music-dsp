/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * logger.cpp
 *
 * Logger implementation
 */

#include <cstdarg>
#include <stdexcept>

#include "lmlogger.h"


log_level_t Logger::__mLogLevel = LL_DEFAULT;
void (*Logger::__printFunc)(const char *, const char *) = nullptr;

/**
 * Interface for the client application to specify log function
 * (see class description)
 */
void Logger::setLogFunc(void (*printFunc)(const char *, const char *))
{
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
