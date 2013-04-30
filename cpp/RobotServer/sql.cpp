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

#include "sql.hpp"

#include <fstream>
#include <utility>
#include <vector>

#include <mysql.h>

#include "singleton.hpp"

namespace robot_control_system {

using ::std::ifstream;
using ::std::make_pair;
using ::std::vector;

struct MySql {
  MYSQL value;
};

struct MySqlResult {
  MYSQL_RES* value = nullptr;
};

struct MySqlRow {
  MYSQL_ROW value = nullptr;
};

SqlQuery::SqlQuery(const string& query)
    : query_(query),
      status_(Status::PENDING),
      result_(new MySqlResult()),
      num_result_fields_(0),
      field_map_(),
      current_row_(new MySqlRow()) {
}

SqlQuery::~SqlQuery() {
  RetrieveRemainingResults();
}

bool SqlQuery::NextResultRow() {
  if (status_ != Status::EXECUTED) return false;
  current_row_->value = mysql_fetch_row(result_->value);
  if (current_row_->value == nullptr) {
      status_ = Status::COMPLETE;
      mysql_free_result(result_->value);
      return false;
  }
  return true;
}

void SqlQuery::RetrieveRemainingResults() {
  while (NextResultRow());
}

bool SqlQuery::HasCurrentResultRow() const {
  return current_row_->value != nullptr;
}

string SqlQuery::GetResultValue(int index) const {
  if (current_row_->value == nullptr || index < 0 || index >= num_result_fields_) return "";
  return current_row_->value[index];
}

string SqlQuery::GetResultValue(const string& field_name) const {
  auto it = field_map_.find(field_name);
  if (it == field_map_.end()) return "";
  return GetResultValue(it->second);
}

void SqlQuery::SetResult(const MySqlResult& result) {
  result_->value = result.value;
  num_result_fields_ = mysql_num_fields(result.value);
  MYSQL_FIELD* fields = mysql_fetch_fields(result.value);
  for (int i = 0; i < num_result_fields_; ++i) {
    field_map_.insert(make_pair(fields[i].name, i));
  }
  status_ = Status::EXECUTED;
}

/**************************************************************************************************/

/*
 * Used to initialize and uninitialize the SQL library.
 */
class SqlLibrary : public Singleton<SqlLibrary> {
    NO_COPY_ASSIGN(SqlLibrary);

  public:
    virtual ~SqlLibrary() {
      mysql_library_end();
    }

  private:
    friend class Singleton;

    // Singleton.
    SqlLibrary() {
      mysql_library_init(0, NULL, NULL);
    }
};

/**************************************************************************************************/

SqlDatabase::SqlDatabase()
    : database_(nullptr),
      open_(false) {
  // Initialize library if necessary.
  SqlLibrary::instance();
  mysql_init(&database_->value);
}

SqlDatabase::~SqlDatabase() {
}

bool SqlDatabase::OpenUsingConfig(const string& config_file_path) {
  ifstream file;
  file.open(config_file_path);
  if (file.good()) {
    vector<string> lines;
    do {
      lines.push_back("");
      std::getline(file, lines.back());
    } while (file.good());
    file.close();
    if (lines.size() >= 5) {
      try {
        return Open(lines[0], std::stoi(lines[1]), lines[2], lines[3], lines[4]);
      } catch (std::out_of_range& e) {
      } catch (std::invalid_argument& e) {
      }
    }
  }
  return false;
}

bool SqlDatabase::Open(const string& database_server,
                       int server_port,
                       const string& database_name,
                       const string& username,
                       const string& password) {
  if (open_) return false;
  database_.reset(new MySql());
  open_ = mysql_real_connect(&database_->value,
                             database_server.c_str(),
                             username.c_str(),
                             password.c_str(),
                             database_name.c_str(),
                             server_port,
                             NULL,  // socket
                             0);    // flags
  return open_;
}

void SqlDatabase::Close() {
  if (!open_) return;
  mysql_close(&database_->value);
  database_.reset(nullptr);
  open_ = false;
}

SqlQuery* SqlDatabase::Query(const string& query) {
  unique_ptr<SqlQuery> sql_query(new SqlQuery(query));
  if (ExecuteQuery(sql_query.get())) {
    return sql_query.release();
  }
  return nullptr;
}

bool SqlDatabase::ExecuteQuery(SqlQuery* query) {
  if (mysql_query(&database_->value, query->query().c_str())) {
    query->set_status(SqlQuery::Status::ERROR);
    return false;
  }
  MySqlResult result;
  result.value = mysql_use_result(&database_->value);
  if (result.value == nullptr) {
    query->set_status(SqlQuery::Status::ERROR);
    return false;
  }
  query->SetResult(result);
  return true;
}

}
