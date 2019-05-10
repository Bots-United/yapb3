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
// ZBot.hpp
//
// Class: ZBot
//
// Description: Contains the 'ZBot' class Function Prototypes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef Z_BOT_INCLUDED
#define Z_BOT_INCLUDED

class ZBot : public FakeClient_NotYaPB
{
	DECLARE_CLASS_WITH_BASE (FakeClient_NotYaPB, ZBot);

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ZBot (HalfLifeEngine::SDK::Classes::Edict *const ZBotEdict) : FakeClient_NotYaPB (ZBotEdict) { /* VOID */ }

		virtual inline ~ZBot (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_ZBot; }
};

inline       ZBot *const Client::GetZBotPointer (void)
{
	// Reliability check.
	InternalAssert (IsZBot ());

	return static_cast <ZBot *const> (this);
}
inline const ZBot *const Client::GetZBotPointer (void) const
{
	// Reliability check.
	InternalAssert (IsZBot ());

	return static_cast <const ZBot *const> (this);
}

#endif	// ifndef Z_BOT_INCLUDED