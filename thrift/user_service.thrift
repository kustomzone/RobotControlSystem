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

namespace cpp robot_control_system
namespace java lib.robotics.rcs.server

enum Command {
  STOP = 1,
  MOVE_FORWARD = 2,
  MOVE_BACKWARD = 3,
  TURN_LEFT = 4,
  TURN_RIGHT = 5,
  TILT_UP = 6,
  TILT_DOWN = 7,
}

struct CommandRequest {
  1: string from,      // user name of sender
  2: string to,        // name of robot
  3: Command command,  // command to be executed by robot
}

enum CommandStatus {
  OK = 1,
  OFFLINE = 2,  // robot is offline
  ERROR = 3,    // other error
}

struct CommandResponse {
  1: CommandStatus status
}

/*
 * RobotServer service for human users. Used to send commands to robots.
 */
service UserService {
  CommandResponse SendCommand(1:CommandRequest request),
}
