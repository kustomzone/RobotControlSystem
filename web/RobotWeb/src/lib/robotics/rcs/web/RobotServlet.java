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
import javax.servlet.http.HttpSession;

import org.apache.catalina.websocket.StreamInbound;

/**
 * @author Tuna Oezer
 *
 * Web Servlet that dispatches control events to robots.
 */
@WebServlet(name="RobotServlet",
			description="Dispatches control requests over WebSocket.",
			urlPatterns={"/robot_service"},
			asyncSupported=true)
public class RobotServlet extends WampServlet {
	private static final long serialVersionUID = -7323276455985060800L;

	@Override
	protected StreamInbound createWebSocketInbound(String subProtocol,
                                                   HttpServletRequest request) {
		HttpSession session = request.getSession();
		UserServiceHandler service_handler =
				(UserServiceHandler) session.getAttribute(UserServiceHandler.kUserServiceHandler);
		if (service_handler == null || !service_handler.isLoggedIn()) {
			return null;  // Not logged in.
		}
		return new WampHandler(session);
	}
}
