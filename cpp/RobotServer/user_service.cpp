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

#include "user_service.hpp"

#include <algorithm>
#include <string>

#include "logging.hpp"
#include "robot.hpp"
#include "robot_db.hpp"
#include "users_robots_db.hpp"

namespace robot_control_system {

using ::std::for_each;
using ::std::pair;

UserService::UserService()
  : logged_in_(false),
    username_(""),
    user_robots_() {
}

bool UserService::Login(const UserLoginRequest& request) {
  static const unsigned int kMaxUsernamePasswordLength = 64;
  if (logged_in_) return false;
  if (request.username.size() > kMaxUsernamePasswordLength ||
      request.password.size() > kMaxUsernamePasswordLength) {
    return false;
  }
  const UsersRobotsDb* users_robots_db = RobotDB::instance()->users_robots_db();
  if (users_robots_db->VerifyUserCredentials(request.username, request.password)) {
    logged_in_ = true;
    username_ = request.username;
    users_robots_db->ListUserRobots(username_, &user_robots_);
    LOG_INFO(username_ + " has logged in with access to " +
             std::to_string(user_robots_.size()) + " robots.");
    return true;
  }
  return false;
}

void UserService::Logout() {
  logged_in_ = false;
  LOG_INFO(username_ + " has logged out.");
}

// static
RobotStatus::type UserService::GetRobotStatus(const Robot* robot) {
  if (robot->online()) return RobotStatus::ONLINE;
  return RobotStatus::OFFLINE;
}

RobotStatus::type UserService::GetRobotStatus(const std::string& robot_id) {
  if (!logged_in_) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::USER_NOT_LOGGED_IN;
    throw access_denied;
  }
  auto it = user_robots_.find(robot_id);
  if (it == user_robots_.end()) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::INSUFFICIENT_RIGHTS;
    throw access_denied;
  }
  return GetRobotStatus(it->second);
}

RobotStatus::type UserService::SetRobotStatus(const std::string& robot_id,
                                              const RobotStatus::type new_status) {
  if (!logged_in_) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::USER_NOT_LOGGED_IN;
    throw access_denied;
  }
  auto it = user_robots_.find(robot_id);
  if (it == user_robots_.end()) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::INSUFFICIENT_RIGHTS;
    throw access_denied;
  }
  switch (new_status) {
    case RobotStatus::OFFLINE: it->second->set_online(false); break;
    case RobotStatus::ONLINE: it->second->set_online(true); break;
    default: break;
  }
  return GetRobotStatus(it->second);
}

void UserService::ListRobots(UserRobots& result) {
  if (!logged_in_) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::USER_NOT_LOGGED_IN;
    throw access_denied;
  }
  for_each (user_robots_.begin(), user_robots_.end(), [&](pair<const string, Robot*> item) {
    result.robots.push_back(RobotInfo());
    RobotInfo& robot_info = result.robots.back();
    robot_info.robot_id = item.second->robot_id();
    robot_info.robot_name = item.second->robot_name();
    robot_info.robot_status = GetRobotStatus(item.second);
  });
}

void UserService::SendCommand(CommandResponse& response, const CommandRequest& request) {
  LOG_INFO("From:" + request.from + " To:" + request.to +
           " Command:" + std::to_string(request.command));
  if (!logged_in_) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::USER_NOT_LOGGED_IN;
    throw access_denied;
  }
  if (request.from != username_ ||
      user_robots_.find(request.to) == user_robots_.end()) {
    AccessDeniedException access_denied;
    access_denied.error = SecurityError::INSUFFICIENT_RIGHTS;
    throw access_denied;
  }
  response.status = CommandStatus::ERROR;
  Robot* robot = RobotDB::instance()->GetRobot(request.to);
  if (robot == nullptr) return;
  if (!robot->online()) {
    response.status = CommandStatus::OFFLINE;
    return;
  }
  if (robot->ExecuteCommand(request)) {
    response.status = CommandStatus::OK;
  }
}

}
