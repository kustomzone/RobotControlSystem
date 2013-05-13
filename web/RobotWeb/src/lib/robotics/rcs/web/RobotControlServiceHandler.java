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

package lib.robotics.rcs.web;

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

	public static final String kServerAddress = "localhost";
	private static final int kServerProxyPort = 6700;
	private static final String kTopicPrefix = "pubsub://rcs/robot/";
	private static final String kTopicSuffix = "/control";
	
	private String robot_id_;
	private boolean running_;
	private TTransport transport_;
	private String topic_;
	private WampTopicListenerDirectory publisher_;
	
	/**
	 * @param robot_id The unique ID of the robot to be controlled.
	 */
	public RobotControlServiceHandler(String robot_id) {
		this.robot_id_ = robot_id;
		this.setName("RobotControlServiceHandler-" + robot_id);
		running_ = false;
		transport_ = null;
		topic_ = kTopicPrefix + robot_id + kTopicSuffix;
		publisher_ = WampTopicListenerDirectory.getInstance();
	}
	
	public String getRobotId() {
		return robot_id_;
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
		return robot_id_;
	}

	@Override
	public void ExecuteCommand(CommandRequest request) {
		publisher_.publish(topic_, request);
	}
	
	@Override
	public void run() {
		TProcessor processor = new RobotControlService.Processor<RobotControlService.Iface>(this);
		transport_ = new TSocket(kServerAddress, kServerProxyPort);
		TProtocolFactory protocol_factory = new TBinaryProtocol.Factory();
		TProtocol input_protocol = protocol_factory.getProtocol(transport_);
		TProtocol output_protocol = protocol_factory.getProtocol(transport_);
		try {
			transport_.open();
			while (running_) {
				running_ = processor.process(input_protocol, output_protocol);
			}
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
