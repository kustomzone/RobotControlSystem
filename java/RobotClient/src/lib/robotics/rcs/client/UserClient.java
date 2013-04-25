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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import lib.robotics.rcs.server.Command;

/**
 * @author Tuna Oezer
 * 
 * Main application for the user service.
 * Connects a robot to the robot control server.
 */
public class UserClient {

	/**
	 * @param args Format: user@server
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		if (args.length < 2) {
			System.out.println("Missing argument(s): user@server robot_id");
			return;
		}
		String user_uri = args[0];
		int i = user_uri.indexOf('@');
		if (i < 0) {
			System.out.println("Invalid user URI.");
		}
		String user_name = user_uri.substring(0, i);
		String server = user_uri.substring(i + 1);
		UserServiceClient client = new UserServiceClient(server);
		String robot_id = args[1];
		User user = new User(user_name, client);
		client.open();
		BufferedReader cin = new BufferedReader(new InputStreamReader(System.in));
		char command;
		do {
			System.out.print("> ");
			System.out.flush();
			String input = cin.readLine();
			command = input.charAt(0);
			switch (command) {
				case 'x': user.sendCommand(robot_id, Command.STOP); break;
				case 'w': user.sendCommand(robot_id, Command.MOVE_FORWARD); break;
				case 's': user.sendCommand(robot_id, Command.MOVE_BACKWARD); break;
				case 'a': user.sendCommand(robot_id, Command.TURN_LEFT); break;
				case 'd': user.sendCommand(robot_id, Command.TURN_RIGHT); break;
				case 'r': user.sendCommand(robot_id, Command.TILT_UP); break;
				case 'f': user.sendCommand(robot_id, Command.TILT_DOWN); break;
				default: break;
			}
		} while (command != 'q');
		client.close();
	}
}
