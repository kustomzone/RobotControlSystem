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

#ifndef ROBOT_CONTROL_SYSTEM_SERVER_HPP_
#define ROBOT_CONTROL_SYSTEM_SERVER_HPP_

#include <boost/shared_ptr.hpp>

#include "macros.hpp"

namespace apache {
  namespace thrift {
    class TProcessorFactory;
    namespace server { class TServer; }
  }
}

namespace robot_control_system {

class StlThread;

/*
 * Base class for servers.
 */
class Server {
    NO_COPY_ASSIGN(Server);

  public:
    // Parameters used to construct a server.
    struct Params {
      // TCP port at which server accepts connections.
      int server_port;

      // Number of threads used to handle requests.
      int num_threads;

      // Requires server_port.
      // Sets other parameters to default values.
      explicit Params(int server_port_param,
                      int num_threads_param = 6)
        : server_port(server_port_param),
          num_threads(num_threads_param) {
      }
    };

    virtual ~Server();

    const Params& params() const {
      return params_;
    }

    // Starts the server on a separate thread.
    void Start();

    // Stops the server. Blocks until the server is stopped.
    void Stop();

    // Joins the server thread. This method blocks until the server is stopped by
    // another thread via a call to Stop();
    void Join();

  protected:
    // processor_factory: Returns Thrift service specific processors.
    Server(const Params& params, apache::thrift::TProcessorFactory* processor_factory);

  private:
    Params params_;
    boost::shared_ptr<apache::thrift::server::TServer> server_;
    boost::shared_ptr<StlThread> server_thread_;
    boost::shared_ptr<apache::thrift::TProcessorFactory> processor_factory_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_SERVER_HPP_ */
