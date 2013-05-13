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
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.servlet.http.HttpSession;

import org.apache.catalina.websocket.MessageInbound;
import org.apache.catalina.websocket.WsOutbound;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * @author Tuna Oezer
 *
 * Handles WAMP communication with the client.
 * Each instance represents one client.
 * PREFIX is currently not supported. 
 */
public class WampHandler extends MessageInbound implements WampTopicListener {
	private static final String kServerVersion = "RCS-WAMP/0.1";
	
	// Maximum input message length.
	private static final int kMaxMessageLength = 262144;
	
	// Maximum number of topics to which a client can subscribe.
	private static final int kMaxTopics = 1024;
	
	// Maximum number of includes or excludes that can be specified in a publish request.
	private static final int kMaxIncludesExcludes = 4096;
	
	private static final int kWELCOME = 0;
	private static final int kPREFIX = 1;
	private static final int kCALL = 2;
	private static final int kCALLRESULT = 3;
	private static final int kCALLERROR = 4;
	private static final int kSUBSCRIBE = 5;
	private static final int kUNSUBSCRIBE = 6;
	private static final int kPUBLISH = 7;
	private static final int kEVENT = 8;
	
	// Global thread pool for RPC methods
	private static ExecutorService rpc_thread_pool = null;
	
	private String session_id_;
	private Set<String> topics_;  // topics to which the client has subscribed
	private Map<String, WampRpcMethod> rpc_methods_;  // method URI -> RPC method
	private ObjectMapper json_processor_;
	
	public WampHandler(HttpSession session) {
		this.session_id_ = session.getId();
		this.json_processor_ = new ObjectMapper();
		topics_ = new HashSet<String>();
		rpc_methods_ = new HashMap<String, WampRpcMethod>();
	}
	
	private static ExecutorService getThreadPool() {
		if (rpc_thread_pool == null) {
			synchronized (WampHandler.class) {
				if (rpc_thread_pool == null) {
					rpc_thread_pool = Executors.newCachedThreadPool();
				}
			}
		}
		return rpc_thread_pool;
	}
	
	private String createReply(Object ... args) {
		if (args.length == 0 || !Integer.class.isAssignableFrom(args[0].getClass()))
			throw new AssertionError("Message ID is missing.");
		try {
			return json_processor_.writeValueAsString(args);
		} catch (JsonProcessingException e) {
			e.printStackTrace();
			throw new AssertionError(e);
		}
	}
	
	@Override
	protected void onOpen(WsOutbound out) {
		try {
			// WELCOME message
			out.writeTextMessage(CharBuffer.wrap(
					createReply(kWELCOME, session_id_, 1, kServerVersion)));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	protected void onClose(int status) {
		WampTopicListenerDirectory.getInstance().unsubscribeAll(this, topics_);
		topics_.clear();
	}

	@Override
	protected void onBinaryMessage(ByteBuffer message) {
		// No binary messages.
	}

	@Override
	protected void onTextMessage(CharBuffer message) {
		String input = message.toString();
		if (input.length() > kMaxMessageLength) return;  // message is too long
		try {
			Object[] args = json_processor_.readValue(message.toString(), Object[].class);
			// Check for valid message ID.
			if (args.length == 0 || !Integer.class.isAssignableFrom(args[0].getClass())) return;
			int message_id = (int) args[0];
			switch (message_id) {
				case kPREFIX: {
					// Not supported.
					break;
				}
				case kCALL: {
					if (args.length < 3 ||
						!(args[1] instanceof String) ||
						!(args[2] instanceof String)) return;  // call ID and/or method URI missing
						WampRpcMethod rpc_method = rpc_methods_.get((String) args[2]);
						if (rpc_method != null) {
							getThreadPool().execute(
									new WampRpcTask(rpc_method,
													new WampRpcArgs(json_processor_, args),
													new WampRpcReturn(this, (String) args[1])));
						} else {
							returnRpcError((String) args[1],
										   "rpc://lib.robotics.rcs/error/no_such_method",
										   "No such method.",
										   null);
						}
					break;
				}
				case kSUBSCRIBE: {
					if (args.length < 2 || !(args[1] instanceof String)) return;  // topic missing
					if (topics_.size() > kMaxTopics) return;  // too many topics
					String topic = (String) args[1];
					topics_.add(topic);
					WampTopicListenerDirectory.getInstance().subscribe(this, topic);
					break;
				}
				case kUNSUBSCRIBE: {
					if (args.length < 2 || !(args[1] instanceof String)) return;  // topic missing
					String topic = (String) args[1];
					topics_.remove(topic);
					WampTopicListenerDirectory.getInstance().unsubscribe(this, topic);
					break;
				}
				case kPUBLISH: {
					// Check for topic and event.
					if (args.length < 3 || !(args[1] instanceof String)) return;
					String topic = (String) args[1];
					String[] excludes = null;
					String[] includes = null;
					if (args.length >= 5 &&
						args[4] != null &&
						args[4] instanceof String[]) {
						// with eligible list
						includes = (String[]) args[4];
						if (includes.length == 0 || includes.length > kMaxIncludesExcludes) {
							includes = null;
						}
					}
					if (includes == null && args.length == 4 && args[3] != null) {
						if (args[3] instanceof String[]) {
							// with exclude list
							excludes = (String[]) args[3];
							if (excludes.length == 0 || excludes.length > kMaxIncludesExcludes) {
								excludes = null;
							}
						} else if (Boolean.class.isAssignableFrom(args[3].getClass()) &&
								   (boolean) args[3]) {
							// with excludeMe
							excludes = new String[] { session_id_ };
						}
					}
					WampTopicListenerDirectory.getInstance().publish(
							topic, args[2], excludes, includes);
					break;
				}
				default: break;
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void receive(String topic, Object event) {
		if (topics_.contains(topic)) {
			try {
				getWsOutbound().writeTextMessage(CharBuffer.wrap(
						createReply(kEVENT, topic, event)));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public String getSessionId() {
		return session_id_;
	}
	
	/**
	 * Returns the RPC result for the specified call to the client.
	 * @param call_id Call ID specified in RPC call.
	 * @param result Result of RPC method.
	 */
	public void returnRpcResult(String call_id, Object result) {
		try {
			getWsOutbound().writeTextMessage(CharBuffer.wrap(
					createReply(kCALLRESULT, call_id, result)));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Returns an error to the client if an RPC call has failed.
	 * @param call_id Call ID specified in RPC call.
	 * @param error_uri URI of the error.
	 * @param error_description Humand readable description of the error.
	 * @param error_details Application specific error data or null.
	 */
	public void returnRpcError(String call_id,
							   String error_uri,
							   String error_description,
							   Object error_details) {
		try {
			if (error_details == null) {
				getWsOutbound().writeTextMessage(CharBuffer.wrap(
						createReply(kCALLERROR, call_id, error_uri, error_description)));
			} else {
				getWsOutbound().writeTextMessage(CharBuffer.wrap(
						createReply(kCALLERROR,
									call_id,
									error_uri,
									error_description,
									error_details)));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Registers an RPC method under the specified URI.
	 * Only one method can be registered for each URI.
	 * @param uri RPC method URI.
	 * @param method RPC method implementation.
	 */
	public void registerRpcMethod(String uri, WampRpcMethod method) {
		rpc_methods_.put(uri, method);
	}
	
	/**
	 * Unregisters the currently registered RPC method from the specified URI.
	 * @param uri RPC method URI.
	 */
	public void unregisterRpcMethod(String uri) {
		rpc_methods_.remove(uri);
	}
}
