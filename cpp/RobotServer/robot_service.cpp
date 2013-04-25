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
#include "robot.hpp"
#include "robot_db.hpp"
#include "robot_service.hpp"

namespace robot_control_system {

RobotService::RobotService() {}

void RobotService::Login(RobotServiceResponse& response, const LoginRequest& request) {
  LOG_INFO("Login " + request.robot_id);
  response.status = RequestStatus::ERROR;
  Robot* robot = RobotDB::GetRobot(request.robot_id);
  if (robot == nullptr) return;
  robot->set_online(true);
  response.status = RequestStatus::OK;
}

void RobotService::Logout(RobotServiceResponse& response, const LogoutRequest& request) {
  LOG_INFO("Logout " + request.robot_id);
  response.status = RequestStatus::ERROR;
  Robot* robot = RobotDB::GetRobot(request.robot_id);
  if (robot == nullptr) return;
  robot->set_online(false);
  response.status = RequestStatus::OK;
}

}
