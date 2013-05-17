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

//Show a message text on the page.
function showMessage(message) {
  document.getElementById("log").textContent += message + "\n";
}

function onEvent(topic, event) {
  if (topic == control_topic) {
	showMessage(event.from + ": " + event.command);
      if (bt) {
          if ("STOP" == event.command) {
              bt.send("dwrite(22,0);dwrite(24,0);dwrite(30,0);dwrite(32,0);");
          } else if ("MOVE_FORWARD" == event.command) {
              bt.send("dwrite(24,0);dwrite(22,1);pwm(11,128);");
              bt.send("dwrite(30,0);dwrite(32,1);pwm(12,128);");
          } else if ("MOVE_BACKWARD" == event.command) {
              bt.send("dwrite(22,0);dwrite(24,1);pwm(11,128);");
              bt.send("dwrite(32,0);dwrite(30,1);pwm(12,128);");
          } else if ("TURN_LEFT" == event.command) {
              bt.send("dwrite(22,0);dwrite(24,1);pwm(11,128);");
              bt.send("dwrite(30,0);dwrite(32,1);pwm(12,128);");
          } else if ("TURN_RIGHT" == event.command) {
              bt.send("dwrite(24,0);dwrite(22,1);pwm(11,128);");
              bt.send("dwrite(32,0);dwrite(30,1);pwm(12,128);");
          } else if ("TILT_UP" == event.command) {
              bt.send("dwrite(28,1);");
          } else if ("TILT_DOWN" == event.command) {
              bt.send("dwrite(28,0);");
          }
      }
  }
}

function onOpen(session, bt_address) {
  wamp_session = session;
  showMessage("WAMP open");
  bt = new Bluetooth();
  bt.open(bt_address, "00001101-0000-1000-8000-00805F9B34FB", function (result) {
     showMessage("BT open");
     wamp_session.subscribe(control_topic, onEvent);
  });
}

function onClose(exit_code, reason) {
  wamp_session = null;
  if (bt) bt.close();
}

//Initializes the WAMP connection.
function initialize(robot_id, bt_address) {
  control_topic = "pubsub://rcs/robot/" + robot_id + "/control";
  ab.connect(kServerAddress, function(session) { onOpen(session, bt_address); }, onClose);
}

//Uninitializes the WAMP connection.
function uninitialize() {
  wamp_session.close();
}
