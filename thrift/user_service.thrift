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

struct UserLoginRequest {
  1: string username,
  2: string password,
}

enum RobotStatus {
  OFFLINE = 1,
  ONLINE = 2,
}

// Information about a robot.
struct RobotInfo {
  1: string robot_id,    // Unique robot ID.
  2: string robot_name,  // Display name.
  3: RobotStatus robot_status,
}

struct UserRobots {
  1: list<RobotInfo> robots,
}

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
  ERROR = 2,    // other error
  OFFLINE = 3,  // robot is offline
}

struct CommandResponse {
  1: CommandStatus status
}

/*
 * Security error codes.
 */
enum SecurityError {
  USER_NOT_LOGGED_IN = 1,  // User is not logged in.
  INSUFFICIENT_RIGHTS = 2, // User has insufficent rights for action.
}

/*
 * Thrown when a user does not have sufficient privileges to execute a method with
 * the specified parameters.
 */
exception AccessDeniedException {
  1: SecurityError error,
}

/*
 * RobotServer service for human users.
 * All UserService methods require that the user is autenticated via Login().
 * UserService methods will fail 
 */
service UserService {
  // Logs in the user. Must be called before any other method can be called.
  // Returns true if the user has been successfully logged in.
  // This method may fail due to an invalid username or password.
  bool Login(1:UserLoginRequest request),

  // Logs out the user. No other methods can be called once the user is logged out.
  // Logged out users can login again.
  void Logout(),

  // Returns the status of the specified robot.
  RobotStatus GetRobotStatus(1:string robot_id) throws (1:AccessDeniedException access_denied),

  // Tries to updates the robot status to the new status and returns the new actual status after
  // the operation.
  RobotStatus SetRobotStatus(1:string robot_id, 2:RobotStatus new_status)
      throws (1:AccessDeniedException access_denied),

  // Returns a list of all robots to which the user has access.
  UserRobots ListRobots() throws (1:AccessDeniedException access_denied),

  // Sends a command to the specified robot.
  CommandResponse SendCommand(1:CommandRequest request)
      throws (1:AccessDeniedException access_denied),
}
