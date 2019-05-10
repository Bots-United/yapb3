//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Msec.cpp
//
// Class: Msec
//
// Description: General msec value computing class definitions for pfnRunPlayerMove() function.
//
// Version: $ID:$
//

#include <Core.hpp>
#if 0
inline const unsigned char YaPB::Msec::_CalculateMsecValue (const int value)
{
	const float v3 (value * 0.001f);

	if (v3 != m_del)
	{
		const float v2 (host_frametime * 0.05f);
		float v5 (v3 - m_del);

		if (v5 <= 0.0f)
		{
			float v6 (-v5);

			if (v2 < v6)
				v6 = host_frametime * 0.05f;

			v5 = -v6;
		}
		else if (v2 < v5)
			v5 = host_frametime * 0.05f;

		m_del += v5;
	}

	m_value = (((signed int)(signed __int64)(m_del * 1000.0f) < 0) - 1) & (signed __int64)(m_del * 1000.0f);

	if (m_value > 100u)
		m_value = 100u;

	return m_value;
}
inline const unsigned char YaPB::Msec::CalculateMsecValue (float value)
{
	int v1 = 100;
	int v2 = 50;
	int value (/*ex_interp.value*/0.1f * 1000.0f);

/*	if ( g_isSpectateOnly )
    v1 = 200;
  if ( cl_updaterate.value > 0.0 )
  {
    v2 = (signed __int64)(1000.0 / cl_updaterate.value);
    if ( v2 < 1 )
      v2 = 1;
  }
*/
	v2 = static_cast <int> (1000.0f / 30.0f);
/*
	if ( value + 1 >= v2 )
  {
    if ( value - 1 <= v1 )
      goto LABEL_15;
    value = v1;
    v9 = v1;
    v10 = v1;
    v8[0] = "ex_interp forced down to %i msec\n";
  }
  else
  {
    value = v2;
    v9 = v2;
    v10 = v2;
    v8[0] = "ex_interp forced up to %i msec\n";
  }

	LABEL_15:*/
	if (value < v2)
		value = v2;
	if (value > v1)
		value = v1;

	return _CalculateMsecValue (value);
}
#endif	// if 0
void YaPB::Msec::Think (const float botFrameIntervalTime)
{
	// the purpose of this function is to compute, according to the specified computation
	// method, the msec value which will be passed as an argument of pfnRunPlayerMove().
	// This function is called every frame for every bot, since the RunPlayerMove() is the function
	// that tells the engine to put the bot character model in movement.
	// This msec value tells the engine how long should the movement of the model extend inside the current frame.
	// It is very important for it to be exact, else one can experience bizarre problems, such as bots getting stuck into each others.
	// That's because the model's bounding boxes, which are the boxes the engine uses to compute and detect all the
	// collisions of the model, only exist, and are only valid, while in the duration of the movement.
	// That's why if you get a pfnRunPlayerMove() for one bot that lasts a little too
	// short in comparison with the frame's duration, the remaining time until the frame
	// elapses, that bot will behave like a ghost: no movement, but bullets and players can pass through it.
	// Then, when the next frame will begin, the stucking problem will arise!

	// The following code calculates the msec value for the physics function.
	// It needs to be dependant on framerate so the bot will move at the correct speed.

	#if defined MULTIPLE_MSEC_METHODS
		switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MsecMethod)->GetValue <Console::Bot::VariableValue_MsecMethod_t> ())
		{
			// this is Tobias Heimann's method for calculating the msec value
			case Console::Bot::VariableValue_MsecMethod_TobiasHeimann:
				if (m_del + m_num * 0.001f < HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - 0.51f || m_del > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					m_del = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - 0.05f;
					m_num = 0.0f;
				}

				const float temp ((HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - m_del) * 1000.0f);

				m_value = static_cast <unsigned char> (temp - m_num);	// optimal msec value since start of 1 sec period
				m_num = temp;	// value ve have to add to reach optimum

				// do we have to start a new 1 sec period?
				if (m_num > 1000.0f)
				{
					m_del += m_num * 0.001f;

					m_num = 0.0f;
				}

				break;

			// this is a Pierre-Marie Baty's method for calculation the msec value, thanks to Jussi Kivilinna for additional code
			case Console::Bot::VariableValue_MsecMethod_PierreMarieBaty:
				const float temp (botFrameIntervalTime * 1000.0f);

				m_value = static_cast <unsigned char> (temp);

				m_del += temp - m_value;

				// remove effect of integer conversion
				if (m_del > 1.5f)
				{
					++m_value;

					--m_del;
				}

				break;

			// this is Rich 'TheFatal' Whitehouse's method for calculating the msec value - from Advanced Bot Frame
			case Console::Bot::VariableValue_MsecMethod_RichWhitehouse:
				// adjust the millisecond delay based on the frame rate interval...
				if (m_del <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if (m_num > 0.0f)
						m_value = static_cast <unsigned char> (450.0f / m_num);

					m_del = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;	// next check in half a second
					m_num = 0.0f;
				}
				else
					++m_num;

				break;

			// this is Leon Hartwig's method for computing the msec value
			case Console::Bot::VariableValue_MsecMethod_LeonHartwig:
	#endif	// if defined MULTIPLE_MSEC_METHODS

			m_value = static_cast <unsigned char> (botFrameIntervalTime * 1000.0f);
//			m_value = static_cast <unsigned char> (/*host_frametime*/HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <double> (0xA83428u) * 1000.0f);

	#if defined MULTIPLE_MSEC_METHODS
				break;

			// this is The Storm's method for computing the msec value
			case 5u:
				m_value = static_cast <unsigned char> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime * 1000.0f);

				break;

			// use Leon Hartwig's method by default
			default:
				AddLogEntry (true, LogLevel_Error, false, "Value of '%s' should be in range (%u...%u)!", Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MsecMethod)->GetName ().GetData (), Console::Bot::VariableValue_MsecMethod_VariableValue_MsecMethod_None + 1u, Console::Bot::VariableValue_MsecMethod_VariableValue_MsecMethod_Total - 1u);

				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MsecMethod)->SetValue (Console::Bot::VariableValue_MsecMethod_LeonHartwig);
		}
	#endif	// if defined MULTIPLE_MSEC_METHODS
#if 0
	// validate range of the msec value (don't allow the msec delay to be too low & don't allow it to last longer than 100 milliseconds either)
	Math::ClampValueInRange <unsigned char> (m_value, 5u, 100u);
#endif	// if 0
}