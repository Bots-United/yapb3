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
// ID.hpp
//
// Class: ID
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_STEAM_ID_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_STEAM_ID_INCLUDED

//lint -save -e1931 -e1927 -e1924 -e613 -e726

// This header file defines the interface between the calling application and the code that
// knows how to communicate with the connection manager (CM) from the Steam service

// This header file is intended to be portable; ideally this 1 header file plus a lib or dll
// is all you need to integrate the client library into some other tree. So please avoid
// including or requiring other header files if possible. This header should only describe the
// interface layer, no need to include anything about the implementation.

typedef void (*PFNLegacyKeyRegistration) (const char *pchCDKey, const char *pchInstallPath);
typedef bool (*PFNLegacyKeyInstalled)(void);

const unsigned int k_unSteamAccountIDMask = 0xFFFFFFFF;
const unsigned int k_unSteamAccountInstanceMask = 0xFFFFF;
// we allow 2 simultaneous user account instances right now, 0 = all, 1 = desktop, 2 = console.
const unsigned int k_unSteamUserDesktopInstance = 1;
const unsigned int k_unSteamUserConsoleInstance = 2;

// Special flags for Chat accounts - they go in the top 8 bits
// of the steam ID's "instance", leaving 12 for the actual instances
enum EChatSteamIDInstanceFlags
{
	k_EChatAccountInstanceMask = 0x00000FFF, // top 8 bits are flags

	k_EChatInstanceFlagClan = (k_unSteamAccountInstanceMask + 1) >> 1,	// top bit
	k_EChatInstanceFlagLobby = (k_unSteamAccountInstanceMask + 1) >> 2,	// next one down, etc
	k_EChatInstanceFlagMMSLobby = (k_unSteamAccountInstanceMask + 1) >> 3,	// next one down, etc

	// Max of 8 flags
};

// Steam ID structure (64 bits total)
class ID
{
	private:
//		#pragma warning (push)
//		#pragma warning (disable: 4201)	// nameless union is nonstandard
		// 64 bits total
		union /* Unnamed */
		{
			#pragma pack (push, 1)	// Structures must be packed (byte-aligned)
				struct /* Unnamed */
				{
					unsigned int                m_unAccountID       : 32;	// unique account identifier
					unsigned int                m_unAccountInstance : 20;	// dynamic instance ID (used for multiseat type accounts only)
					unsigned int                m_EAccountType      : 4;	// type of account - can't show as EAccountType, due to signed / unsigned difference
					Constants::Steam::EUniverse m_EUniverse         : 8;	// universe this account belongs to
				};	// sizeof (/* Unnamed */) == 4 (1)
			#pragma pack (pop)	// Reset default packing.

			unsigned long long m_unAll64Bits;
		};
//		#pragma warning (pop)	// no more anonymous unions until next time

