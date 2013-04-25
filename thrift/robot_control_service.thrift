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

include "user_service.thrift"

namespace cpp robot_control_system
namespace java lib.robotics.rcs.server

/*
 * The robot control service runs on the robot and accepts control commands
 * from the RCS server.
 */
service RobotControlService {
  // Returns the globally unique ID of the robot.
  string GetRobotId(),

  // Executes the specified command.
  void ExecuteCommand(1:user_service.CommandRequest request),
}
