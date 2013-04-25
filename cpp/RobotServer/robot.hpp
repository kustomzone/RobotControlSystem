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

#ifndef ROBOT_CONTROL_SYSTEM_ROBOT_HPP_
#define ROBOT_CONTROL_SYSTEM_ROBOT_HPP_

#include <string>
#include <boost/shared_ptr.hpp>

#include "macros.hpp"

namespace robot_control_system {

using ::std::string;

class CommandRequest;
class RobotControlServiceClient;

/*
 * Represents a Robot.
 */
class Robot {
    NO_COPY_ASSIGN(Robot);

  public:
    explicit Robot(const string& robot_id);

    // Globally unique robot id.
    const string& robot_id() const {
      return robot_id_;
    }

	// True if the robot can be reached and execute commands.
    bool online() const {
      return online_;
    }

    void set_online(bool online) {
      online_ = online;
    }

    // Assigns the Thrift client that enables outgoing communication to the robot via the proxy
    // sever. The proxy is set by the RobotControlServer.
    void set_robot_client(boost::shared_ptr<RobotControlServiceClient>* robot_client) {
      robot_client_ = *robot_client;
    }

    // Executes the specified command on the robot. The robot must be online and accepting
    // commands through the proxy server.
    // Returns true if the command was successfully sent to the robot.
    bool ExecuteCommand(const CommandRequest& request);

  private:
    string robot_id_;
    bool online_;
    boost::shared_ptr<RobotControlServiceClient> robot_client_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_ROBOT_HPP_ */
