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

#ifndef ROBOT_CONTROL_SYSTEM_USER_SERVICE_HPP_
#define ROBOT_CONTROL_SYSTEM_USER_SERVICE_HPP_

#include <string>
#include <unordered_map>

#include "macros.hpp"
#include "thrift_server.hpp"
#include "../RcsThrift/UserService.h"

namespace robot_control_system {

using ::std::string;
using ::std::unordered_map;

class Robot;

/*
 * Implementation of Thrift service UserService.
 * Handles requests initiated by users.
 */
class UserService : public UserServiceIf {
    NO_COPY_ASSIGN(UserService);

  public:
    UserService();

    virtual bool Login(const UserLoginRequest& request);

    virtual void Logout();

    // Throws AccessDeniedException.
    virtual RobotStatus::type GetRobotStatus(const string& robot_id);

    // Throws AccessDeniedException.
    virtual RobotStatus::type SetRobotStatus(const string& robot_id,
                                             const RobotStatus::type new_status);

    // Throws AccessDeniedException.
    virtual void ListRobots(UserRobots& result);

    // Throws AccessDeniedException.
    virtual void SendCommand(CommandResponse& response, const CommandRequest& request);

  private:
    static RobotStatus::type GetRobotStatus(const Robot* robot);

    bool logged_in_;
    string username_;
    unordered_map<string, Robot*> user_robots_;  // Robots accessible by the user.
};

DEFINE_THRIFT_SERVER(UserService);

}

#endif /* ROBOT_CONTROL_SYSTEM_USER_SERVICE_HPP_ */
