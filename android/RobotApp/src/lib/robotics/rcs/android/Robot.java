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

package lib.robotics.rcs.android;

import java.io.IOException;
import java.io.OutputStreamWriter;

import lib.robotics.rcs.server.Command;

import android.os.Handler;

/**
 * @author Tuna Oezer
 * 
 * Represents the local robot.
 */
public class Robot {
	
	// Globally unique robot ID.
	private String robot_id_;
	
	// Client used to connect to RCS.
	private RobotServiceClient client_;
	
	// Whether robot is logged in.
	private boolean online_;
	
	// UI handler.
	private Handler handler_;
	
	// Connection to Arduino.
	private OutputStreamWriter arduino_;

	/**
	 * @param robot_id Globally unique robot ID.
	 * @param handler UI handler.
	 */
	public Robot(String robot_id, Handler handler) {
		this.robot_id_ = robot_id;
		client_ = null;
		online_ = false;
		this.handler_ = handler;
	}
	
	// Called by main app.
	public void setRobotId(String robot_id) {
		this.robot_id_ = robot_id;
	}
	
	// Called by main app.
	public void setServiceClient(RobotServiceClient client) {
		this.client_ = client;
	}
	
	// Called by main app.
	public void setArduinoOut(OutputStreamWriter arduino) {
		this.arduino_ = arduino;
	}
	
	public String getRobotId() {
		return robot_id_;
	}
	
	public boolean isOnline() {
		return online_;
	}
	
	/**
	 * Logs the robot into RCS.
	 */
	public void login() {
		if (online_) return;
		online_ = client_.login(this);
	}
	
	/**
	 * Logs out of RCS.
	 */
	public void logout() {
		if (!online_) return;
		client_.logout(this);
		online_ = false;
	}
	
	/**
	 * Sends the specified command to the local robot.
	 * Appends any protocol specific terminal characters. 
	 * @param command Command to send to robot.
	 * @return True if command was sent successfully.
	 */
	public boolean sendToRobot(String command) {
		if (arduino_ == null) return false;
		try {
			arduino_.write(command + "#");
			arduino_.flush();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public boolean stop() {
		return sendToRobot("X");
	}
	
	public boolean forward() {
		return sendToRobot("F");
	}
	
	public boolean backward() {
		return sendToRobot("B");
	}
	
	public boolean turn_left() {
		return sendToRobot("R");
	}
	
	public boolean turn_right() {
		return sendToRobot("L");
	}
	
	public boolean tilt_up() {
		return sendToRobot("U");
	}
	
	public boolean tilt_down() {
		return sendToRobot("N");
	}
	
	public void execute(Command command) {
		String message = null;
		switch (command) {
			case STOP:
				stop();
				message = "stop";
				break;
			case MOVE_FORWARD:
				forward();
				message = "forward";
				break;
			case MOVE_BACKWARD:
				backward();
				message = "backward";
				break;
			case TURN_LEFT:
				turn_left();
				message = "left";
				break;
			case TURN_RIGHT:
				turn_right();
				message = "right";
				break;
			case TILT_UP:
				tilt_up();
				message = "tilt up";
				break;
			case TILT_DOWN:
				tilt_down();
				message = "tilt down";
				break;
			default: break;
		}
		if (message != null) {
			handler_.obtainMessage(0, message).sendToTarget();
		}
	}
}
