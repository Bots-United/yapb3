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
// Entity.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_EXPORT_REPLACED_FUNCTIONS_ENTITY_INCLUDED
#define HALF_LIFE_ENGINE_EXPORT_REPLACED_FUNCTIONS_ENTITY_INCLUDED

inline void Spawn (const SDK::Classes::BasePlayer::FunctionSpawn_t originalFunction, SDK::Classes::BasePlayer *const player)
{
	// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual
	// world, in other words to 'display') the entity pointed to by 'player' in the game.
	// The Spawn() function is one of the functions any entity is supposed to have in the game DLL,
	// and any MOD is supposed to implement one for each of its entities.

	// pre call the original BasePlayer::Spawn() function for "player" entity
	(player->*originalFunction) ();

	// post send this event to client/bot for updating some variables/conditions/tasks... (for CSDM mod, or something in this sort :) )
	g_server->GetClientManager ()->ClientSpawnPost (player->GetEdict ());
}

inline void Player_RoundRespawn (const SDK::Classes::BasePlayer::FunctionRoundRespawn_t originalFunction, SDK::Classes::BasePlayer *const player)
{
	// pre call the original BasePlayer::RoundRespawn() function for "player" entity
	(player->*originalFunction) ();

	// post send this event to client/bot for updating some variables/conditions/tasks... (for CSDM mod, or something in this sort :) )
	g_server->GetClientManager ()->ClientRoundRespawnPost (player->GetEdict ());
}

inline const BOOL TakeDamage (const SDK::Classes::BasePlayer::FunctionTakeDamage_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Structures::EntityVariables_t *const inflictor, SDK::Structures::EntityVariables_t *const attacker, float damage, SDK::Constants::DamageType_t damageBits)
{
	// Reliability checks.
	InternalAssert (inflictor != NULL);
	InternalAssert (attacker != NULL);
/*
	#if defined _DEBUG
		DynamicString damageTypesString;
		const DynamicString damageTypeStrings[] =
		{
			"crush",			// Crushed by falling or moving object.
			"bullet",			// Shot.
			"slash",			// Cut, clawed, stabbed.
			"burn",				// Heat burned.
			"freeze",			// Frozen.
			"fall",				// Fell too far.
			"blast",			// Explosive blast damage.
			"club",				// Crowbar, punch, headbutt.
			"shock",			// Electric shock.
			"sonic",			// Sound pulse shockwave.
			"energy beam",		// Laser or other high energy beam.

			"UNKNOWN",			// Unused by HL engine ?!? :(

			"never gib",		// With this bit OR'd in, no damage type will be able to gib victims upon death.
			"always gib",		// With this bit OR'd in, any damage type can be made to gib victims upon death.
			"drown",			// Drowning.
			"paralyze",			// Slows affected creature down.
			"nerve gas",		// Nerve toxins, very bad.
			"poison",			// Blood poisioning.
			"radiation",		// Radiation exposure.
			"drown recover",	// Drowning recovery.
			"acid",				// Toxic chemicals or acid burns.
			"slow burn",		// In an oven.
			"slow freeze",		// In a subzero freezer.
			"mortar",			// Hit by air raid (done to distinguish grenade from mortar).
			"HE grenade"		// Explosive grenade.
		};

		if (damageBits == SDK::Constants::DamageType_Generic)
			damageTypesString = "generic";	// Generic damage was done.
//		else if (damageBits & SDK::Constants::DamageType_TimeBased)
//			damageTypesString = "time based";	// Mask for time-based damage.
		else
		{
			STORE_FLAGS_TO_STRING (damageBits, damageTypesString, 0u, GET_STATIC_ARRAY_SIZE (damageTypeStrings), damageTypeStrings, "+");
		}

		Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "TakeDamage(): player: \"%s\", inflictor: \"%s\", attacker: \"%s\", damage=%.2f, type: '%s'.\n", player->GetName ().GetData (), inflictor->containingEntity->IsPlayer () ? inflictor->containingEntity->GetNetName ().GetData () : inflictor->containingEntity->GetClassName ().GetData (), attacker->containingEntity->IsPlayer () ? attacker->containingEntity->GetNetName ().GetData () : attacker->containingEntity->GetClassName ().GetData (), damage, damageTypesString.GetData ());
	#endif	// if defined _DEBUG
*/
	const Math::Angles3D oldPunchAngles (player->m_variables->punchAngles);
	const Math::Vector3D oldVelocity (player->m_variables->velocity);

	const BOOL result ((player->*originalFunction) (inflictor, attacker, damage, damageBits));

	player->m_variables->punchAngles = oldPunchAngles;
	player->m_variables->velocity = oldVelocity;
	player->m_punishVelocityModifier = 1.0f;

	return result;
}

