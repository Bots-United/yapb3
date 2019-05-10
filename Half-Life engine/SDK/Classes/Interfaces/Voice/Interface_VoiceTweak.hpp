//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef IVOICETWEAK_H
#define IVOICETWEAK_H
#ifdef _WIN32
#pragma once
#endif

// These provide access to the voice controls.
enum VoiceTweakControl
{
	MicrophoneVolume,	// values 0-1.
	OtherSpeakerScale,	// values 0-1. Scales how loud other players are.
	MicBoost
};

struct IVoiceTweak
{
	// These turn voice tweak mode on and off. While in voice tweak mode, the user's voice is echoed back without sending to the server.
/*! off=0(0) */	int   (*StartVoiceTweakMode) (void);	// Returns 0 on error.
/*! off=4(1) */	void  (*EndVoiceTweakMode) (void);
	
	// Get/set control values.
/*! off=8(2) */	void  (*SetControlFloat) (VoiceTweakControl iControl, float value);
/*! off=12(3) */	float (*GetControlFloat) (VoiceTweakControl iControl);

/*! off=16(4) */	int (*GetUNKNOWN) (void);
};	// sizeof (IVoiceTweak) == 20 (5)


#endif // IVOICETWEAK_H
