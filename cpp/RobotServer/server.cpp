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

#include "server.hpp"

#include <arpa/inet.h>
#include <thrift/TProcessor.h>
#include <thrift/concurrency/Thread.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TServerTransport.h>
#include <thrift/transport/TTransport.h>

#include "stl_thread.hpp"

namespace robot_control_system {

using apache::thrift::TProcessorFactory;
using apache::thrift::concurrency::ThreadFactory;
using apache::thrift::concurrency::ThreadManager;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::server::TThreadPoolServer;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TServerTransport;
using apache::thrift::transport::TTransportFactory;

Server::Server(const Params& params, TProcessorFactory* processor_factory)
  : params_(params),
    server_(),
    server_thread_(),
    processor_factory_(processor_factory) {
}

Server::~Server() {
  Stop();
}

void Server::Start() {
  boost::shared_ptr<ThreadFactory> thread_factory(new StlThreadFactory());
  boost::shared_ptr<ThreadManager> thread_manager =
      ThreadManager::newSimpleThreadManager(params_.num_threads);
  thread_manager->threadFactory(thread_factory);
  boost::shared_ptr<TServerTransport> transport(new TServerSocket(params_.server_port));
  boost::shared_ptr<TTransportFactory> transport_factory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
  server_.reset(new TThreadPoolServer(
      processor_factory_,
      transport,
      transport_factory,
      protocol_factory,
      thread_manager));
  server_thread_.reset(new StlThread(server_));
  server_->thread(server_thread_);

  thread_manager->start();
  server_thread_->start();
}

void Server::Stop() {
  if (server_thread_.get() == nullptr) return;
  server_->stop();
  server_thread_->join();
  server_thread_.reset();
}

void Server::Join() {
  if (server_thread_.get() == nullptr) return;
  server_thread_->join();
}

}
