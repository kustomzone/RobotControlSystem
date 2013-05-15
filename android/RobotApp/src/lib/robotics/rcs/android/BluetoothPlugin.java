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

package lib.robotics.rcs.android;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Set;
import java.util.UUID;

import org.apache.cordova.CordovaArgs;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.api.CallbackContext;
import org.apache.cordova.api.CordovaInterface;
import org.apache.cordova.api.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

/**
 * @author Tuna Oezer
 *
 * Cordova Bluetooth plugin. Allows enumerating and connecting to Bluetooth devices at
 * the client through JavaScript. The plugin can send Bluetooth messages to connected
 * devices. Only one Bluetooth connection can be open at a time.
 * 
 * Tested with Cordova 2.7.0.
 */
public class BluetoothPlugin extends CordovaPlugin {
	private static final String k_isBluetoothEnabled = "isBluetoothEnabled";
	private static final String k_listBluetoothDevices = "listBluetoothDevices";
	private static final String k_open = "open";
	private static final String k_isOpen = "isOpen";
	private static final String k_close = "close";
	private static final String k_send = "send";
	
	private BluetoothAdapter bt_adapter_;
	private BluetoothSocket bt_socket_;
	private OutputStreamWriter output_;
	
	@Override
	public void initialize(CordovaInterface cordova, CordovaWebView web_view) {
		super.initialize(cordova, web_view);
		bt_adapter_ = BluetoothAdapter.getDefaultAdapter();
		bt_socket_ = null;
		output_ = null;
	}
	
	@Override
	public boolean execute(String action, CordovaArgs args, CallbackContext callback)
			throws JSONException {
		if (k_isBluetoothEnabled.equals(action)) {
			callback.success(isBluetoothEnabled() ? 1 : 0);
		} else if (k_listBluetoothDevices.equals(action)) {
			callback.success(listBluetoothDevices());
		} else if (k_open.equals(action)) {
			if (open(args.getString(0), args.getString(1))) {
				callback.success();
			} else {
				callback.error("Failed to open bluetooth connection.");
			}
		} else if (k_isOpen.equals(action)) {
			callback.success(isOpen() ? 1 : 0);
		} else if (k_close.equals(action)) {
			close();
			callback.success();
		} else if (k_send.equals(action)) {
			if (send(args.getString(0))) {
				callback.success();
			} else {
				callback.error("Transmission error.");
			}
		} else {
			return false;
		}
		return true;
	}
	
	/**
	 * @return If Bluetooth is present and enabled on the client machine.
	 */
	public boolean isBluetoothEnabled() {
		return bt_adapter_ != null;
	}
	
	/**
	 * @return List of all paired Bluetooth devices.
	 */
	public JSONArray listBluetoothDevices() {
		JSONArray result = new JSONArray();
		if (bt_adapter_ != null) {
			Set<BluetoothDevice> bt_devices = bt_adapter_.getBondedDevices();
			for (BluetoothDevice bt_device : bt_devices) {
				JSONObject item = new JSONObject();
				try {
					item.put("name", bt_device.getName());
					item.put("address", bt_device.getAddress());
				} catch (JSONException e) {
					e.printStackTrace();
				}
				result.put(item);
			}
		}
		return result;
	}
	
	/**
	 * Opens a connection to a Bluetooth device.
	 * @param address MAC address of Bluetooth device.
	 * @param uuid UUID of Bluetooth service to connect to.
	 * @return 1 if successful.
	 */
	public boolean open(String address, String uuid) {
		if (bt_adapter_ == null) return false;
		if (bt_socket_ != null) return false;
		BluetoothDevice bt_device = bt_adapter_.getRemoteDevice(address);
		try {
			bt_socket_ = bt_device.createRfcommSocketToServiceRecord(UUID.fromString(uuid));
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		try {
			bt_socket_.connect();
		} catch (IOException e) {
			e.printStackTrace();
			bt_socket_ = null;
			return false;
		}
		try {
			output_ = new OutputStreamWriter(bt_socket_.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
			output_ = null;
			try {
				bt_socket_.close();
			} catch (IOException inner_e) {
				inner_e.printStackTrace();
			}
			bt_socket_ = null;
			return false;
		}
		return true;
	}
	
	/**
	 * @return True if a connection is currently open.
	 */
	public boolean isOpen() {
		return output_ != null;
	}
	
	/**
	 * Closes the currently open connection.
	 */
	public void close() {
		if (bt_socket_ == null) return;
		try {
			bt_socket_.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		output_ = null;
		bt_socket_ = null;
	}
	
	/**
	 * Sends the specified data to the Bluetooth device.
	 * @param data Data to transtmit.
	 * @return True if the data was successully transmitted.
	 */
	public boolean send(String data) {
		try {
			output_.write(data);
			output_.flush();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
}
