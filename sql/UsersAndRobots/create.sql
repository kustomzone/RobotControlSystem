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

/* List of all users. */
CREATE TABLE IF NOT EXISTS Users (
  user_id SERIAL,
  username VARCHAR(128) UNIQUE NOT NULL,
  PRIMARY KEY (user_id));

/* Emails of users.
 * A user may have multiple emails.
 * An email address might be shared by multiple users.
 */
CREATE TABLE IF NOT EXISTS Emails (
  email_id SERIAL,
  user_id BIGINT UNSIGNED NOT NULL,
  email TINYTEXT NOT NULL,
  PRIMARY KEY (email_id),
  FOREIGN KEY (user_id) REFERENCES Users(user_id));

/* User passwords. Passwords are hashed. */
CREATE TABLE IF NOT EXISTS Passwords (
  user_id BIGINT UNSIGNED NOT NULL,
  password TEXT NOT NULL,
  salt TINYTEXT NOT NULL,
  PRIMARY KEY (user_id),
  FOREIGN KEY (user_id) REFERENCES Users(user_id));

/* List of all robots. */
CREATE TABLE IF NOT EXISTS Robots (
  robot_id SERIAL,
  robot_name TINYTEXT NOT NULL,
  PRIMARY KEY (robot_id));

/* User access rights to robots.
 * A user may have access to multiple robots and a robot may be used by
 * multiple users.
 */
CREATE TABLE IF NOT EXISTS UserRobots (
  id SERIAL,
  user_id BIGINT UNSIGNED NOT NULL,
  robot_id BIGINT UNSIGNED NOT NULL,
  permissions INT UNSIGNED NOT NULL,
  PRIMARY KEY (id),
  FOREIGN KEY (user_id) REFERENCES Users(user_id),
  FOREIGN KEY (robot_id) REFERENCES Robots(robot_id),
  UNIQUE (user_id, robot_id));
