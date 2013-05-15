<%-- Robot Control System
     Copyright (C) 2013 Tuna Oezer
 
     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU Affero General Public License as
     published by the Free Software Foundation, either version 3 of the
     License, or (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU Affero General Public License for more details.

     You should have received a copy of the GNU Affero General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 --%>
 
<?xml version="1.0" encoding="UTF-8" ?>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="lib.robotics.rcs.server.RobotStatus" %>
<%@ page import="lib.robotics.rcs.web.RobotControlServiceHandler" %>
<%@ page import="lib.robotics.rcs.web.UserServiceHandler" %>
<%
	String user_agent = request.getHeader("User-Agent");
	boolean is_mobile = user_agent != null && user_agent.contains("RcsMobileApp");
	String action = request.getParameter("action");
	if (action == null) action = "stop";
	String robot_id = request.getParameter("robot_id");
	String robot_name = request.getParameter("robot_name");
	String username = null;
	UserServiceHandler user_service = null;
	if (!session.isNew()) {
		username = (String) session.getAttribute("username");
		user_service = (UserServiceHandler) session.getAttribute("UserServiceHandler");
	}
	boolean logged_in = username != null && user_service != null && user_service.isLoggedIn();
	boolean start = logged_in && "start".equals(action);
	boolean stop = logged_in && "stop".equals(action);
	if (start) {
		user_service.SetRobotStatus(robot_id, RobotStatus.ONLINE);
		RobotControlServiceHandler robot_service = new RobotControlServiceHandler(robot_id);
		robot_service.startService();
		session.setAttribute("RobotControlServiceHandler", robot_service);
	} else if (stop) {
		RobotControlServiceHandler robot_service =
				(RobotControlServiceHandler) session.getAttribute("RobotControlServiceHandler");
		if (robot_service != null) {
			robot_service.stopService();
			session.removeAttribute("RobotControlServiceHandler");
			user_service.SetRobotStatus(robot_service.getRobotId(), RobotStatus.OFFLINE);
			robot_service = null;
		}
	}
 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <title>RCS Robot</title>
  <% if (is_mobile) { %>
	  <script type="text/javascript" src="js/cordova-2.7.0.js"></script>
	  <script type="text/javascript" src="js/webSocket.min.js"></script>
          <script type="text/javascript" src="js/bluetooth.js"></script>
	  <script type="text/javascript">
	  	var WebSocket = plugins.WebSocket;
	  </script>
  <% } %>
  <script type="text/javascript" src="js/autobahn.min.js"></script>
  <script type="text/javascript" src="js/user_service_types.js"></script>
  <script type="text/javascript" src="js/robot.js"></script>
  <script type="text/javascript">
  	var g_robot_id = "<%= robot_id %>";
  
    function loadPage() {
    	initialize(g_robot_id);
    }
    
    function unloadPage() {
    	uninitialize();
    }
  </script>
</head>
<body onload="loadPage()" onunload="unloadPage()">
	<% if (logged_in) { %>
	   	<table bgcolor="steelblue" width="100%">
	   		<tr>
	   			<td><font color="white" size="6">Welcome <%= username %></font></td>
	   			<td align="right">
	   				<input type="button" value="Return" style="height: 40px;width:120px" onclick="window.location = 'home.jsp'" />
	  				</td>
	   			<td align="right" width="180px">
	   				<form action="logout.jsp" method="get">
	   					<input type="submit" value="Logout" style="height: 40px;width:120px" />
	   				</form>
	  				</td>
	   		</tr>
	   	</table>
		<hr />
	<% 		if (start) { %>
		<h3><%= robot_name %> has started.</h3>
		<div id="message_box"></div>
	<%
			} else if (stop) {
	 %>
		<h3><%= robot_name %> has stopped.</h3>
	<%
	   		}
		} else {
	   		response.sendRedirect("login.html");
		}
	 %>
</body>
</html>