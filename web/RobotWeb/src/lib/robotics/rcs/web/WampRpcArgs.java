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

import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * @author Tuna Oezer
 * 
 * Holds arguments of a WAMP RPC call.
 * WampRpcArgs allows converting the arguments into an instance of a user defined class.
 */
public class WampRpcArgs {
	/* Offset to the first actual RPC method argument.
	 * The first few argument entries contain system information. 
	 */
	private static final int kArgOffset = 3;
	
	private ObjectMapper json_processor_;
	private Object[] args_;
	
	public WampRpcArgs(ObjectMapper json_processor, Object[] args) {
		this.json_processor_ = json_processor;
		this.args_ = args;
	}
	
	/**
	 * @return Number of arguments.
	 */
	public int size() {
		return args_.length - kArgOffset;
	}
	
	/**
	 * Converts the argument at the specified index to the specified type and returns the result.
	 * @param index Zero-based argument index.
	 * @param type Type of the argument.
	 * @return The argument after conversion to the specified type.
	 */
	public <T> T getArg(int index, Class<T> type) {
		return json_processor_.convertValue(args_[index + kArgOffset], type);
	}
}
