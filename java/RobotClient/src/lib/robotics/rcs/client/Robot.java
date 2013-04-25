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

package lib.robotics.rcs.client;

import lib.robotics.rcs.server.Command;

/**
 * @author Tuna Oezer
 * 
 * Represents a robot.
 */
public class Robot {
	
	private String robot_id_;
	private RobotServiceClient client_;
	private boolean online_;
	
	/**
	 * @param robot_id A globally unique robot ID.
	 * @param client Client used to connect to the server.
	 */
	public Robot(String robot_id, RobotServiceClient client) {
		this.robot_id_ = robot_id;
		this.client_ = client;
		online_ = false;
	}
	
	public String getRobotId() {
		return robot_id_;
	}
	
	public boolean isOnline() {
		return online_;
	}
	
	/**
	 * Logs into the Robot Control Server.
	 */
	public void login() {
		if (online_) return;
		online_ = client_.login(this);
	}
	
	/**
	 * Logs out of the Robot Control Server.
	 */
	public void logout() {
		if (!online_) return;
		client_.logout(this);
		online_ = false;
	}
	
	public void stop() {
		System.out.println(robot_id_ + ": stop");
	}
	
	public void moveForward() {
		System.out.println(robot_id_ + ": forward");
	}
	
	public void moveBackward() {
		System.out.println(robot_id_ + ": backward");
	}
	
	public void turnLeft() {
		System.out.println(robot_id_ + ": left");
	}
	
	public void turnRight() {
		System.out.println(robot_id_ + ": right");
	}
	
	public void tiltUp() {
		System.out.println(robot_id_ + ": tilt up");
	}
	
	public void tiltDown() {
		System.out.println(robot_id_ + ": tilt down");
	}
	
	public void execute(Command command) {
		switch (command) {
			case STOP: stop(); break;
			case MOVE_FORWARD: moveForward(); break;
			case MOVE_BACKWARD: moveBackward(); break;
			case TURN_LEFT: turnLeft(); break;
			case TURN_RIGHT: turnRight(); break;
			case TILT_UP: tiltUp(); break;
			case TILT_DOWN: tiltDown(); break;
			default: System.out.println("Unknown command: " + command.name()); break;
		}
	}
}
