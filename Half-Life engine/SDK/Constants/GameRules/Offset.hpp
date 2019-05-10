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
// Offset.hpp
//
// Description: Game rules offsets.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_GAME_RULES_OFFSET_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_GAME_RULES_OFFSET_INCLUDED

enum WinnerTeam_t
{
	WinnerTeam_None,

	WinnerTeam_CounterTerrorist,	// CT's won last round
	WinnerTeam_Terrorist,			// Terrorists did

	WinnerTeam_Nobody				// Draw, no winner
};

enum VIPSafetyZoneState_t
{
	WinnerTeam_Uninitialized,	// Uninitialized

	WinnerTeam_Exists,			// has VIP safety zone
	WinnerTeam_NotExists		// DOES not have VIP safetyzone
};

enum Offset_t
{
	Offset_Pointer                                     = 11u,									// Offset to 'g_pGameRules' pointer from HLSDK.

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Offset_IsFreezePeriod                              = 4u,									// TRUE at beginning of round, set to FALSE when the period expires (as bool, but BOOL)
	Offset_IsBombDropped                               = 8u,									// (as bool, but BOOL)

	//--> Classes::VoiceGameManager START
		Offset_MessagePlayerVoiceMask                  = 6u,									// (as int)
		Offset_MessageRequestState                     = 7u,									// (as int)
		Offset_Helper                                  = 8u,									// (as Classes::VoiceGameManagerHelper *)
		Offset_MaximumPlayers                          = 9u,									// (as int)
		Offset_UpdateIntervalTime                      = 10u,									// (as double)
	//<-- Classes::VoiceGameManager END

	Offset_RestartRoundTime                            = 12u,									// The global time when the round is supposed to end, if this is not 0 (as float)

////	Offset_UNKNOWN0                                    = 13u,									// (as unsigned int)

	Offset_RoundCount                                  = 14u,									// (as float)
	Offset_RoundTime                                   = 15u,									// (From mp_roundtime) - How many seconds long this round is. (as unsigned int)
	Offset_RoundTimeSecs                               = 16u,									// (Current) round timer - set to 'Offset_IntroRoundTime', then 'Offset_RoundTime'. (as unsigned int)
	Offset_IntroRoundTime                              = 17u,									// (From mp_freezetime) - How many seconds long the intro round (when players are frozen) is. (as unsigned int)
	Offset_IntroRoundCount                             = 18u,									// The global time when the intro round ends and the real one starts. (as float)

	Offset_Account                                     = 19u,									// (as unsigned int[2])
		Offset_AccountTerrorist                        = Offset_Account,						// (as unsigned int)
		Offset_AccountCounterTerrorist                 = Offset_Account + 1u,					// (as unsigned int)

	// The number of players on the team (this is generated at the end of a round)
	Offset_PlayerCount                                 = 21u,									// (as unsigned int[2])
		Offset_PlayerCount_Terrorist                   = Offset_PlayerCount,					// The number of terrorists on the team (this is generated at the end of a round) (as unsigned int)
		Offset_PlayerCount_CounterTerrorist            = Offset_PlayerCount + 1u,				// The number of CTs on the team (this is generated at the end of a round) (as unsigned int)

	Offset_SpawnableNumber                             = 23u,									// (as unsigned int[2])
		Offset_SpawnableNumber_Terrorist               = Offset_SpawnableNumber,				// (as unsigned int)
		Offset_SpawnableNumber_CounterTerrorist        = Offset_SpawnableNumber + 1u,			// (as unsigned int)

	Offset_SpawnPointCount                             = 25u,									// (as unsigned int[2])
		Offset_SpawnPointCount_Terrorist               = Offset_SpawnPointCount,				// Number of Terrorist spawn points. (as unsigned int)
		Offset_SpawnPointCount_CounterTerrorist        = Offset_SpawnPointCount + 1u,			// Number of CT spawn points. (as unsigned int)

	Offset_HostagesRescued                             = 27u,									// (as unsigned int)
	Offset_HostagesTouched                             = 28u,									// (as unsigned int)
	Offset_RoundWinStatus                              = 29u,									// 1 == CT's won last round, 2 == Terrorists did, 3 == Draw, no winner. (as unsigned int (from WinnerTeam_t))

	Offset_Score                                       = 60u,									// (as unsigned short[2])
		Offset_Score_Terrorist                         = Offset_Score + 1u,						// (as unsigned short)
		Offset_Score_CounterTerrorist                  = Offset_Score,							// (as unsigned short)

	// BOMB MAP VARIABLES
	Offset_IsTargetBombed                              = 124u,									// Whether or not the bomb has been bombed. (as bool)
	Offset_IsBombDefused                               = 125u,									// Whether or not the bomb has been defused. (as bool)
	Offset_IsMapHasBombTarget                          = 126u,									// (as bool)
	Offset_IsMapHasBombZone                            = 127u,									// (as bool)

	Offset_IsMapHasBuyZone                             = 128u,									// (as bool)
	Offset_IsMapHasRescueZone                          = 129u,									// (as bool)
	Offset_IsMapHasEscapeZone                          = 130u,									// (as bool)

