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

#ifndef ROBOT_CONTROL_SYSTEM_PROXY_SERVER_HPP_
#define ROBOT_CONTROL_SYSTEM_PROXY_SERVER_HPP_

#include <boost/shared_ptr.hpp>

#include "macros.hpp"
#include "stl_thread.hpp"

namespace apache { namespace thrift { namespace protocol { class TProtocol; } } }

namespace robot_control_system {

class ProxyServerHandler;

/*
 * ProxyServer represents a Thrift server running on a remote machine. It is used to invert the
 * transport between a server and a client. This may be necessary if the server is behind a
 * firewall and cannot accept connections from a client. ProxyServer accepts client connections
 * on behalf of the remote server. On the local machine, the ProxyServer supplies a Thrift client
 * that can be used to communicate with the remote server.
 *
 * ProxyServer is an abstract base class. Derived classes implement proxy servers for a particular
 * Thrift service.
 */
class ProxyServer {
    NO_COPY_ASSIGN(ProxyServer);

  public:
    virtual ~ProxyServer();

    // Starts the ProxyServer on a new thread and returns.
    void Start();

    // Stops the ProxyServer. Blocks until the ProxyServer has shut down.
    void Stop();

  protected:
    // server_port: TCP port of the ProxyServer.
    explicit ProxyServer(int server_port);

    // Called when the proxy server has accepted a connection from a remote server.
    // Derived classes must implement this method to create a specific Thrift client for the
    // remote server.
    //
    // The protocol parameter represents the protocol handler created for the remote server.
    //
    // This method is called on the server thread and should return quickly. The proxy server
    // is unable to accept new connections until this method returns.
    virtual void AcceptRemoteServer(
        boost::shared_ptr<apache::thrift::protocol::TProtocol>* protocol) = 0;

  private:
    friend class ProxyServerHandler;

    // Implements the network handler for the ProxyServer.
    boost::shared_ptr<ProxyServerHandler> server_handler_;

    // The server thread accepts connections from remote servers.
    boost::shared_ptr<StlThread> server_thread_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_PROXY_SERVER_HPP_ */
