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

#include <string>

#include "logging.hpp"
#include "robot.hpp"
#include "robot_db.hpp"

namespace robot_control_system {

UserService::UserService() {}

void UserService::SendCommand(CommandResponse& response, const CommandRequest& request) {
  LOG_INFO("From:" + request.from + " To:" + request.to +
           " Command:" + std::to_string(request.command));
  response.status = CommandStatus::ERROR;
  Robot* robot = RobotDB::GetRobot(request.to);
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
