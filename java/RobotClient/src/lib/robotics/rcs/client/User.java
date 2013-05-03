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

import java.util.ArrayList;
import java.util.List;

import lib.robotics.rcs.server.Command;

/**
 * @author Tuna Oezer
 * 
 * Represents a human user.
 */
public class User {

	private String user_name_;
	private UserServiceClient client_;
	private List<Robot> robots_;

	/**
	 * @param user_name User name to be used to communicate with the Robot Control Server.
	 * @param client Client used to connect to the server.
	 */
	public User(String user_name, UserServiceClient client) {
		this.user_name_ = user_name;
		this.client_ = client;
		robots_ = null;
	}
	
	public String getUserName() {
		return user_name_;
	}
	
	public List<Robot> getRobots() {
		return robots_;
	}
	
	/**
	 * Logs in the user.
	 * @param password User password.
	 * @return True if login is successful.
	 */
	public boolean login(String password) {
		if (client_.login(this, password)) {
			robots_ = client_.listRobots();
			if (robots_ == null) robots_ = new ArrayList<Robot>();
			return true;
		}
		return false;
	}
	
	/**
	 * Logs out user.
	 */
	public void logout() {
		client_.logout();
	}
	
	/**
	 * Sends a command to the specified robot. 
	 * @param robot_id URI of robot.
	 * @param command Command to be sent.
	 */
	public void sendCommand(String robot_id, Command command) {
		client_.sendCommand(this, robot_id, command);
	}
}
