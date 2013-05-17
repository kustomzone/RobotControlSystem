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
	String robot_id = request.getParameter("robot_id");
	String robot_name = request.getParameter("robot_name");
	String bt_address = request.getParameter("bt_address");
	String username = null;
	UserServiceHandler user_service = null;
	username = (String) session.getAttribute("username");
	user_service = (UserServiceHandler) session.getAttribute("UserServiceHandler");
	boolean logged_in = username != null && user_service != null && user_service.isLoggedIn();
	if (logged_in) {
		user_service.SetRobotStatus(robot_id, RobotStatus.ONLINE);
		RobotControlServiceHandler robot_service = new RobotControlServiceHandler(robot_id);
		robot_service.startService();
		session.setAttribute("RobotControlServiceHandler", robot_service);
	}
 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <title>RCS Robot</title>
  <script type="text/javascript" src="js/cordova-2.7.0.js"></script>
  <script type="text/javascript" src="js/webSocket.min.js"></script>
  <script type="text/javascript">
  	var WebSocket = plugins.WebSocket;
  </script>
  <script type="text/javascript" src="js/autobahn.min.js"></script>
  <script type="text/javascript" src="js/user_service_types.js"></script>
  <script type="text/javascript" src="js/bluetooth.js"></script>
  <script type="text/javascript" src="js/robot.js"></script>
  <script type="text/javascript">
    function loadPage() {
    	initialize("<%= robot_id %>");
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
	   			<td>
	   				<font color="white" size="6">
	   					<%= username %>: <%= robot_name %>
	   				</font>
	   			</td>
	   			<td align="right">
	   				<form action="robot_disconnect.jsp" method="get">
	   					<input type="submit" value="Home" style="height:40px;width:120px" />
	   				</form>
	  			</td>
	   		</tr>
	   	</table>
		<hr />
		<p><%= robot_name %> is running.</p> 
	    <p style="margin-left:5%">Activity log:</p>
	    <textarea id="log" style="margin-left:5%;height:70%;width:90%" readonly="readonly">
	    </textarea>
	<%
		} else {
	   		response.sendRedirect("login.html");
		}
	 %>
</body>
</html>