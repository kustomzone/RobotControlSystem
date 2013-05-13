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

/**
 * @author Tuna Oezer
 *
 * Handles returns from a WAMP RPC call.
 */
public class WampRpcReturn {
	private WampHandler handler_;
	private String call_id_;
	
	public WampRpcReturn(WampHandler handler, String call_id) {
		this.handler_ = handler;
		this.call_id_ = call_id;
	}
	
	/**
	 * Returns an RPC call.
	 * @param result The result of the RPC call or null if the call does not return a result.
	 */
	public void returnRpc(Object result) {
		handler_.returnRpcResult(call_id_, result);
	}
	
	/**
	 * Throws an RPC error to the client.
	 * @param error_uri The URI of the error. 
	 * @param description Human readable description.
	 */
	public void throwError(String error_uri, String description) {
		handler_.returnRpcError(call_id_, error_uri, description, null);
	}
	
	/**
	 * Throws an RPC error to the client with application specific error data.
	 * @param error_uri The URI of the error. 
	 * @param description Human readable description.
	 * @param error_data Additional data related to the error.
	 */
	public void throwError(String error_uri, String description, Object error_data) {
		handler_.returnRpcError(call_id_, error_uri, description, error_data);
	}
}
