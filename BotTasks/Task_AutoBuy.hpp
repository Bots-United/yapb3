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
// Task_AutoBuy.hpp
//
// Class: Task_AutoBuy
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_AUTO_BUY_INCLUDED
#define TASK_AUTO_BUY_INCLUDED

class Task_AutoBuy : public Task
{
	//
	// Group: Members.
	//
	public:
		DECLARE_TYPED_ENUMERATION (unsigned char, BuyState_t)
		{
			BuyState_PrimaryWeapon,
			BuyState_Armor,
			BuyState_SecondaryWeapon,
			BuyState_Grenades,
			BuyState_Defuser,
			BuyState_Ammo,
			BuyState_NightVisionGoggles
		};

	//
	// Group: Private members.
	//
	private:
		BuyState_t m_buyState;		// current count in buying
		float      m_nextBuyTime;	// next buy time (delay)

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_AutoBuy (YaPB *const bot, const BuyState_t startBuyState = BuyState_PrimaryWeapon) :
			Task (bot),

			m_buyState (startBuyState),
			m_nextBuyTime (g_randomNumberGenerator.GetValueBetween (bot->m_profile->baseReactTime + 0.6f, bot->m_profile->baseReactTime + 4.2f))
		{ /*VOID*/ }	// going here

	//
	// Group: Private functions.
	//
	private:
		const bool IsMorePowerfulWeaponCanBeBought (void) const;

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_AutoBuy; }

		inline void       Run   (void)
		{
			// Call original function in base class....
			Task::Run ();

			m_nextBuyTime += HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);
};

#endif	// ifndef TASK_AUTO_BUY_INCLUDED