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

/* Sets the password of a user.
 * This method is intended to be used by an administrator. The password
 * parameter is the real clear text password of the user.
 * This method is not intended to be used to update the password programatically.
 */
DELIMITER //
CREATE PROCEDURE SetRealPassword (
    param_username VARCHAR(128),
    param_password VARCHAR(32))
MODIFIES SQL DATA
BEGIN
    DECLARE local_user_id BIGINT UNSIGNED;
    DECLARE local_salt TINYTEXT;

    SELECT user_id
    INTO local_user_id
    FROM Users
    WHERE username = param_username;

    IF ((SELECT count(local_user_id)) = 1) THEN
        SELECT salt
        INTO local_salt
        FROM Passwords
        WHERE user_id = local_user_id;

        UPDATE Passwords
        SET password = sha2(concat(local_salt, sha(concat(param_password, param_username))), 512)
        WHERE user_id = local_user_id;
    END IF;
END//
DELIMITER ;
