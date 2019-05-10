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
// Localizer.hpp
//
// Class: Localizer
//
// Description: Texts localizer.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef LOCALIZER_INCLUDED
#define LOCALIZER_INCLUDED

class Localizer
{
	//
	// Group: Constants.
	//
	public:
		#define LOCALIZER_CONFIG_FILENAME                "language"
		#define LOCALIZER_CONFIG_FILENAME_EXTANTION      "cfg"
		#define LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION LOCALIZER_CONFIG_FILENAME "." LOCALIZER_CONFIG_FILENAME_EXTANTION

		enum LocalizedStringID_t
		{
			LocalizedStringID_None = -1,

//			LocalizedStringID_Empty,	/// MAYBE I SHOULD REMOVE IT?????????????????????????????????????????????????????????????????????????????????

			#if defined _DEBUG
			#	pragma message ("\n\tThere stills some unknown translations!!!\n")
			#endif	// if defined _DEBUG

				LocalizedStringID_TODO,	/// @warning This should be removed in release build, used for translations thats should be involved later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			LocalizedStringID_UnableToCreateYaPB_Reason,
			LocalizedStringID_BothTeamsReachedMaximumPlayers,
			LocalizedStringID_MaximumPlayersReached,
			LocalizedStringID_InvalidSkillGivenUsingRandomSkill,
			LocalizedStringID_ConnectingBot,
			LocalizedStringID_FillServerWithBots,
			LocalizedStringID_YaPBs,
			LocalizedStringID_Clients,
			LocalizedStringID_Humans,
			LocalizedStringID_FakeClients,
			LocalizedStringID_AllKickedFromServer,
			LocalizedStringID_YaPBKicked,
			LocalizedStringID_AllKilled,
			LocalizedStringID_BotsHealthIsSetTo,
			LocalizedStringID_ThereAreNoYaPBOnTheServer,
			LocalizedStringID_WeaponModeSelected,
			LocalizedStringID_ChooseWeaponFrom1To7Range,
			LocalizedStringID_PleaseSpecifyOrderCommand,
			LocalizedStringID_PlayerIsNotYaPB,
			LocalizedStringID_AllBotsHasExecutedCommand,
			LocalizedStringID_BotHasExecutedCommand,
			LocalizedStringID_BotWithThatUserIDNotFound,
			LocalizedStringID_BotWithThatNameNotFound,
			LocalizedStringID_CurrentTimeIs,
			LocalizedStringID_CurrentDateIs,
			LocalizedStringID_PleaseSpecifyHealth,
			LocalizedStringID_YouDeadAndHaveNoAccessToThisMenu,
			LocalizedStringID_ExecutingMainConfigurationFile,
			LocalizedStringID_NoMainConfigurationFileFound,
			LocalizedStringID_ExecutingMapSpecificConfigurationFile,
			LocalizedStringID_NoMapSpecificConfigurationFileFound,
			LocalizedStringID_PlayerHadLostRemoteAccessToYaPB,
			LocalizedStringID_PlayerHadGainedFullRemoteAccessToYaPB,
			LocalizedStringID_CommandNotSupportedOnDedicatedServer,
			LocalizedStringID_AutoSelect,

			LocalizedStringID_MOTD_Title_BotsList,

			// Waypoints....
			LocalizedStringID_MapNotWaypointed,
			LocalizedStringID_WaypointsHasBeenChanged,
			LocalizedStringID_PathAlreadyExists,
			LocalizedStringID_PathAddedFromTo,
			LocalizedStringID_ThisIsNotCampingWaypoint,
			LocalizedStringID_CannotConnectWaypointToItself,

			// Navigation mesh....
			LocalizedStringID_NavigationMesh_NotExits,

			// Console variables....
			LocalizedStringID_Variables_StartOffset,	/// @note This is not a translation/variable, just starting index of console variables in a 'LocalizedStringID_t' enumeration!

			// Bot console variables....
			LocalizedStringID_Variable_AutoVacate = LocalizedStringID_Variables_StartOffset,
			LocalizedStringID_Variable_Quota,
			LocalizedStringID_Variable_QuotaMode,
			LocalizedStringID_Variable_MinSkill,
			LocalizedStringID_Variable_MaxSkill,
			LocalizedStringID_Variable_FollowUser,
			LocalizedStringID_Variable_TimerSound,
			LocalizedStringID_Variable_TimerPickup,
			LocalizedStringID_Variable_TimerGrenade,
			LocalizedStringID_Variable_DebugGoal,
			LocalizedStringID_Variable_Chat,
			LocalizedStringID_Variable_Synth,
			LocalizedStringID_Variable_KnifeMode,
			LocalizedStringID_Variable_SkillTags,
			LocalizedStringID_Variable_Stop,
			LocalizedStringID_Variable_ThruWalls,
			LocalizedStringID_Variable_WallSensitivity,
			LocalizedStringID_Variable_Votes,
			LocalizedStringID_Variable_Spray,
			LocalizedStringID_Variable_BotBuy,

			#if defined _DEBUG
				LocalizedStringID_Variable_Debug,
			#endif	// if defined _DEBUG

