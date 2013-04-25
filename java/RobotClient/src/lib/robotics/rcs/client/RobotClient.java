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

import java.io.IOException;

/**
 * @author Tuna Oezer
 * 
 * Main application for the robot service.
 * Connects a robot to the robot control server.
 */
public class RobotClient {

	/**
	 * @param args Format: robot@server
	 */
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Missing argument: robot@server");
			return;
		}
		String robot_uri = args[0];
		int i = robot_uri.indexOf('@');
		if (i < 0) {
			System.out.println("Invalid robot URI.");
		}
		String robot_id = robot_uri.substring(0, i);
		String server = robot_uri.substring(i + 1);
		RobotServiceClient client = new RobotServiceClient(server);
		Robot robot = new Robot(robot_id, client);
		client.open();
		System.out.println("Logging in...");
		robot.login();
		if (!robot.isOnline()) System.out.println("Log in error.");
		System.out.println("Starting RobotControlServiceHandler...");
		RobotControlServiceHandler handler = new RobotControlServiceHandler(server, robot);
		handler.startService();
		System.out.println("RobotControlServiceHandler is running.");
		try {
			System.in.read();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Stopping RobotControlServiceHandler...");
		handler.stopService();
		System.out.println("Logging out...");
		robot.logout();
		client.close();
	}

}
