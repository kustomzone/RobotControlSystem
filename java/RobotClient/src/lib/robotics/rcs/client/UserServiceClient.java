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

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import lib.robotics.rcs.server.Command;
import lib.robotics.rcs.server.CommandRequest;
import lib.robotics.rcs.server.CommandResponse;
import lib.robotics.rcs.server.CommandStatus;
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
