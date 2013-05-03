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

import java.net.SocketException;

import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.protocol.TProtocolFactory;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import lib.robotics.rcs.server.CommandRequest;
import lib.robotics.rcs.server.RobotControlService;

/**
 * @author Tuna Oezer
 *
 * Thrift service handler for the RobotControlService.
 * The RobotControlServiceHandler receives robot control messages from the RCS server via
 * an RCS proxy server and executes the command on the local robot.
 */
public class RobotControlServiceHandler extends Thread implements RobotControlService.Iface {
	
	public static final int kServerProxyPort = 6700;
	
	private String server_address_;
	private Robot robot_;
	private boolean running_;
	private TTransport transport_;
	
	/**
	 * @param server_address Internet address of robot control server.
	 * @param robot The robot to be controlled.
	 */
	public RobotControlServiceHandler(String server_address, Robot robot) {
		this.server_address_ = server_address;
		this.robot_ = robot;
		this.setName("RobotControlServiceHandler");
		running_ = false;
		transport_ = null;
	}
	
	/**
	 * Starts the RobotControlServiceHandler on separate thread.
	 */
	public void startService() {
		running_ = true;
		start();
	}
	
	/**
	 * Stops the RobotControlServiceHandler and blocks until the handler is stopped.
	 */
	public void stopService() {
		running_ = false;
		if (transport_.isOpen()) {
			transport_.close();
		}
		if (isAlive()) {
			try {
				join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public String GetRobotId() {
		return robot_.getRobotId();
	}

	@Override
	public void ExecuteCommand(CommandRequest request) {
		System.out.println("Received command from " + request.getFrom());
		robot_.execute(request.getCommand());
	}
	
	@Override
	public void run() {
		TProcessor processor = new RobotControlService.Processor<RobotControlService.Iface>(this);
		transport_ = new TSocket(server_address_, kServerProxyPort);
		TProtocolFactory protocol_factory = new TBinaryProtocol.Factory();
		TProtocol input_protocol = protocol_factory.getProtocol(transport_);
		TProtocol output_protocol = protocol_factory.getProtocol(transport_);
		try {
			System.out.println("Connecting to proxy server...");
			transport_.open();
			System.out.println("Connected to proxy server.");
			while (running_) {
				running_ = processor.process(input_protocol, output_protocol);
			}
			System.out.println("Closing connection to proxy server...");
			if (transport_.isOpen()) {
				transport_.close();
			}
		} catch (TTransportException e) {
			if (e.getCause() == null ||
				e.getCause().getClass() != SocketException.class) {
				e.printStackTrace();
			}
		} catch (TException e) {
			e.printStackTrace();
		}
	}
}
