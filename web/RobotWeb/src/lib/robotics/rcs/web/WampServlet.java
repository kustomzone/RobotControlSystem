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

import java.util.List;

import org.apache.catalina.websocket.WebSocketServlet;

/**
 * @author Tuna Oezer
 *
 * Base class for WAMP WebServlets.
 */
public abstract class WampServlet extends WebSocketServlet {
	private static final long serialVersionUID = 1L;
	private static final String kWAMP = "WAMP";

	@Override
	protected String selectSubProtocol(List<String> sub_protocols) {
		for (String sub_protocol : sub_protocols) {
			if (kWAMP.equalsIgnoreCase(sub_protocol)) {
				return sub_protocol;
			}
		}
		return null;
	}
}