inline void Hostage_Spawn (const SDK::Classes::Hostage::FunctionSpawn_t originalFunction, SDK::Classes::Hostage *const hostage)
{
	// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual
	// world, in other words to 'display') the entity pointed to by 'entity' in the game.
	// The Spawn() function is one of the functions any entity is supposed to have in the game DLL,
	// and any MOD is supposed to implement one for each of its entities.

	// pre call the original Hostage::Spawn() function for "hostage_entity" entity
	(hostage->*originalFunction) ();

	g_server->GetHostageManager ().HostageSpawnPost (hostage->GetEdict ());
}

inline const BOOL Hostage_TakeDamage (const SDK::Classes::Hostage::FunctionTakeDamage_t originalFunction, SDK::Classes::Hostage *const hostage, SDK::Structures::EntityVariables_t *const inflictor, SDK::Structures::EntityVariables_t *const attacker, float damage, SDK::Constants::DamageType_t damageBits)
{
/*	#if defined _DEBUG
		Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Hostage::TakeDamage(): hostage: %s, current health=%i, inflictor: %s, attacker: %s, damage=%.2f, damageBits=%i.\n", hostage->GetName ().GetData (), hostage->GetEdict ()->GetHealth (), inflictor->containingEntity->IsPlayer () ? inflictor->containingEntity->GetNetName ().GetData () : inflictor->containingEntity->GetClassName ().GetData (), attacker->containingEntity->GetClassName ().GetData (), damage, damageBits);
	#endif	// if defined _DEBUG
*/
	// pre call the original Hostage::TakeDamage() function for "hostage_entity" entity
	const BOOL result ((hostage->*originalFunction) (inflictor, attacker, damage, damageBits));

	// Is hostage died? (hook here, since hostage entity is not really died - he just become invisible....)
	if (!result)
	{
		// Reliability checks.
		InternalAssert (inflictor != NULL);
		InternalAssert (attacker != NULL);
		InternalAssert (inflictor->containingEntity->IsValid ());
		InternalAssert (inflictor->containingEntity->IsNotWorldspawnPlayer ());
/*
		#if defined _DEBUG
			Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Hostage::TakeDamage(): Hostage deied.\n");
		#endif	// if defined _DEBUG
*/
		g_server->GetHostageManager ().HostageKilled (hostage->GetEdict (), g_server->GetClientManager ()->GetClient (inflictor->containingEntity));
	}

	return result;
}

inline void Hostage_Use (const SDK::Classes::Hostage::FunctionUse_t originalFunction, SDK::Classes::Hostage *const hostage, SDK::Classes::BaseEntity *const activator, SDK::Classes::BaseEntity *const caller, SDK::Constants::UseType_t type, float value)
{
	// this function is called when two entities interact upon another (using a special "use"
	// flag). For example, a player is likely to press a switch for, let's say, opening a door or
	// raising an elevator. When the 'IN_USE' flag is set in the player's input buttons structure,
	// and some usable entity is in range, this function is called. The 'activator' pointer
	// represents the entity being used (button, switch, etc.), whereas the 'entity' pointer
	// points to the entity taking the action (usually a player).

	SDK::Classes::Edict *const thisEntity (hostage->GetEdict ());
	const SDK::Classes::Edict *const previousRescuerEdict (thisEntity->GetHostageRescuer ());

	// post call the original Hostage::Use() function for "func_button" entities
	(hostage->*originalFunction) (activator, caller, type, value);

	const SDK::Classes::Edict *const currentRescuerEdict (thisEntity->GetHostageRescuer ());

	if (previousRescuerEdict != currentRescuerEdict)
		g_server->GetHostageManager ().HostageUsePost (thisEntity, previousRescuerEdict == NULL ? NULL : g_server->GetClientManager ()->GetClient (previousRescuerEdict), currentRescuerEdict == NULL ? NULL : g_server->GetClientManager ()->GetClient (currentRescuerEdict));
}

