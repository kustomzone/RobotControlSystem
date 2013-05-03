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

#include "robot_db.hpp"

#include <algorithm>
#include <utility>

#include "logging.hpp"
#include "robot.hpp"
#include "users_robots_db.hpp"

namespace robot_control_system {

using ::std::for_each;
using ::std::pair;

RobotDB::RobotDB()
  : users_robots_db_(nullptr) {
}

RobotDB::~RobotDB() {
  for_each(robots_.begin(), robots_.end(), [](pair<const string, Robot*> item) {
    delete item.second;
  });
}

void RobotDB::Init(const UsersRobotsDb* users_robots_db) {
  if (users_robots_db_ != nullptr) {
    LOG_ERROR("Duplicate initialization of RobotDB.");
    return;
  }
  users_robots_db_ = users_robots_db;
  int robot_count = users_robots_db_->LoadAllRobots();
  LOG_INFO("Loaded " + std::to_string(robot_count) + " robots.");
}

Robot* RobotDB::AddRobot(const string& robot_id, const string& robot_name) {
  unordered_map<string, Robot*>::iterator it = robots_.find(robot_id);
  if (it != robots_.end()) return it->second;
  Robot* robot = new Robot(robot_id, robot_name);
  robots_.insert(std::make_pair(robot_id, robot));
  return robot;
}

Robot* RobotDB::GetRobot(const string& robot_id) const {
  unordered_map<string, Robot*>::const_iterator it = robots_.find(robot_id);
  if (it == robots_.end()) return nullptr;
  return it->second;
}

}
