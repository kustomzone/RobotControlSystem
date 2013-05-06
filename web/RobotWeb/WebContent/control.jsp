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
<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ page import="lib.robotics.rcs.web.UserServiceHandler" %>
<%
	String robot_id = request.getParameter("robot_id");
	String robot_name = request.getParameter("robot_name");
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
  <title>RCS Robot Control</title>
  <script type="text/javascript" src="js/thrift.js"></script>
  <script type="text/javascript" src="js/websocket_transport.js"></script>
  <script type="text/javascript" src="js/user_service_types.js"></script>
  <script type="text/javascript" src="js/UserService.js"></script>
  <script type="text/javascript" src="js/sha1.js"></script>
  <script type="text/javascript" src="js/control.js"></script>
  <script type="text/javascript">
  	var g_username = "<%= username %>";
  	var g_robot_id = "<%= robot_id %>";
  
    function loadPage() {
    	initUserService();
    }
    
    function unloadPage() {
    	shutDownUserService();
    }
    
    function onKeyDown(event) {
    	var e = window.event ? window.event : event;
    	switch (e.keyCode) {
    	case 88: sendCommand("STOP"); break;
    	case 38:
    	case 87: sendCommand("MOVE_FORWARD"); break;
    	case 37:
    	case 65: sendCommand("TURN_LEFT"); break;
    	case 40:
    	case 83: sendCommand("MOVE_BACKWARD"); break;
    	case 39:
    	case 68: sendCommand("TURN_RIGHT"); break;
    	case 33:
    	case 81: sendCommand("TILT_UP"); break;
    	case 34:
    	case 69: sendCommand("TILT_DOWN"); break;
    	}
    }
  </script>
</head>
<body onload="loadPage()" onunload="unloadPage()" onkeydown="onKeyDown(event)">
	<% if (username != null && user_service != null && user_service.isLoggedIn()) { %>
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
      <h3>Controlling <%= robot_name %></h3>
	  <table border="0" cellpadding="16px">
	    <tr>
	      <td>
	      </td>
	      <td>
	        <img title="Forward"
				 src="img/up_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/up_2.jpeg'"
				 onmouseout="this.src='img/up_1.jpeg'"
				 onclick="sendCommand('MOVE_FORWARD')" />
	      </td>
	      <td></td>
	      <td>
	        <img title="Tilt Up"
				 src="img/up_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/up_2.jpeg'"
				 onmouseout="this.src='img/up_1.jpeg'"
				 onclick="sendCommand('TILT_UP')" />
	      </td>
	    </tr>
	    <tr>
	      <td>
	        <img title="Left"
				 src="img/left_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/left_2.jpeg'"
				 onmouseout="this.src='img/left_1.jpeg'"
				 onclick="sendCommand('TURN_LEFT')" />
	      </td>
	      <td>
	        <img title="Stop"
				 src="img/stop_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/stop_2.jpeg'"
				 onmouseout="this.src='img/stop_1.jpeg'"
				 onclick="sendCommand('STOP')" />
	      </td>
	      <td>
	        <img title="Right"
				 src="img/right_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/right_2.jpeg'"
				 onmouseout="this.src='img/right_1.jpeg'"
				 onclick="sendCommand('TURN_RIGHT')" />
	      </td>
	      <td></td>
	    </tr>
	    <tr>
	      <td></td>
	      <td>
	        <img title="Backward"
				 src="img/down_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/down_2.jpeg'"
				 onmouseout="this.src='img/down_1.jpeg'"
				 onclick="sendCommand('MOVE_BACKWARD')" />
	      </td>
	      <td></td>
	      <td>
	        <img title="Tilt Down"
				 src="img/down_1.jpeg"
				 style="cursor: pointer"
				 onmouseover="this.src='img/down_2.jpeg'"
				 onmouseout="this.src='img/down_1.jpeg'"
				 onclick="sendCommand('TILT_DOWN')" />
	      </td>
	    </tr>
	  </table>
	  <br/>
      <p>
      	 Click the buttons above or use keyboard to control the robot.<br/>
         Keyboard: Use arrow keys or W (up), A (left), S (back), D (right), X (stop), Q (tilt up), E (tilt down).
      </p>
	  <p id="message_box"></p>
	<%
	   	} else {
	   		response.sendRedirect("login.html");
		}
	 %>
</body>
</html>