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
class UsersRobotsDb;

/*
 * Database of robots.
 */
class RobotDB : public Singleton<RobotDB> {
    NO_COPY_ASSIGN(RobotDB);

  public:
    virtual ~RobotDB();

    // Initializes the local RobotDB using the specified user-robot SQL database.
    // Caller maintains ownership of the pointer.
    void Init(const UsersRobotsDb* users_robots_db);

    // Adds a robot with the given name and id to the database. Each robot must have a
    // unique id.
    // If the robot does not already exist in the database creates a new Robot instance.
    // Returns a pointer to the robot. Maintains ownership of the robot.
    Robot* AddRobot(const string& robot_id, const string& robot_name);

    // Returns a pointer to the robot with the specified id or null if no such robot
    // exists.
    Robot* GetRobot(const string& robot_id) const;

    const UsersRobotsDb* users_robots_db() const {
        return users_robots_db_;
    }

  private:
    friend class Singleton;

    // Singleton.
    RobotDB();

    unordered_map<string, Robot*> robots_;
    const UsersRobotsDb* users_robots_db_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_ROBOT_DB_HPP_ */
