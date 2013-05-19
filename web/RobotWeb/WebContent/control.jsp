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
  <link rel="stylesheet" href="http://code.jquery.com/ui/1.10.3/themes/smoothness/jquery-ui.css" />
  <link href="http://anthonyterrien.com/css/layout.css" rel="stylesheet">
  <style type="text/css">
    .ui-slider .ui-slider-handle {
      width: 150px;
    }
  </style>
  <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
  <title>RCS Robot Control</title>
  <script src="http://code.jquery.com/jquery-1.9.1.js"></script>
  <script src="http://code.jquery.com/ui/1.10.3/jquery-ui.js"></script>
  <script src="http://anthonyterrien.com/js/jquery.knob.js"></script>
  <script type="text/javascript" src="js/autobahn.min.js"></script>
  <script type="text/javascript" src="js/user_service_types.js"></script>
  <script type="text/javascript" src="js/control.js"></script>
  <script type="text/javascript">
  	var g_username = "<%= username %>";
  	var g_robot_id = "<%= robot_id %>";
  
    $(document).ready(function () {
    	initialize();

        $("#speed_slider").slider({
            orientation: "vertical",
            range: "min",
            min: -100,
            max: 100,
            value: 0,
            animate: "slow",
            slide: function(event, ui) {
              $("#speed_amount").val(ui.value);
              if (ui.value > 50) {
            	  sendCommand("MOVE_FORWARD");
              } else if (ui.value < -50) {
            	  sendCommand("MOVE_BACKWARD");
              } else {
            	  sendCommand("STOP");
              }
            }
          });
        $("#speed_amount").val($("#speed_slider").slider("value"));

        $(".knob").knob({
            change : function (value) {
            	if (value < 45) {
            		sendCommand("STOP");
            	} else if (value < 180) {
            		sendCommand("TURN_RIGHT");
            	} else if (value < 315) {
            		sendCommand("TURN_LEFT");
            	} else {
            		sendCommand("STOP");
            	}
            },
            draw : function () {
              // "tron" case
              if(this.$.data('skin') == 'tron') {
                var a = this.angle(this.cv),  // Angle
                    sa = this.startAngle,     // Previous start angle
                    sat = this.startAngle,    // Start angle
                    ea,                       // Previous end angle
                    eat = sat + a,            // End angle
                    r = true;

                    this.g.lineWidth = this.lineWidth;

                    this.o.cursor
                         && (sat = eat - 0.3)
                         && (eat = eat + 0.3);

                    if (this.o.displayPrevious) {
                      ea = this.startAngle + this.angle(this.value);
                      this.o.cursor
                        && (sa = ea - 0.3)
                        && (ea = ea + 0.3);
                      this.g.beginPath();
                      this.g.strokeStyle = this.previousColor;
                      this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sa, ea, false);
                      this.g.stroke();
                    }

                    this.g.beginPath();
                    this.g.strokeStyle = r ? this.o.fgColor : this.fgColor ;
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sat, eat, false);
                    this.g.stroke();

                    this.g.lineWidth = 2;
                    this.g.beginPath();
                    this.g.strokeStyle = this.o.fgColor;
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth + 1 + this.lineWidth * 2 / 3, 0, 2 * Math.PI, false);
                    this.g.stroke();

                    return false;
              }
            }
        });
    });

    $(window).unload(function() {
    	uninitialize();
    });

    function onStop() {
      $("#speed_slider").slider({value: 0});
      $("#speed_amount").val($("#speed_slider").slider("value"));
      sendCommand("STOP");
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
<body onkeydown="onKeyDown(event)">
	<% if (username != null && user_service != null && user_service.isLoggedIn()) { %>
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
        <h3>Controlling <%= robot_name %></h3>
	  <br/>
		  <table width="90%" cellpadding="50px">
		    <tr>
		      <td width="35%" align="center">
		        <p>
		          <label for="direction_knob">Direction</label>
		          <br/><br/>
		          <input id="direction_knob"
		               class="knob"
		               data-width="200"
		               data-cursor="true"
		               data-fgColor="#222222"
		               data-thickness=".6"
		               data-max="359"
		               data-displayPrevious="true"
		               value="0"/>
		        </p>
		      </td>
		      <td width="40%">
		        <p>
		          <label for="speed_amount">Speed:</label>
		          <input type="text" id="speed_amount" style="border: 0; color: #f6931f; font-weight: bold;" />
		        </p>
		        <br/>
		        <div id="speed_slider" style="width:150px;height:250px;"></div>
		      </td>
		      <td width="25%" align="center">
		        <button style="background-color:red;width:90%;height:60px;" onclick="onStop()">STOP</button>
		      </td>
		    </tr>
		  </table>
      <p>
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
