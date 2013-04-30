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

#ifndef ROBOT_CONTROL_SYSTEM_SQL_HPP_
#define ROBOT_CONTROL_SYSTEM_SQL_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "macros.hpp"

namespace robot_control_system {

using ::std::string;
using ::std::unique_ptr;
using ::std::unordered_map;

// MySQL related variables.
// Avoids having to include MySQL headers into this header.
struct MySql;
struct MySqlResult;
struct MySqlRow;

/*
 * Represents a SQL query.
 * SqlQuery is used to execute SQL queries and retrieve database results.
 */
class SqlQuery {
    NO_COPY_ASSIGN(SqlQuery);

  public:
    // The status of the SQL query.
    enum class Status {
        ERROR,      // query was unsuccessfully executed
      PENDING,   // query has not been executed yet
      EXECUTED,  // query has been successfully executed and results are ready to be retrieved
      COMPLETE   // all results have been retrieved
    };

    explicit SqlQuery(const string& query);

    // Ensures that no remaining results are left on the server.
    ~SqlQuery();

    Status status() const {
      return status_;
    }

    // Returns the query in SQL syntax.
    const string& query() const {
      return query_;
    }

    // Returns the number of columns in the result.
    int num_result_fields() const {
      return num_result_fields_;
    }

    // Retrieves the next result row from the database.
    // Returns false if all rows have already been retrieved.
    // This method can be used to retrieve all results once the query has been executed
    // by SqlDatabase.
    bool NextResultRow();

    // Retrieves the remaining results form the database.
    // All results for a query must be retrieved.
    void RetrieveRemainingResults();

    // Returns true if there is a currently loaded result row that has been retrieved via
    // NextResultRow().
    bool HasCurrentResultRow() const;

    // Returns the value of the cell at 'index' in the current result row retrieved via
    // the last call to NextResultRow().
    // This method can be used to retrieve all values once a result row has been retrieved
    // via NextResultRow().
    // Index must be 0 <= index < next_result_fields().
    // Returns the empty string if there is no currently loaded result row or index is invalid.
    string GetResultValue(int index) const;

    // Similar to GetResultValue(int) but returns the value for the column with the specified
    // name. Returns the empty string if there is no such column.
    string GetResultValue(const string& field_name) const;

    // Called by SqlDatabase.
    void set_status(Status status) {
      status_ = status;
    }

    // Called by SqlDatabase.
    void SetResult(const MySqlResult& result);

  private:
    string query_;  // query in SQL syntax
    Status status_;
    unique_ptr<MySqlResult> result_;
    int num_result_fields_;
    unordered_map<string, int> field_map_;  // field name -> result index
    unique_ptr<MySqlRow> current_row_;  // currently loaded result row
};

/*
 * Represents a SQL database.
 * SqlDatabase is used to connect to a SQL database and execute SQL queries on the database.
 */
class SqlDatabase {
    NO_COPY_ASSIGN(SqlDatabase);

  public:
    SqlDatabase();

    ~SqlDatabase();

    bool is_open() const {
      return open_;
    }

    // Opens a connection to the database using the configuration in the specified file.
    // Returns true if successful.
    bool OpenUsingConfig(const string& config_file_path);

    // Opens a connection to the database and returns true if successful.
    bool Open(const string& database_server,
              int server_port,
              const string& database_name,
              const string& username,
              const string& password);

    // Closes the connection to the database.
    void Close();

    // Executes the specified SQL query on the database.
    // If successful returns a SqlQuery object that can be used to retrieve the results.
    // Otherwise, returns null.
    // The caller owns the returned pointer.
    SqlQuery* Query(const string& query);

    // Executes the specified SqlQuery.
    // Returns true if the query was successfully executed.
    // After execution, the SqlQuery object can be used to retrieve all results.
    bool ExecuteQuery(SqlQuery* query);

  private:
    unique_ptr<MySql> database_;
    bool open_;
};

}

#endif /* ROBOT_CONTROL_SYSTEM_SQL_HPP_ */