inline void Killed (const SDK::Classes::BasePlayer::FunctionKilled_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Structures::EntityVariables_t *const killer, SDK::Constants::GibType_t shouldGib)
{
	// Reliability checks.
	InternalAssert (killer != NULL && killer->containingEntity != NULL);

//	Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Killed(): player: %s, killer: %s, shouldGib: %s.\n", player->GetEdict ()->GetClassName ().GetData (), killer->containingEntity->GetClassName ().GetData (), shouldGib == SDK::Constants::GibType_Normal ? "normal" : shouldGib == SDK::Constants::GibType_Never ? "never" : "always");

	// Called from here since "DeathMsg" not always informs on player kill.
	g_server->GetClientManager ()->ClientKilled (player->GetEdict (), killer->containingEntity);	// Notice client/bot that has been killed.

	(player->*originalFunction) (killer, shouldGib);
}

inline const BOOL AddPlayerItem (const SDK::Classes::BasePlayer::FunctionAddPlayerItem_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::BasePlayerItem *const item)
{
	// pre call the original BasePlayer::AddPlayerItem() function for "player" entity
	const BOOL result ((player->*originalFunction) (item));

	// If the item is really added...
	if (result)
	{
		// post send this event to client/bot for updating some variables....
		g_server->GetClientManager ()->ClientAddWeapon (player->GetEdict (), item->GetEdict ());
	}

	return result;
}

inline const BOOL RemovePlayerItem (const SDK::Classes::BasePlayer::FunctionRemovePlayerItem_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::BasePlayerItem *const item)
{
	// pre call the original BasePlayer::RemovePlayerItem() function for "player" entity
	const BOOL result ((player->*originalFunction) (item));

	// If the item is really removed...
	if (result)
	{
		// post send this event to client/bot for updating some variables....
		g_server->GetClientManager ()->ClientRemoveWeapon (player->GetEdict (), item->GetEdict ());
	}

	return result;
}

inline void Player_ImpulseCommands (const SDK::Classes::BasePlayer::FunctionImpulseCommands_t originalFunction, SDK::Classes::BasePlayer *const player)
{
	const SDK::Classes::Edict *const playerEntity (player->GetEdict ());

	// Reliability check.
	InternalAssert (playerEntity->IsValid ());

	if (playerEntity->variables.impulse > SDK::Constants::PlayerImpulseID_None)
		g_server->GetClientManager ()->ClientImpulseCommand (playerEntity, playerEntity->variables.impulse);

	(player->*originalFunction) ();
}

inline void Player_OnTouchingWeapon (const SDK::Classes::BasePlayer::FunctionOnTouchingWeapon_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::WeaponBox *const weaponBox)
{
	// Reliability check.
	InternalAssert (weaponBox != NULL && weaponBox->GetEdict ()->IsValid ());

//	Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "BasePlayer::OnTouchingWeapon(): Player \"%s\" touched weapon \"%s\".\n", weaponBox->GetEdict ()->GetClassName ().GetData (), player->GetEdict ()->GetClassName ().GetData ());

	(player->*originalFunction) (weaponBox);

	// post send this event to client/bot for updating some variables....
	g_server->GetClientManager ()->ClientOnTouchingWeapon (player->GetEdict (), weaponBox->GetEdict ());
}

inline void Player_Touch (const SDK::Classes::BasePlayer::FunctionTouch_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::BaseEntity *const other)
{
	// Reliability checks.
	InternalAssert (g_server->GetClientManager ()->GetClient (player->GetEdict ())->IsValid () && other != NULL && other->GetEdict () != NULL);

//	Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "BasePlayer::Touch(): Entity \"%s\" touched player \"%s\".\n", other->GetEdict ()->GetClassName ().GetData (), player->GetName ().GetData ());

	if (g_server->GetClientManager ()->GetClient (player->GetEdict ())->IsYaPB ())
		g_server->GetYaPBManager ()->PlayerTouched (player->GetEdict (), other->GetEdict ());

	(player->*originalFunction) (other);
}

inline void Touch (const SDK::Classes::BaseEntity::FunctionTouch_t originalFunction, SDK::Classes::BaseEntity *const entity, SDK::Classes::BaseEntity *const other)
{
	// Reliability checks.
	InternalAssert (other != NULL && other->GetEdict () != NULL);

//	Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Touch(): Entity \"%s\" touched entity \"%s\".\n", other->GetEdict ()->GetClassName ().GetData (), entity->GetEdict ()->GetClassName ().GetData ());

	(entity->*originalFunction) (other);

	if (other->GetEdict ()->IsPlayer ())
		g_server->GetClientManager ()->ClientTouchPost (other->GetEdict (), entity->GetEdict ());
}

