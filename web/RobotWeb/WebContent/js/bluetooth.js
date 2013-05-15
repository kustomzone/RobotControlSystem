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

function showError(error) {
	alert("Error: " + error);
}

// Allows accessing and communicating with Bluetooth devices connected to the client machine.
// Each Bluetooth object can connect to one Bluetooth device at a time.
var Bluetooth = function() {};
Bluetooth.prototype = {
    kBluetoothPlugin : "BluetoothPlugin",
		
    // Returns 1 via the callback if the client device has Bluetooth.
    isBluetoothEnabled : function(callback) {
	cordova.exec(
	    callback,  function(error) {}, this.kBluetoothPlugin, "isBluetoothEnabled", []);
    },

    // Returns an array of all Bluetooth devices that have been paired with the client machine.
    // For each device, returns the name and address of the device.
    // Specifically, devices[i].name is the name of the device and devices[i].address is the address
    // where devices is the argument passed to the callback.
    listBluetoothDevices : function(callback) {
	cordova.exec(
	    callback,  function(error) {}, this.kBluetoothPlugin, "listBluetoothDevices", []);
    },

    // Opens a communication to the Bluetooth device with the specified address and connects to the
    // service with the specified UUID. Returns 1 to the callback if the connection was successfully
    // opened.
    open : function(address, uuid, callback) {
	cordova.exec(callback,  showError, this.kBluetoothPlugin, "open", [address, uuid]);
    },

    // Returns true if a connection is currently open.
    isOpen : function(callback) {
	cordova.exec(callback,  function(error) {}, this.kBluetoothPlugin, "isOpen", []);
    },

    // Closes the connection to the Bluetooth device if it is currently open.
    close : function() {
	cordova.exec(
	    function(result) {}, function(error) {}, this.kBluetoothPlugin, "close", []);
    },

    // Sends the specified data to the Bluetooth device. The connection must be open.
    send : function(data) {
	cordova.exec(function(result) {},  showError, this.kBluetoothPlugin, "send", [data]);
    },
};
