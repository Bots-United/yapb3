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
// Interface definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_DEFINITIONS_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_DEFINITIONS_INCLUDED

#include <Half-Life engine/SDK/Classes/Interfaces/Interface.hpp>

abstract_class IBaseSystem;

///@{ FROM HL SOURCE IAppSystem.h FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (UNUSED IN HL 1 ?!?!?!?!??!?!?!?!?!??!?!?!?!?!?!??!?!?!?!?!?!)
// Purpose: An application framework.

//-----------------------------------------------------------------------------
// Client systems are singleton objects in the client codebase responsible for
// various tasks
// The order in which the client systems appear in this list are the
// order in which they are initialized and updated. They are shut down in
// reverse order from which they are initialized.
//-----------------------------------------------------------------------------

enum InitReturnVal_t
{
	INIT_FAILED,
	INIT_OK,
//	INIT_TRY_LATER
};

abstract_class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other
	virtual bool Connect (Interface::CreateFunction_t factory) = 0;
	virtual void Disconnect (void) = 0;

	// Here's where systems can access other interfaces implemented by this object
	// Returns NULL if it doesn't implement the requested interface
	virtual void *const QueryInterface (const char *const interfaceName) = 0;

	// Init, shutdown
	virtual InitReturnVal_t Init (void) = 0;
	virtual void Shutdown (void) = 0;
};
///@}

#include <Half-Life engine/SDK/Classes/Interfaces/Interface_Engine.hpp>
#include <Half-Life engine/SDK/Classes/Interfaces/Interface_FileSystem.hpp>

#include <Half-Life engine/SDK/Classes/Interfaces/Interface_CareerUI.hpp>
#include <Half-Life engine/SDK/Classes/Interfaces/Interface_GameUI.hpp>

namespace Voice
{
	#include <Half-Life engine/SDK/Classes/Interfaces/Voice/Interface_VoiceTweak.hpp>
	#include <Half-Life engine/SDK/Classes/Interfaces/Voice/Interface_VoiceCodec.hpp>
	#include <Half-Life engine/SDK/Classes/Interfaces/Voice/Interface_VoiceRecord.hpp>
}

abstract_class ICDAudio
{
	public:
		virtual int  Init     (void) = 0;
		virtual void Shutdown (void) = 0;

		virtual void Play     (int track, bool looping) = 0;
		virtual void PlayFile (const char *const filename, bool looping) = 0;
		virtual void Pause    (void) = 0;
		virtual void Resume   (void) = 0;
		virtual void Frame    (void) = 0;
		virtual void UNKNOWN1 (void) = 0;
		virtual void UNKNOWN2 (void) = 0;
};
//-----------------------------------------------------------------------------
// Purpose: Implements cd audio handling
//-----------------------------------------------------------------------------
class CDAudio : public ICDAudio
{
	public:
		virtual ~CDAudio (void) = 0;

	public:
		enum { MAX_REMAP = 100u };

	public:
/*!off=4(1)*/		float        m_flPlayTime;	// Length of the currently playing cd track
/*!off=8(2)*/		double       m_dStartTime;	// Time when we started playing the cd
/*!off=16(4)*/		double       m_dPauseTime;	// Time when we entered pause
/*!off=24(6)*/		float        m_UNKNOWN1;
/*!off=28(7)*/		unsigned int m_UNKNOWN2;
/*!off=32(8)*/		bool         m_bIsValid;
/*!off=33(8.25)*/		bool         m_bIsPlaying;
/*!off=34(8.5)*/		bool         m_bWasPlaying;
/*!off=35(8.75)*/		bool         m_bInitialized;
/*!off=36(9)*/		bool         m_bEnabled;
/*!off=37(9.25)*/		bool         m_bIsLooping;
/*!off=40(10)*/		float        m_flVolume;
/*!off=44(11)*/		unsigned int m_UNKNOWN3;
/*!off=48(12)*/		int          m_nPlayTrack;
/*!off=564(141)*/		int          m_nMaxTrack;
/*!off=568(142)*/		bool         m_bResumeOnSwitch;	// Special flag for resuming an actually playing track when we switch to/from launcher.
/*!off=572(143)*/		unsigned int m_uiDeviceID;
/*!off=576(144)*/		int          m_rgRemapCD[MAX_REMAP];
/*!off=976(244)*/		bool         m_UNKNOWN4;
/*!off=977(244.25)*/		bool         m_UNKNOWN5;
/*!off=978(244.5)*/		bool         m_UNKNOWN6;
/*!off=979(244.75)*/		char         m_MP3Title[512u];
/*!off=1492(373)*/		int          m_MP3PlayTrack;
/*!off=1500(375)*/		int          m_MP3Volume;
};

abstract_class IDemoPlayer
{
	public:
		// theres more but i don't have time to reverse more of the vtable
		virtual void   UnknownFunction0x00 (void) = 0;
		virtual void   UnknownFunction0x04 (void) = 0;
		virtual void   UnknownFunction0x08 (void) = 0;
		virtual void   UnknownFunction0x0C (void) = 0;
		virtual void   UnknownFunction0x10 (void) = 0;
		virtual void   UnknownFunction0x14 (void) = 0;
		virtual void   UnknownFunction0x18 (void) = 0;
		virtual void   UnknownFunction0x1c (void) = 0;
		virtual void   UnknownFunction0x20 (void) = 0;
		virtual void   UnknownFunction0x24 (void) = 0;
		virtual void   UnknownFunction0x28 (void) = 0;
		virtual void   UnknownFunction0x2c (void) = 0;
		virtual double GetDemoTime         (void) = 0;
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_DEFINITIONS_INCLUDED