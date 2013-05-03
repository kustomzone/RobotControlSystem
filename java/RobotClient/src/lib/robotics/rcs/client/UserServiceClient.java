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

import org.apache.commons.codec.digest.DigestUtils;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import lib.robotics.rcs.server.AccessDeniedException;
import lib.robotics.rcs.server.Command;
import lib.robotics.rcs.server.CommandRequest;
import lib.robotics.rcs.server.CommandResponse;
import lib.robotics.rcs.server.CommandStatus;
import lib.robotics.rcs.server.RobotInfo;
import lib.robotics.rcs.server.RobotStatus;
import lib.robotics.rcs.server.UserLoginRequest;
import lib.robotics.rcs.server.UserService;

/**
 * @author Tuna Oezer
 * 
 * Thrift service client that sends user requests to the Robot Control Server.
 */
public class UserServiceClient {

	public static final int kServerPort = 6100;

	private String server_address_;
	private TTransport transport_;
	private UserService.Client client_;
	
	/**
	 * @param server_address Internet address of robot control server.
	 */
	public UserServiceClient(String server_address) {
		this.server_address_ = server_address;
		transport_ = null;
		client_ = null;
	}

	/**
	 * Opens a connection to the robot control server.
	 */
	public void open() {
		if (transport_ != null) return;
		transport_ = new TSocket(server_address_, kServerPort);
		try {
			transport_.open();
			TProtocol protocol = new TBinaryProtocol(transport_);
			client_ = new UserService.Client(protocol);
		} catch (TTransportException e) {
			e.printStackTrace();
			transport_ = null;
		}
	}
	
	/**
	 * Closes the connection to the robot control server.
	 */
	public void close() {
		if (transport_ == null) return;
		transport_.close();
		transport_ = null;
		client_ = null;
	}
	
	/**
	 * Sends a login request. 
	 * @param user User to be logged in.
	 * @param password User password.
	 * @return True if successfully logged in.
	 */
	public boolean login(User user, String password) {
		UserLoginRequest request = new UserLoginRequest();
		request.setUsername(user.getUserName());
		request.setPassword(DigestUtils.shaHex(password + user.getUserName()));
		try {
			return client_.Login(request);
		} catch (TException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	/**
	 * Sends a logout request.
	 */
	public void logout() {
		try {
			client_.Logout();
		} catch (TException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * Logs in the specified robot.
	 * @param robot Robot.
	 * @return True if logged in.
	 */
	public boolean loginRobot(Robot robot) {
		if (robot.isOnline()) return true;
		try {
			if (client_.SetRobotStatus(robot.getRobotId(), RobotStatus.ONLINE) == RobotStatus.ONLINE) {
				robot.setOnline(true);
				return true;
			}
		} catch (AccessDeniedException e) {
			e.printStackTrace();
		} catch (TException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	/**
	 * Logs out the specified robot.
	 * @param robot Robot.
	 */
	public void logoutRobot(Robot robot) {
		if (!robot.isOnline()) return;
		try {
			if (client_.SetRobotStatus(robot.getRobotId(), RobotStatus.ONLINE) == RobotStatus.ONLINE) {
				robot.setOnline(false);
			}
		} catch (AccessDeniedException e) {
			e.printStackTrace();
		} catch (TException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Retrieves all robots to which user has access.
	 * @return List of user robots or null if error.
	 */
	public List<Robot> listRobots() {
		try {
			List<Robot> robots = new ArrayList<Robot>();
			List<RobotInfo> robot_infos = client_.ListRobots().getRobots();
			for (RobotInfo info : robot_infos) {
				robots.add(new Robot(info.getRobot_id(),
						   info.getRobot_name(),
						   info.getRobot_status() == RobotStatus.ONLINE));
			}
			return robots;
		} catch (TException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	/**
	 * Send a user command to a robot.
	 * @param user The user sending the command.
	 * @param robot_id The ID of the destination robot.
	 * @param command The command.
	 * @return True if the command accepted by the server succesfully.
	 */
	public boolean sendCommand(User user, String robot_id, Command command) {
		CommandRequest request = new CommandRequest();
		request.setFrom(user.getUserName());
		request.setTo(robot_id);
		request.setCommand(command);
		try {
			CommandResponse response = client_.SendCommand(request);
			return response.getStatus() == CommandStatus.OK;
		} catch (TException e) {
			e.printStackTrace();
		}
		return false;
	}
}
