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
// OtherBot.hpp
//
// Class: FakeClient_NotYaPB
//
// Description: Contains the 'FakeClient_NotYaPB' class Function Prototypes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef OTHER_BOT_INCLUDED
#define OTHER_BOT_INCLUDED

class FakeClient_NotYaPB : public FakeClient
{
	DECLARE_CLASS_WITH_BASE (FakeClient, FakeClient_NotYaPB);

	//
	// Group: (Con/De)structors.
	//
	public:
		inline FakeClient_NotYaPB (HalfLifeEngine::SDK::Classes::Edict *const notYaPBFakeClient) :
			FakeClient (notYaPBFakeClient)
		{ /* VOID */ }

		virtual inline ~FakeClient_NotYaPB (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		virtual inline const Type_t GetType (void) const { return Type_FakeClient_NotYaPB; }

		inline void CheckWalk (void)
		{
			if (!IsDucking () && IsOnFloor () ? m_edict->GetSpeedSquared2D () <= HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared : IsOnLadder () && m_edict->GetSpeedSquared () <= HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared)
				m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_RUN;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsYaPB
		//
		// Description: Checks whether client is YaPB.
		//
		// Returns: True if client is YaPB, false otherwise.
		//
		inline const bool              IsYaPB                    (void) const { return false; }

		//
		// Function: IsOtherBot
		//
		// Description: Checks whether client is other bot, not a YaPB.
		//
		// Returns: True if client is other bot, false otherwise.
		//
		inline const bool              IsOtherBot                (void) const { return true; }
};

inline       FakeClient_NotYaPB *const Client::GetFakeClientNotYaPBPointer (void)
{
	// Reliability check.
	InternalAssert (IsOtherBot ());

	return static_cast <FakeClient_NotYaPB *const> (this);
}
inline const FakeClient_NotYaPB *const Client::GetFakeClientNotYaPBPointer (void) const
{
	// Reliability check.
	InternalAssert (IsOtherBot ());

	return static_cast <const FakeClient_NotYaPB *const> (this);
}

#endif	// ifndef OTHER_BOT_INCLUDED