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

#include "stl_thread.hpp"

#include <pthread.h>

namespace robot_control_system {

StlThread::StlThread(boost::shared_ptr<apache::thrift::concurrency::Runnable> runnable)
  : thread_(nullptr) {
  this->runnable(runnable);
}

void StlThread::start() {
  thread_.reset(new ::std::thread(&apache::thrift::concurrency::Runnable::run, runnable().get()));
}

void StlThread::join() {
  if (thread_.get() == nullptr) return;
  if (!thread_->joinable()) return;
  thread_->join();
}

apache::thrift::concurrency::Thread::id_t StlThread::getId() {
  if (thread_.get() == nullptr) return 0;
  return pthread_self();
}

boost::shared_ptr<apache::thrift::concurrency::Thread> StlThreadFactory::newThread(
    boost::shared_ptr<apache::thrift::concurrency::Runnable> runnable) const {
  return boost::shared_ptr<apache::thrift::concurrency::Thread>(new StlThread(runnable));
}

apache::thrift::concurrency::Thread::id_t StlThreadFactory::getCurrentThreadId() const {
  return pthread_self();
}

}
