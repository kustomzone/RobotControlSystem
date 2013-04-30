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

#include "robot.hpp"

#include "../RcsThrift/RobotControlService.h"
#include "../RcsThrift/UserService.h"

namespace robot_control_system {

Robot::Robot(const string& robot_id, const string& robot_name)
  : robot_id_(robot_id),
    robot_name_(robot_name),
    online_(false),
    robot_client_() {
}

bool Robot::ExecuteCommand(const CommandRequest& request) {
  if (!online_ || robot_client_.get() == nullptr) return false;
  robot_client_->ExecuteCommand(request);
  return true;
}

}
