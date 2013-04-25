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

struct LoginRequest {
  1: string robot_id,
}

struct LogoutRequest {
  1: string robot_id,
}

enum RequestStatus {
  OK = 1,
  ERROR = 2,
}

struct RobotServiceResponse {
  1: RequestStatus status,
}

/*
 * RobotServer service for robots.
 */
service RobotService {
  RobotServiceResponse Login(1:LoginRequest request),
  RobotServiceResponse Logout(1:LogoutRequest request),
}