inline void Use (const SDK::Classes::BaseEntity::FunctionUse_t originalFunction, SDK::Classes::BaseEntity *const entity, SDK::Classes::BaseEntity *const activator, SDK::Classes::BaseEntity *const caller, SDK::Constants::UseType_t type, float value)
{
	// this function is called when two entities interact upon another (using a special "use"
	// flag). For example, a player is likely to press a switch for, let's say, opening a door or
	// raising an elevator. When the 'IN_USE' flag is set in the player's input buttons structure,
	// and some usable entity is in range, this function is called. The 'activator' pointer
	// represents the entity being used (button, switch, etc.), whereas the 'entity' pointer
	// points to the entity taking the action (usually a player).

	const SDK::Classes::Edict *const thisEntity (entity->GetEdict ());
	const SDK::Classes::Edict *const activatorEntity (activator->GetEdict ());
	const SDK::Classes::Edict *const callerEntity (caller->GetEdict ());

//	Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Use(): Entity: \"%s\", activator: \"%s\", caller: \"%s\", use type: \"%s\", value == \"%f\".\n", thisEntity->IsValid () ? thisEntity->GetClassName ().GetData () : "NULL", activatorEntity->IsValid () ? activatorEntity->GetClassName () == "player" ? activatorEntity->GetNetName ().GetData () : activatorEntity->GetClassName ().GetData () : "NULL", callerEntity->IsValid () ? callerEntity->GetClassName () == "player" ? callerEntity->GetNetName ().GetData () : callerEntity->GetClassName ().GetData () : "NULL", type == SDK::Constants::UseType_Off ? "off" : type == SDK::Constants::UseType_On ? "on" : type == SDK::Constants::UseType_Set ? "set" : type == SDK::Constants::UseType_Toggle ? "toggle" : "ERROR!!!", value);

	// IsValid() - check added for KZ_* maps where activator is NULL on each player spawn...
	if (activatorEntity->IsValid () && activatorEntity->IsPlayer ())
		g_server->GetClientManager ()->ClientUse (activatorEntity, thisEntity, callerEntity, type, value);	// pre send this event to client/bot to know, that the necessary entity is really used...

	// post call the original Entity::Use() function for "func_button" entities
	(entity->*originalFunction) (activator, caller, type, value);
}

inline const BOOL Item_Deploy (const SDK::Classes::BasePlayerItem::FunctionDeploy_t originalFunction, SDK::Classes::BasePlayerItem *const item)
{
	// Reliability check.
	InternalAssert (item != NULL);

	// pre call the original BasePlayerItem::Deploy() function for "weapon" entity
	const BOOL result ((item->*originalFunction) ());

	SDK::Classes::Edict *const thisEntity (item->GetEdict ());

	// Reliability check.
	if (thisEntity->IsValid ())	// OCCURS!
	{
		// post send this event to client/bot current weapon for updating some variables....
		g_server->GetClientManager ()->ClientItemDeployPost (thisEntity->GetPrivateDataReference <SDK::Classes::BaseEntity *const> (SDK::Constants::OFFSET_WEAPON_OWNER)->GetEdict (), thisEntity);
	}

	return result;
}

inline void Weapon_PrimaryAttack (const SDK::Classes::BasePlayerWeapon::FunctionReload_t originalFunction, SDK::Classes::BasePlayerWeapon *const weapon)
{
	const SDK::Classes::Edict *const thisEntity (weapon->GetEdict ());

	// Reliability checks.
	InternalAssert (thisEntity->IsValid ());

	const Client *const client (g_server->GetClientManager ()->GetClient (weapon->m_pPlayer->GetEdict ()));
	const SDK::Constants::WeaponID_t currentWeaponID (client->GetCurrentWeapon ()->GetID ());
	const float MY_vecSpread (client->GetCurrentWeapon ()->GetSpreadOffset ());
	const float MY_flDistance (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].maximumShootDistance1);
	const int MY_iPenetration (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].penetrationPower2);
	const int MY_iBulletType (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].bulletType);
	const int MY_iDamage (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].damage1);
	const float MY_flRangeModifier (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].obstaclePierce1);
	const Math::Vector2D MY_result (client->GetCurrentWeapon ()->GetSpreadOffset2D ());
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%s::%s::PrimaryAttack(): MY_vecSpread=%.5f, MY_flDistance=%.2f, MY_iPenetration=%i, MY_iBulletType=%i, MY_iDamage=%i, MY_flRangeModifier=%.2f, MY_shared_rand=%u, MY_result=[%.10f, %.10f].\n", weapon->m_pPlayer->GetEdict ()->GetNetName ().GetData (), thisEntity->GetClassName ().GetData (), MY_vecSpread, MY_flDistance, MY_iPenetration, MY_iBulletType, MY_iDamage, MY_flRangeModifier, weapon->m_pPlayer->random_seed, MY_result.x, MY_result.y);

	// pre call the original BasePlayerWeapon::PrimaryAttack() function for "weapon" entity
	(weapon->*originalFunction) ();
}

