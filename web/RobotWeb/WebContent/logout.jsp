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
<%@ page import="org.apache.thrift.TException" %>
<%@ page import="lib.robotics.rcs.web.UserServiceHandler" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%
	UserServiceHandler user_service = (UserServiceHandler) session.getAttribute("UserServiceHandler");
	if (user_service != null && user_service.isOpen()) {
		if (user_service.isLoggedIn()) {
			try {
				user_service.Logout();
			} catch (TException e) {
				e.printStackTrace();
			}
		}
		user_service.close();
	}
 %>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>RCS Logout</title>
</head>
<body>
	<br/>
	<p align="center">You have successfully logged out.</p>
	<p align="center"><a href="login.html">Login again</a></p>
</body>
</html>