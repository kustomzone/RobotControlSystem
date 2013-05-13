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

import javax.servlet.http.HttpSessionBindingEvent;
import javax.servlet.http.HttpSessionBindingListener;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import lib.robotics.rcs.server.AccessDeniedException;
import lib.robotics.rcs.server.CommandRequest;
import lib.robotics.rcs.server.CommandResponse;
import lib.robotics.rcs.server.CommandStatus;
import lib.robotics.rcs.server.RobotStatus;
import lib.robotics.rcs.server.UserLoginRequest;
import lib.robotics.rcs.server.UserRobots;
import lib.robotics.rcs.server.UserService;

/**
 * @author Tuna Oezer
 * 
 * Thrift service handler for the UserService service. This handler converts JSON
 * frontend user service requests into binary format and forwards them to the RCS
 * backend server.
 */
public class UserServiceHandler implements UserService.Iface, HttpSessionBindingListener {
	public static final String kUserServiceHandler = "UserServiceHandler";

	public static final String kServerAddress = "localhost";
	public static final int kServerPort = 6100;

	private TTransport transport_;
	private UserService.Client client_;
	private boolean logged_in_;
	
	public UserServiceHandler() {
		transport_ = null;
		client_ = null;
		logged_in_ = false;
	}
	
	/**
	 * Opens a new connection to the RCS server.
	 */
	public boolean open() {
		transport_ = new TSocket(kServerAddress, kServerPort);
		try {
			transport_.open();
			TProtocol protocol = new TBinaryProtocol(transport_);
			client_ = new UserService.Client(protocol);
			return true;
		} catch (TTransportException e) {
			e.printStackTrace();
			transport_ = null;
		}
		return false;
	}
	
	/**
	 * @return True if a connection the RCS server is open.
	 */
	public boolean isOpen() {
		return transport_ != null && transport_.isOpen();
	}
	
	/**
	 * @return True if a connection to the RCS server is open and the user is logged in.
	 */
	public boolean isLoggedIn() {
		return isOpen() && logged_in_;
	}
	
	/**
	 * Closes the connection to the RCS.
	 */
	public void close() {
		if (transport_ == null) return;
		transport_.close();
		transport_ = null;
		client_ = null;
	}

	@Override
	public RobotStatus GetRobotStatus(String robot_id)
			throws AccessDeniedException, TException {
		return client_.GetRobotStatus(robot_id);
	}

	@Override
	public UserRobots ListRobots() throws AccessDeniedException, TException {
		return client_.ListRobots();
	}

	@Override
	public boolean Login(UserLoginRequest request) throws TException {
		logged_in_ = client_.Login(request);
		return logged_in_;
	}

	@Override
	public void Logout() throws TException {
		logged_in_ = false;
		if (client_ != null) client_.Logout();
	}

	@Override
	public RobotStatus SetRobotStatus(String robot_id, RobotStatus new_status)
			throws AccessDeniedException, TException {
		return client_.SetRobotStatus(robot_id, new_status);
	}

	@Override
	public CommandResponse SendCommand(CommandRequest request) {
		try {
			return client_.SendCommand(request);
		} catch (TException e) {
			e.printStackTrace();
		}
		CommandResponse response = new CommandResponse();
		response.setStatus(CommandStatus.ERROR);
		return response; 
	}

	@Override
	public void valueBound(HttpSessionBindingEvent event) {
	}

	@Override
	public void valueUnbound(HttpSessionBindingEvent event) {
		try {
			Logout();
			close();
		} catch (TException e) {
			e.printStackTrace();
		}
	}
}
