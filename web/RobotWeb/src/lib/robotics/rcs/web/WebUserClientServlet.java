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

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;

import org.apache.catalina.websocket.StreamInbound;
import org.apache.catalina.websocket.WebSocketServlet;

/**
 * @author Tuna Oezer
 * 
 * Web Servlet that responds to requests initiated via a user over a WebSocket connection.
 */
@WebServlet(name="WebUserClientServlet",
		    description="Responds to control requests over WebSocket.",
		    urlPatterns={"/user_service"},
		    asyncSupported=true)
public class WebUserClientServlet extends WebSocketServlet {
	private static final long serialVersionUID = 8976837269798223716L;

	@Override
	protected StreamInbound createWebSocketInbound(String subProtocol,
                                                   HttpServletRequest request) {
		return new WebUserClientHandler();
	}
}
