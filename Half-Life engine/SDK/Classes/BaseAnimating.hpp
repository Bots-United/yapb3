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
// BaseAnimating.hpp
//
// Class: BaseAnimating
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BASE_ANIMATING_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_BASE_ANIMATING_INCLUDED

class BaseAnimating : public BaseDelay
{
	public:
#if 0
		virtual int Save (CSave &save);
		virtual int Restore (CRestore &restore);

		static TYPEDESCRIPTION m_SaveData[];

		// Basic Monster Animation functions
		float StudioFrameAdvance (float flInterval = 0.0f);	// accumulate animation frame time from last time called until now
		int GetSequenceFlags (void);
		int LookupActivity (int activity);
		int LookupActivityHeaviest (int activity);
		int LookupSequence (const char *label);
		void ResetSequenceInfo (void);
		void DispatchAnimEvents (float flFutureInterval = 0.1f);	// Handle events that have happend since last time called up until X seconds into the future
		virtual void HandleAnimEvent (MonsterEvent_t *pEvent) { /* Empty */ }
		float SetBoneController (int iController, float flValue);
		void InitBoneControllers (void);
		float SetBlending (int iBlender, float flValue);
		void GetBonePosition (int iBone, Math::Vector3D &origin, Math::Vector3D &angles);
		void GetAutomovement (Math::Vector3D &origin, Math::Vector3D &angles, float flInterval = 0.1f);
		int  FindTransition (int iEndingSequence, int iGoalSequence, int *piDir);
		void GetAttachment (int iAttachment, Math::Vector3D &origin, Math::Vector3D &angles);
		void SetBodygroup (int iGroup, int iValue);
		int GetBodygroup (int iGroup);
		int ExtractBbox (int sequence, Math::Vector3D &mins, Math::Vector3D &maxs);
		void SetSequenceBox (void);
#endif	// if 0
		// animation needs
		float m_flFrameRate;		// computed FPS for current sequence
		float m_flGroundSpeed;		// computed linear movement rate for current sequence
		float m_flLastEventCheck;	// last time the event list was checked
		BOOL  m_fSequenceFinished;	// flag set when StudioAdvanceFrame moves across a frame boundry
		BOOL  m_fSequenceLoops;		// true if the sequence loops
};

#endif // HALF_LIFE_ENGINE_SDK_CLASSES_BASE_ANIMATING_INCLUDED