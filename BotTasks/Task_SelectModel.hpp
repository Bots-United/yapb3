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
// Task_SelectModel.hpp
//
// Class: Task_SelectModel
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_SELECT_MODEL_INCLUDED
#define TASK_SELECT_MODEL_INCLUDED

class Task_SelectModel : public Task
{
	//
	// Group: Private members.
	//
	private:
		const float m_delayTime;	// Time bot "choosing" his team.... (thinking about it)

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_SelectModel (YaPB *const bot, const float delay = 0.5f) :
			Task (bot),

			m_delayTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + bot->m_profile->baseReactTime * g_randomNumberGenerator.GetValueBetween (0.2f, 1.0f) + delay)
		{ /* VOID */ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_SelectModel; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (void)
		{
			// The purpose of this function is to make the bot browse down through the model selection menus of Counter-Strike.

			if (m_delayTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;	// YaPB: "I'm thinking....".

			// Handle Counter-Strike stuff here...

			// Reliability check.
			InternalAssert (m_owner->m_gameAction == YaPB::GameAction_ModelSelect);
			InternalAssert (!m_owner->IsSpectator ());

			// bot has now joined the game (doesn't need to be started)
			m_owner->m_gameAction = YaPB::GameAction_Idle;	// switch back to idle, bot should be ready to fight now.

			// select the model the bot wishes to use....
			m_owner->ExecuteCommandFormat ("menuselect %u", m_owner->m_profile->model[m_owner->GetRealTeam () - 1u]);
/*
			// check for greeting other players, since we connected
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 86u - 2u * g_server->GetYaPBManager ()->GetClientsCount ())
				m_owner->ChatMessage (ChatType_Welcome);	// say hello here
*/
			Complete ();
		}
};

#endif	// ifndef TASK_SELECT_MODEL_INCLUDED