			LocalizedStringID_Variable_Prefix,
			LocalizedStringID_Variable_Language,
			LocalizedStringID_Variable_EnableClientLanguages,
			LocalizedStringID_Variable_Version,
			LocalizedStringID_Variable_NavPointLookMethod,
			LocalizedStringID_Variable_AimMethod,
			LocalizedStringID_Variable_TargetingMethod,
			LocalizedStringID_Variable_DamperCoeff_X,
			LocalizedStringID_Variable_DamperCoeff_Y,
			LocalizedStringID_Variable_Deviation_X,
			LocalizedStringID_Variable_Deviation_Y,
			LocalizedStringID_Variable_Influence_XonY,
			LocalizedStringID_Variable_Influence_YonX,
			LocalizedStringID_Variable_SlowdownRatio,
			LocalizedStringID_Variable_OffsetDelay,
			LocalizedStringID_Variable_SpringStiff_X,
			LocalizedStringID_Variable_SpringStiff_Y,
			LocalizedStringID_Variable_AnticipRatio,
			LocalizedStringID_Variable_Password,
			LocalizedStringID_Variable_PasswordKey,
			LocalizedStringID_Variable_ChatterPath,
			LocalizedStringID_Variable_TKPunish,
			LocalizedStringID_Variable_Communication,
			LocalizedStringID_Variable_Economics,
			LocalizedStringID_Variable_ForceTeam,
			LocalizedStringID_Variable_RestrictedWeapons,
			LocalizedStringID_Variable_DangerFactor,
			LocalizedStringID_Variable_DontShoot,
			LocalizedStringID_Variable_HardcoreMode,
			LocalizedStringID_Variable_AutofollowPercent,
			LocalizedStringID_Variable_ExplosiveGrenadeUsePercent,
			LocalizedStringID_Variable_FlashBangGrenadeUsePercent,
			LocalizedStringID_Variable_SmokeGrenadeUsePercent,
			LocalizedStringID_Variable_LogoTypes,

			#if defined MULTIPLE_MSEC_METHODS
				LocalizedStringID_Variable_MsecMethod,
			#endif	// if defined MULTIPLE_MSEC_METHODS

			LocalizedStringID_Variable_LogLevel,
			LocalizedStringID_Variable_Config_LoadTime,
			LocalizedStringID_Variable_WalkAllowed,
			LocalizedStringID_Variable_Deathmatch,
			LocalizedStringID_Variable_ThinkFPS,
			LocalizedStringID_Variable_Ping,		// The ping you want displayed (min: 0, max: 4095).
			LocalizedStringID_Variable_PingFlux,	// Fake ping fluctuation amount (0 = none).
			LocalizedStringID_Variable_DeferToHuman,

			// Voice system....
			LocalizedStringID_Variable_Voice_inputfromfile,
			LocalizedStringID_Variable_Voice_recordtofile,
			LocalizedStringID_Variable_Voice_avggain,
			LocalizedStringID_Variable_Voice_maxgain,
			LocalizedStringID_Variable_Voice_scale,
			LocalizedStringID_Variable_Voice_loopback,
			LocalizedStringID_Variable_Voice_fadeouttime,		// It fades to no sound at the tail end of your voice data when you release the key.

			// Debugging cvars.
			LocalizedStringID_Variable_Voice_profile,
			LocalizedStringID_Variable_Voice_showchannels,		// 1 = list channels, 2 = show timing info, etc
			LocalizedStringID_Variable_Voice_showincoming,		// show incoming voice data

			LocalizedStringID_Variable_Voice_forcemicrecord,	// Have it force your mixer control settings so waveIn comes from the microphone. CD rippers change your waveIn to come from the CD drive

			LocalizedStringID_Variable_Voice_overdrive,			// When voice is on, all other sounds are decreased by this factor.
			LocalizedStringID_Variable_Voice_overdrivefadetime,	// How long it takes to fade in and out of the voice overdrive.

			LocalizedStringID_Variable_MaximumAFKTime,

			#if defined MULTIPLE_DRAW_METHODS
				LocalizedStringID_Variable_DrawMethod,
			#endif	// if defined MULTIPLE_DRAW_METHODS

			// Navigation mesh console variables....
			LocalizedStringID_Variable_nav_edit,
			LocalizedStringID_Variable_nav_quicksave,
			LocalizedStringID_Variable_nav_show_hiding_spots,
			LocalizedStringID_Variable_nav_show_approach_points,
			LocalizedStringID_Variable_nav_show_danger,
			LocalizedStringID_Variable_nav_show_player_counts,
			LocalizedStringID_Variable_nav_show_occupy_time,
			LocalizedStringID_Variable_nav_show_battlefront,
			LocalizedStringID_Variable_nav_show_area_info,
			LocalizedStringID_Variable_nav_snap_to_grid,
			LocalizedStringID_Variable_nav_create_place_on_ground,
			LocalizedStringID_Variable_nav_create_area_at_feet,
			LocalizedStringID_Variable_nav_show_nodes,
			LocalizedStringID_Variable_nav_show_compass,
			LocalizedStringID_Variable_nav_slope_limit,
			LocalizedStringID_Variable_nav_slope_tolerance,
			LocalizedStringID_Variable_nav_restart_after_analysis,

			#if defined DEBUG_NAV_NODES
				LocalizedStringID_Variable_nav_show_node_id,
				LocalizedStringID_Variable_nav_test_node,
				LocalizedStringID_Variable_nav_test_node_crouch,
				LocalizedStringID_Variable_nav_test_node_crouch_dir,
			#endif	// if defined DEBUG_NAV_NODES

			LocalizedStringID_Variable_nav_coplanar_slope_limit,
			LocalizedStringID_Variable_nav_split_place_on_ground,
			#if defined MULTIPLE_DRAW_METHODS
				LocalizedStringID_Variable_nav_area_bgcolor,
			#endif	// if defined MULTIPLE_DRAW_METHODS
			LocalizedStringID_Variable_nav_area_max_size,
			LocalizedStringID_Variable_nav_corner_adjust_adjacent,

