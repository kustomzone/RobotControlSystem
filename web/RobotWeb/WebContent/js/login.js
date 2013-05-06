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

function login() {
	var username = document.getElementById("username_input").value;
	if (username.length == 0) {
		alert("Username is missing.");
		return;
	}
	var password = document.getElementById("password_input").value;
	if (password.length == 0) {
		alert("Passowrd is missing.");
		return;
	}
	document.login_form.username.value = username;
	document.login_form.password.value = SHA1(password + username);
	document.login_form.submit();
}
