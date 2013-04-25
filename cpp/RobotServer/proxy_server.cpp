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

#include "proxy_server.hpp"

#include <arpa/inet.h>
#include <thrift/concurrency/Thread.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TTransportException.h>

namespace robot_control_system {

using apache::thrift::concurrency::Runnable;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TTransportException;
using apache::thrift::transport::TTransportFactory;

class ProxyServerHandler : public Runnable {
    NO_COPY_ASSIGN(ProxyServerHandler);

  public:
    ProxyServerHandler(ProxyServer* server, int server_port)
      : server_(server),
        server_port_(server_port),
        running_(false),
        server_socket_() {

    }

    virtual ~ProxyServerHandler() {}

    bool running() const {
      return running_;
    }

    // Starts the handler and returns without blocking.
    void Start() {
      running_ = true;
    }

    // Stops the handler and returns without blocking.
    void Stop() {
      running_ = false;
      if (server_socket_.get() != nullptr) {
        server_socket_->interrupt();
      }
    }

    // Implements the Runnable interface. Should not be called directly.
    virtual void run() {
      boost::shared_ptr<TTransportFactory> transport_factory(new TBufferedTransportFactory());
      boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
      server_socket_.reset(new TServerSocket(server_port_));
      server_socket_->listen();
      try {
        while (running_) {
          boost::shared_ptr<TTransport> socket = server_socket_->accept();
          boost::shared_ptr<TTransport> transport(transport_factory->getTransport(socket));
          boost::shared_ptr<TProtocol> protocol(protocol_factory->getProtocol(transport));
          server_->AcceptRemoteServer(&protocol);
        }
      } catch (const TTransportException& e) {}
      server_socket_->close();
    }

  private:
    // The ProxyServer to which this handler belongs.
    ProxyServer* server_;

    // TCP port of server.
    int server_port_;

    // True if the server is started.
    bool running_;

    // Socket used to accept connections from remote servers.
    boost::shared_ptr<apache::thrift::transport::TServerSocket> server_socket_;
};

ProxyServer::ProxyServer(int server_port)
  : server_handler_(new ProxyServerHandler(this, server_port)),
    server_thread_() {
}

ProxyServer::~ProxyServer() {}

void ProxyServer::Start() {
  if (server_handler_->running()) return;
  server_thread_.reset(new StlThread(server_handler_));
  server_handler_->thread(server_thread_);
  server_handler_->Start();
  server_thread_->start();
}

void ProxyServer::Stop() {
  if (!server_handler_->running()) return;
  server_handler_->Stop();
  server_thread_->join();
}

}
