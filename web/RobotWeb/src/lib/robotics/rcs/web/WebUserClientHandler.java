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

import javax.servlet.http.HttpSession;

import lib.robotics.rcs.server.CommandRequest;

/**
 * @author Tuna Oezer
 * 
 * Handles control requests initiated via a user over a WebSocket connection.
 */
public class WebUserClientHandler extends WampHandler {

	private static final String kSendCommandRpcUri =
			"rpc://lib.robotics.rcs.server.UserService/SendCommand"; 
	
	private UserServiceHandler service_handler_;
	
	public WebUserClientHandler(HttpSession session, UserServiceHandler service_handler) {
		super(session);
		if (service_handler == null)
			throw new AssertionError("UserServiceHandler must not be null.");
		this.service_handler_ = service_handler;
		
		registerRpcMethod(kSendCommandRpcUri, new WampRpcMethod() {
			@Override
			public void run(WampRpcArgs args, WampRpcReturn callback) {
				if (args.size() != 1) {
					callback.throwError(kSendCommandRpcUri + "/error/incorrect_args",
										"Incorrect number of arguments.");
					return;
				}
				callback.returnRpc(
						service_handler_.SendCommand(args.getArg(0, CommandRequest.class)));
			}
		});
	}
}
