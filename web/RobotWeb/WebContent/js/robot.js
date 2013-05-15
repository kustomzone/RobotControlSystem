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
var bt;
var last_pin = -1;

//Show a message text on the page.
function showMessage(message) {
    var x = document.createElement("p");
    x.textContent = message;
    document.getElementById("message_box").appendChild(x);
}

function onEvent(topic, event) {
    if (topic == control_topic) {
	showMessage(event.from + ": " + event.command);
        if (bt) {
            var pin = 0;
            if ("STOP" == event.command) pin = 2;
            else if ("MOVE_FORWARD" == event.command) pin = 3;
            else if ("MOVE_BACKWARD" == event.command) pin = 4;
            else if ("TURN_LEFT" == event.command) pin = 5;
            else if ("TURN_RIGHT" == event.command) pin = 6;
            else if ("TILT_UP" == event.command) pin = 8;
            else if ("TILT_DOWN" == event.command) pin = 9;
            if (pin > 0) {
                bt.send("dwrite(" + pin + ",1);");
                if (last_pin > 0) {
                    bt.send("dwrite(" + last_pin + ",0);");
                }
                last_pin = pin;
            }
        }
    }
}

function onOpen(session) {
    wamp_session = session;
    bt = new Bluetooth();
    bt.open("00:00:00:00:00:00", "00001101-0000-1000-8000-00805F9B34FB", function (result) {
       wamp_session.subscribe(control_topic, onEvent);
    });
}

function onClose(exit_code, reason) {
    wamp_session = null;
    if (bt) bt.close();
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
