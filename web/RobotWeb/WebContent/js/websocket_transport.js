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

var ws_message_handler = null;
var ws_message_data = "";
var thrift_method_name = "";

// Set a handle that will be called when messages from the server are received.
function setWebSocketMessageHandler(message_handler) {
	ws_message_handler = message_handler;
}

// Implements a WebSocket transport for Thrift.
WebSocketTransport = function(server_address) {
	this.server_address_ = server_address;
	this.socket_ = null;
};

WebSocketTransport.prototype = {
  open: function() {
	  socket_ = new WebSocket("ws://" + this.server_address_);
	  socket_.onopen = this.onOpen;
	  socket_.onerror = this.onError;
	  socket_.onmessage = this.onMessage;
  },

  close: function() {
	  socket.close();
  },
  
  onOpen: function() {
	  showResponse("connection opened");
  },
  
  onError: function(error) {
	  showResponse("error: " + error);
  },
  
  onMessage: function(message) {
	  ws_message_data = message.data;
          thrift_method_name = ws_message_data.substr(4);
          thrift_method_name = thrift_method_name.substr(0, thrift_method_name.indexOf("\""));
	  if (ws_message_handler) ws_message_handler();
  },
		
  flush: function(async) {
	  return ws_message_data;
  },

  write: function(buf) {
	  socket_.send(buf);
  },

  readAll: function() {
	  return ws_message_data;
  }
};
