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

#ifndef ROBOT_CONTROL_SYSTEM_SINGLETON_HPP_
#define ROBOT_CONTROL_SYSTEM_SINGLETON_HPP_

#include <memory>
#include <mutex>

#include "macros.hpp"

namespace robot_control_system {

/*
 * Template for singleton classes.
 * Instances are created lazily when the first instance is requested.
 */
template<class T>
class Singleton {
    NO_COPY_ASSIGN(Singleton);

  public:
    virtual ~Singleton() {}

    static const T* instance() {
      return mutable_instance();
    }

    static T* mutable_instance() {
      if (singleton_instance == nullptr) {
        ::std::lock_guard< ::std::mutex> lock(singleton_instance_mutex);
        if (singleton_instance == nullptr) {
          singleton_instance.reset(new T());
        }
      }
      return singleton_instance.get();
    }

  protected:
    Singleton() {}

  private:
    static ::std::unique_ptr<T> singleton_instance;
    static ::std::mutex singleton_instance_mutex;
};

template<class T> ::std::unique_ptr<T> Singleton<T>::singleton_instance(nullptr);
template<class T> ::std::mutex Singleton<T>::singleton_instance_mutex;

}

#endif /* ROBOT_CONTROL_SYSTEM_SINGLETON_HPP_ */
