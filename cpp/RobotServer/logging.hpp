/* Robot Control System
 * Copyright (C) 2013 Tuna Oezer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ROBOT_CONTROL_SYSTEM_LOGGING_HPP_
#define ROBOT_CONTROL_SYSTEM_LOGGING_HPP_

#include <string>

#include "macros.hpp"

namespace robot_control_system {

using ::std::string;

/*
 * Logs messages.
 */
class Logger {
    NO_INSTANCE(Logger);

  public:
    /*
     * Writes log output to the console.
     *
     * The format of the output is:
     * [<time> <tag> <file>:<line>] <message>
     *
     * Log outputs are associated with a tag that can be used to specify the type of
     * output, e.g. error, warning, info, etc.
     */
    static void Log(const string& filename,
                    int line_number,
                    const string& tag,
                    const string& message);
};

#define LOG(tag, message) Logger::Log(__FILE__, __LINE__, tag, message)

// Logs informational messages
#define LOG_INFO(message) LOG("INFO", message)

// Logs warning messages
#define LOG_WARNING(message) LOG("WARNING", message)

// Logs error messages
#define LOG_ERROR(message) LOG("ERROR", message)

}

#endif /* ROBOT_CONTROL_SYSTEM_LOGGING_HPP_ */
