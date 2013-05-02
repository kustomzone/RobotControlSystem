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

#include "macros.hpp"
#include "thrift_server.hpp"
#include "../RcsThrift/UserService.h"

namespace robot_control_system {

/*
 * Implementation of Thrift service UserService.
 * Handles requests initiated by users.
 */
class UserService : public UserServiceIf {
    NO_COPY_ASSIGN(UserService);

  public:
    UserService();

    virtual void SendCommand(CommandResponse& response, const CommandRequest& request);
};

/*
 * Thrift server for the UserService.
 */
class UserServiceServer : public THRIFT_SERVER(UserService)
{
    NO_COPY_ASSIGN(UserServiceServer);

  public:
    explicit UserServiceServer(const Params& params)
      : ThriftServer(params) {
    }
};

}

#endif /* ROBOT_CONTROL_SYSTEM_USER_SERVICE_HPP_ */
