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

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import lib.robotics.rcs.server.CommandRequest;
import lib.robotics.rcs.server.CommandResponse;
import lib.robotics.rcs.server.CommandStatus;
import lib.robotics.rcs.server.UserService;

/**
 * @author Tuna Oezer
 * 
 * Thrift service handler for the UserService service. This handler converts JSON
 * frontend user service requests into binary format and forwards them to the RCS
 * backend server.
 */
public class UserServiceHandler implements UserService.Iface {
	
	private static class ServiceClient {

		public static final String kServerAddress = "localhost";
		public static final int kServerPort = 6100;
		
		private static ServiceClient instance = null;

		private TTransport transport_;
		private UserService.Client client_;

		private ServiceClient() {
			transport_ = new TSocket(kServerAddress, kServerPort);
			try {
				transport_.open();
				TProtocol protocol = new TBinaryProtocol(transport_);
				client_ = new UserService.Client(protocol);
			} catch (TTransportException e) {
				e.printStackTrace();
				transport_ = null;
			}
		}
		
		public static synchronized ServiceClient getInstance() {
			if (instance == null) {
				instance = new ServiceClient();
			}
			return instance;
		}
		
		public synchronized CommandResponse sendCommand(CommandRequest request) {
			try {
				return client_.SendCommand(request);
			} catch (TException e) {
				e.printStackTrace();
			}
			CommandResponse response = new CommandResponse();
			response.setStatus(CommandStatus.ERROR);
			return response; 
		}
	}

	@Override
	public CommandResponse SendCommand(CommandRequest request) {
		return ServiceClient.getInstance().sendCommand(request);
	}
}
