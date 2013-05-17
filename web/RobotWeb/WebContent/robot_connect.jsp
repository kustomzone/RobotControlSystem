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
<%
	String username = (String) session.getAttribute("username");
	String robot_id = request.getParameter("robot_id");
	String robot_name = request.getParameter("robot_name");
 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	 <style>
	   html { height: 100%; }
	   body { height: 90%; }
	   .even {
	     background-color: yellow;
	     cursor: pointer;
	     text-align: center;
	   }
	   .odd {
	     background-color: orange;
	     cursor: pointer;
	     text-align: center;
	   }
	 </style>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<title>RCS Robot</title>
	<script type="text/javascript" src="js/cordova-2.7.0.js"></script>
    <script type="text/javascript" src="js/bluetooth.js"></script>
    <script type="text/javascript">
    	function fillTable(devices) {
			var table = document.getElementById("device_table");
    		for (var i = 0; i < devices.length; i++) {
    			var row = document.createElement("tr");
    			row.className = i % 2 == 0 ? "even" : "odd";
    			row.onclick = function() {
    				window.location = "robot.jsp?" +
    						"robot_id=<%= robot_id %>&" +
    						"robot_name=<%= robot_name %>&" +
    						"bt_address=" + devices[i].address;
    			};
    			table.appendChild(row);
    			var cell = document.createElement("td");
    			cell.textContent = devices[i].name;
    			row.appendChild(cell);
    			cell = document.createElement("td");
    			cell.textContent = devices[i].address;
    			row.appendChild(cell);
    		}
    	}
    
    	function listDevices() {
    		bt = new Bluetooth();
    		bt.listBluetoothDevices(fillTable);
    	}
    </script>
</head>
<body onload="listDevices()">
   	<table bgcolor="steelblue" width="100%">
   		<tr>
   			<td>
   				<font color="white" size="6">
   					<%= username %>: <%= robot_name %>
   				</font>
   			</td>
   			<td align="right">
   				<form action="home.jsp" method="get">
   					<input type="submit" value="Home" style="height: 40px;width:120px" />
   				</form>
  			</td>
   		</tr>
   	</table>
	<hr />
	<p align="center">Select a Bluetooth device:</p>
	<table id="device_table" align="center">
	  <tr align="center">
	    <td bgcolor="black" width="50%"><font color="white"><b>Name</b></font></td>
	    <td bgcolor="black" width="50%"><font color="white"><b>Address</b></font></td>
	  </tr>
	</table>
</body>
</html>