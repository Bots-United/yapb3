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
// Buy.hpp
//
// Class: BuyManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BUY_INCLUDED
#define BUY_INCLUDED

class BuyManager
{
	//
	// Group: Private members.
	//
	private:
		typedef DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> BuyPriorityArray_t;

		BuyPriorityArray_t m_buyPriority[TeamArrayID_Total][Personality_Total];	// buy priorities for both teams

	//
	// Group: (Con/De)structors.
	//
	public:
		inline BuyManager (void)
		{
			SetDefaults ();
			LoadBuyPriority ();
		}

	//
	// Group: Private functions.
	//
	private:
		inline void SetDefaults (void)
		{
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_AK47;

			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_M249;

			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_AWP;

			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal] += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;

			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher] += HalfLifeEngine::SDK::Constants::WeaponID_M249;

			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful] += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
		}
		inline void ResetDefaults (void)
		{
			m_buyPriority[TeamArrayID_Terrorist][Personality_Normal].RemoveAll ();
			m_buyPriority[TeamArrayID_Terrorist][Personality_Rusher].RemoveAll ();
			m_buyPriority[TeamArrayID_Terrorist][Personality_Careful].RemoveAll ();

			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Normal].RemoveAll ();
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Rusher].RemoveAll ();
			m_buyPriority[TeamArrayID_CounterTerrorist][Personality_Careful].RemoveAll ();

			SetDefaults ();
		}
		void LoadBuyPriority (void);	// load the buy priority
		inline void ReloadBuyPriority (void)	// load the buy priority
		{
			ResetDefaults ();
			LoadBuyPriority ();
		}

	//
	// Group: Functions.
	//
	public:
		const bool                                       IsWeaponRestricted    (const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID) const;
		const bool                                       IsItemRestricted      (const HalfLifeEngine::SDK::Constants::RestrictableItem_t &item) const;
		const HalfLifeEngine::SDK::Constants::WeaponID_t ChoosePrimaryWeapon   (const YaPB *const bot);
		const HalfLifeEngine::SDK::Constants::WeaponID_t ChooseSecondaryWeapon (const YaPB *const bot);
};

#endif	// ifndef BUY_INCLUDED