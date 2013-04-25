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

#ifndef ROBOT_CONTROL_SYSTEM_STL_THREAD_HPP_
#define ROBOT_CONTROL_SYSTEM_STL_THREAD_HPP_

#include <memory>
#include <thread>
#include <boost/shared_ptr.hpp>
#include <thrift/concurrency/Thread.h>

#include "macros.hpp"

/*
 * Thrift adapter for STL threads.
 */
namespace robot_control_system {

class StlThread : public apache::thrift::concurrency::Thread {
    NO_COPY_ASSIGN(StlThread);

  public:
    explicit StlThread(boost::shared_ptr<apache::thrift::concurrency::Runnable> runnable);

    virtual void start();

    virtual void join();

    virtual apache::thrift::concurrency::Thread::id_t getId();

  private:
    ::std::unique_ptr< ::std::thread> thread_;
};

/*
 * Thread factory for STL threads.
 */
class StlThreadFactory : public apache::thrift::concurrency::ThreadFactory {
  public:
    virtual boost::shared_ptr<apache::thrift::concurrency::Thread> newThread(
        boost::shared_ptr<apache::thrift::concurrency::Runnable> runnable) const;

    virtual apache::thrift::concurrency::Thread::id_t getCurrentThreadId() const;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_STL_THREAD_HPP_ */