			LocalizedStringID_Variable_NavigationMesh_MaximumOffPlaneTolerance,
			LocalizedStringID_Variable_NavigationMesh_MaximumSampleStepTime,

			LocalizedStringID_Variable_NavigationMesh_GenerateFenceTopsAreas,
			LocalizedStringID_Variable_NavigationMesh_SquareUpAreas,

			LocalizedStringID_Variables_EndOffset,	/// @note This is not a translation/variable, just end index of console variables in a 'LocalizedStringID_t' enumeration!
			LocalizedStringID_Variable_Total = LocalizedStringID_Variables_EndOffset - LocalizedStringID_Variables_StartOffset,	/// @note This is not a translation/variable, just total number of console variables in a 'LocalizedStringID_t' enumeration!

			// Console commands....

			// Main ------------------------------------------------------------------------------------------------------------------------
			LocalizedStringID_ConsoleCommandCanOnlyBeExecutedFromServerConsole = LocalizedStringID_Variables_EndOffset,
			LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedFromServerConsole,
			LocalizedStringID_ConsoleVariableCanBeOnlySetViaRCONAccess,
			LocalizedStringID_ClientHasExecutedConsoleVariableCanBeOnlySetViaRCONAccess,
			LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning,
			LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedWhenServerIsRunning,
			LocalizedStringID_UnknownConsoleCommand,
			LocalizedStringID_ClientHasExecutedUnknownConsoleCommand,

			// Localizer -------------------------------------------------------------------------------------------------------------------
			LocalizedStringID_ConsoleCommand_Localizer,
				LocalizedStringID_ConsoleCommand_Localizer_Reload,
				LocalizedStringID_ConsoleCommand_Localizer_LoadOriginalStrings,

			// Bot -------------------------------------------------------------------------------------------------------------------------
			LocalizedStringID_DisplaysBotConsoleCommandsContainer,
				LocalizedStringID_DisplaysVersionInformation,
				LocalizedStringID_ShowsAboutBotInformation,
				LocalizedStringID_FillsServerWithRandomBots,
				LocalizedStringID_DisconnectsAllBotsFromCurrentGame,
				LocalizedStringID_KillsAllBotsInCurrentGame,
				LocalizedStringID_CreatesBotInCurrentGame,
				LocalizedStringID_DisconnectsRandomBotFromGame,
				LocalizedStringID_SelectsBotWeaponMode,
				LocalizedStringID_DisplaysBotsProfilesDatabase,
				LocalizedStringID_DisplaysBotsCommandMenu,
				LocalizedStringID_DisplaysBotsListCurrentlyPlaying,
				LocalizedStringID_ExecutesSpecificCommandBySpecifiedBot,
				LocalizedStringID_DisplaysCurrentTimeOnServer,

			// Navigation mesh -------------------------------------------------------------------------------------------------------------
			LocalizedStringID_NavigationMesh_DisplaysConsoleCommandsContainer,
				LocalizedStringID_NavigationMesh_Generate,
				LocalizedStringID_NavigationMesh_Analyze,
				LocalizedStringID_NavigationMesh_Save,
				LocalizedStringID_NavigationMesh_Load,
				LocalizedStringID_NavigationMesh_ReloadPlaceDatabase,
				LocalizedStringID_NavigationMesh_PrintPlaceList,
				LocalizedStringID_NavigationMesh_TogglePlaceMode,
				LocalizedStringID_NavigationMesh_MakeSniperSpots,
				LocalizedStringID_NavigationMesh_Transient,
				LocalizedStringID_NavigationMesh_Delete,
				LocalizedStringID_NavigationMesh_TogglePlacePainting,
				LocalizedStringID_NavigationMesh_PrintAllPlaces,
				LocalizedStringID_NavigationMesh_UsePlace,
				LocalizedStringID_NavigationMesh_PlaceReplace,
				LocalizedStringID_NavigationMesh_Mark,
				LocalizedStringID_NavigationMesh_Draw,
				LocalizedStringID_NavigationMesh_PrintBSPEntities,
				LocalizedStringID_NavigationMesh_CreateWalkFacesBMPFile,
				LocalizedStringID_NavigationMesh_CreateNavigationAreasBMPFile,

			// Waypoints -------------------------------------------------------------------------------------------------------------------
			LocalizedStringID_DisplaysWaypointConsoleCommandsContainer,
				LocalizedStringID_TogglesAutowppointing,

			// Other -----------------------------------------------------------------------------------------------------------------------
			//

			// Menus....

