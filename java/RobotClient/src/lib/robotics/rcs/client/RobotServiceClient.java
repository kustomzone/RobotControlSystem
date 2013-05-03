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

import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import lib.robotics.rcs.server.RobotService;

/**
 * @author Tuna Oezer
 * 
 * Thrift service client that sends robot requests to the Robot Control Server.
 */
public class RobotServiceClient {
	
	public static final int kServerPort = 6600;
	
	private String server_address_;
	private TTransport transport_;
	private RobotService.Client client_;

	/**
	 * @param server_address Internet address of robot control server.
	 */
	public RobotServiceClient(String server_address) {
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
			client_ = new RobotService.Client(protocol);
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
		if (client_ == null) return;  // Suppress warning. TODO: remove
		client_ = null;
	}
}
