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

var kServerAddress = "ws://localhost:8080/RobotWeb/robot_service";
var control_topic;

var wamp_session;

//Show a message text on the page.
function showMessage(message) {
	document.getElementById("message_box").textContent = message;
}

function onEvent(topic, event) {
	if (topic == control_topic) {
		showMessage(event.from + ": " + event.command);
	}
}

function onOpen(session) {
	wamp_session = session;
	wamp_session.subscribe(control_topic, onEvent);
}

function onClose(exit_code, reason) {
	wamp_session = null;
}

// Initializes the WAMP connection.
function initialize(robot_id) {
	control_topic = "pubsub://rcs/robot/" + robot_id + "/control";
	ab.connect(kServerAddress, onOpen, onClose);
}

// Uninitializes the WAMP connection.
function uninitialize() {
	wamp_session.close();
}
