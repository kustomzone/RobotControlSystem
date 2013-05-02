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

#ifndef ROBOT_CONTROL_SYSTEM_THRIFT_SERVER_HPP_
#define ROBOT_CONTROL_SYSTEM_THRIFT_SERVER_HPP_

#include <type_traits>

#include "server.hpp"

namespace apache { namespace thrift { class TConnectionInfo; } }

namespace robot_control_system {

/**
 * Template class for ThriftServers. Instantiates a Thrift Server for the specified handler.
 * Defines a handler factory that creates a separate handler for each connection. This allows
 * storing client specific information for each connection.
 *
 * THandler: The handler implementation. Must derive from THandlerIf.
 * THandlerIf: Handler interface for Thrift service generated by thrift compiler.
 * THandlerIfFactory: Handler factory interface generated by thrift compiler.
 * TProcessorFactory: Processor factory for Thrift service. Usually, the processor factory
 *     generated by the Thrift compiler can be used.
 *
 * For convenience, the macros below can be used to derive from or define a ThriftServer.
 * For example, for the Thrift service 'MyService', a server can be defined with
 *     DEFINE_THRIFT_SERVER(MyService);
 */
template<class THandler, class THandlerIf, class THandlerIfFactory, class TProcessorFactory>
class ThriftServer : public Server {
    NO_COPY_ASSIGN(ThriftServer);
    static_assert(::std::is_base_of<THandlerIf, THandler>::value,
        "THandler must be a derived class of THandlerIf");
    static_assert(::std::is_base_of<apache::thrift::TProcessorFactory, TProcessorFactory>::value,
        "TProcessorFactory must be a derived class of apache::thrift::TProcessorFactory");

  public:
    explicit ThriftServer(const Params& params)
      : Server(params,
               new TProcessorFactory(boost::shared_ptr<THandlerIfFactory>(new HandlerFactory()))) {
    }

  protected:
    /* Creates a separate handler for each connection. */
    class HandlerFactory : public THandlerIfFactory {
        NO_COPY_ASSIGN(HandlerFactory);

      public:
        HandlerFactory() {}

        virtual THandler* getHandler(const apache::thrift::TConnectionInfo& connection_info) {
          return new THandler();
        }

        virtual void releaseHandler(THandlerIf* handler) {
          delete handler;
        }
    };
};

// Convenience macro for ThriftServer declarations using standard naming conventions.
// This macro can be used to derive from a ThriftServer.
// This macro uses the default processor factory generated by the Thrift compiler.
#define THRIFT_SERVER(THandler) \
    ThriftServer<THandler, THandler ## If, THandler ## IfFactory, THandler ## ProcessorFactory>

// Convenience macro to define a default class that derives from ThriftServer.
// The default class only specializes ThriftServer.
#define DEFINE_THRIFT_SERVER_WITH_NAME(THandler, ServerClassName) \
    class ServerClassName : public THRIFT_SERVER(THandler) { \
        NO_COPY_ASSIGN(ServerClassName); \
      public: \
        explicit ServerClassName(const Params& params) : ThriftServer(params) {} \
    }

// Convenience macro to define a default class the derives from ThriftServer using standard
// naming conventions. For a service handler named 'MyHandler', a server named 'MyHandlerServer'
// will be defined.
// The default class only specializes ThriftServer.
#define DEFINE_THRIFT_SERVER(THandler) \
    DEFINE_THRIFT_SERVER_WITH_NAME(THandler, THandler ## Server)

}

#endif /* ROBOT_CONTROL_SYSTEM_THRIFT_SERVER_HPP_ */
