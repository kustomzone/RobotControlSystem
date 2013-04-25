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

#include "robot_control_server.hpp"

#include <string>
#include <boost/shared_ptr.hpp>

#include "../RcsThriftCpp/RobotControlService.h"
#include "logging.hpp"
#include "robot.hpp"
#include "robot_db.hpp"

namespace robot_control_system {

using ::std::string;

RobotControlServer::RobotControlServer(int server_port)
  : ProxyServer(server_port) {
}

void RobotControlServer::AcceptRemoteServer(
        boost::shared_ptr<apache::thrift::protocol::TProtocol>* protocol) {
  boost::shared_ptr<RobotControlServiceClient> robot_client(new RobotControlServiceClient(*protocol));
  string robot_id;
  robot_client->GetRobotId(robot_id);
  LOG_INFO("Accepted proxy connection from robot " + robot_id);
  Robot* robot = RobotDB::GetRobot(robot_id);
  if (robot == nullptr) {
    LOG_WARNING("Unknown robot " + robot_id);
    return;
  }
  if (!robot->online()) {
    LOG_WARNING("Robot " + robot_id + " is offline");
    return;
  }
  robot->set_robot_client(&robot_client);
}

}