			// Bot -------------------------------------------------------------------------------------------------------------------------
			LocalizedStringID_Menu_Bot_Main_Title,								// Main YaPB menu.
				LocalizedStringID_Menu_Bot_Main_Item_Control,
				LocalizedStringID_Menu_Bot_Main_Item_Features,
				LocalizedStringID_Menu_Bot_Main_Item_FillServer,
				LocalizedStringID_Menu_Bot_Main_Item_EndRound,
			LocalizedStringID_Menu_Bot_Features_Title,							// Bot features menu.
				LocalizedStringID_Menu_Bot_Features_Item_WeaponModeMenu,
				LocalizedStringID_Menu_Bot_Features_Item_WaypointMenu,
				LocalizedStringID_Menu_Bot_Features_Item_SelectPersonality,
				LocalizedStringID_Menu_Bot_Features_Item_ToggleDebugMode,
				LocalizedStringID_Menu_Bot_Features_Item_CommandMenu,
			LocalizedStringID_Menu_Bot_Control_Title,							// Bot control menu.
				LocalizedStringID_Menu_Bot_Control_Item_AddBotQuick,
				LocalizedStringID_Menu_Bot_Control_Item_AddBotSpecified,
				LocalizedStringID_Menu_Bot_Control_Item_KickRandomBot,
				LocalizedStringID_Menu_Bot_Control_Item_KickAllBots,
				LocalizedStringID_Menu_Bot_Control_Item_KickBotMenu,
			LocalizedStringID_Menu_Bot_WeaponModeSelect_Title,					// Bot weapon mode select menu.
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_KnivesOnly,
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_PistolsOnly,
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_ShotgunsOnly,
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_MachineGunsOnly,
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_RiflesOnly,
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_SniperWeaponsOnly,
				LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_AllWeapons,
			LocalizedStringID_Menu_Bot_PersonalitySelect_Title,					// Bot personality select menu.
				LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Normal,
				LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Aggressive,
				LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Careful,
				LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Random,
			LocalizedStringID_Menu_Bot_SkillSelect_Title,						// Bot skill select menu.
				LocalizedStringID_Menu_Bot_SkillSelect_Item_Stupid,
				LocalizedStringID_Menu_Bot_SkillSelect_Item_Newbie,
				LocalizedStringID_Menu_Bot_SkillSelect_Item_Average,
				LocalizedStringID_Menu_Bot_SkillSelect_Item_Advanced,
				LocalizedStringID_Menu_Bot_SkillSelect_Item_Professional,
				LocalizedStringID_Menu_Bot_SkillSelect_Item_Godlike,
			LocalizedStringID_Menu_Bot_TeamSelect_Title,						// Bot team select menu.
				LocalizedStringID_Menu_Bot_TeamSelect_Item_Terrorist,
				LocalizedStringID_Menu_Bot_TeamSelect_Item_CounterTerrorist,
			LocalizedStringID_Menu_Bot_ModelSelect_Title,						// Bot terrorist model select menu.
				// Bot terrorist model select menu items.
				LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Phoenix,
				LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Leet,
				LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Arctic,
				LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Guerilla,
				// Bot counter-terrorist model select menu items.
				LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_Urban,
				LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GSG9,
				LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_SAS,
				LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GIGN,
			LocalizedStringID_Menu_Bot_Kick_Title,								// Bot kick menu.
			LocalizedStringID_Menu_Bot_Command,									// Bot command menu.
				LocalizedStringID_Menu_Bot_Command_Item_MakeDoubleJump,
				LocalizedStringID_Menu_Bot_Command_Item_FinishDoubleJump,
				LocalizedStringID_Menu_Bot_Command_Item_DropC4Bomb,
				LocalizedStringID_Menu_Bot_Command_Item_DropWeapon,

			// Waypoints -------------------------------------------------------------------------------------------------------------------
			LocalizedStringID_Menu_Waypoint_Main_Title,							// Waypoint main menu.
				LocalizedStringID_Menu_Waypoint_Main_Item_ToggleWaypoints,
				LocalizedStringID_Menu_Waypoint_Main_Item_CacheWaypoint,
				LocalizedStringID_Menu_Waypoint_Main_Item_CreatePath,
				LocalizedStringID_Menu_Waypoint_Main_Item_DeletePath,
				LocalizedStringID_Menu_Waypoint_Main_Item_AddWaypoint,
				LocalizedStringID_Menu_Waypoint_Main_Item_DeleteWaypoint,
				LocalizedStringID_Menu_Waypoint_Main_Item_SetAutopathDistance,
				LocalizedStringID_Menu_Waypoint_Main_Item_SetRadius,
				LocalizedStringID_Menu_Waypoint_Main_Item_WaypointStats,
				LocalizedStringID_Menu_Waypoint_Main_Item_ToggleAutoWaypoint,
				LocalizedStringID_Menu_Waypoint_Main_Item_SetFlags,
				LocalizedStringID_Menu_Waypoint_Main_Item_SaveWaypoints,
				LocalizedStringID_Menu_Waypoint_Main_Item_SaveWithoutChecking,
				LocalizedStringID_Menu_Waypoint_Main_Item_LoadWaypoints,
				LocalizedStringID_Menu_Waypoint_Main_Item_CheckWaypoints,
				LocalizedStringID_Menu_Waypoint_Main_Item_ToggleNoclipCheat,
			LocalizedStringID_Menu_Waypoint_RadiusSelect_Title,					// Waypoint radius select menu.
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_0,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_8,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_16,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_32,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_48,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_64,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_80,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_96,
				LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_128,
			LocalizedStringID_Menu_Waypoint_TypeSelect_Title,					// Waypoint add menu.
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Normal,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_TerroristImportant,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CounterTerroristImportant,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_BlockWithHostageOrLadder,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_RescueZone,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Camping,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CampEnd,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_MapGoal,
				LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Jump,
			LocalizedStringID_Menu_Waypoint_FlagSet_Title,						// Waypoint flag set menu.
				LocalizedStringID_Menu_Waypoint_FlagSet_Item_BlockWithHostageOrLadder,
				LocalizedStringID_Menu_Waypoint_FlagSet_Item_TerroristSpecific,
				LocalizedStringID_Menu_Waypoint_FlagSet_Item_CounterTerroristSpecific,
				LocalizedStringID_Menu_Waypoint_FlagSet_Item_UseElevator,
				LocalizedStringID_Menu_Waypoint_FlagSet_Item_SniperPoint,
			LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Title,	// Waypoint auto-path maximum distance set.
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_0,
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_100,
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_130,
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_160,
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_190,
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_220,
				LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_250,
			LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Title,			// Waypoint path connections set.
				LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Outgoing,
				LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Incoming,
				LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Bidirectional,

