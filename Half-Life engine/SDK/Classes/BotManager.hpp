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
// BotManager.hpp
//
// Class: BotManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BOT_MANAGER_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_BOT_MANAGER_INCLUDED

class BotManager : public VoidBaseHookedClassWithVirtualFunctionTable
{
	//
	// Group: Type definitions.
	//
	public:
		typedef void (BotManager::*FunctionOnEvent_t) (const Constants::GameEventType_t gameEvent, Classes::BaseEntity *const argument1, void *const argument2);

	public:
		enum MapType_t
		{
			MapType_None,

			MapType_De,	// Defusion map.
			MapType_Cs,	// Rescue map.
			MapType_As	// Assassination map.
		};

		enum { MaximumMapZones = 4u };

		struct MapZone_t
		{
			enum { MaximumAreas = 16u };

/*!off=0(0)/20(5)*/			BaseEntity     *entity;
/*!off=4(1)/24(6)*/			NavigationArea *containingAreas[MaximumAreas];	// Areas that Math::IsBoundingBoxesTouching() with ours 'absoluteBoundingBox'.
/*!off=68(17)/88(22)*/			unsigned int    containingAreasNumber;
/*!off=72(18)/92(23)*/			Math::Vector3D  center;
/*!off=84(21)/104(26)*/			bool            isInfoBombTargetZone;			// Is "info_bomb_target" entity containing?
/*!off=88(22)/108(27)*/			unsigned int    index;							// This zone index.
/*!off=mins[92(23)/112(28)],maxs[104(26)/124(31)]*/			Math::Extent3D  absoluteBoundingBox;			// (if 'isBombPlantZone' always: mins -256.0, maxs 256.0)
		};	// sizeof (MapZone_t) == 116 (29)

		class ActiveGrenade
		{
			public:
/*!off=0(0)*/				Constants::WeaponID_t  m_id;
/*!off=4(1)*/				CGrenade              *m_entity;
/*!off=8(2)*/				Math::Vector3D         m_position;
/*!off=20(5)*/				float                  m_explodeTime;

			public:
				/// @todo !!!
//				inline const bool IsValid (void) const { return m_entity != NULL || m_explodeTime >= Globals::g_halfLifeEngine->GetTime (); }
		};	// sizeof (ActiveGrenade) == 24 (6)
		typedef CUtlLinkedList <ActiveGrenade *> ActiveGrenadeList_t;

		enum Offset_t
		{
			Offset_Pointer        = Constants::GameRules::Offset_Pointer - 1u + sizeof (GameRules *) + 26u,	// Offset to 'TheBots' pointer from Counter-Strike.

			//----------------------------------------------------------------------------------------------------------------------------------------------------------
			Offset_MapType        = 4u,		// Map type (as MapType_t)
			Offset_MapZones       = 5u,		// ? (as MapZone_t[MaximumMapZones])
			Offset_MapZonesNumber = 121u	// (as unsigned int)
		};

		enum VirtualTableFunctionID_t
		{
			// void BotManager::OnEvent (Constants::GameEventType_t, BaseEntity *, void *)
			VirtualTableFunctionID_OnEvent = 9u
		};

	public:
/*!off=4(1)*/		ActiveGrenadeList_t m_activeGrenades;	// Thrown smoke grenades.

		// Only for CS: CZ!!!
/*!off=16(4)*/		MapType_t           m_mapType;	// Scenario.
/*!off=20(5)*/		MapZone_t           m_mapZones[MaximumMapZones];
/*!off=484(121)*/		unsigned int        m_mapZonesNumber;
#if 0
	public:
		
#endif	// if 0
};

class CSBotManager : public BotManager
{
	public:
		enum Offset_t
		{
			Offset_RadioMessageTimestamps           = 1u,	// (as float * ?) (Using: reinterpret_cast <float *> (this)[teamID + 2u * 'Constants::GameEventType_RADIO_*' + Offset_RadioMessageTimestamps])

			Offset_C4_IsPlanted                     = 488u,	// (as bool)
			Offset_C4_PlantedTime                   = 123u,	// (as float)

			Offset_C4_Defuser                       = 125u,	// (as BasePlayer *)
			Offset_LoosedC4                         = 126u,	// (as Edict *, but EntityHandle)
			Offset_LoosedC4NavigationArea           = 128u,	// (as NavigationArea *)
			Offset_IsRoundEnded                     = 516u,	// (as bool)

			Offset_LastEnemySpottedRadioCommandTime = 178u,	// (as float)
			Offset_FreezeEndTime                    = 179u	// (as float)
		};

	public:
/*!off=488(122)*/		bool                       m_isBombPlanted;
/*!off=492(123)*/		float                      m_bombPlantTimestamp;
/*!off=496(124)*/		float                      m_timeToPlantBomb;	// Random time after which bots can plant bomb.
/*!off=500(125)*/		BasePlayer                *m_bombDefuser;
/*!off=504(126)*/		EntityHandle               m_looseBomb;
/*!off=512(128)*/		NavigationArea            *m_looseBombArea;
/*!off=516(129)*/		bool                       m_isRoundOver;
/*!off=520(130)*/		float                      m_radioMessageTimestamps[Constants::MaximumPlayableTeamsNumber][Constants::GameEventType_RADIO_Total];	// Note: Only for ZBot!	//! @warning DON'T USE ME!!!
/*!off=712(178)*/		float                      m_lastSeenEnemyTimestamp;	// last enemy spotted radio command timestamp
/*!off=716(179)*/		float                      m_roundStartTime;	// freeze end timestamp
/*!off=720(180)*/		bool                       m_isDefenseRushing;
/*!off=724(181)*/		Place_t                    m_currentNavPlace;
/*!off=728(182)*/		Utilities::CountdownTimer  m_UNKNOWN1;	// Sets to 0, -1.0f in CSBotManager::CSBotManager() function and never used?!?!?!?!?!??!?!?!??! (@warning NOT SHURE ABOUT THIS!!!)
/*!off=736(184)*/		bool                       m_UNKNOWN2;	// Sets to false in CSBotManager::RestartRound() function and never used?!?!?!?!?!??!?!?!??!
/*!off=737(184.25)*/		bool                       m_UNKNOWN3;	// Sets to true in CSBotManager::RestartRound() function and never used?!?!?!?!?!??!?!?!??!
#if 0
	public:
		CSBotManager (void);

	public:
		virtual BOOL ClientCommand (BasePlayer *pPlayer, const char *pcmd);
		virtual void ServerActivate (void);
		virtual void ServerDeactivate (void);
		virtual void ServerCommand (char const *command);
		virtual void AddServerCommand (char const *command);
		virtual void AddServerCommands (void);
		virtual void RestartRound (void);
		virtual void StartFrame (void);
		virtual void OnEvent (Constants::GameEventType_t event, BaseEntity *argument1, void *argument2);
		virtual void GetPlayerPriority (BasePlayer *player) const;
		virtual bool IsImportantPlayer (BasePlayer *player) const;
#endif	// if 0
};	// sizeof (CSBotManager) == 740 (185)

#endif // HALF_LIFE_ENGINE_SDK_CLASSES_BOT_MANAGER_INCLUDED