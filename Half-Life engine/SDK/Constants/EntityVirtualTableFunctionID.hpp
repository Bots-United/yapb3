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
// EntityVirtualTableFunctionID.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_VIRTUAL_TABLE_FUNCTION_ID_INCLUDED
#define ENTITY_VIRTUAL_TABLE_FUNCTION_ID_INCLUDED

/**
* A few notes about all of the following functions:
*   - Not all functions will do as you expect on all mods.
*     If a function does not do what you would believe it should
*     DO NOT file a bug report, you will be ignored.
*
*   - Passing invalid parameters has potential to crash the server
*     So be careful, and adequately test!
*
*   - All functions take (and pass) a "this" index as the first param.
*     This is the entity from which the function is being executed on.
*
*   - All functions and forwards (eg: {Register,Execute}EntityVirtualTableFunctionID_t[B]) require
*     the mod to have the pev and base keys in addition to the function
*     keys for the corresponding mod/operating system in hamdata.ini
*
*   - Some functions that return booleans may need to be logically ANDed
*     to get the results desired. E.g: if (ExecuteFunction (EntityVirtualTableFunctionID_TS_IsObjective, this) & 0x0000FFFF != 0) { // true.. }
*     because the module will return the full integer value.
*/
enum EntityVirtualTableFunctionID_t
{
	/**
	* Description:		This is typically called whenever an entity is created.
	*					It is the virtual equivilent of spawn from the engine.
	*					Some mods call this on player spawns too.
	* Forward params:	function(this)
	* Return type:		None.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Spawn, this);
	*/
	EntityVirtualTableFunctionID_Spawn,

	/**
	* Description:		Returns an integer number that corresponds with what type of entity this is.
	* Forward params:	function(this)
	* Return type:		Integer.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Classify, this);
	*/
	EntityVirtualTableFunctionID_Classify = 9u,

	/**
	* Description:		Usually called whenever an entity takes any kind of damage.
	*					Inflictor is the entity that caused the damage (such as a gun).
	*					Attacker is the entity that tirggered the damage (such as the gun's owner).
	* Forward params:	function(this, idinflictor, idattacker, Float:damage, damagebits);
	* Return type:		Integer.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_TakeDamage, this, idinflictor, idattacker, Float:damage, damagebits);
	*/
	EntityVirtualTableFunctionID_TakeDamage = 12u,

	/**
	* Description:		Normally called whenever an entity dies.
	* Forward params:	function(this, idattacker, shouldgib)
	* Return type:		None.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Killed, this, idattacker, shouldgib);
	*/
	EntityVirtualTableFunctionID_Killed = 14u,

	/**
	* Description:		Adds an item to the player's inventory.
	* Forward params:	function(this, idother);
	* Return type:		Integer.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_AddPlayerItem, this, idother);
	*/
	EntityVirtualTableFunctionID_AddPlayerItem = 23u,

	/**
	* Description:		Removes an item to the player's inventory.
	* Forward params:	function(this, idother);
	* Return type:		Integer.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_RemovePlayerItem, this, idother);
	*/
	EntityVirtualTableFunctionID_RemovePlayerItem = 24u,

	/**
	* Description:		Whether or not the entity is a player.
	* Forward params:	function(this);
	* Return type:		Integer (boolean).
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_IsPlayer, this);
	*/
	EntityVirtualTableFunctionID_IsPlayer = 40u,

	/**
	* Description:		Called whenever an entity thinks.
	* Forward params:	function(this)
	* Return type:		None.
	* Execute params:	ExecuteFunction(EntityVirtualTableFunctionID_Think, this);
	*/
	EntityVirtualTableFunctionID_Think = 44u,

	/**
	* Description:		Called whenever two entities touch.
	* Forward params:	function(this, idother);
	* Return type:		None.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Touch, this, idother);
	*/
	EntityVirtualTableFunctionID_Touch = 45u,

	/**
	* Description:		Called whenver one entity uses another.
	* Forward params:	function(this, idcaller, idactivator, use_type, Float:value)
	* Return type:		None.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Use, this, idcaller, idactivator, use_type, Float:value);
	*/
	EntityVirtualTableFunctionID_Use = 46u,

	/**
	* Description:		Returns current player light level plus virtual muzzle flash.
	* Forward params:	function(this)
	* Return type:		Integer (unsigned char).
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Player_Illumination, this);
	*/
	EntityVirtualTableFunctionID_Illumination = 55u,

	/**
	* Description:		Deploys the entity (usually a weapon).
	* Forward params:	function(this);
	* Return type:		Integer (boolean).
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Item_Deploy, this);
	*/
	EntityVirtualTableFunctionID_Item_Deploy = 64u,

	/**
	* Players have all the attributes of normal entities, in addition to these.
	*/

	/**
	* Description:		Is player a bot?
	* Forward param:	function(this)
	* Return type:		Integer (boolean).
	* Execute params:	ExecuteFunction(EntityVirtualTableFunctionID_Player_IsBot, this);
	*/
	EntityVirtualTableFunctionID_Player_IsBot = 81u,

	/**
	* Description:		Called whenever an impulse command is executed.
	* Forward param:	function(this)
	* Return type:		None.
	* Execute params:	ExecuteFunction(EntityVirtualTableFunctionID_Player_ImpulseCommands, this);
	*/
	EntityVirtualTableFunctionID_Player_ImpulseCommands = 83u,

	/**
	* Description:		Respawn function for players/bots only! Do not use this on non player/bot entities!
	* Forward params:	function(this);
	* Return type:		None.
	* Execute params:	ExecuteHam(EntityVirtualTableFunctionID_Player_RoundRespawn, this);
	*/
	EntityVirtualTableFunctionID_Player_RoundRespawn = 84u,

	/**
	* Description:		Called whenever an player is touched a weapon box.
	* Forward param:	function(this)
	* Return type:		None.
	* Execute params:	ExecuteFunction(EntityVirtualTableFunctionID_Player_OnTouchingWeapon, this, WeaponBox *weapon);
	*/
	EntityVirtualTableFunctionID_Player_OnTouchingWeapon = 87u,

	/**
	* Description:		Called when the main attack of a weapon is triggered.
	* Forward params:	function(this)
	* Return type:		None.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Weapon_PrimaryAttack, this);
	*/
	EntityVirtualTableFunctionID_Weapon_PrimaryAttack = 87u,

	/**
	* Description:		Called when the weapon is reloaded.
	* Forward params:	function(this)
	* Return type:		None.
	* Execute params:	ExecuteFunction (EntityVirtualTableFunctionID_Weapon_Reload, this);
	*/
	EntityVirtualTableFunctionID_Weapon_Reload = 89u,

	/**
	* DONT USE ME LOL
	*/
	EntityVirtualTableFunctionID_Total
};

#endif	// ifndef ENTITY_VIRTUAL_TABLE_FUNCTION_ID_INCLUDED