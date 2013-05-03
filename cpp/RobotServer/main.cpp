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

#include <memory>
#include <signal.h>
#include <string>

#include "logging.hpp"
#include "macros.hpp"
#include "robot_control_server.hpp"
#include "robot_db.hpp"
#include "robot_service.hpp"
#include "singleton.hpp"
#include "users_robots_db.hpp"
#include "user_service.hpp"

static const int kUserServerPort = 6100;
static const int kRobotServerPort = 6600;
static const int kRobotProxyServerPort = 6700;
static const char kSqlConfigFilePath[] = "sql.config";

namespace robot_control_system {

using ::std::unique_ptr;

/*
 * Main class. Runs the program.
 */
class Main : public Singleton<Main> {
    NO_COPY_ASSIGN(Main);

  public:
    // Initializes data structures and starts up the servers.
    // The servers are run on separate threads. Run() blocks for the duration of the program.
    // Returns 0 on success. Returns a non-zero value on error.
    int Run() {
      if (signal(SIGTERM, &Main::SignalHandler) == SIG_ERR) {
        LOG_ERROR("Failed to register signal handler.");
        return 1;
      }

      LOG_INFO("Connecting to SQL database...");
      users_robots_db_.reset(UsersRobotsDb::Open(kSqlConfigFilePath));
      if (users_robots_db_.get() == nullptr) {
        LOG_ERROR("Failed to connect to the SQL database.");
        return 1;
      }
      RobotDB::mutable_instance()->Init(users_robots_db_.get());

      LOG_INFO("Starting robot server...");
      Server::Params robot_server_params(kRobotServerPort);
      robot_server_.reset(new RobotServiceServer(robot_server_params));
      robot_server_->Start();
      LOG_INFO("Robot server is running on port " + std::to_string(kRobotServerPort));

      LOG_INFO("Starting robot proxy server...");
      robot_proxy_server_.reset(new RobotControlServer(kRobotProxyServerPort));
      robot_proxy_server_->Start();
      LOG_INFO("Robot proxy server is running on port " + std::to_string(kRobotProxyServerPort));

      LOG_INFO("Starting user server...");
      Server::Params user_server_params(kUserServerPort);
      user_server_.reset(new UserServiceServer(user_server_params));
      user_server_->Start();
      LOG_INFO("User server is running on port " + std::to_string(kUserServerPort));

      LOG_INFO("Accepting connections...");
      // User server is shut down before robot server.
      robot_server_->Join();
      LOG_INFO("Done.");

      return 0;
    }

    // Shuts down all servers and waits for all pending requests to complete.
    void Exit() {
      LOG_INFO("Disconnecting from SQL database.");
      if (users_robots_db_.get() != nullptr) users_robots_db_->Close();
      LOG_INFO("Shutting down user server...");
      if (user_server_.get() != nullptr) user_server_->Stop();
      LOG_INFO("Shutting down robot proxy server...");
      if (robot_proxy_server_.get() != nullptr) robot_proxy_server_->Stop();
      LOG_INFO("Shutting down robot server...");
      if (robot_server_.get() != nullptr) robot_server_->Stop();
      LOG_INFO("Shutdown complete.");
    }

  private:
    friend class Singleton;

    Main()
      : robot_server_(nullptr),
        user_server_(nullptr) {
    }

    // Implements SIGTERM signal handler to shut down the program.
    static void SignalHandler(int signum) {
      if (signum == SIGTERM) {
        Main::mutable_instance()->Exit();
      }
    }

    unique_ptr<UsersRobotsDb> users_robots_db_;
    unique_ptr<Server> robot_server_;
    unique_ptr<ProxyServer> robot_proxy_server_;
    unique_ptr<Server> user_server_;
};

}

int main(int argc, char** argv) {
  return robot_control_system::Main::mutable_instance()->Run();
}