			LocalizedStringID_Menu_Item_Next,
			LocalizedStringID_Menu_Item_Previous,
			LocalizedStringID_Menu_Item_Exit,

			LocalizedStringID_Total
		};

		// Reliability check.
		CompileTimeAssert (LocalizedStringID_Variable_Total == Console::Bot::VariableID_Total);

		typedef /*Short*/DynamicString LanguageName_t;
/*
		struct Localization_t
		{
			DynamicString string;
			bool          isLoaded;		// True when original 'string' replaced with string from configuration file.
			bool          hasFormat;	// True when 'string' has '%*'.
		};
*/
	//
	// Group: Private members.
	//
	private:
		static const LanguageName_t sm_defaultLanguageName;				// Default language name. (Case-insensitive, example: "RU", "En", "ch", ....)
		LanguageName_t              m_currentLanguageName;				// Current loaded language name. (Case-insensitive, example: "RU", "En", "ch", ....)
		DynamicString               m_strings[LocalizedStringID_Total];	// Array of all possible original/translated strings.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Localizer (void)
		{
			LoadOriginalStrings ();	// Load original strings....
		}
		inline Localizer (const LanguageName_t &languageName)
		{
			LoadOriginalStrings ();	// Load original strings....
			LoadConfigurationFile (languageName);	// Load the replacements or translated strings (depending on the given language) from the configuration file....
		}

	//
	// Group: Functions.
	//
	public:
		static inline const LanguageName_t &GetDefaultLanguageName (void)                                              { return sm_defaultLanguageName; }
		inline const LanguageName_t        &GetCurrentLanguageName (void)                                        const { return m_currentLanguageName; }
		inline const DynamicString         &GetLocalizedString     (const LocalizedStringID_t localizedStringID) const
		{
			// Reliability check.
			InternalAssert (localizedStringID > LocalizedStringID_None && localizedStringID < LocalizedStringID_Total);

			return m_strings[localizedStringID];
		}
		inline const DynamicString &GetBotConsoleVariableLocalizedString (const Console::Bot::VariableID_t botConsoleVariableID) const
		{
			return GetLocalizedString (static_cast <const LocalizedStringID_t> (LocalizedStringID_Variables_StartOffset + botConsoleVariableID));
		}

		void                        LoadOriginalStrings    (void);
		const bool                  LoadConfigurationFile  (const LanguageName_t &languageName = Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ());

		inline void                 Reload                 (void)
		{
			// Just reload it....
			LoadOriginalStrings ();		// Load original strings...,
			LoadConfigurationFile ();	// Load the replacements or translated strings (depending on the given language) from the configuration file....
		}
};

extern Localizer g_localizer;

class LocalizerManager
{
	public:
		#define CLIENT_USER_INFO_LANGUAGE_KEY "lang"	// Same as in AMXX....

		typedef DynamicArray <Localizer *, unsigned char> Array_t;

		Array_t m_array;

	public:
		inline  LocalizerManager (void) { m_array += &g_localizer; }
		inline ~LocalizerManager (void) { DeleteAndPurgeAll (); }

	private:
		inline void DeleteAndPurgeAll (void)
		{
			// m_array.DeleteAll():
			{
				for (Array_t::IndexType_t index (0u); index < m_array.GetElementNumber (); ++index)
					if (m_array[index] != &g_localizer)
						delete m_array[index];
			}

			m_array.RemoveAll ();
		}

		inline Localizer *const GetLocalizerForLanguage (const Localizer::LanguageName_t &languageName) const
		{
			for (Array_t::IndexType_t index (0u); index < m_array.GetElementNumber (); ++index)
				if (m_array[index]->GetCurrentLanguageName ().CompareWithoutCaseCheck (languageName) == 0)
					return m_array[index];

			return NULL;
		}
		inline Localizer *const AddLocalizerForLanguage (const Localizer::LanguageName_t &languageName)
		{
			Localizer *const localizer (new Localizer (languageName));

			if (localizer == NULL)
				return &g_localizer;	// Return server localizer on failure....

			if (!m_array.PushBack (localizer))
			{
				delete localizer;

				return &g_localizer;	// Return server localizer on failure....
			}

			return localizer;
		}
		inline const bool RemoveLocalizer (const Localizer *const localizer)
		{
			// Reliability checks.
			InternalAssert (localizer != NULL);
			InternalAssert (localizer != &g_localizer);

			return m_array.Pop (const_cast <Localizer *const> (localizer));
		}

	public:
		inline Localizer *const TryAddLocalizerForLanguage (const Localizer::LanguageName_t &languageName)
		{
			// Reliability checks.
			InternalAssert (!languageName.IsEmpty ());
			InternalAssert (languageName.IsAlpha ());	// OCCURS FOR "ru"!!!! WITH 'isalpha' FUNCTION!!!!!!!!!!!!!!!!!!!

			Localizer *const localizer (GetLocalizerForLanguage (languageName));

			return localizer != NULL ? localizer : AddLocalizerForLanguage (languageName);
		}
		inline const bool TryRemoveLocalizer (const Localizer *const localizer, const Human *const owner);	// Note: This function is declared in Core.hpp.
};

extern LocalizerManager g_localizerManager;

