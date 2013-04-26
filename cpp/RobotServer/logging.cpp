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

#include "logging.hpp"

#include <ctime>
#include <iostream>

namespace robot_control_system {

// static
void Logger::Log(const string& filename,
                 int line_number,
                 const string& tag,
                 const string& message) {
  static const int kBufferSize = 26;
  static const char kFormat[] = "%F %T";
  char time_str[kBufferSize];
  std::time_t t;
  std::time(&t);
  std::strftime(time_str, kBufferSize, kFormat, std::gmtime(&t));
  std::cout << "[" << time_str << " " << tag
            << " " << filename << ":" << line_number << "] "
            << message
            << std::endl;
}

}
