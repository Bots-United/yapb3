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
// Interface_GameUI.hpp
//
// Class: Interface_GameUI
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_GAME_UI_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_GAME_UI_INCLUDED

//-----------------------------------------------------------------------------
// Purpose: contains all the functions that the GameUI dll exports GameUI_GetInterface() is exported via dll export table to get this table
//-----------------------------------------------------------------------------
abstract_class IGameUI : public Base
{
	// the interface version is the number to call GameUI_GetInterface(int interfaceNumber) with
	#define GAMEUI_INTERFACE_VERSION "GameUI007"

	public:
		// Phil: Implementation detail: actual factorys are from unknown, vgui2.dll, filesystem.dll, gameui.dll, client.dll
		virtual void Initialize( CreateFunction_t *factories, int count ) = 0;
		virtual void Start(Structures::ClientEngineFunction_t *engineFuncs, int interfaceVersion, IBaseSystem *system) = 0;
		virtual void Shutdown() = 0;

		// Activates the menus, returns 0 if it doesn't want to handle it
		virtual bool ActivateGameUI() = 0;	// activates the menus, returns 0 if it doesn't want to handle it
		virtual bool ActivateDemoUI() = 0;	// activates the demo player, returns 0 if it doesn't want to handle it

		virtual bool HasExclusiveInput() = 0;

		virtual void RunFrame() = 0;

		virtual void ConnectToServer(const char *game, int IP, int port) = 0;
		virtual void DisconnectFromServer() = 0;
		virtual void HideGameUI() = 0;

		virtual bool IsGameUIActive() = 0;	// returns true if the game UI is currently visible
		
		virtual void LoadingStarted(const char *resourceType, const char *resourceName) = 0;
		virtual void LoadingFinished(const char *resourceType, const char *resourceName) = 0;

		virtual void StartProgressBar(const char *progressType, int numProgressPoints) = 0;
		virtual int	 ContinueProgressBar(int progressPoint, float progressFraction) = 0;
		virtual void StopProgressBar(bool bError, const char *failureReasonIfAny, const char *extendedReason) = 0;
		virtual int  SetProgressBarStatusText(const char *statusText) = 0;
		virtual void SetSecondaryProgressBar(float progress /* range [0..1] */) = 0;
		virtual void SetSecondaryProgressBarText(const char *statusText) = 0;

		// prompts user for new key if the current key is invalid, force = true means force it to prompt
		virtual void ValidateCDKey(bool force, bool inConnect) = 0;	// { /* Empty */ }

		enum DisconnectType_t
		{
			DisconnectType_Common,				// Just call DisconnectFromServer()....

			DisconnectType_SteamUserIDTicket,	// "Invalid STEAM UserID Ticket\n"
			DisconnectType_NoSteamLogon,		// "No Steam logon\n"
			DisconnectType_VACBanned,			// "VAC banned from secure server\n"
			DisconnectType_LoggedInElsewhere,	// "This Steam account is being used in another location\n"
			DisconnectType_ConnectionLost,		// "Steam connection lost\n"
			DisconnectType_NoSteamConnection	// "Unable to connect to Steam\n"
		};

		virtual void ExplainDisconnectFromServer(const DisconnectType_t type, const char *const UNKNOWN_reason) = 0;
};

class GameUI : public IGameUI
{
	public:
/*off=4*/		bool m_bTryingToLoadTracker;
/*off=5*/		char m_szPlatformDir[260];
/*off=268*/		int m_iNumFactories;
/*off=272*/		CreateFunction_t m_FactoryList[5];

	// details about the game we're currently in
/*off=292*/		int m_iGameIP;
/*off=296*/		int m_iGamePort;
/*off=300*/		bool m_bActivatedUI;

/*off=304*/		float m_tryStartTrackerTime;

/*off=308*/		const char *m_pszCurrentProgressType;
/*off=312*/		float m_flProgressStartTime;

/*off=316*/		class CSocket *m_pMaster;

// EXISTS?????????????????????????????????		VGUI::DHANDLE<class CCDKeyEntryDialog> m_hCDKeyEntryDialog;
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_GAME_UI_INCLUDED