namespace Console
{
	namespace Bot
	{
		// Bot console variables callbacks....
		inline void VariableManager::OnConsoleVariablePrintHelp_AutoVacate (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_AutoVacate);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_AutoVacate) + '\n', sm_variableList[VariableID_AutoVacate].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Quota (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Quota);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Quota) + '\n', sm_variableList[VariableID_Quota].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_QuotaMode (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_QuotaMode);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_QuotaMode) + '\n', sm_variableList[VariableID_QuotaMode].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_MinSkill (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_MinSkill);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_MinSkill) + '\n', sm_variableList[VariableID_MinSkill].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_MaxSkill (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_MaxSkill);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_MaxSkill) + '\n', sm_variableList[VariableID_MaxSkill].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_FollowUser (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_FollowUser);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_FollowUser) + '\n', sm_variableList[VariableID_FollowUser].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_TimerSound (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_TimerSound);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_TimerSound) + '\n', sm_variableList[VariableID_TimerSound].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_TimerPickup (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_TimerPickup);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_TimerPickup) + '\n', sm_variableList[VariableID_TimerPickup].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_TimerGrenade (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_TimerGrenade);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_TimerGrenade) + '\n', sm_variableList[VariableID_TimerGrenade].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_DebugGoal (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DebugGoal);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DebugGoal) + '\n', sm_variableList[VariableID_DebugGoal].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Chat (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Chat);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Chat) + '\n', sm_variableList[VariableID_Chat].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Synth (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Synth);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Synth) + '\n', sm_variableList[VariableID_Synth].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_KnifeMode (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_KnifeMode);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_KnifeMode) + '\n', sm_variableList[VariableID_KnifeMode].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_SkillTags (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_SkillTags);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_SkillTags) + '\n', sm_variableList[VariableID_SkillTags].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Stop (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Stop);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Stop) + '\n', sm_variableList[VariableID_Stop].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_ThruWalls (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_ThruWalls);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_ThruWalls) + '\n', sm_variableList[VariableID_ThruWalls].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_WallSensitivity (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_WallSensitivity);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_WallSensitivity) + '\n', sm_variableList[VariableID_WallSensitivity].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Votes (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Votes);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Votes) + '\n', sm_variableList[VariableID_Votes].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Spray (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Spray);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Spray) + '\n', sm_variableList[VariableID_Spray].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_BotBuy (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_BotBuy);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_BotBuy) + '\n', sm_variableList[VariableID_BotBuy].m_defaultString);
		}

		#if defined _DEBUG
			inline void VariableManager::OnConsoleVariablePrintHelp_Debug (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Debug);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Debug) + '\n', sm_variableList[VariableID_Debug].m_defaultString);
			}
		#endif	// if defined _DEBUG

		inline void VariableManager::OnConsoleVariablePrintHelp_Prefix (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Prefix);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Prefix) + '\n', sm_variableList[VariableID_Prefix].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Language (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Language);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Language) + '\n', sm_variableList[VariableID_Language].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_EnableClientLanguages (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_EnableClientLanguages);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_EnableClientLanguages) + '\n', sm_variableList[VariableID_EnableClientLanguages].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Version (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Version);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Version) + '\n');
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_NavPointLookMethod (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_NavPointLookMethod);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_NavPointLookMethod) + '\n', sm_variableList[VariableID_NavPointLookMethod].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_AimMethod (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_AimMethod);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_AimMethod) + '\n', sm_variableList[VariableID_AimMethod].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_TargetingMethod (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_TargetingMethod);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_TargetingMethod) + '\n', sm_variableList[VariableID_TargetingMethod].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_DamperCoeff_X (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DamperCoeff_X);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DamperCoeff_X) + '\n', sm_variableList[VariableID_DamperCoeff_X].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_DamperCoeff_Y (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DamperCoeff_Y);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DamperCoeff_Y) + '\n', sm_variableList[VariableID_DamperCoeff_Y].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Deviation_X (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Deviation_X);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Deviation_X) + '\n', sm_variableList[VariableID_Deviation_X].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Deviation_Y (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Deviation_Y);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Deviation_Y) + '\n', sm_variableList[VariableID_Deviation_Y].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Influence_XonY (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Influence_XonY);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Influence_XonY) + '\n', sm_variableList[VariableID_Influence_XonY].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Influence_YonX (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Influence_YonX);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Influence_YonX) + '\n', sm_variableList[VariableID_Influence_YonX].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_SlowdownRatio (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_SlowdownRatio);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_SlowdownRatio) + '\n', sm_variableList[VariableID_SlowdownRatio].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_OffsetDelay (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_OffsetDelay);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_OffsetDelay) + '\n', sm_variableList[VariableID_OffsetDelay].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_SpringStiff_X (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_SpringStiff_X);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_SpringStiff_X) + '\n', sm_variableList[VariableID_SpringStiff_X].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_SpringStiff_Y (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_SpringStiff_Y);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_SpringStiff_Y) + '\n', sm_variableList[VariableID_SpringStiff_Y].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_AnticipRatio (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_AnticipRatio);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_AnticipRatio) + '\n', sm_variableList[VariableID_AnticipRatio].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Password (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Password);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Password) + '\n', sm_variableList[VariableID_Password].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_PasswordKey (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_PasswordKey);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_PasswordKey) + '\n', sm_variableList[VariableID_PasswordKey].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_ChatterPath (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_ChatterPath);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_ChatterPath) + '\n', sm_variableList[VariableID_ChatterPath].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_TKPunish (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_TKPunish);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_TKPunish) + '\n', sm_variableList[VariableID_TKPunish].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Communication (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Communication);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Communication) + '\n', sm_variableList[VariableID_Communication].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Economics (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Economics);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Economics) + '\n', sm_variableList[VariableID_Economics].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_ForceTeam (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_ForceTeam);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_ForceTeam) + '\n', sm_variableList[VariableID_ForceTeam].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_RestrictedWeapons (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_RestrictedWeapons);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_RestrictedWeapons) + '\n', sm_variableList[VariableID_RestrictedWeapons].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_DangerFactor (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DangerFactor);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DangerFactor) + '\n', sm_variableList[VariableID_DangerFactor].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_DontShoot (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DontShoot);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DontShoot) + '\n', sm_variableList[VariableID_DontShoot].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_HardcoreMode (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_HardcoreMode);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_HardcoreMode) + '\n', sm_variableList[VariableID_HardcoreMode].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_AutofollowPercent (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_AutofollowPercent);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_AutofollowPercent) + '\n', sm_variableList[VariableID_AutofollowPercent].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_ExplosiveGrenadeUsePercent (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_ExplosiveGrenadeUsePercent);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_ExplosiveGrenadeUsePercent) + '\n', sm_variableList[VariableID_ExplosiveGrenadeUsePercent].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_FlashBangGrenadeUsePercent (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_FlashBangGrenadeUsePercent);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_FlashBangGrenadeUsePercent) + '\n', sm_variableList[VariableID_FlashBangGrenadeUsePercent].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_SmokeGrenadeUsePercent (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_SmokeGrenadeUsePercent);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_SmokeGrenadeUsePercent) + '\n', sm_variableList[VariableID_SmokeGrenadeUsePercent].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_LogoTypes (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_LogoTypes);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_LogoTypes) + '\n', sm_variableList[VariableID_LogoTypes].m_defaultString);
		}

		#if defined MULTIPLE_MSEC_METHODS
			inline void VariableManager::OnConsoleVariablePrintHelp_MsecMethod (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_MsecMethod);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_MsecMethod) + '\n', sm_variableList[VariableID_MsecMethod].m_defaultString);
			}
		#endif	// if defined MULTIPLE_MSEC_METHODS

		inline void VariableManager::OnConsoleVariablePrintHelp_LogLevel (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_LogLevel);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_LogLevel) + '\n', sm_variableList[VariableID_LogLevel].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Config_LoadTime (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Config_LoadTime);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Config_LoadTime) + '\n', sm_variableList[VariableID_Config_LoadTime].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_WalkAllowed (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_WalkAllowed);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_WalkAllowed) + '\n', sm_variableList[VariableID_WalkAllowed].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Deathmatch (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Deathmatch);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Deathmatch) + '\n', sm_variableList[VariableID_Deathmatch].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_ThinkFPS (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_ThinkFPS);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_ThinkFPS) + '\n', sm_variableList[VariableID_ThinkFPS].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Ping (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Ping);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Ping) + '\n', sm_variableList[VariableID_Ping].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_PingFlux (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_PingFlux);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_PingFlux) + '\n', sm_variableList[VariableID_PingFlux].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_DeferToHuman (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DeferToHuman);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DeferToHuman) + '\n', sm_variableList[VariableID_DeferToHuman].m_defaultString);
		}

		// Voice system....
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_inputfromfile (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_inputfromfile);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_inputfromfile) + '\n', sm_variableList[VariableID_Voice_inputfromfile].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_recordtofile (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_recordtofile);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_recordtofile) + '\n', sm_variableList[VariableID_Voice_recordtofile].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_avggain (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_avggain);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_avggain) + '\n', sm_variableList[VariableID_Voice_avggain].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_maxgain (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_maxgain);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_maxgain) + '\n', sm_variableList[VariableID_Voice_maxgain].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_scale (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_scale);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_scale) + '\n', sm_variableList[VariableID_Voice_scale].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_loopback (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_loopback);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_loopback) + '\n', sm_variableList[VariableID_Voice_loopback].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_fadeouttime (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_fadeouttime);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_fadeouttime) + '\n', sm_variableList[VariableID_Voice_fadeouttime].m_defaultString);
		}

		// Debugging cvars.
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_profile (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_profile);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_profile) + '\n', sm_variableList[VariableID_Voice_profile].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_showchannels (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_showchannels);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_showchannels) + '\n', sm_variableList[VariableID_Voice_showchannels].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_showincoming (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_showincoming);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_showincoming) + '\n', sm_variableList[VariableID_Voice_showincoming].m_defaultString);
		}

		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_forcemicrecord (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_forcemicrecord);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_forcemicrecord) + '\n', sm_variableList[VariableID_Voice_forcemicrecord].m_defaultString);
		}

		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_overdrive (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_overdrive);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_overdrive) + '\n', sm_variableList[VariableID_Voice_overdrive].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_Voice_overdrivefadetime (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_Voice_overdrivefadetime);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_Voice_overdrivefadetime) + '\n', sm_variableList[VariableID_Voice_overdrivefadetime].m_defaultString);
		}

		inline void VariableManager::OnConsoleVariablePrintHelp_MaximumAFKTime (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_MaximumAFKTime);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_MaximumAFKTime) + '\n', sm_variableList[VariableID_MaximumAFKTime].m_defaultString);
		}

		#if defined MULTIPLE_DRAW_METHODS
			inline void VariableManager::OnConsoleVariablePrintHelp_DrawMethod (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_DrawMethod);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_DrawMethod) + '\n', sm_variableList[VariableID_DrawMethod].m_defaultString);
			}
		#endif	// if defined MULTIPLE_DRAW_METHODS

		// Navigation mesh console variables callbacks....
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_edit (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_edit);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_edit) + '\n', sm_variableList[VariableID_nav_edit].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_quicksave (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_quicksave);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_quicksave) + '\n', sm_variableList[VariableID_nav_quicksave].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_hiding_spots (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_hiding_spots);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_hiding_spots) + '\n', sm_variableList[VariableID_nav_show_hiding_spots].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_approach_points (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_approach_points);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_approach_points) + '\n', sm_variableList[VariableID_nav_show_approach_points].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_danger (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_danger);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_danger) + '\n', sm_variableList[VariableID_nav_show_danger].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_player_counts (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_player_counts);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_player_counts) + '\n', sm_variableList[VariableID_nav_show_player_counts].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_occupy_time (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_occupy_time);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_occupy_time) + '\n', sm_variableList[VariableID_nav_show_occupy_time].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_battlefront (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_battlefront);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_battlefront) + '\n', sm_variableList[VariableID_nav_show_battlefront].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_area_info (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_area_info);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_area_info) + '\n', sm_variableList[VariableID_nav_show_area_info].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_snap_to_grid (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_snap_to_grid);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_snap_to_grid) + '\n', sm_variableList[VariableID_nav_snap_to_grid].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_create_place_on_ground (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_create_place_on_ground);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_create_place_on_ground) + '\n', sm_variableList[VariableID_nav_create_place_on_ground].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_create_area_at_feet (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_create_area_at_feet);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_create_area_at_feet) + '\n', sm_variableList[VariableID_nav_create_area_at_feet].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_nodes (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_nodes);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_nodes) + '\n', sm_variableList[VariableID_nav_show_nodes].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_compass (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_compass);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_compass) + '\n', sm_variableList[VariableID_nav_show_compass].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_slope_limit (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_slope_limit);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_slope_limit) + '\n', sm_variableList[VariableID_nav_slope_limit].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_slope_tolerance (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_slope_tolerance);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_slope_tolerance) + '\n', sm_variableList[VariableID_nav_slope_tolerance].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_restart_after_analysis (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_restart_after_analysis);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_restart_after_analysis) + '\n', sm_variableList[VariableID_nav_restart_after_analysis].m_defaultString);
		}

		#if defined DEBUG_NAV_NODES
			inline void VariableManager::OnConsoleVariablePrintHelp_nav_show_node_id (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_show_node_id);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_show_node_id) + '\n', sm_variableList[VariableID_nav_show_node_id].m_defaultString);
			}
			inline void VariableManager::OnConsoleVariablePrintHelp_nav_test_node (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_test_node);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_test_node) + '\n', sm_variableList[VariableID_nav_test_node].m_defaultString);
			}
			inline void VariableManager::OnConsoleVariablePrintHelp_nav_test_node_crouch (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_test_node_crouch);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_test_node_crouch) + '\n', sm_variableList[VariableID_nav_test_node_crouch].m_defaultString);
			}
			inline void VariableManager::OnConsoleVariablePrintHelp_nav_test_node_crouch_dir (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_test_node_crouch_dir);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_test_node_crouch_dir) + '\n', sm_variableList[VariableID_nav_test_node_crouch_dir].m_defaultString);
			}
		#endif	// if defined DEBUG_NAV_NODES

		inline void VariableManager::OnConsoleVariablePrintHelp_nav_coplanar_slope_limit (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_coplanar_slope_limit);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_coplanar_slope_limit) + '\n', sm_variableList[VariableID_nav_coplanar_slope_limit].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_split_place_on_ground (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_split_place_on_ground);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_split_place_on_ground) + '\n', sm_variableList[VariableID_nav_split_place_on_ground].m_defaultString);
		}
		#if defined MULTIPLE_DRAW_METHODS
			inline void VariableManager::OnConsoleVariablePrintHelp_nav_area_bgcolor (void)
			{
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_area_bgcolor);

				// Print help string....
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_area_bgcolor) + '\n', sm_variableList[VariableID_nav_area_bgcolor].m_defaultString);
			}
		#endif	// if defined MULTIPLE_DRAW_METHODS
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_area_max_size (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_area_max_size);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_area_max_size) + '\n', sm_variableList[VariableID_nav_area_max_size].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_nav_corner_adjust_adjacent (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_nav_corner_adjust_adjacent);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_nav_corner_adjust_adjacent) + '\n', sm_variableList[VariableID_nav_corner_adjust_adjacent].m_defaultString);
		}

		inline void VariableManager::OnConsoleVariablePrintHelp_NavigationMesh_MaximumOffPlaneTolerance (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_NavigationMesh_MaximumOffPlaneTolerance);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_NavigationMesh_MaximumOffPlaneTolerance) + '\n', sm_variableList[VariableID_NavigationMesh_MaximumOffPlaneTolerance].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_NavigationMesh_MaximumSampleStepTime (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_NavigationMesh_MaximumSampleStepTime);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_NavigationMesh_MaximumSampleStepTime) + '\n', sm_variableList[VariableID_NavigationMesh_MaximumSampleStepTime].m_defaultString);
		}

		inline void VariableManager::OnConsoleVariablePrintHelp_NavigationMesh_GenerateFenceTopsAreas (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_NavigationMesh_GenerateFenceTopsAreas);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_NavigationMesh_GenerateFenceTopsAreas) + '\n', sm_variableList[VariableID_NavigationMesh_GenerateFenceTopsAreas].m_defaultString);
		}
		inline void VariableManager::OnConsoleVariablePrintHelp_NavigationMesh_SquareUpAreas (void)
		{
			// Print original string to server console (call the original function....)
			variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (VariableID_NavigationMesh_SquareUpAreas);

			// Print help string....
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetBotConsoleVariableLocalizedString (VariableID_NavigationMesh_SquareUpAreas) + '\n', sm_variableList[VariableID_NavigationMesh_SquareUpAreas].m_defaultString);
		}
	}
}

#endif	// ifndef LOCALIZER_INCLUDED