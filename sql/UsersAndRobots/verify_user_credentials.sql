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

/* Verifies the credentials of a user.
 * Returns true if and only if param_password is the password of param_username
 */
CREATE FUNCTION VerifyUserCredentials (
    param_username VARCHAR(128),
    param_password VARCHAR(64))
RETURNS BOOL
READS SQL DATA
RETURN
    (SELECT count(stored.password) > 0 AND
            sha2(concat(stored.salt, param_password), 512) = convert(stored.password USING utf8)
     FROM
         (SELECT password, salt
          FROM Passwords
          NATURAL JOIN Users
          WHERE username = param_username)
         AS stored);
