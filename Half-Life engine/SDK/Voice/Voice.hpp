//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VOICE_H
#define VOICE_H
#pragma once

/*! @defgroup Voice Voice
Defines the engine's interface to the voice code.
@{
*/


#define VOICE_OUTPUT_SAMPLE_RATE 11025	// Sample rate that we feed to the mixer.


//! Returned on error from certain voice functions.
#define VOICE_CHANNEL_ERROR			-1
#define VOICE_CHANNEL_IN_TWEAK_MODE	-2	// Returned by AssignChannel if currently in tweak mode (not an error).


//! Initialize the voice code.
bool Voice_Init (const char *pCodec, const unsigned char quality);

//! Shutdown the voice code.
void Voice_Deinit (void);

//! Returns true if the user can hear themself speak.
bool Voice_GetLoopback (void);

//! This is called periodically by the engine when the server acks the local player talking.
//! This tells the client DLL that the local player is talking and fades after about 200ms.
void Voice_LocalPlayerTalkingAck (void);

//! Call every frame to update the voice stuff.
void Voice_Idle (float frametime);

//! Returns true if mic input is currently being recorded.
bool Voice_IsRecording (void);

//! Begin recording input from the mic.
bool Voice_RecordStart
(
	//! Filename to store incoming mic data, or NULL if none.
	const char *pUncompressedFile,

	//! Filename to store the output of compression and decompressiong with the codec, or NULL if none.
	const char *pDecompressedFile,

	//! If this is non-null, the voice manager will use this file for input instead of the mic.
	const char *pMicInputFile
);

//! Stop recording from the mic.
bool Voice_RecordStop (void);

//! Get the most recent N bytes of compressed data. If nCount is less than the number of
//! available bytes, it discards the first bytes and gives you the last ones.
//! Set bFinal to true on the last call to this (it will flush out any stored voice data).
const unsigned short Voice_GetCompressedData (char *pchData, int nCount, bool bFinal);

//! Pass incoming data from the server into here.
//! The data should have been compressed and gotten through a Voice_GetCompressedData call.
int Voice_AddIncomingData
(
	//! Channel index.
	int nChannel,
	//! Compressed data to add to the channel.
	const char *pchData,
	//! Number of bytes in pchData.
	int nCount,
	//! Sequence number. If a packet is missed, it adds padding so the time isn't squashed.
	int iSequenceNumber
);

//! Call this to reserve a voice channel for the specified entity to talk into.
//! \return A channel index for use with Voice_AddIncomingData or VOICE_CHANNEL_ERROR on error.
int Voice_AssignChannel (int nEntity);

//! Call this to get the channel index that the specified entity is talking into.
//! \return A channel index for use with Voice_AddIncomingData or VOICE_CHANNEL_ERROR if the entity isn't talking.
int Voice_GetChannel (int nEntity);

extern const Classes::Interface::Voice::IVoiceTweak g_VoiceTweakAPI;

/*! @} */

#endif // VOICE_H