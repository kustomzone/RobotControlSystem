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

import org.apache.cordova.Config;
import org.apache.cordova.DroidGap;

import android.os.Bundle;

/**
 * @author Tuna Oezer
 * 
 * Main Android activity.
 */
public class RobotAppActivity extends DroidGap {
	private static final String kUserAgentSuffix = " RcsMobileApp/0.1"; 
	
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        super.loadUrl(Config.getStartUrl());
        appView.getSettings().setUserAgentString(
        		appView.getSettings().getUserAgentString() + kUserAgentSuffix);
    }
}
