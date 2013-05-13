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

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * @author Tuna Oezer
 * 
 * Maintains a global directory of topic listeners.
 */
public class WampTopicListenerDirectory {
	private static WampTopicListenerDirectory instance = null;
	
	public static WampTopicListenerDirectory getInstance() {
		if (instance == null) {
			synchronized (WampTopicListenerDirectory.class) {
				if (instance == null) {
					instance = new WampTopicListenerDirectory();
				}
			}
		}
		return instance;
	}
	
	// topic -> listener
	private Map<String, Set<WampTopicListener>> directory_;
	
	private WampTopicListenerDirectory() {
		directory_ = new HashMap<String, Set<WampTopicListener>>();
	}
	
	/**
	 * Adds a listener to a topic.
	 * @param listener Topic listener.
	 * @param topic Topic URI.
	 */
	public synchronized void subscribe(WampTopicListener listener, String topic) {
		Set<WampTopicListener> listeners = directory_.get(topic);
		if (listeners == null) {
			listeners = new HashSet<WampTopicListener>();
			directory_.put(topic, listeners);
		}
		listeners.add(listener);
	}
	
	/**
	 * Removes the listener from a topic.
	 * @param listener Topic listener.
	 * @param topic Topic URI.
	 */
	public synchronized void unsubscribe(WampTopicListener listener, String topic) {
		Set<WampTopicListener> listeners = directory_.get(topic);
		if (listeners == null) return;
		listeners.remove(listener);
		if (listeners.isEmpty()) {
			directory_.remove(listeners);
		}
	}
	
	/**
	 * Unsubscribes the listener from all specified topics.
	 * This method should be preferred over unsubscribe() for mass unsubscriptions due to
	 * performance reasons.
	 * @param listener Topic listener.
	 * @param topics Set of topic URI's.
	 */
	public synchronized void unsubscribeAll(WampTopicListener listener, Set<String> topics) {
		for (String topic : topics) {
			Set<WampTopicListener> listeners = directory_.get(topic);
			if (listeners == null) continue;
			listeners.remove(listener);
			if (listeners.isEmpty()) {
				directory_.remove(listeners);
			}
		}
	}
	
	/**
	 * Sends the event to all listeners subscribed to the topic.
	 * @param topic Topic URI.
	 * @param event Even data to publish.
	 */
	public void publish(String topic, Object event) {
		publish(topic, event, null, null);
	}
	
	/**
	 * Sends the event to all listeners subscribed to the topic.
	 * If excludes is not null, excludes listeners with the specified session ID's.
	 * If includes is not null, only sends to listeners wih the specified session ID's.
	 * If includes is not null, excludes will be ignored.
	 * Due to performance reasons, this method is not thread-safe. Subscriptions and
	 * unsubscriptions may not become immidiately effective.
	 * @param topic Topic URI.
	 * @param event Even data to publish.
	 * @param excludes Null or list of session ID's to exclude.
	 * @param includes Null or explicit list of session ID's of targeted listeners.
	 */
	public void publish(String topic, Object event, String[] excludes, String[] includes) {
		Set<WampTopicListener> listeners = directory_.get(topic);
		if (listeners == null) return;
		if (includes != null) {
			Set<String> include_set = new HashSet<String>();
			for (String include : includes) include_set.add(include);
			for (WampTopicListener listener : listeners) {
				if (include_set.contains(listener.getSessionId())) {
					listener.receive(topic, event);
				}
			}
		} else if (excludes != null) {
			Set<String> exclude_set = new HashSet<String>();
			for (String exclude : excludes) exclude_set.add(exclude);
			for (WampTopicListener listener : listeners) {
				if (!exclude_set.contains(listener.getSessionId())) {
					listener.receive(topic, event);
				}
			}
		} else {
			for (WampTopicListener listener : listeners) {
				listener.receive(topic, event);
			}
		}
	}
}
