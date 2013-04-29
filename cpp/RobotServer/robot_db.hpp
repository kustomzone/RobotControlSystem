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

#ifndef ROBOT_CONTROL_SYSTEM_ROBOT_DB_HPP_
#define ROBOT_CONTROL_SYSTEM_ROBOT_DB_HPP_

#include <string>
#include <unordered_map>

#include "macros.hpp"
#include "singleton.hpp"

namespace robot_control_system {

using ::std::string;
using ::std::unordered_map;

class Robot;

/*
 * Database of robots.
 */
class RobotDB : public Singleton<RobotDB> {
    NO_COPY_ASSIGN(RobotDB);

  public:
    virtual ~RobotDB();

    // Initializes the database with built-in robots.
    static void Initialize();

    // Adds a robot with the given id to the database. Each robot must have a unique id.
    // If the robot does not already exist in the database creates a new Robot instance.
    // Returns a pointer to the robot. Maintains ownership of the robot.
    static Robot* AddRobot(const string& robot_id);

    // Returns a pointer to the robot with the specified id or null if no such robot
    // exists.
    static Robot* GetRobot(const string& robot_id);

  private:
    friend class Singleton;

    // Singleton.
    RobotDB();

    Robot* AddRobotImpl(const string& robot_id);

    Robot* GetRobotImpl(const string& robot_id) const;

    unordered_map<string, Robot*> robots_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_ROBOT_DB_HPP_ */
