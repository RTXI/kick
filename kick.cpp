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

#include <kick.h>
#include <math.h>
#include <QtGui>

extern "C" Plugin::Object *
createRTXIPlugin(void)
{
  return new kick();
}

static DefaultGUIModel::variable_t vars[] =
  {
    { "Kick", "Kick signal", DefaultGUIModel::OUTPUT, },
    { "Amplitude", "Amplitude", DefaultGUIModel::PARAMETER
        | DefaultGUIModel::DOUBLE, },
    { "Delay (ms)", "Delay (ms)", DefaultGUIModel::PARAMETER
        | DefaultGUIModel::DOUBLE, }, };

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

kick::kick(void) :
  DefaultGUIModel("Kick", ::vars, ::num_vars)
{
  setWhatsThis(
      "<p><b>Kick:</b><br>/* A module that sends a single value as a trigger or "
        "\"kick\" to another module. It outputs a user-specified value when triggered "
        "and a value of 0 otherwise. It can be used to manually mark events that a user "
        "sees in the data or used to kick other modules that require a trigger (such as "
        "those accepting spike detections).</p>");

  createGUI(vars, num_vars);
  initParameters();
  update( INIT);
  refresh();
}

kick::~kick(void)
{
}

void
kick::execute(void)
{
  systime = count * dt; // time in seconds

  switch (state)
    {
  case 0:
    if (systime > delay)
      {
        output(0) = amplitude;
        state = 1;
      }
    break;
  case 1:
    output(0) = 0;
  default:
    break;
    }

  count++;
}

void
kick::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag)
    {
  case INIT:
    setParameter("Amplitude", QString::number(amplitude));
    setParameter("Delay (ms)", QString::number(delay * 1e3)); // display in ms
    break;
  case MODIFY:
    amplitude = getParameter("Amplitude").toDouble();
    delay = getParameter("Delay (ms)").toDouble() * 1e-3; // convert to s
    break;
  case PERIOD:
    dt = RT::System::getInstance()->getPeriod() * 1e-9; // time in seconds
  case PAUSE:
    output(0) = 0.0;
    state = 0;
    break;
  case UNPAUSE:
    systime = 0;
    count = 0;
    break;
  default:
    break;
    }
}

void
kick::initParameters()
{
  amplitude = 1;
  delay = 0.2; // initialized in s
  dt = RT::System::getInstance()->getPeriod() * 1e-9; // time in seconds
  state = 0;
}

