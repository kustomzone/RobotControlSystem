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

<?xml version="1.0" encoding="ISO-8859-1" ?>
<%@ page language="java" contentType="text/html; charset=ISO-8859-1" pageEncoding="ISO-8859-1"%>
<%@ page import="java.util.List" %>
<%@ page import="org.apache.thrift.TException" %>
<%@ page import="lib.robotics.rcs.server.AccessDeniedException" %>
<%@ page import="lib.robotics.rcs.server.RobotInfo" %>
<%@ page import="lib.robotics.rcs.server.RobotStatus" %>
<%@ page import="lib.robotics.rcs.web.UserServiceHandler" %>
<%
	String username = null;
	UserServiceHandler user_service = null;
	if (!session.isNew()) {
		username = (String) session.getAttribute("username");
		user_service = (UserServiceHandler) session.getAttribute("UserServiceHandler");
	}
 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
	<title>RCS Home</title>
	<style>
		a { color: white; }
	</style>
</head>
<body>
    <% if (username != null && user_service != null && user_service.isLoggedIn()) { %>
    	<table bgcolor="steelblue" width="100%">
    		<tr>
    			<td><font color="white" size="6">Welcome <%= username %></font></td>
    			<td align="right">
    				<form action="logout.jsp" method="get">
    					<input type="submit" value="Logout" style="height: 40px;width:120px" />
    				</form>
   				</td>
    		</tr>
    	</table>
		<hr />
		<table width="500px">
			<tr align="center">
				<td bgcolor="black"><font color="white"><b>Robot</b></font></td>
				<td bgcolor="black"><font color="white"><b>State</b></font></td>
			</tr>
	<%
			try {
				List<RobotInfo> robots = user_service.ListRobots().getRobots();
				for (RobotInfo robot : robots) {
					if (robot.getRobot_status() == RobotStatus.ONLINE) {
     %>
     				<tr bgcolor="green" align="center" onmouseover="this.bgColor='lime'" onmouseout="this.bgColor='green'">
     					<td>
   							<a title="Connect" href="control.jsp?robot_id=<%= robot.getRobot_id() %>&robot_name=<%= robot.getRobot_name() %>">
								<%= robot.getRobot_name() %>
							</a>
						</td>
     					<td><font color="white">online</font></td>
     				</tr>
 				<% } else { %>
     				<tr bgcolor="lightgray" align="center">
     					<td><%= robot.getRobot_name() %></td>
     					<td>offline</td>
     				</tr>
	<%
     				}
				}
			} catch (AccessDeniedException e) {
			} catch (TException e) {
			}
	  %>
	 	</table>
	<%
    	} else {
    		response.sendRedirect("login.html");
		}
	 %>
</body>
</html>