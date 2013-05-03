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

#ifndef ROBOT_CONTROL_SYSTEM_USERS_ROBOTS_DB_HPP_
#define ROBOT_CONTROL_SYSTEM_USERS_ROBOTS_DB_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "macros.hpp"

namespace robot_control_system {

using ::std::string;
using ::std::unique_ptr;
using ::std::unordered_map;

class Robot;
class SqlDatabase;

/*
 * Interface to the UsersRobots SQL database.
 *
 * The UsersRobots database stores information about all users and robots and their relationships
 * with each other. This class exposes methods that can be used to query that database.
 *
 * Information on how to connect to the database is provided in a separate config file that is
 * loaded by Create();
 */
class UsersRobotsDb {
    NO_COPY_ASSIGN(UsersRobotsDb);

  public:
    // Creates and returns a new UsersRobotsDb instance. The caller owns the returned instance.
    //
    // Loads the specified config file. Returns null if the file does not exist or cannot
    // be opened.
    //
    // The config file must specify the following items on each line:
    //   1. database server host name
    //   2. database port
    //   3. database name
    //   4. database username
    //   5. username password
    static UsersRobotsDb* Open(const string& config_file_path);

    // If there is an open connection, calls Close().
    ~UsersRobotsDb();

    // Disconnects from the database and frees all resources.
    void Close();

    // Returns true iff a user with the username exists and the password is correct.
    // Passwords are hashed and salted with cryptographically secure algorithms before comparison.
    // The password string supplied to this method may be also hashed and salted but the caller
    // does not need to worry about cryptographic security.
    bool VerifyUserCredentials(const string& username, const string& password) const;

    // Loads information about all robots from the database.
    // Inserts all robots into the local RobotDB instance.
    // Returns the number of robots loaded or -1 if there was an error.
    int LoadAllRobots() const;

    // Lists all robots to with the user has access. Returns the robots in result indexed by
    // robot ID.
    // Owernship of the returned Robot pointers is retained.
    // Returns false if the list could not be obtained.
    bool ListUserRobots(const string& username, unordered_map<string, Robot*>* result) const;

  private:
    // UsersRobotsDb is instantiated via Create();
    explicit UsersRobotsDb(SqlDatabase* database);

    unique_ptr<SqlDatabase> database_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_USERS_ROBOTS_DB_HPP_ */