	Offset_IsMapHasVIPSafetyZone                       = 33u,									// 0 = uninitialized; 1 = has VIP safety zone; 2 = DOES not have VIP safetyzone (as unsigned int (from VIPSafetyZoneState_t))
//	Offset_UNKNOWN1                                    = 34u,									// (something with trigger_camera in ClientPutInServer()) (as unsigned int)
	Offset_C4Timer                                     = 35u,									// How long from when the C4 is armed until it blows. (as unsigned int)
	Offset_C4Guy                                       = 36u,									// The current Terrorist who has the C4. (as unsigned int)
	Offset_LoserBonus                                  = 37u,									// The amount of money the losing team gets. This scales up as they lose more rounds in a row. (as unsigned int)

	Offset_ConsecutiveLosesNumber                      = 38u,									// The number of rounds the CT's have lost in a row. (as unsigned int[2])
		Offset_ConsecutiveLosesNumber_Terrorist        = Offset_ConsecutiveLosesNumber + 1u,	// The number of rounds the Terrorists have lost in a row. (as unsigned int)
		Offset_ConsecutiveLosesNumber_CounterTerrorist = Offset_ConsecutiveLosesNumber,			// The number of rounds the CT's have lost in a row. (as unsigned int)

	Offset_MaximumIdlePeriod                           = 40u,									// For the idle kick functionality. This is tha maximum amount of time that the player has to be idle before being kicked (as float)
	Offset_LimitTeams                                  = 41u,									// Maximum # of players 1 team can have over another. (as unsigned int)
	Offset_IsLevelInitialized                          = 168u,									// (as bool)
	Offset_IsRoundTerminating                          = 169u,									// (as bool)
	Offset_IsCompleteReset                             = 170u,									// Set to TRUE to have the scores reset next time round restarts. (as bool)

	Offset_RequiredEscapeRatio                         = 43u,									// (as float)
	Offset_EscapersNumber                              = 44u,									// Number of escaped terrorist (for escape maps) (as unsigned int)
	Offset_HaveEscaped                                 = 45u,									// (as unsigned int)

	// Who can and can't buy.
	Offset_IsCantBuy                                   = 184u,									// (as bool[2])
		Offset_IsCantBuy_Terrorist                     = Offset_IsCantBuy,						// (as bool)
		Offset_IsCantBuy_CounterTerrorist              = Offset_IsCantBuy + 1u,					// (as bool)

	Offset_C4_ExplosionRadius                          = 47u,									// (as float)
	Offset_ConsecutiveVIP                              = 48u,									// (as unsigned int)
	Offset_WeaponsNumber                               = 49u,									// (Including smoke grenade) (as unsigned int)
	Offset_GrenadesNumber                              = 50u,									// (Only HE and flashbang grenades) (as unsigned int)
	Offset_ArmorNumber                                 = 51u,									// (Assault suit and kevlar) (as unsigned int)
	Offset_UnBalancedRounds                            = 52u,									// Keeps track of the # of consecutive rounds that have gone by where one team outnumbers the other team by more than 2. (as unsigned int)
	Offset_EscapeRoundsNumber                          = 53u,									// Keeps track of the # of consecutive rounds of escape played.. Teams will be swapped after 8 rounds. (as unsigned int)
	Offset_MapsVotesNumbers                            = 54u,									// (as unsigned int[MaximumVoteMapListLength])
	Offset_LastPick                                    = 154u,									// (as unsigned int)
////	Offset_UNKNOWN2                                    = 34u,									// (as UNKNOWN)
	Offset_MaximumRounds                               = 156u,									// (as unsigned int)
	Offset_TotalRoundsPlayed                           = 157u,									// (as unsigned int)
	Offset_WinLimit                                    = 158u,									// (as unsigned int)
	Offset_AllowSpectators                             = 159u,									// (as int)
	Offset_ForceCamera                                 = 160u,									// (as float)
	Offset_ForceChaseCamera                            = 161u,									// (as float)
	Offset_FadeToBlack                                 = 162u,									// (as float)
	Offset_VIP                                         = 163u,									// (as Classes::BasePlayer *)
	Offset_VIPQueue                                    = 164u,									// (as Classes::BasePlayer **, but Classes::BasePlayer *[5])
	Offset_IntermissionEndTime                         = 169u,									// (as float)
	Offset_IntermissionStartTime                       = 170u,									// (as float)
	Offset_EndIntermissionButtonHit                    = 684u,									// (as bool, but BOOL)
	Offset_NextPeriodicThinkTime                       = 172u,									// (updates every 1 second) (as float)
	Offset_IsGameCommenced                             = 692u,									// (as bool)
	Offset_IsCareerMatch                               = 693u,									// (as bool)

//	Offset_UNKNOWN3                                    = 174u,									// (as unsigned int)
//	Offset_UNKNOWN4                                    = 175u,									// (as unsigned int)
	Offset_WinDifference                               = 176u,									// (as int)
//	Offset_UNKNOWN5                                    = 177u,									// (as unsigned int)

	Offset_IsShouldSkipSpawn                           = 712u,									// (as bool)
	// HalfLifeMultiplay END

//	Offset_UNKNOWN7Time                                = 180u,									// (as float)
////	Offset_UNKNOWN8                                    = 181u,									// (as unsigned int)
//	Offset_UNKNOWN9                                    = 182u,									// (as unsigned int)
//	Offset_UNKNOWN10                                   = 183u,									// (as unsigned int)
	Offset_IsShowVGUIMenus                             = 736u									// (as bool)
	// HalfLifeTraining END
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_GAME_RULES_OFFSET_INCLUDED