inline void Weapon_Reload (const SDK::Classes::BasePlayerWeapon::FunctionReload_t originalFunction, SDK::Classes::BasePlayerWeapon *const weapon)
{
	const SDK::Classes::Edict *const thisEntity (weapon->GetEdict ());

	// Reliability checks.
	InternalAssert (thisEntity->IsValid ());
	InternalAssert (g_currentReloader == NULL);

	g_currentReloader = g_server->GetClientManager ()->GetClient (thisEntity->GetPrivateDataReference <SDK::Classes::BaseEntity *const> (SDK::Constants::OFFSET_WEAPON_OWNER)->GetEdict ());
	g_isFirstReloadSoundMessageCall = true;

	// Reliability check.
	InternalAssert (g_currentReloader->IsValid ());

	// pre call the original BasePlayerWeapon::Reload() function for "weapon" entity
	(weapon->*originalFunction) ();

	#if defined _DEBUG
		g_currentReloader = NULL;
	#endif	// if defined _DEBUG

	// 'm_isInReload' is set to TRUE in BasePlayerWeapon::DefaultReload()
	// We are only checking if this variable is set to true because
	// BasePlayerWeapon::Reload() is being called as long as player holds the reload key
	// But its only called once if weapon is being reloaded, and doesnt call more while its reloading
	if (!thisEntity->GetPrivateDataReference <bool> (SDK::Constants::OFFSET_WEAPON_IN_RELOAD))
		return;

	// Weapon has started reload....

	// post send this event to client/bot current weapon for updating some variables....
	g_server->GetClientManager ()->ClientWeaponReloadPost (thisEntity->GetPrivateDataReference <SDK::Classes::BaseEntity *const> (SDK::Constants::OFFSET_WEAPON_OWNER)->GetEdict (), thisEntity);
}

inline void Weapon_Shotgun_Reload (const SDK::Classes::BasePlayerWeapon::FunctionReload_t originalFunction, SDK::Classes::BasePlayerWeapon *const shotgun)
{
	const SDK::Classes::Edict *const thisEntity (shotgun->GetEdict ());

	// Reliability check.
	InternalAssert (thisEntity->IsValid ());

	const unsigned char shotgunSpecialReloadStatePre (static_cast <const unsigned char> (thisEntity->GetPrivateDataReference <unsigned int> (SDK::Constants::Offset_Weapon_SpecialReloadState)));

	g_currentReloader = g_server->GetClientManager ()->GetClient (thisEntity->GetPrivateDataReference <SDK::Classes::BaseEntity *const> (SDK::Constants::OFFSET_WEAPON_OWNER)->GetEdict ());
	g_isFirstReloadSoundMessageCall = true;

	// Reliability check.
	InternalAssert (g_currentReloader->IsValid ());

	// pre call the original BasePlayerWeapon::Reload() function for "weapon" entity
	(shotgun->*originalFunction) ();

	#if defined _DEBUG
		g_currentReloader = NULL;
	#endif	// if defined _DEBUG

	// in Reload() there is switch of m_specialReloadState variable
	// On case 0 it is being set to 1 and some weapon idle/nextattacks are being set to 0.55
	// case 1: bullet input animation
	// case 2: adding bullet to clip itself
	// this case 1-2-1 is being looped while the shotgun is reloading
	if (shotgunSpecialReloadStatePre != 0u || thisEntity->GetPrivateDataReference <unsigned int> (SDK::Constants::Offset_Weapon_SpecialReloadState) != 1u)
		return;

	// Shotgun has started reload....

	// post send this event to client/bot current weapon for updating some variables....
	g_server->GetClientManager ()->ClientWeaponReloadPost (thisEntity->GetPrivateDataReference <SDK::Classes::BaseEntity *const> (SDK::Constants::OFFSET_WEAPON_OWNER)->GetEdict (), thisEntity);
}

#endif	// ifndef HALF_LIFE_ENGINE_EXPORT_REPLACED_FUNCTIONS_ENTITY_INCLUDED