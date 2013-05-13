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

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import lib.robotics.rcs.server.UserLoginRequest;

import org.apache.thrift.TException;

/**
 * @author Tuna Oezer
 * 
 * HTTP Servlet that handles user logins.
 */
@WebServlet(name="LoginServlet",
			description="Handles user logins.",
			urlPatterns={"/dologin"},
			asyncSupported=true)
public class LoginServlet extends HttpServlet {
	private static final long serialVersionUID = 8443152281086140584L;

	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) {
		HttpSession session = request.getSession();
		UserServiceHandler user_service =
				(UserServiceHandler) session.getAttribute("UserServiceHandler");
		if (user_service != null && user_service.isOpen()) {
			user_service.close();
		}
		String username = request.getParameter("username");
		String password = request.getParameter("password");
		try {
			if (username != null || password != null) {
				user_service = new UserServiceHandler();
				if (user_service.open()) {
					UserLoginRequest login_request = new UserLoginRequest();
					login_request.setUsername(username);
					login_request.setPassword(password);
					try {
						if (user_service.Login(login_request)) {
							session.setAttribute("username", username);
							session.setAttribute(
									UserServiceHandler.kUserServiceHandler, user_service);
							response.sendRedirect("home.jsp");
							return;
						}
					} catch (TException e) {
						e.printStackTrace();
					}
					user_service.close();
				}
			}
			
			// Error handling.
			PrintWriter out = response.getWriter();
			response.setContentType("text/html");
			out.write("<html>" +
					  "<head><title>Login Error</title></head>" +
					  "<body>" +
					  "<p align='center'>Login error.</p>" +
					  "<p align='center'><a href='login.html'>Login again</a></p>" +
					  "</body>" +
					  "</html>");
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
