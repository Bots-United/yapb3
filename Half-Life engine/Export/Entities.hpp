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
// Entities.hpp
//
// Description: Export entities from mod DLL back to the Half-Life engine.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_EXPORT_ENTITIES_INCLUDED
#define HALF_LIFE_ENGINE_EXPORT_ENTITIES_INCLUDED

// Handy definition for exporting the MOD entities.
#define LINK_ENTITY_TO_CLASS(EntityFunction)														\
	EXPORT inline void EntityFunction (SDK::Structures::EntityVariables_t *const entityVariables)	\
	{																								\
		/* Call the entity function.... */															\
		Globals::g_halfLifeEngine->CallGameEntity (#EntityFunction, entityVariables[0u]);			\
	}																								\
/*! This forwarder functions to game DLL is not needed anymore, dynamic linkent code handles this too!
// Entities in Counter-Strike 1.6...
LINK_ENTITY_TO_CLASS (DelayedUse);
LINK_ENTITY_TO_CLASS (ambient_generic);
LINK_ENTITY_TO_CLASS (ammo_338magnum);
LINK_ENTITY_TO_CLASS (ammo_357sig);
LINK_ENTITY_TO_CLASS (ammo_45acp);
LINK_ENTITY_TO_CLASS (ammo_50ae);
LINK_ENTITY_TO_CLASS (ammo_556nato);
LINK_ENTITY_TO_CLASS (ammo_556natobox);
LINK_ENTITY_TO_CLASS (ammo_57mm);
LINK_ENTITY_TO_CLASS (ammo_762nato);
LINK_ENTITY_TO_CLASS (ammo_9mm);
LINK_ENTITY_TO_CLASS (ammo_buckshot);
LINK_ENTITY_TO_CLASS (armoury_entity);
LINK_ENTITY_TO_CLASS (beam);
LINK_ENTITY_TO_CLASS (bodyque);
LINK_ENTITY_TO_CLASS (button_target);
LINK_ENTITY_TO_CLASS (cycler);
LINK_ENTITY_TO_CLASS (cycler_prdroid);
LINK_ENTITY_TO_CLASS (cycler_sprite);
LINK_ENTITY_TO_CLASS (cycler_weapon);
LINK_ENTITY_TO_CLASS (cycler_wreckage);
LINK_ENTITY_TO_CLASS (env_beam);
LINK_ENTITY_TO_CLASS (env_beverage);
LINK_ENTITY_TO_CLASS (env_blood);
LINK_ENTITY_TO_CLASS (env_bombglow);
LINK_ENTITY_TO_CLASS (env_bubbles);
LINK_ENTITY_TO_CLASS (env_debris);
LINK_ENTITY_TO_CLASS (env_explosion);
LINK_ENTITY_TO_CLASS (env_fade);
LINK_ENTITY_TO_CLASS (env_funnel);
LINK_ENTITY_TO_CLASS (env_global);
LINK_ENTITY_TO_CLASS (env_glow);
LINK_ENTITY_TO_CLASS (env_laser);
LINK_ENTITY_TO_CLASS (env_lightning);
LINK_ENTITY_TO_CLASS (env_message);
LINK_ENTITY_TO_CLASS (env_rain);
LINK_ENTITY_TO_CLASS (env_render);
LINK_ENTITY_TO_CLASS (env_shake);
LINK_ENTITY_TO_CLASS (env_shooter);
LINK_ENTITY_TO_CLASS (env_snow);
LINK_ENTITY_TO_CLASS (env_sound);
LINK_ENTITY_TO_CLASS (env_spark);
LINK_ENTITY_TO_CLASS (env_sprite);
LINK_ENTITY_TO_CLASS (fireanddie);
LINK_ENTITY_TO_CLASS (func_bomb_target);
LINK_ENTITY_TO_CLASS (func_breakable);
LINK_ENTITY_TO_CLASS (func_button);
LINK_ENTITY_TO_CLASS (func_buyzone);
LINK_ENTITY_TO_CLASS (func_conveyor);
LINK_ENTITY_TO_CLASS (func_door);
LINK_ENTITY_TO_CLASS (func_door_rotating);
LINK_ENTITY_TO_CLASS (func_escapezone);
LINK_ENTITY_TO_CLASS (func_friction);
LINK_ENTITY_TO_CLASS (func_grencatch);
LINK_ENTITY_TO_CLASS (func_guntarget);
LINK_ENTITY_TO_CLASS (func_healthcharger);
LINK_ENTITY_TO_CLASS (func_hostage_rescue);
LINK_ENTITY_TO_CLASS (func_illusionary);
LINK_ENTITY_TO_CLASS (func_ladder);
LINK_ENTITY_TO_CLASS (func_monsterclip);
LINK_ENTITY_TO_CLASS (func_mortar_field);
LINK_ENTITY_TO_CLASS (func_pendulum);
LINK_ENTITY_TO_CLASS (func_plat);
LINK_ENTITY_TO_CLASS (func_platrot);
LINK_ENTITY_TO_CLASS (func_pushable);
LINK_ENTITY_TO_CLASS (func_rain);
LINK_ENTITY_TO_CLASS (func_recharge);
LINK_ENTITY_TO_CLASS (func_rot_button);
LINK_ENTITY_TO_CLASS (func_rotating);
LINK_ENTITY_TO_CLASS (func_snow);
LINK_ENTITY_TO_CLASS (func_tank);
LINK_ENTITY_TO_CLASS (func_tankcontrols);
LINK_ENTITY_TO_CLASS (func_tanklaser);
LINK_ENTITY_TO_CLASS (func_tankmortar);
LINK_ENTITY_TO_CLASS (func_tankrocket);
LINK_ENTITY_TO_CLASS (func_trackautochange);
LINK_ENTITY_TO_CLASS (func_trackchange);
LINK_ENTITY_TO_CLASS (func_tracktrain);
LINK_ENTITY_TO_CLASS (func_train);
LINK_ENTITY_TO_CLASS (func_traincontrols);
LINK_ENTITY_TO_CLASS (func_vehicle);
LINK_ENTITY_TO_CLASS (func_vehiclecontrols);
LINK_ENTITY_TO_CLASS (func_vip_safetyzone);
LINK_ENTITY_TO_CLASS (func_wall);
LINK_ENTITY_TO_CLASS (func_wall_toggle);
LINK_ENTITY_TO_CLASS (func_water);
LINK_ENTITY_TO_CLASS (func_weaponcheck);
LINK_ENTITY_TO_CLASS (game_counter);
LINK_ENTITY_TO_CLASS (game_counter_set);
LINK_ENTITY_TO_CLASS (game_end);
LINK_ENTITY_TO_CLASS (game_player_equip);
LINK_ENTITY_TO_CLASS (game_player_hurt);
LINK_ENTITY_TO_CLASS (game_player_team);
LINK_ENTITY_TO_CLASS (game_score);
LINK_ENTITY_TO_CLASS (game_team_master);
LINK_ENTITY_TO_CLASS (game_team_set);
LINK_ENTITY_TO_CLASS (game_text);
LINK_ENTITY_TO_CLASS (game_zone_player);
LINK_ENTITY_TO_CLASS (gibshooter);
LINK_ENTITY_TO_CLASS (grenade);
LINK_ENTITY_TO_CLASS (hostage_entity);
LINK_ENTITY_TO_CLASS (info_bomb_target);
LINK_ENTITY_TO_CLASS (info_hostage_rescue);
LINK_ENTITY_TO_CLASS (info_intermission);
LINK_ENTITY_TO_CLASS (info_landmark);
LINK_ENTITY_TO_CLASS (info_map_parameters);
LINK_ENTITY_TO_CLASS (info_null);
LINK_ENTITY_TO_CLASS (info_player_deathmatch);
LINK_ENTITY_TO_CLASS (info_player_start);
LINK_ENTITY_TO_CLASS (info_target);
LINK_ENTITY_TO_CLASS (info_teleport_destination);
LINK_ENTITY_TO_CLASS (info_vip_start);
LINK_ENTITY_TO_CLASS (infodecal);
LINK_ENTITY_TO_CLASS (item_airtank);
LINK_ENTITY_TO_CLASS (item_antidote);
LINK_ENTITY_TO_CLASS (item_assaultsuit);
LINK_ENTITY_TO_CLASS (item_battery);
LINK_ENTITY_TO_CLASS (item_healthkit);
LINK_ENTITY_TO_CLASS (item_kevlar);
LINK_ENTITY_TO_CLASS (item_longjump);
LINK_ENTITY_TO_CLASS (item_security);
LINK_ENTITY_TO_CLASS (item_sodacan);
LINK_ENTITY_TO_CLASS (item_suit);
LINK_ENTITY_TO_CLASS (item_thighpack);
LINK_ENTITY_TO_CLASS (light);
LINK_ENTITY_TO_CLASS (light_environment);
LINK_ENTITY_TO_CLASS (light_spot);
LINK_ENTITY_TO_CLASS (momentary_door);
LINK_ENTITY_TO_CLASS (momentary_rot_button);
LINK_ENTITY_TO_CLASS (monster_hevsuit_dead);
LINK_ENTITY_TO_CLASS (monster_mortar);
LINK_ENTITY_TO_CLASS (monster_scientist);
LINK_ENTITY_TO_CLASS (multi_manager);
LINK_ENTITY_TO_CLASS (multisource);
LINK_ENTITY_TO_CLASS (path_corner);
LINK_ENTITY_TO_CLASS (path_track);
LINK_ENTITY_TO_CLASS (player);	// This function calls game.DLL player() function, in case to create player entity in game.
LINK_ENTITY_TO_CLASS (player_loadsaved);
LINK_ENTITY_TO_CLASS (player_weaponstrip);
LINK_ENTITY_TO_CLASS (soundent);
LINK_ENTITY_TO_CLASS (spark_shower);
LINK_ENTITY_TO_CLASS (speaker);
LINK_ENTITY_TO_CLASS (target_cdaudio);
LINK_ENTITY_TO_CLASS (test_effect);
LINK_ENTITY_TO_CLASS (trigger);
LINK_ENTITY_TO_CLASS (trigger_auto);
LINK_ENTITY_TO_CLASS (trigger_autosave);
LINK_ENTITY_TO_CLASS (trigger_camera);
LINK_ENTITY_TO_CLASS (trigger_cdaudio);
LINK_ENTITY_TO_CLASS (trigger_changelevel);
LINK_ENTITY_TO_CLASS (trigger_changetarget);
LINK_ENTITY_TO_CLASS (trigger_counter);
LINK_ENTITY_TO_CLASS (trigger_endsection);
LINK_ENTITY_TO_CLASS (trigger_gravity);
LINK_ENTITY_TO_CLASS (trigger_hurt);
LINK_ENTITY_TO_CLASS (trigger_monsterjump);
LINK_ENTITY_TO_CLASS (trigger_multiple);
LINK_ENTITY_TO_CLASS (trigger_once);
LINK_ENTITY_TO_CLASS (trigger_push);
LINK_ENTITY_TO_CLASS (trigger_relay);
LINK_ENTITY_TO_CLASS (trigger_teleport);
LINK_ENTITY_TO_CLASS (trigger_transition);
LINK_ENTITY_TO_CLASS (weapon_ak47);
LINK_ENTITY_TO_CLASS (weapon_aug);
LINK_ENTITY_TO_CLASS (weapon_awp);
LINK_ENTITY_TO_CLASS (weapon_c4);
LINK_ENTITY_TO_CLASS (weapon_deagle);
LINK_ENTITY_TO_CLASS (weapon_elite);
LINK_ENTITY_TO_CLASS (weapon_famas);
LINK_ENTITY_TO_CLASS (weapon_fiveseven);
LINK_ENTITY_TO_CLASS (weapon_flashbang);
LINK_ENTITY_TO_CLASS (weapon_g3sg1);
LINK_ENTITY_TO_CLASS (weapon_galil);
LINK_ENTITY_TO_CLASS (weapon_glock18);
LINK_ENTITY_TO_CLASS (weapon_hegrenade);
LINK_ENTITY_TO_CLASS (weapon_knife);
LINK_ENTITY_TO_CLASS (weapon_m249);
LINK_ENTITY_TO_CLASS (weapon_m3);
LINK_ENTITY_TO_CLASS (weapon_m4a1);
LINK_ENTITY_TO_CLASS (weapon_mac10);
LINK_ENTITY_TO_CLASS (weapon_mp5navy);
LINK_ENTITY_TO_CLASS (weapon_p228);
LINK_ENTITY_TO_CLASS (weapon_p90);
LINK_ENTITY_TO_CLASS (weapon_scout);
LINK_ENTITY_TO_CLASS (weapon_sg550);
LINK_ENTITY_TO_CLASS (weapon_sg552);
LINK_ENTITY_TO_CLASS (weapon_shield);
LINK_ENTITY_TO_CLASS (weapon_smokegrenade);
LINK_ENTITY_TO_CLASS (weapon_tmp);
LINK_ENTITY_TO_CLASS (weapon_ump45);
LINK_ENTITY_TO_CLASS (weapon_usp);
LINK_ENTITY_TO_CLASS (weapon_xm1014);
LINK_ENTITY_TO_CLASS (weaponbox);
LINK_ENTITY_TO_CLASS (world_items);
LINK_ENTITY_TO_CLASS (worldspawn);
*/
#endif	// ifndef HALF_LIFE_ENGINE_EXPORT_ENTITIES_INCLUDED