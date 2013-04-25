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

#ifndef ROBOT_CONTROL_SYSTEM_ROBOT_CONTROL_SERVER_HPP_
#define ROBOT_CONTROL_SYSTEM_ROBOT_CONTROL_SERVER_HPP_

#include "macros.hpp"
#include "proxy_server.hpp"

namespace robot_control_system {

/*
 * Proxy server for the robot control service running on a remote robot.
 *
 * The robot control service accepts control commands and executes them on a remote robot.
 * Since the robot will typically be behind a firewall and have a local IP, the robot control
 * service is typically not accessible from RCS services running in the cloud.
 *
 * This proxy server solves that problem by serving as a proxy between the robot and RCS cloud
 * services. Both the robot and RCS services can connect as clients to the proxy. Since the proxy
 * runs in the cloud together with other RCS services, it is reachable by all RCS services.
 */
class RobotControlServer : public ProxyServer {
    NO_COPY_ASSIGN(RobotControlServer);

  public:
    explicit RobotControlServer(int server_port);

  protected:
    virtual void AcceptRemoteServer(
        boost::shared_ptr<apache::thrift::protocol::TProtocol>* protocol);
};

}

#endif /* ROBOT_CONTROL_SYSTEM_ROBOT_CONTROL_SERVER_HPP_ */
