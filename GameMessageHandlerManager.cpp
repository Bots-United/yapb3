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
// GameMessageHandlerManager.cpp
//
// Class: GameMessageHandlerManager
//
// Version: $ID:$
//

#include <Core.hpp>
/*
static Server::GameMessageHandlerManager::GameMessageHandler_ForClient_AmmoPickup    sm_gameMessageHandler_ForClient_AmmoPickup ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForClient_AmmoX         sm_gameMessageHandler_ForClient_AmmoX ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForClient_CurrentWeapon sm_gameMessageHandler_ForClient_CurrentWeapon ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_Damage          sm_gameMessageHandler_ForYaPB_Damage ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ScreenShake     sm_gameMessageHandler_ForYaPB_ScreenShake ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ScreenFade      sm_gameMessageHandler_ForYaPB_ScreenFade ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForHuman_ShowMenu       sm_gameMessageHandler_ForHuman_ShowMenu ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ShowMenu        sm_gameMessageHandler_ForYaPB_ShowMenu ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_StatusIcon      sm_gameMessageHandler_ForYaPB_StatusIcon ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ReloadSound     sm_gameMessageHandler_ForYaPB_ReloadSound ();
static Server::GameMessageHandlerManager::GameMessageHandler_TeamInfo                sm_gameMessageHandler_TeamInfo ();
static Server::GameMessageHandlerManager::GameMessageHandler_Text                    sm_gameMessageHandler_Text ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_Text            sm_gameMessageHandler_ForYaPB_Text ();
static Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_VGUIMenu        sm_gameMessageHandler_ForYaPB_VGUIMenu ();
*/
const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_t Server::GameMessageHandlerManager::sm_handlerConstructorFunctions[HalfLifeEngine::Engine::GameMessageContainer::Type_BotTotal/*HalfLifeEngine::SDK::Constants::MaximumUserMessages*/] =
{
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_AmmoPickup,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_AmmoX,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_CurrentWeapon,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_Damage,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ScreenShake,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ScreenFade,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ShowMenu,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_StatusIcon,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ReloadSound,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_TeamInfo,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_Text,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_VGUIMenu
/*
	&GameMessageHandlerConstructorFunction_NULL,	// VoiceMask
	&GameMessageHandlerConstructorFunction_NULL,	// ReqState

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_CurrentWeapon,

	&GameMessageHandlerConstructorFunction_NULL,	// Geiger
	&GameMessageHandlerConstructorFunction_NULL,	// Flashlight
	&GameMessageHandlerConstructorFunction_NULL,	// FlashBat
	&GameMessageHandlerConstructorFunction_NULL,	// Health

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_Damage,

	&GameMessageHandlerConstructorFunction_NULL,	// Battery
	&GameMessageHandlerConstructorFunction_NULL,	// Train
	&GameMessageHandlerConstructorFunction_NULL,	// HudTextPro
	&GameMessageHandlerConstructorFunction_NULL,	// HudText
	&GameMessageHandlerConstructorFunction_NULL,	// SayText

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_Text,

	&GameMessageHandlerConstructorFunction_NULL,	// WeaponList
	&GameMessageHandlerConstructorFunction_NULL,	// ResetHUD
	&GameMessageHandlerConstructorFunction_NULL,	// InitHUD
	&GameMessageHandlerConstructorFunction_NULL,	// ViewMode
	&GameMessageHandlerConstructorFunction_NULL,	// GameTitle
	&GameMessageHandlerConstructorFunction_NULL,	// DeathMsg
	&GameMessageHandlerConstructorFunction_NULL,	// ScoreAttrib
	&GameMessageHandlerConstructorFunction_NULL,	// ScoreInfo

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_TeamInfo,

	&GameMessageHandlerConstructorFunction_NULL,	// TeamScore
	&GameMessageHandlerConstructorFunction_NULL,	// GameMode
	&GameMessageHandlerConstructorFunction_NULL,	// MOTD
	&GameMessageHandlerConstructorFunction_NULL,	// ServerName

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_AmmoPickup,

	&GameMessageHandlerConstructorFunction_NULL,	// WeapPickup
	&GameMessageHandlerConstructorFunction_NULL,	// ItemPickup
	&GameMessageHandlerConstructorFunction_NULL,	// HideWeapon
	&GameMessageHandlerConstructorFunction_NULL,	// SetFOV

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ShowMenu,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ScreenShake,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ScreenFade,
	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_AmmoX,

	&GameMessageHandlerConstructorFunction_NULL,	// SendAudio
	&GameMessageHandlerConstructorFunction_NULL,	// RoundTime
	&GameMessageHandlerConstructorFunction_NULL,	// Money
	&GameMessageHandlerConstructorFunction_NULL,	// ArmorType
	&GameMessageHandlerConstructorFunction_NULL,	// BlinkAcct
	&GameMessageHandlerConstructorFunction_NULL,	// StatusValue
	&GameMessageHandlerConstructorFunction_NULL,	// StatusText

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_StatusIcon,

	&GameMessageHandlerConstructorFunction_NULL,	// BarTime
	&GameMessageHandlerConstructorFunction_NULL,	// ReloadSound
	&GameMessageHandlerConstructorFunction_NULL,	// Crosshair
	&GameMessageHandlerConstructorFunction_NULL,	// NVGToggle
	&GameMessageHandlerConstructorFunction_NULL,	// Radar
	&GameMessageHandlerConstructorFunction_NULL,	// Spectator

	&Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_VGUIMenu,

	&GameMessageHandlerConstructorFunction_NULL,	// TutorText
	&GameMessageHandlerConstructorFunction_NULL,	// TutorLine
	&GameMessageHandlerConstructorFunction_NULL,	// TutorState
	&GameMessageHandlerConstructorFunction_NULL,	// TutorClose
	&GameMessageHandlerConstructorFunction_NULL,	// AllowSpec
	&GameMessageHandlerConstructorFunction_NULL,	// BombDrop
	&GameMessageHandlerConstructorFunction_NULL,	// BombPickup
	&GameMessageHandlerConstructorFunction_NULL,	// ClCorpse
	&GameMessageHandlerConstructorFunction_NULL,	// HostagePos
	&GameMessageHandlerConstructorFunction_NULL,	// HostageK
	&GameMessageHandlerConstructorFunction_NULL,	// HLTV
	&GameMessageHandlerConstructorFunction_NULL,	// SpecHealth
	&GameMessageHandlerConstructorFunction_NULL,	// ForceCam
	&GameMessageHandlerConstructorFunction_NULL,	// ADStop
	&GameMessageHandlerConstructorFunction_NULL,	// ReceiveW
	&GameMessageHandlerConstructorFunction_NULL,	// CZCareer
	&GameMessageHandlerConstructorFunction_NULL,	// CZCareerHUD
	&GameMessageHandlerConstructorFunction_NULL,	// ShadowIdx
	&GameMessageHandlerConstructorFunction_NULL,	// TaskTime
	&GameMessageHandlerConstructorFunction_NULL,	// Scenario
	&GameMessageHandlerConstructorFunction_NULL,	// BotVoice
	&GameMessageHandlerConstructorFunction_NULL,	// BuyClose
	&GameMessageHandlerConstructorFunction_NULL,	// SpecHealth2
	&GameMessageHandlerConstructorFunction_NULL,	// BarTime2
	&GameMessageHandlerConstructorFunction_NULL,	// ItemStatus
	&GameMessageHandlerConstructorFunction_NULL,	// Location
	&GameMessageHandlerConstructorFunction_NULL,	// BotProgress
	&GameMessageHandlerConstructorFunction_NULL,	// Brass
	&GameMessageHandlerConstructorFunction_NULL,	// Fog
	&GameMessageHandlerConstructorFunction_NULL,	// ShowTimer
	&GameMessageHandlerConstructorFunction_NULL		// HudTextArgs*/
};