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

/**
 * @author Tuna Oezer
 *
 * All WAMP pub/sub topic listeners must implement this interface.
 */
public interface WampTopicListener {
	/**
	 * Called when a message is published under the topic.
	 * @param topic Topic
	 * @param event Published event
	 */
	void receive(String topic, Object event);
	
	/**
	 * Used to include or exclude listeners from messages.
	 * @return WAMP session ID of listener. 
	 */
	String getSessionId();
}
