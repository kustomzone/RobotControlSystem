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

var kServerAddress = "ws://ec2-54-245-59-56.us-west-2.compute.amazonaws.com:8080/RobotWeb/user_service";
var kSendCommandUri = "rpc://lib.robotics.rcs.server.UserService/SendCommand";

var wamp_session;

function onOpen(session) {
	wamp_session = session;
}

function onClose(exit_code, reason) {
	wamp_session = null;
}

// Initializes the WAMP connection.
function initialize() {
	ab.connect(kServerAddress, onOpen, onClose);
}

// Uninitializes the WAMP connection.
function uninitialize() {
	wamp_session.close();
}

// Show a message text on the page.
function showMessage(message) {
	document.getElementById("message_box").textContent = message;
}

//Handles control button clicks.
function sendCommand(command) {
	showMessage("Last command: " + command);
	var request = new CommandRequest();
	request.from = g_username;
	request.to = g_robot_id;
	request.command = Command[command] - 1;
	wamp_session.call(kSendCommandUri, request).then(serverResponseHandler);
}

// Dispatches the response.
function serverResponseHandler(result) {
	if (result.status == "OK") return;
	if (result.status == "OFFLINE") {
		showMessage("The robot is offline.");
		return;
	}
	showMessage("A transmission error occurred.");
}
