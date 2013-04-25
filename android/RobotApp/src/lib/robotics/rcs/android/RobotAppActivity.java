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
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.Set;
import java.util.UUID;

import com.tunaoezer.rcs.android.R;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.StrictMode;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.ToggleButton;

/**
 * @author Tuna Oezer
 * 
 * Main Android activity.
 */
public class RobotAppActivity extends Activity {
	/**
	 *  Handles clicks of the RCS connection button.
	 */
	private class ConnectButtonListener implements CompoundButton.OnCheckedChangeListener {
		@Override
		public void onCheckedChanged(CompoundButton buttonView,
								     boolean isChecked) {
			if (isChecked) {
				connectToServer();
			} else {
				disconnectFromServer();
			}
		}
	}
	
	/**
	 * Handles clicks of the robot connection button.
	 */
	private class ArduinoButtonListener implements CompoundButton.OnCheckedChangeListener {
		@Override
		public void onCheckedChanged(CompoundButton buttonView,
								     boolean isChecked) {
			if (isChecked) {
				connectToArduino();
			} else {
				disconnectFromArduino();
			}
		}
	}
	
	/**
	 * UI handler for updating the response text.
	 */
	private static class ResponseHandler extends Handler {
		
		private EditText response_text_; 
		
		public ResponseHandler(Looper looper, EditText response_text) {
			super(looper);
			this.response_text_ = response_text;
		}
		
		@Override
		public void handleMessage(Message inputMessage) {
			response_text_.setText((String) inputMessage.obj);
		}
	}
	
	private Robot robot_;
	private RobotServiceClient service_client_;
	private RobotControlServiceHandler control_handler_;
	private ResponseHandler handler_;
	private BluetoothSocket bt_socket_;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_robot_app);
		
		StrictMode.setThreadPolicy(StrictMode.ThreadPolicy.LAX);
		
		EditText response_text = (EditText) findViewById(R.id.console_text);

		handler_ = new ResponseHandler(Looper.getMainLooper(), response_text);
		robot_ = new Robot("r1", handler_);

		ToggleButton toggle_button = (ToggleButton) findViewById(R.id.toggleButton_connect);
		toggle_button.setOnCheckedChangeListener(new ConnectButtonListener());
		
		toggle_button = (ToggleButton) findViewById(R.id.toggleButton_bt);
		toggle_button.setOnCheckedChangeListener(new ArduinoButtonListener());
		
		Button button = (Button) findViewById(R.id.button_forward);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.forward();
			}
		});
		button = (Button) findViewById(R.id.button_back);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.backward();
			}
		});
		button = (Button) findViewById(R.id.button_left);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.turn_left();
			}
		});
		button = (Button) findViewById(R.id.button_right);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.turn_right();
			}
		});
		button = (Button) findViewById(R.id.button_stop);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.stop();
			}
		});
		button = (Button) findViewById(R.id.button_tilt_up);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.tilt_up();
			}
		});
		button = (Button) findViewById(R.id.button_tilt_down);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				robot_.tilt_down();
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.robot_app, menu);
		return true;
	}

	/**
	 * Connects to the RCS server.
	 */
	private void connectToServer() {
		// Default server address.
		String server_address = "ec2-54-245-59-56.us-west-2.compute.amazonaws.com";
		
		EditText login_text = (EditText) findViewById(R.id.login_text);
		if (login_text.getText().length() > 0) {
			String text = login_text.getText().toString();
			int i = text.indexOf('@');
			if (i > 0) {
				String robot_id = text.substring(0, i);
				robot_.setRobotId(robot_id);
				server_address = text.substring(i + 1);
			}
		}
		
		service_client_ = new RobotServiceClient(server_address);
		robot_.setServiceClient(service_client_);
		if (!service_client_.open()) {
			Toast.makeText(getBaseContext(), "Connection error.", Toast.LENGTH_SHORT).show();
			return;
		}
		robot_.login();
		if (!robot_.isOnline()) {
			Toast.makeText(getBaseContext(), "Failed to log in.", Toast.LENGTH_SHORT).show();
			return;
		}
		control_handler_ = new RobotControlServiceHandler(server_address, robot_);
		control_handler_.startService();
	}
	
	/**
	 * Disconnects from the RCS server.
	 */
	private void disconnectFromServer() {
		robot_.logout();
		service_client_.close();
		control_handler_.stopService();
	}
	
	/**
	 * Connects to the local robot via bluetooth.
	 */
	private void connectToArduino() {
		BluetoothAdapter bt_adapter = BluetoothAdapter.getDefaultAdapter();
		if (bt_adapter == null) {
			Toast.makeText(getBaseContext(), "No Bluetooth.", Toast.LENGTH_SHORT).show();
			return;
		}
		Set<BluetoothDevice> bt_devices = bt_adapter.getBondedDevices();
		BluetoothDevice arduino = null;
		for (BluetoothDevice device : bt_devices) {
			if (device.getName().equals("RN42-CD27")) {
				arduino = device;
			}
		}
		if (arduino == null) {
			Toast.makeText(getBaseContext(), "Could not find Arduino.", Toast.LENGTH_SHORT).show();
			return;
		}
		UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
		OutputStreamWriter arduino_writer = null;
		try {
			bt_socket_ = arduino.createRfcommSocketToServiceRecord(uuid);
			bt_socket_.connect();
			OutputStream arduino_out = bt_socket_.getOutputStream();
			arduino_writer = new OutputStreamWriter(arduino_out);
		} catch (IOException e) {
			e.printStackTrace();
			Toast.makeText(
					getBaseContext(), "Could not connec to to Arduino.", Toast.LENGTH_SHORT)
					.show();
			return;
		}
		robot_.setArduinoOut(arduino_writer);
	}
	
	/**
	 * Disconnects from the local robot.
	 */
	private void disconnectFromArduino() {
		try {
			bt_socket_.close();
		} catch (IOException e) {
			Toast.makeText(
					getBaseContext(), "Failed to close Arduino connection.", Toast.LENGTH_SHORT)
					.show();
			e.printStackTrace();
		}
	}
}
