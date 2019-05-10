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
// BotRadioCommandsManager.hpp
//
// Class: RadioCommandsmanager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_RADIO_COMMANDS_MANAGER_INCLUDED
#define BOT_RADIO_COMMANDS_MANAGER_INCLUDED

class RadioCommandsManager
{
	//
	// Group: Type definitions.
	//
	public:
class RadioCommand
{
	//
	// Group: Private members.
	//
	private:
		const Client *const                             m_caller;
		HalfLifeEngine::SDK::Constants::RadioCommand_t  m_command;
		float                                           m_time;
		float                                           m_reactTime;	// time to react on sent radio messages.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline RadioCommand (const Client *const caller, const HalfLifeEngine::SDK::Constants::RadioCommand_t command, const float reactTime) :
			m_caller (caller),	// Set caller edict.
			m_command (command),
			m_time (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()),
			m_reactTime (reactTime)
		{
			// Reliability check.
			InternalAssert (caller->IsValid ());
		}

	//
	// Group: Private operators.
	//
	private:
		inline RadioCommand &operator = (const RadioCommand &/*right*/);	// Avoid "warning C4512: 'RadioCommand' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		inline const Client *const                                  GetCaller    (void) const { return m_caller; }
		inline const HalfLifeEngine::SDK::Constants::RadioCommand_t GetCommand   (void) const { return m_command; }
		inline const float                                          GetTime      (void) const { return m_time; }
		inline const float                                          GetReactTime (void) const { return m_reactTime; }
};

		typedef PriorityQueue <RadioCommand *, unsigned char> RadioCommandsPriorityQueue;

	//
	// Group: Private members.
	//
	private:
		RadioCommandsPriorityQueue m_lastRadioCommands;

	//
	// Group: Private functions.
	//
	private:
		static inline const bool IsLowerPriority (const RadioCommand *&node1, const RadioCommand *&node2)
		{
			// Nodes with greater time to react are lower priority.
			return node1->GetReactTime () <= node2->GetReactTime ();
		}

	//
	// Group: (Con/De)structors.
	//
	public:
		inline  RadioCommandsManager (void) : m_lastRadioCommands (/*WTF!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?*/reinterpret_cast <RadioCommandsPriorityQueue::LessFunction_t> (IsLowerPriority)) { /*VOID*/ }
		inline ~RadioCommandsManager (void) { m_lastRadioCommands.DeleteAll (); }

	//
	// Group: Functions.
	//
	public:
		inline       RadioCommandsPriorityQueue &GetRadioCommands (void)       { return m_lastRadioCommands; }
		inline const RadioCommandsPriorityQueue &GetRadioCommands (void) const { return m_lastRadioCommands; }

		inline void HearRadioCommand (const Client *const caller, const HalfLifeEngine::SDK::Constants::RadioCommand_t command, const float reactTime)
		{
			RadioCommand *const newRadioCommand (new RadioCommand (caller, command, reactTime));

			#if defined _DEBUG
				// Reliability check.
				if (newRadioCommand == NULL)
					TerminateOnMalloc ();
			#endif	// if defined _DEBUG

			m_lastRadioCommands += newRadioCommand;
		}

		inline void RemoveTop (void)
		{
			// Reliability check.
			InternalAssert (!m_lastRadioCommands.IsEmpty ());

			m_lastRadioCommands.DeleteFront ();
		}
};

#endif	// ifndef BOT_RADIO_COMMANDS_MANAGER_INCLUDED