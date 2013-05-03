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

#include "users_robots_db.hpp"

#include <utility>

#include "robot.hpp"
#include "robot_db.hpp"
#include "sql.hpp"

namespace robot_control_system {

UsersRobotsDb::UsersRobotsDb(SqlDatabase* database)
  : database_(database) {
}

UsersRobotsDb::~UsersRobotsDb() {
  Close();
}

// static
UsersRobotsDb* UsersRobotsDb::Open(const string& config_file_path) {
  unique_ptr<SqlDatabase> database(new SqlDatabase());
  if (database->OpenUsingConfig(config_file_path)) {
    return new UsersRobotsDb(database.release());
  } else {
    return nullptr;
  }
}

void UsersRobotsDb::Close() {
  database_->Close();
}

bool UsersRobotsDb::VerifyUserCredentials(const string& username, const string& password) const {
  string query(
    "SELECT VerifyUserCredentials('" + username + "', '" + password + "');");
  unique_ptr<SqlQuery> sql_query(database_->Query(query));
  if (sql_query.get() != nullptr &&
      sql_query->status() == SqlQuery::Status::EXECUTED &&
      sql_query->NextResultRow()) {
    return sql_query->GetResultValue(0) == "1";
  }
  return false;
}

int UsersRobotsDb::LoadAllRobots() const {
  static const string kQuery("SELECT * FROM Robots;");
  unique_ptr<SqlQuery> sql_query(database_->Query(kQuery));
  if (sql_query.get() != nullptr && sql_query->status() == SqlQuery::Status::EXECUTED) {
    int count = 0;
    RobotDB* robot_db = RobotDB::mutable_instance();
    while (sql_query->NextResultRow()) {
        robot_db->AddRobot(sql_query->GetResultValue("robot_id"),
                           sql_query->GetResultValue("robot_name"));
      count++;
    }
    return count;
  }
  return -1;
}

bool UsersRobotsDb::ListUserRobots(const string& username,
                                   unordered_map<string, Robot*>* result) const {
  string query(
    "SELECT robot_id "
    "FROM Robots "
    "NATURAL JOIN UserRobots "
    "NATURAL JOIN Users "
    "WHERE username = '" + username + "';");
  unique_ptr<SqlQuery> sql_query(database_->Query(query));
  if (sql_query.get() != nullptr && sql_query->status() == SqlQuery::Status::EXECUTED) {
    const RobotDB* robot_db = RobotDB::instance();
    while (sql_query->NextResultRow()) {
      Robot* robot = robot_db->GetRobot(sql_query->GetResultValue("robot_id"));
      if (robot != nullptr) {
        result->insert(std::make_pair(robot->robot_id(), robot));
      }
    }
    return true;
  }
  return false;
}

}
