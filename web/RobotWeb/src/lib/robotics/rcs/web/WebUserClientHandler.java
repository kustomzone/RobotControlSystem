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

package lib.robotics.rcs.web;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;

import org.apache.catalina.websocket.MessageInbound;

import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TJSONProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.protocol.TProtocolFactory;
import org.apache.thrift.transport.AutoExpandingBuffer;
import org.apache.thrift.transport.AutoExpandingBufferWriteTransport;
import org.apache.thrift.transport.TMemoryInputTransport;

import lib.robotics.rcs.server.UserService;

/**
 * @author Tuna Oezer
 * 
 * Handles control requests initiated via a user over a WebSocket connection.
 */
public class WebUserClientHandler extends MessageInbound {
	
	private static final int kMaxInputSize = 1 << 17;
	private static final int kInitBufferSize = 4096;
	private static final double kBufferGrowthFactor = 1.5;
	
	private AutoExpandingBuffer input_buffer_;
	private TMemoryInputTransport input_;
	private AutoExpandingBufferWriteTransport output_;
	private TProtocol input_protocol_, output_protocol_;
	private TProcessor processor_;
	
	public WebUserClientHandler() {
		input_buffer_ = new AutoExpandingBuffer(kInitBufferSize, kBufferGrowthFactor);
		input_ = new TMemoryInputTransport();
		output_ = new AutoExpandingBufferWriteTransport(kInitBufferSize, kBufferGrowthFactor);
		TProtocolFactory protocol_factory = new TJSONProtocol.Factory();
		input_protocol_ = protocol_factory.getProtocol(input_);
		output_protocol_ = protocol_factory.getProtocol(output_);
		UserServiceHandler service_handler = new UserServiceHandler();
		processor_ = new UserService.Processor<UserService.Iface>(service_handler);
	}

	@Override
	protected void onBinaryMessage(ByteBuffer message) {
		// No binary messages are transmitted.
	}

	@Override
	protected void onTextMessage(CharBuffer message) {
		if (message.length() > kMaxInputSize) return;  // Overflow protection.
		try {
			input_buffer_.resizeIfNecessary(message.length());
			byte[] bytes = input_buffer_.array();
			for (int i = 0; i < message.length(); i++) {
				bytes[i] = (byte) message.charAt(i);
			}
			output_.reset();
			input_.reset(bytes, 0, message.length());
			processor_.process(input_protocol_, output_protocol_);
			bytes = output_.getBuf().array();
			char[] chars = new char[output_.getPos()];
			for (int i = 0; i < chars.length; i++) {
				chars[i] = (char) bytes[i];
			}
			getWsOutbound().writeTextMessage(CharBuffer.wrap(chars));
		} catch (IOException e) {
			e.printStackTrace();
		} catch (TException e) {
			e.printStackTrace();
		}
	}
}
