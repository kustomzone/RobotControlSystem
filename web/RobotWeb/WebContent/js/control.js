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

var kServerAddress = "localhost:8080/RobotWeb/user_service";

var transport;
var protocol;
var user_service;

// Initializes the user service thrift client.
function initUserService() {
	transport = new WebSocketTransport(kServerAddress);
	protocol = new Thrift.Protocol(transport);
	user_service = new UserServiceClient(protocol);
	setWebSocketMessageHandler(serverResponseHandler);
	transport.open();
}

// Uninitializes the user service thrift client.
function shutDownUserService() {
	transport.close();
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
	request.command = Command[command];
	user_service.send_SendCommand(request);
}

// Dispatches the response.
function serverResponseHandler() {
    var method = 0;
    if (thrift_method_name == "Login") method = 1;
    else if (thrift_method_name == "Logout") method = 2;
    else if (thrift_method_name == "SendCommand") method = 3;
    switch (method) {
		case 1: {
			user_service.recv_Login();
		}
		case 2: {
			user_service.recv_Logout();
		}
		case 3: {
			var result = user_service.recv_SendCommand();
			switch (result.status) {
			    case CommandStatus["OK"]: break;
                case CommandStatus["OFFLINE"]: showMessage("The robot is offline."); break;
                default: showMessage("A transmission error occurred."); break;
	        }
        }
        default: break;
    }
}