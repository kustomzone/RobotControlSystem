<?xml version="1.0" encoding="UTF-8" ?>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="lib.robotics.rcs.server.RobotStatus" %>
<%@ page import="lib.robotics.rcs.web.RobotControlServiceHandler" %>
<%@ page import="lib.robotics.rcs.web.UserServiceHandler" %>
<%
  RobotControlServiceHandler robot_service =
  (RobotControlServiceHandler) session.getAttribute("RobotControlServiceHandler");
  if (robot_service != null) {
	  robot_service.stopService();
	  session.removeAttribute("RobotControlServiceHandler");
	  robot_service = null;
  }
  UserServiceHandler user_service =
	(UserServiceHandler) session.getAttribute("UserServiceHandler");
  if (user_service != null) {
  	user_service.SetRobotStatus(robot_service.getRobotId(), RobotStatus.OFFLINE);
  }
  response.sendRedirect("home.jsp");
 %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
</html>