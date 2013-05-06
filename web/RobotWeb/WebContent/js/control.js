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

const kServerAddress = "localhost:8080/RobotWeb/user_service";
const kUserNameId = "text_username";
const kPasswordId = "text_password";
const kRobotId = "text_robotid";
const kStopId = "button_stop";
const kForwardId = "button_forward";
const kBackwardId = "button_backward";
const kLeftId = "button_left";
const kRightId = "button_right";
const kTiltUpId = "button_tilt_up";
const kTitltDownId = "button_tilt_down";
const kResponseId = "p_response";

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

function shutDownUserService() {
	transport.close();
}

// Show a response text on the page.
function showResponse(response_text) {
	var response_paragraph = document.getElementById(kResponseId);
	response_paragraph.textContent = response_text;
}

// Logs in the user.
function login(sender) {
	var username = document.getElementById(kUserNameId).value;
	var password = document.getElementById(kPasswordId).value;
	var digest = SHA1(password + username);
	var request = new UserLoginRequest();
	request.username = username;
	request.password = digest;
	user_service.send_Login(request);
}

// Logs out the user.
function logout(sender) {
	user_service.send_Logout();
}

// Retunrs the command code given an onControl event sender. 
function getCommand(sender) {
	var id = sender.id;
	if (id == kStopId) return Command["STOP"];
	if (id == kForwardId) return Command["MOVE_FORWARD"];
	if (id == kBackwardId) return Command["MOVE_BACKWARD"];
	if (id == kLeftId) return Command["TURN_LEFT"];
	if (id == kRightId) return Command["TURN_RIGHT"];
	if (id == kTiltUpId) return Command["TILT_UP"];
	if (id == kTitltDownId) return Command["TILT_DOWN"];
	return 0;
}

// Handles control button clicks.
function onControl(sender) {
	var username = document.getElementById(kUserNameId).value;
	if (username == "") {
		showResponse("Username is missing.");
		return;
	}
	var robot_id = document.getElementById(kRobotId).value;
	if (robot_id == "") {
		showResponse("Robot ID is missing.");
		return;
	}
	var request = new CommandRequest();
	request.from = username;
	request.to = robot_id;
	request.command = getCommand(sender);
	user_service.send_SendCommand(request);
}

function serverResponseHandler() {
        var method = 0;
        if (thrift_method_name == "Login") method = 1;
        else if (thrift_method_name == "Logout") method = 2;
        else if (thrift_method_name == "SendCommand") method = 3;
	switch (method) {
		case 1: {
			var result = user_service.recv_Login();
			if (result) showResponse("Login OK");
			else showResponse("Login Failed");
		}
		case 2: {
			user_service.recv_Logout();
                        showResponse("Logged out.");
		}
		case 3: {
			var result = user_service.recv_SendCommand();
			switch (result.status) {
			    case CommandStatus["OK"]: showResponse("ok"); break;
                            case CommandStatus["OFFLINE"]: showResponse("robot is offline"); break;
			    default: showResponse("error"); break;
		        }
	        }
	        default: break;
	}
}