	private:
		// These are defined here to prevent accidental implicit conversion of a u32AccountID to a ID.
		// If you get a compiler error about an ambiguous constructor/function then it may be because you're
		// passing a 32-bit int to a function that takes a ID. You should explicitly create the SteamID
		// using the correct Universe and account Type/Instance values.
		ID (unsigned int);
		ID (int);

public:
	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	//-----------------------------------------------------------------------------
	ID (void)
	{
		m_unAccountID = 0;
		m_EAccountType = Constants::Steam::k_EAccountTypeInvalid;
		m_EUniverse = Constants::Steam::k_EUniverseInvalid;
		m_unAccountInstance = 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	ID (unsigned int unAccountID, Constants::Steam::EUniverse eUniverse, Constants::Steam::EAccountType eAccountType)
	{
		Set (unAccountID, eUniverse, eAccountType);
	}

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			unAccountInstance - instance
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	ID (unsigned int unAccountID, unsigned int unAccountInstance, Constants::Steam::EUniverse eUniverse, Constants::Steam::EAccountType eAccountType)
	{/*
#if defined(_SERVER) && defined(Assert)
		Assert (!(Constants::Steam::k_EAccountTypeIndividual == eAccountType && k_unSteamUserDesktopInstance != unAccountInstance));	// enforce that for individual accounts, instance is always k_unSteamUserDesktopInstance
#endif // _SERVER
*/		InstancedSet (unAccountID, unAccountInstance, eUniverse, eAccountType);
	}

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	// Note:	Will not accept a unsigned int or int as input, as that is a probable mistake.
	//			See the stubbed out overloads in the private: section for more info.
	//-----------------------------------------------------------------------------
	ID (unsigned long long ulSteamID)
	{
		SetFromUint64 (ulSteamID);
	}

	// this set of functions is hidden, will be moved out of class
	explicit ID (const char *const pchSteamID, Constants::Steam::EUniverse eDefaultUniverse = Constants::Steam::k_EUniverseInvalid);

public:
	//-----------------------------------------------------------------------------
	// Purpose: Sets parameters for steam ID
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	void Set (unsigned int unAccountID, Constants::Steam::EUniverse eUniverse, Constants::Steam::EAccountType eAccountType)
	{
		m_unAccountID = unAccountID;
		m_EUniverse = eUniverse;
		m_EAccountType = eAccountType;

		if (eAccountType == Constants::Steam::k_EAccountTypeClan)
			m_unAccountInstance = 0;
		else
			m_unAccountInstance = k_unSteamUserDesktopInstance;	// by default we pick the desktop instance
	}

	//-----------------------------------------------------------------------------
	// Purpose: Sets parameters for steam ID
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	void InstancedSet (unsigned int unAccountID, unsigned int unInstance, Constants::Steam::EUniverse eUniverse, Constants::Steam::EAccountType eAccountType)
	{
		m_unAccountID = unAccountID;
		m_EUniverse = eUniverse;
		m_EAccountType = eAccountType;
		m_unAccountInstance = unInstance;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from its 52 bit parts and universe/type
	// Input  : ulIdentifier - 52 bits of goodness
	//-----------------------------------------------------------------------------
	void FullSet (unsigned long long ulIdentifier, Constants::Steam::EUniverse eUniverse, Constants::Steam::EAccountType eAccountType)
	{
		m_unAccountID = (ulIdentifier & k_unSteamAccountIDMask);						// account ID is low 32 bits
		m_unAccountInstance = ((ulIdentifier >> 32) & k_unSteamAccountInstanceMask);			// account instance is next 20 bits
		m_EUniverse = eUniverse;
		m_EAccountType = eAccountType;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from its 64-bit representation
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	//-----------------------------------------------------------------------------
	void SetFromUint64 (unsigned long long ulSteamID)
	{
/*		m_unAccountID = (ulSteamID & k_unSteamAccountIDMask);							// account ID is low 32 bits
		m_unAccountInstance = ((ulSteamID >> 32) & k_unSteamAccountInstanceMask);		// account instance is next 20 bits

		m_EAccountType = (Constants::Steam::EAccountType) ((ulSteamID >> 52) & 0xF);	// type is next 4 bits
		m_EUniverse = (Constants::Steam::EUniverse) ((ulSteamID >> 56) & 0xFF);			// universe is next 8 bits
*/		m_unAll64Bits = ulSteamID;
	}
/*
#if defined (INCLUDED_STEAM_COMMON_STEAMCOMMON_H)
	//-----------------------------------------------------------------------------
	// Purpose: Initializes a steam ID from a Steam2 ID structure
	// Input:	pTSteamGlobalUserID -	Steam2 ID to convert
	//			eUniverse -				universe this ID belongs to
	//-----------------------------------------------------------------------------
	void SetFromSteam2 (TSteamGlobalUserID *pTSteamGlobalUserID, Constants::Steam::EUniverse eUniverse)
	{
		m_unAccountID = pTSteamGlobalUserID->m_SteamLocalUserID.Split.Low32bits * 2 + pTSteamGlobalUserID->m_SteamLocalUserID.Split.High32bits;
		m_EUniverse = eUniverse;		// set the universe
		m_EAccountType = Constants::Steam::k_EAccountTypeIndividual;	// Steam 2 accounts always map to account type of individual
		m_unAccountInstance = k_unSteamUserDesktopInstance;	// individual accounts always have an account instance ID of k_unSteamUserDesktopInstance
	}

	//-----------------------------------------------------------------------------
	// Purpose: Fills out a Steam2 ID structure
	// Input:	pTSteamGlobalUserID -	Steam2 ID to write to
	//-----------------------------------------------------------------------------
	void ConvertToSteam2 (TSteamGlobalUserID *pTSteamGlobalUserID) const
	{
		// only individual accounts have any meaning in Steam 2, only they can be mapped
		// Assert (m_EAccountType == Constants::Steam::k_EAccountTypeIndividual);

		pTSteamGlobalUserID->m_SteamInstanceID = 0;
		pTSteamGlobalUserID->m_SteamLocalUserID.Split.High32bits = m_unAccountID % 2;
		pTSteamGlobalUserID->m_SteamLocalUserID.Split.Low32bits = m_unAccountID / 2;
	}
#endif // defined (INCLUDED_STEAM_COMMON_STEAMCOMMON_H)
*/
	//-----------------------------------------------------------------------------
	// Purpose: Converts steam ID to its 64-bit representation
	// Output : 64-bit representation of a Steam ID
	//-----------------------------------------------------------------------------
	unsigned long long ConvertToUint64 (void) const
	{
//		return (unsigned long long) ((((unsigned long long) m_EUniverse) << 56) + (((unsigned long long) m_EAccountType) << 52) + (((unsigned long long) m_unAccountInstance) << 32) + m_unAccountID);
		return m_unAll64Bits;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Converts the static parts of a steam ID to a 64-bit representation.
	//			For multiseat accounts, all instances of that account will have the
	//			same static account key, so they can be grouped together by the static
	//			account key.
	// Output : 64-bit static account key
	//-----------------------------------------------------------------------------
	unsigned long long GetStaticAccountKey (void) const
	{
		// note we do NOT include the account instance (which is a dynamic property) in the static account key
		return (unsigned long long) ((((unsigned long long) m_EUniverse) << 56) + ((unsigned long long) m_EAccountType << 52) + m_unAccountID);
	}

	//-----------------------------------------------------------------------------
	// Purpose: create an anonymous game server login to be filled in by the AM
	//-----------------------------------------------------------------------------
	void CreateBlankAnonLogon (Constants::Steam::EUniverse eUniverse)
	{
		m_unAccountID = 0;
		m_EAccountType = Constants::Steam::k_EAccountTypeAnonGameServer;
		m_EUniverse = eUniverse;
		m_unAccountInstance = 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: create an anonymous game server login to be filled in by the AM
	//-----------------------------------------------------------------------------
	void CreateBlankAnonUserLogon (Constants::Steam::EUniverse eUniverse)
	{
		m_unAccountID = 0;
		m_EAccountType = Constants::Steam::k_EAccountTypeAnonUser;
		m_EUniverse = eUniverse;
		m_unAccountInstance = 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous game server login that will be filled in?
	//-----------------------------------------------------------------------------
	bool BBlankAnonAccount (void) const
	{
		return m_unAccountID == 0 && BAnonAccount () && m_unAccountInstance == 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a game server account id?
	//-----------------------------------------------------------------------------
	bool BGameServerAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeGameServer || m_EAccountType == Constants::Steam::k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a content server account id?
	//-----------------------------------------------------------------------------
	bool BContentServerAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeContentServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a clan account id?
	//-----------------------------------------------------------------------------
	bool BClanAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeClan;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a chat account id?
	//-----------------------------------------------------------------------------
	bool BChatAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeChat;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a chat account id?
	//-----------------------------------------------------------------------------
	bool IsLobby (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeChat && m_unAccountInstance & k_EChatInstanceFlagLobby;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an individual user account id?
	//-----------------------------------------------------------------------------
	bool BIndividualAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeIndividual || m_EAccountType == Constants::Steam::k_EAccountTypeConsoleUser;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous account?
	//-----------------------------------------------------------------------------
	bool BAnonAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeAnonUser || m_EAccountType == Constants::Steam::k_EAccountTypeAnonGameServer;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this an anonymous user account? ( used to create an account or reset a password )
	//-----------------------------------------------------------------------------
	bool BAnonUserAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeAnonUser;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Is this a faked up Steam ID for a PSN friend account?
	//-----------------------------------------------------------------------------
	bool BConsoleUserAccount (void) const
	{
		return m_EAccountType == Constants::Steam::k_EAccountTypeConsoleUser;
	}

	// simple accessors
	void SetAccountID (unsigned int unAccountID) { m_unAccountID = unAccountID; }
	void SetAccountInstance (unsigned int unInstance) { m_unAccountInstance = unInstance; }
	void ClearIndividualInstance (void)
	{
		if (BIndividualAccount ())
			m_unAccountInstance = 0;
	}
	bool HasNoIndividualInstance (void) const		{ return BIndividualAccount () && m_unAccountInstance == 0; }
	unsigned int GetAccountID (void) const					{ return m_unAccountID; }
	unsigned int GetUnAccountInstance (void) const			{ return m_unAccountInstance; }
	Constants::Steam::EAccountType GetEAccountType (void) const		{ return static_cast <Constants::Steam::EAccountType> (m_EAccountType); }
	Constants::Steam::EUniverse GetEUniverse (void) const				{ return m_EUniverse; }
	void SetEUniverse (Constants::Steam::EUniverse eUniverse)	{ m_EUniverse = eUniverse; }
	bool IsValid (void) const
	{
		if (m_EAccountType <= Constants::Steam::k_EAccountTypeInvalid || m_EAccountType >= Constants::Steam::k_EAccountTypeMax)
			return false;

		if (m_EUniverse <= Constants::Steam::k_EUniverseInvalid || m_EUniverse >= Constants::Steam::k_EUniverseMax)
			return false;

		if (m_EAccountType == Constants::Steam::k_EAccountTypeIndividual)
		{
			if (m_unAccountID == 0 || m_unAccountInstance > k_unSteamUserConsoleInstance)
				return false;
		}
		else if (m_EAccountType == Constants::Steam::k_EAccountTypeClan)
		{
			if (m_unAccountID == 0 || m_unAccountInstance != 0)
				return false;
		}

		return true;
	}

	const char *const Render (void) const				// renders this steam ID to string
	{
		static char szSteamID[64];

		switch (m_EAccountType)
		{
			case Constants::Steam::k_EAccountTypeInvalid:
			case Constants::Steam::k_EAccountTypeIndividual:
				_snprintf (szSteamID, sizeof (szSteamID), "STEAM_0:%u:%u", (m_unAccountID % 2) ? 1 : 0, static_cast <int> (m_unAccountID) / 2);

				return szSteamID;

			default:
				_snprintf (szSteamID, sizeof (szSteamID), "%llu", ConvertToUint64 ());

				return szSteamID;
		}
	}
	static const char *const Render (unsigned long long ulSteamID)	// static method to render a unsigned long long representation of a steam ID to a string
	{
		return ID (ulSteamID).Render ();
	}

	const char *const SteamRender (void) const // renders this steam ID to string using the new rendering style
	{
		const unsigned char k_cBufLen = 37;
		const unsigned char k_cBufs = 4;
		char *pchBuf;

		static char rgchBuf[k_cBufs][k_cBufLen];
		static unsigned char nBuf = 0;

		pchBuf = rgchBuf[nBuf++];
		nBuf %= k_cBufs;

		switch (m_EAccountType)
		{
			case Constants::Steam::k_EAccountTypeAnonGameServer:
				_snprintf (pchBuf, k_cBufLen, "[A:%u:%u:%u]", m_EUniverse, m_unAccountID, m_unAccountInstance);

				return pchBuf;

			case Constants::Steam::k_EAccountTypeGameServer:
				_snprintf (pchBuf, k_cBufLen, "[G:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;

			case Constants::Steam::k_EAccountTypeMultiseat:
				_snprintf (pchBuf, k_cBufLen, "[M:%u:%u:%u]", m_EUniverse, m_unAccountID, m_unAccountInstance);

				return pchBuf;

			case Constants::Steam::k_EAccountTypePending:
				_snprintf (pchBuf, k_cBufLen, "[P:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;

			case Constants::Steam::k_EAccountTypeContentServer:
				_snprintf (pchBuf, k_cBufLen, "[C:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;

			case Constants::Steam::k_EAccountTypeClan:
				_snprintf (pchBuf, k_cBufLen, "[g:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;

			case Constants::Steam::k_EAccountTypeChat:
				switch (m_unAccountInstance & ~k_EChatAccountInstanceMask)
				{
					case k_EChatInstanceFlagClan:
						_snprintf (pchBuf, k_cBufLen, "[c:%u:%u]", m_EUniverse, m_unAccountID);

						return pchBuf;

					case k_EChatInstanceFlagLobby:
						_snprintf (pchBuf, k_cBufLen, "[L:%u:%u]", m_EUniverse, m_unAccountID);

						return pchBuf;

					default:
						_snprintf (pchBuf, k_cBufLen, "[T:%u:%u]", m_EUniverse, m_unAccountID);

						return pchBuf;
				}

			case Constants::Steam::k_EAccountTypeInvalid:
				_snprintf (pchBuf, k_cBufLen, "[I:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;

			case Constants::Steam::k_EAccountTypeIndividual:
				_snprintf (pchBuf, k_cBufLen, "[U:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;

			default:
				_snprintf (pchBuf, k_cBufLen, "[i:%u:%u]", m_EUniverse, m_unAccountID);

				return pchBuf;
		}
	}

	static const char *const SteamRender (unsigned long long ulSteamID)	// static method to render a uint64 representation of a steam ID to a string
	{
		return ID (ulSteamID).SteamRender ();
	}

	void SetFromString (const char *const pchSteamID, Constants::Steam::EUniverse eDefaultUniverse);
	bool SetFromSteam2String (const char *const pchSteam2ID, Constants::Steam::EUniverse eUniverse);

	// DEBUG function
	bool BValidExternalSteamID (void) const;

public:
	inline const bool operator == (const ID &val) const { return m_unAll64Bits == val.m_unAll64Bits; }
	inline const bool operator != (const ID &val) const { return !operator == (val); }
	inline const bool operator  < (const ID &val) const { return ConvertToUint64 () < val.ConvertToUint64 (); }
	inline const bool operator  > (const ID &val) const { return ConvertToUint64 () > val.ConvertToUint64 (); }

	inline operator       unsigned long long & (void)       { return m_unAll64Bits; }
	inline operator const unsigned long long   (void) const { return ConvertToUint64 (); }
};	// sizeof (ID) == 8 (2)

// generic invalid ID
const ID k_steamIDNil ();

// This steamID comes from a user game connection to an out of date GS that hasnt implemented the protocol to provide its steamID
const ID k_steamIDOutofDateGS (0, 0, Constants::Steam::k_EUniverseInvalid, Constants::Steam::k_EAccountTypeInvalid);
// This steamID comes from a user game connection to an sv_lan GS
const ID k_steamIDLanModeGS (0, 0, Constants::Steam::k_EUniversePublic, Constants::Steam::k_EAccountTypeInvalid);
// This steamID can come from a user game connection to a GS that has just booted but hasnt yet even initialized its steam3 component and started logging on.
const ID k_steamIDNotInitYetGS (1, 0, Constants::Steam::k_EUniverseInvalid, Constants::Steam::k_EAccountTypeInvalid);

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_STEAM_ID_INCLUDED