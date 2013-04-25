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

#ifndef ROBOT_CONTROL_SYSTEM_MACROS_HPP_
#define ROBOT_CONTROL_SYSTEM_MACROS_HPP_

// Removes default copy constructor and assignment operator from a class.
// Prevents accidental copying of instances.
#define NO_COPY_ASSIGN(type) \
    type(const type&) = delete; \
    type& operator = (const type&) = delete

// Removes all default constructors from a class.
// Prevents instantiation of a class unless a constructor is declared explicitly.
#define NO_INSTANCE(type) \
    type() = delete; \
    NO_COPY_ASSIGN(type)

#endif /* ROBOT_CONTROL_SYSTEM_MACROS_HPP_ */
