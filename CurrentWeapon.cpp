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
// CurrentWeapon.cpp
//
// Class: CurrentWeapon
//
// Description: Contains the client/bot current weapon class.
//
// Version: $ID:$
//

#include <Core.hpp>

const Client::CurrentWeaponConstructorFunctionsContainer::CurrentWeaponConstructorFunction_t Client::CurrentWeaponConstructorFunctionsContainer::s_currentWeaponConstructorFunctions[HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u] =
{
	ConstructorFunction_CurrentWeapon_None,

	ConstructorFunction_CurrentWeapon_P228,
	ConstructorFunction_CurrentWeapon_Shield,
	ConstructorFunction_CurrentWeapon_Scout,
	ConstructorFunction_CurrentWeapon_ExplosiveGrenade,
	ConstructorFunction_CurrentWeapon_XM1014,
	ConstructorFunction_CurrentWeapon_C4,
	ConstructorFunction_CurrentWeapon_MAC10,
	ConstructorFunction_CurrentWeapon_Aug,
	ConstructorFunction_CurrentWeapon_SmokeGrenade,
	ConstructorFunction_CurrentWeapon_Elite,
	ConstructorFunction_CurrentWeapon_FiveSeven,
	ConstructorFunction_CurrentWeapon_UMP45,
	ConstructorFunction_CurrentWeapon_SG550,
	ConstructorFunction_CurrentWeapon_Galil,
	ConstructorFunction_CurrentWeapon_Famas,
	ConstructorFunction_CurrentWeapon_USP,
	ConstructorFunction_CurrentWeapon_Glock18,
	ConstructorFunction_CurrentWeapon_AWP,
	ConstructorFunction_CurrentWeapon_MP5,
	ConstructorFunction_CurrentWeapon_M249,
	ConstructorFunction_CurrentWeapon_M3,
	ConstructorFunction_CurrentWeapon_M4A1,
	ConstructorFunction_CurrentWeapon_TMP,
	ConstructorFunction_CurrentWeapon_G3SG1,
	ConstructorFunction_CurrentWeapon_FlashBangGrenade,
	ConstructorFunction_CurrentWeapon_Deagle,
	ConstructorFunction_CurrentWeapon_SG552,
	ConstructorFunction_CurrentWeapon_AK47,
	ConstructorFunction_CurrentWeapon_Knife,
	ConstructorFunction_CurrentWeapon_P90
};