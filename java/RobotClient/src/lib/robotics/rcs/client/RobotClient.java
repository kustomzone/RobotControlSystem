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

import java.io.Console;
import java.util.List;

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
		Console console = System.console();
		if (args.length < 1) {
			console.printf("Missing argument(s): user@server\n");
			return;
		}
		String user_uri = args[0];
		int z = user_uri.indexOf('@');
		if (z < 0) {
			console.printf("Invalid user URI.\n");
		}
		String user_name = user_uri.substring(0, z);
		String server = user_uri.substring(z + 1);
		UserServiceClient client = new UserServiceClient(server);
		User user = new User(user_name, client);
		client.open();
		char[] password = console.readPassword("Password: ");
		if (user.login(String.valueOf(password))) {
			console.printf("Robots:\n");
			List<Robot> robots = user.getRobots();
			for (int i = 0; i < robots.size(); i++) {
				Robot robot = robots.get(i);
				console.printf("%s: %s %s\n",
							   robot.getRobotId(),
							   robot.getRobotName(),
							   robot.isOnline() ? "(online)" : "(offline)");
			}
			String robot_id = console.readLine("Log in with: ");
			if (!robot_id.equals("q")) {
				Robot robot = null;
				for (Robot r : user.getRobots()) {
					if (r.getRobotId().equals(robot_id)) {
						robot = r;
						break;
					}
				}
				if (!client.loginRobot(robot)) {
					console.printf("Robot login error.\n");
				}
				console.printf("Starting RobotControlServiceHandler...\n");
				RobotControlServiceHandler handler = new RobotControlServiceHandler(server, robot);
				handler.startService();
				console.printf("RobotControlServiceHandler is running.");
				console.readLine();
				console.printf("Stopping RobotControlServiceHandler...");
				handler.stopService();
				client.logoutRobot(robot);
			}
                        user.logout();
		} else {
			console.printf("Log in error.\n");
		}
		client.close();
	}

}
