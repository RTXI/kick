/*
 Copyright (C) 2011 Georgia Institute of Technology
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

/*
 * A module that sends a single value as a trigger or "kick" to another module. It
 * outputs a user-specified value when triggered and a value of 0 otherwise. It can
 * be used to manually mark events that a user sees in the data or used to kick
 * other modules that require a trigger (such as those accepting spike detections).
 */

#include <default_gui_model.h>

class kick : public DefaultGUIModel
{

	public:
		kick(void);
		virtual ~kick(void);

		virtual void execute(void);

	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		void initParameters();

		double amplitude;
		double delay;
		int state;

		double dt; // system period
		double systime;
		long long count;
};
