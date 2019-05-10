/*==============================================================================
   Copyright (c) Valve, LLC. 1999
   All Rights Reserved.  Proprietary.
--------------------------------------------------------------------------------
   Author : DSpeyrer
   Purpose:
--------------------------------------------------------------------------------
 * $Log:                      $
 *
 * $NoKeywords:
==============================================================================*/

#include <Core.hpp>

namespace HalfLifeEngine
{
	namespace SDK
	{
		namespace Voice
		{
void Voice_EndChannel (int iChannel);

// Special entity index used for tweak mode.
#define TWEAKMODE_ENTITYINDEX				-500

// Special channel index passed to Voice_AddIncomingData when in tweak mode.
#define TWEAKMODE_CHANNELINDEX				-100


// How long does the sign stay above someone's head when they talk?
#define SPARK_TIME		0.2f

// How long a voice channel has to be inactive before we free it.
#define DIE_COUNTDOWN	0.5f

#define VOICE_RECEIVE_BUFFER_SIZE			 (VOICE_OUTPUT_SAMPLE_RATE * BYTES_PER_SAMPLE)

#define TIME_PADDING						0.2f	// Time between receiving the first voice packet and actually starting
													// to play the sound. This accounts for frametime differences on the clients
													// and the server.

#define LOCALPLAYERTALKING_TIMEOUT			0.2f	// How long it takes for the client to decide the server isn't sending acks
													// of voice data back.

// The format we sample voice in.
const WAVEFORMATEX g_VoiceSampleFormat =
{
	WAVE_FORMAT_PCM,		// wFormatTag
	1,						// nChannels
	8000,					// nSamplesPerSec
	16000,					// nAvgBytesPerSec
	2,						// nBlockAlign
	16,						// wBitsPerSample
	sizeof (WAVEFORMATEX)	// cbSize
};
/*
int g_nVoiceFadeSamples		= 1;							// Calculated each frame from the cvar.
float g_VoiceFadeMul		= 1.0f;							// 1.0f / (g_nVoiceFadeSamples - 1).

// While in tweak mode, you can't hear anything anyone else is saying, and your own voice data goes directly to the speakers.
bool g_bInTweakMode = false;
*/
bool g_bVoiceAtLeastPartiallyInitted = false;
/*
IMixerControls *g_pMixerControls = NULL;

// Timing info for each frame.
static double	g_CompressTime = 0.0;
static double	g_DecompressTime = 0.0;
static double	g_GainTime = 0.0;
static double	g_UpsampleTime = 0.0;

class CVoiceTimer
{
	private:
		double m_startTime;

	public:
		inline CVoiceTimer (void) : m_startTime (0.0) { /* VOID *//* }

	public:
		inline void Start (void)
		{
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_profile)->GetValue <bool> ())
				m_startTime = Globals::g_halfLifeEngine->GetSystemTime ();
		}
		inline void End (double &out)
		{
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_profile)->GetValue <bool> ())
				out += Globals::g_halfLifeEngine->GetSystemTime () - m_startTime;
		}
};

static bool		g_bLocalPlayerTalkingAck = false;
static float	g_LocalPlayerTalkingTimeout = 0.0f;
*/
Classes::Interface::CSysModule *g_hVoiceCodecDLL = NULL;

//static IVoiceRecord *g_pVoiceRecord = NULL;	// Voice recorder. Can be waveIn, DSound, or whatever.
static Classes::Interface::Voice::IVoiceCodec  *g_pEncodeCodec = NULL;

static bool			g_bVoiceRecording = false;	// Are we recording at the moment?
/*
// Hacked functions to create the inputs and codecs..
extern IVoiceRecord *CreateVoiceRecord_DSound (int nSamplesPerSec);

//
// Used for storing incoming voice data from an entity.
//
class CVoiceChannel
{
	public:
		inline CVoiceChannel (void) : m_iEntity (-1), m_pVoiceCodec (NULL) { /* VOID *//* }

		// Called when someone speaks and a new voice channel is setup to hold the data.
		inline void Init (const int nEntity)
		{
			m_iEntity = nEntity;
			m_bStarved = false;
			m_Buffer.Flush ();
			m_TimePad = TIME_PADDING;
			m_LastSample = 0;
			m_LastFraction = 0.999;

			m_AutoGain.Reset (128, Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_maxgain)->GetValue <float> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_avggain)->GetValue <float> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_scale)->GetValue <float> ());
		}

	public:
		int								m_iEntity;		// Number of the entity speaking on this channel (index into cl_entities).
														// This is -1 when the channel is unused.

		CSizedCircularBuffer <VOICE_RECEIVE_BUFFER_SIZE> m_Buffer;		// Circular buffer containing the voice data.

		// Used for upsampling..
		double							m_LastFraction;	// Fraction between m_LastSample and the next sample.
		short							m_LastSample;

		bool							m_bStarved;		// Set to true when the channel runs out of data for the mixer.
														// The channel is killed at that point.

		float							m_TimePad;		// Set to TIME_PADDING when the first voice packet comes in from a sender.
														// We add time padding (for frametime differences)
														// by waiting a certain amount of time before starting to output the sound.

		Classes::Interface::Voice::IVoiceCodec						*m_pVoiceCodec;	// Each channel gets is own IVoiceCodec instance so the codec can maintain state.

		CAutoGain						m_AutoGain;		// Gain we're applying to this channel

		CVoiceChannel					*m_pNext;
};

// Incoming voice channels.
CVoiceChannel g_VoiceChannels[VOICE_NUM_CHANNELS];
*/
// These are used for recording the wave data into files for debugging.
#define MAX_WAVEFILEDATA_LEN	1024*1024
char *g_pUncompressedFileData = NULL;
int g_nUncompressedDataBytes = 0;
const char *g_pUncompressedDataFilename = NULL;

char *g_pDecompressedFileData = NULL;
int g_nDecompressedDataBytes = 0;
const char *g_pDecompressedDataFilename = NULL;

int dword_24A96FC = 0;

char *g_pMicInputFileData = NULL;
unsigned int g_nMicInputFileBytes = 0u;
int g_CurMicInputFileByte = 0;
double g_MicStartTime;
/*
inline void ApplyFadeToSamples (short *const pSamples, const int nSamples, const int fadeOffset, const float fadeMul)
{
	for (int i (0); i < nSamples; ++i)
	{
		const float percent ((i + fadeOffset) * fadeMul);

		pSamples[i] = static_cast <short> (pSamples[i] * (1.0f - percent));
	}
}

int Voice_GetOutputData
(
	const int iChannel,			//! The voice channel it wants samples from.
	char *copyBufBytes,			//! The buffer to copy the samples into.
	const int copyBufSize,		//! Maximum size of copyBuf.
	const int samplePosition,	//! Which sample to start at.
	const int sampleCount		//! How many samples to get.
)
{
	CVoiceChannel *const pChannel = &g_VoiceChannels[iChannel];
	short *const pCopyBuf = (short *)copyBufBytes;
	const int maxOutSamples = copyBufSize / BYTES_PER_SAMPLE;

	// Find out how much we want and get it from the received data channel.
	CCircularBuffer *const pBuffer = &pChannel->m_Buffer;
	int nBytesToRead = pBuffer->GetReadAvailable ();
	nBytesToRead = Math::GetMinimumValueBetween <int> (Math::GetMinimumValueBetween <int> (nBytesToRead, maxOutSamples), sampleCount * BYTES_PER_SAMPLE);
	int nSamplesGotten = pBuffer->Read (pCopyBuf, nBytesToRead) / BYTES_PER_SAMPLE;

	// Are we at the end of the buffer's data? If so, fade data to silence so it doesn't clip.
	const int readSamplesAvail (pBuffer->GetReadAvailable () / BYTES_PER_SAMPLE);
	if (readSamplesAvail < g_nVoiceFadeSamples)
	{
		int bufferFadeOffset = Math::GetMaximumValueBetween <int> ((readSamplesAvail + nSamplesGotten) - g_nVoiceFadeSamples, 0);
		int globalFadeOffset = Math::GetMaximumValueBetween <int> (g_nVoiceFadeSamples - (readSamplesAvail + nSamplesGotten), 0);

		ApplyFadeToSamples
		(
			pCopyBuf + bufferFadeOffset,
			nSamplesGotten - bufferFadeOffset,
			globalFadeOffset,
			g_VoiceFadeMul
		);
	}

	// If there weren't enough bytes in the received data channel, pad it with zeros.
	if (nSamplesGotten < sampleCount)
	{
		int wantedSampleCount = Math::GetMinimumValueBetween <int> (sampleCount, maxOutSamples);
		memset (pCopyBuf + nSamplesGotten, 0, (wantedSampleCount - nSamplesGotten) * BYTES_PER_SAMPLE);
		nSamplesGotten = wantedSampleCount;
	}

	// If the buffer is out of data, mark this channel to go away.
	if (pBuffer->GetReadAvailable () == 0)
		pChannel->m_bStarved = true;

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_showchannels)->GetValue <int> () >= 2)
		Globals::g_halfLifeEngine->ServerPrintFormat ("Voice - mixed %d samples from channel %d\n", nSamplesGotten, iChannel);

///	VoiceSE_MoveMouth (pChannel->m_iEntity, (short*)copyBufBytes, nSamplesGotten);

	return nSamplesGotten;
}

void Voice_OnAudioSourceShutdown (int iChannel)
{
	Voice_EndChannel (iChannel);
}

// ------------------------------------------------------------------------ //
// Internal stuff.
// ------------------------------------------------------------------------ //

CVoiceChannel* GetVoiceChannel (int iChannel, bool bAssert = true)
{
	if (iChannel < 0 || iChannel >= VOICE_NUM_CHANNELS)
	{
		if (bAssert)
			InternalAssert (false);

		return NULL;
	}

	return g_VoiceChannels + iChannel;
}
*/
bool Voice_Init (const char *pCodecName, const unsigned char quality)
{
//	AddLogEntry (false, LogLevel_Critical, true, "%u", (unsigned int)&g_VoiceChannels[0].m_AutoGain - (unsigned int)g_VoiceChannels);

	if (!Console::Server::variableManager.GetVariable (Console::Server::VariableID_VoiceEnable)->GetValue <bool> ())
		return false;

	// Reliability check.
	InternalAssert (!IsNullOrEmptyString (pCodecName));

	Voice_Deinit ();

	g_bVoiceAtLeastPartiallyInitted = true;

///	if (!VoiceSE_Init ())
///		return false;

////	const int rate (g_VoiceSampleFormat.nSamplesPerSec);
/*
	// Get the voice input device.
	g_pVoiceRecord = NULL;////CreateVoiceRecord_DSound (rate);

	if (g_pVoiceRecord == NULL)
		Globals::g_halfLifeEngine->ServerPrint ("Unable to initialize DirectSoundCapture. You won't be able to speak to other players through a microphone.\n");
*/
	// Get the codec.
	char dllName[256u];

	_snprintf (dllName, sizeof (dllName), "%s.dll", pCodecName);

	Globals::g_halfLifeEngine->GetFileSystem ()->GetLocalCopy ("MSS32.DLL");	// voice_miles.dll will load this...
	Globals::g_halfLifeEngine->GetFileSystem ()->GetLocalCopy (dllName);

	if (g_hVoiceCodecDLL == NULL && (g_hVoiceCodecDLL = reinterpret_cast <Classes::Interface::CSysModule *> (LoadLibrary (dllName))) == NULL)
	{
		Globals::g_halfLifeEngine->ServerPrintFormat ("Unable to load voice codec DLL '%s'. Voice disabled.\n", pCodecName);

		Label_DeinitAndExit:
			Voice_Deinit ();

			return false;
	}

	const Classes::Interface::CreateFunction_t createCodecFn (reinterpret_cast <Classes::Interface::CreateFunction_t> (GetProcAddress (reinterpret_cast <HMODULE> (g_hVoiceCodecDLL), CREATEINTERFACE_PROCNAME)));

	if (createCodecFn == NULL)
	{
		Globals::g_halfLifeEngine->ServerPrintFormat ("Unable to get a factory for voice codec '%s'. Voice disabled.\n", pCodecName);

		goto Label_DeinitAndExit;
	}

	/// CALL IT BEFORE THAN IT CALLS CLIENT - UNABLE TO INIT CODEC CLIENT MESSAGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if ((g_pEncodeCodec = static_cast <Classes::Interface::Voice::IVoiceCodec *> (createCodecFn (pCodecName, NULL))) == NULL)
	{
		Globals::g_halfLifeEngine->ServerPrintFormat ("Unable to create interface for voice codec '%s'. Voice disabled.\n", pCodecName);

		goto Label_DeinitAndExit;
	}

	if (!g_pEncodeCodec->Init (quality))
	{
		Globals::g_halfLifeEngine->ServerPrintFormat ("Unable to initialize voice codec '%s'. Voice disabled.\n", pCodecName);

		goto Label_DeinitAndExit;
	}
/*
	for (int i = 0; i < VOICE_NUM_CHANNELS; ++i)
	{
		CVoiceChannel *pChannel = &g_VoiceChannels[i];

		if ((pChannel->m_pVoiceCodec = static_cast <Classes::Interface::Voice::IVoiceCodec *> (createCodecFn (pCodecName, NULL))) == NULL || !pChannel->m_pVoiceCodec->Init (quality))
		{
			Globals::g_halfLifeEngine->ServerPrintFormat ("Unable to get voice codec (%s) interface for channel %d. Voice disabled.\n", pCodecName, i);

			goto Label_DeinitAndExit;
		}
	}

	g_pMixerControls = GetMixerControls ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_forcemicrecord)->GetValue <bool> () && g_pMixerControls != NULL)
		g_pMixerControls->SelectMicrophoneForWaveInput ();
*/
	return true;
}
/*
void Voice_EndChannel (int iChannel)
{
	InternalAssert (iChannel >= 0 && iChannel < VOICE_NUM_CHANNELS);

	CVoiceChannel *pChannel = &g_VoiceChannels[iChannel];

	if (pChannel->m_iEntity != -1)
	{
//		int iTempEnt = pChannel->m_iEntity;
		pChannel->m_iEntity = -1;

///		VoiceSE_EndChannel (iChannel);
//		ClientDLL_VoiceStatus (iTempEnt, false);
///		VoiceSE_CloseMouth (iTempEnt);
	}
}

void Voice_EndAllChannels (void)
{
	for (int i = 0; i < VOICE_NUM_CHANNELS; ++i)
		Voice_EndChannel (i);
}
*/
void Voice_Deinit (void)
{
	// This call tends to be expensive and when voice is not enabled it will continually
	// call in here, so avoid the work if possible.
	if (!g_bVoiceAtLeastPartiallyInitted)
		return;

//	Voice_EndAllChannels ();

	Voice_RecordStop ();
/*
	for (int i=0; i < VOICE_NUM_CHANNELS; i++)
	{
		CVoiceChannel *pChannel = &g_VoiceChannels[i];

		if (pChannel->m_pVoiceCodec != NULL)
		{
			pChannel->m_pVoiceCodec->Release ();
			pChannel->m_pVoiceCodec = NULL;
		}
	}
*/
	if (g_pEncodeCodec != NULL)
	{
		g_pEncodeCodec->Release ();
		g_pEncodeCodec = NULL;
	}

	if (g_hVoiceCodecDLL != NULL)
	{
		FreeModule (reinterpret_cast <HMODULE> (g_hVoiceCodecDLL));
		g_hVoiceCodecDLL = NULL;
	}
/*
	if (g_pVoiceRecord != NULL)
	{
		g_pVoiceRecord->Release ();
		g_pVoiceRecord = NULL;
	}

///	VoiceSE_Term ();

	if (g_pMixerControls != NULL)
	{
		g_pMixerControls->Release ();
		g_pMixerControls = NULL;
	}
*/
	g_bVoiceAtLeastPartiallyInitted = false;
}
/*
bool Voice_GetLoopback (void)
{
	return Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_loopback)->GetValue <bool> ();
}

void Voice_LocalPlayerTalkingAck (void)
{
	if (!g_bLocalPlayerTalkingAck)
	{
		// Tell the client DLL when this changes.
//		ClientDLL_VoiceStatus (-2, TRUE);
	}

	g_bLocalPlayerTalkingAck = true;
	g_LocalPlayerTalkingTimeout = 0;
}

void Voice_UpdateVoiceTweakMode (void)
{
	if (!g_bInTweakMode || !g_pVoiceRecord)
		return;

	char uchVoiceData[4096];
	int nDataLength = Voice_GetCompressedData (uchVoiceData, sizeof (uchVoiceData), false);

	Voice_AddIncomingData (TWEAKMODE_CHANNELINDEX, uchVoiceData, nDataLength, 0);
}

void Voice_Idle (float frametime)
{
	if (!Console::Server::variableManager.GetVariable (Console::Server::VariableID_VoiceEnable)->GetValue <bool> ())
	{
		Voice_Deinit ();
		return;
	}

	if (g_bLocalPlayerTalkingAck)
	{
		g_LocalPlayerTalkingTimeout += frametime;
		if (g_LocalPlayerTalkingTimeout > LOCALPLAYERTALKING_TIMEOUT)
		{
			g_bLocalPlayerTalkingAck = false;

			// Tell the client DLL.
//			ClientDLL_VoiceStatus (-2, FALSE);
		}
	}

	// Precalculate these to speedup the voice fadeout.
	g_nVoiceFadeSamples = Math::GetMaximumValueBetween <int> (static_cast <int> (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_fadeouttime)->GetValue <float> () * VOICE_OUTPUT_SAMPLE_RATE), 2);
	g_VoiceFadeMul = 1.0f / (g_nVoiceFadeSamples - 1);

	if (g_pVoiceRecord != NULL)
		g_pVoiceRecord->Idle ();

	// If we're in voice tweak mode, feed our own data back to us.
	Voice_UpdateVoiceTweakMode ();

	// Age the channels.
	int nActive = 0;
	for (int i=0; i < VOICE_NUM_CHANNELS; i++)
	{
		CVoiceChannel *pChannel = &g_VoiceChannels[i];

		if (pChannel->m_iEntity != -1)
		{
			if (pChannel->m_bStarved)
			{
				// Kill the channel. It's done playing.
				Voice_EndChannel (i);
			}
			else
			{
				float oldpad = pChannel->m_TimePad;
				pChannel->m_TimePad -= frametime;
				if (oldpad > 0.0f && pChannel->m_TimePad <= 0.0f)
				{
					// Start its audio.
///					VoiceSE_StartChannel (i);
//					ClientDLL_VoiceStatus (pChannel->m_iEntity, TRUE);

///					VoiceSE_InitMouth (pChannel->m_iEntity);
				}

				++nActive;
			}
		}
	}

///	if (nActive == 0)
///		VoiceSE_EndOverdrive ();

///	VoiceSE_Idle (frametime);

	// voice_showchannels.
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_showchannels)->GetValue <int> () >= 1)
	{
		for (int i=0; i < VOICE_NUM_CHANNELS; i++)
		{
			CVoiceChannel *pChannel = &g_VoiceChannels[i];

			if (pChannel->m_iEntity == -1)
				continue;

			Globals::g_halfLifeEngine->ServerPrintFormat ("Voice - chan %d, ent %d, bufsize: %d\n", i, pChannel->m_iEntity, pChannel->m_Buffer.GetReadAvailable ());
		}
	}

	// Show profiling data?
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_profile)->GetValue <bool> ())
	{
		Globals::g_halfLifeEngine->ServerPrintFormat
		(
			"Voice - compress: %7.2fu, decompress: %7.2fu, gain: %7.2fu, upsample: %7.2fu, total: %7.2fu\n",
			g_CompressTime*1000000.0,
			g_DecompressTime*1000000.0,
			g_GainTime*1000000.0,
			g_UpsampleTime*1000000.0,
			(g_CompressTime+g_DecompressTime+g_GainTime+g_UpsampleTime)*1000000.0
		);

		g_CompressTime = g_DecompressTime = g_GainTime = g_UpsampleTime = 0.0;
	}
}
*/
bool Voice_IsRecording (void)
{
	return g_bVoiceRecording;// && !g_bInTweakMode;
}

bool Voice_RecordStart
(
	const char *pUncompressedFile,
	const char *pDecompressedFile,
	const char *pMicInputFile
)
{
	if (g_pEncodeCodec == NULL)
		return false;

	Voice_RecordStop ();

	g_pEncodeCodec->ResetState ();

	if (pMicInputFile != NULL)
	{
		unsigned short a, b;
		unsigned int c;

		ReadWaveFile (pMicInputFile, g_pMicInputFileData, g_nMicInputFileBytes, a, b, c);
		g_CurMicInputFileByte = 0;
		g_MicStartTime = Globals::g_halfLifeEngine->GetSystemTime ();
	}

	if (pUncompressedFile != NULL)
	{
		g_pUncompressedFileData = new char[MAX_WAVEFILEDATA_LEN];
		g_nUncompressedDataBytes = 0;
		g_pUncompressedDataFilename = pUncompressedFile;
	}

	if (pDecompressedFile != NULL)
	{
		g_pDecompressedFileData = new char[MAX_WAVEFILEDATA_LEN];
		g_nDecompressedDataBytes = 0;
		g_pDecompressedDataFilename = pDecompressedFile;
	}

	g_bVoiceRecording = true;////false;
/*
	if (g_pVoiceRecord != NULL)
	{
		g_bVoiceRecording = g_pVoiceRecord->RecordStart ();
//		if (g_bVoiceRecording)
//			ClientDLL_VoiceStatus (-1, TRUE);		// Tell the client DLL.
	}
*/
	return g_bVoiceRecording;
}

bool Voice_RecordStop (void)
{
	// Write the files out for debugging.
	if (g_pMicInputFileData != NULL)
	{
		delete [] g_pMicInputFileData;
		g_pMicInputFileData = NULL;
	}

	if (g_pUncompressedFileData != NULL)
	{
		WriteWaveFile (g_pUncompressedDataFilename, g_pUncompressedFileData, g_nUncompressedDataBytes, g_VoiceSampleFormat.wBitsPerSample, g_VoiceSampleFormat.nChannels, g_VoiceSampleFormat.nSamplesPerSec);
		delete [] g_pUncompressedFileData;
		g_pUncompressedFileData = NULL;
	}

	if (g_pDecompressedFileData != NULL)
	{
		WriteWaveFile (g_pDecompressedDataFilename, g_pDecompressedFileData, g_nDecompressedDataBytes, g_VoiceSampleFormat.wBitsPerSample, g_VoiceSampleFormat.nChannels, g_VoiceSampleFormat.nSamplesPerSec);
		delete [] g_pDecompressedFileData;
		g_pDecompressedFileData = NULL;
	}
/*
	if (g_pVoiceRecord != NULL)
		g_pVoiceRecord->RecordStop ();
*/
//	if (g_bVoiceRecording)
//		ClientDLL_VoiceStatus (-1, FALSE);	// Tell the client DLL.

	g_bVoiceRecording = false;

	return true;
}

const unsigned short Voice_GetCompressedData (char *pchDest, int nCount, bool bFinal)
{
	if (/***g_pVoiceRecord == NULL || */g_pEncodeCodec == NULL)
		return 0u;

	short tempData[8192];
	int gotten (0);
//	CVoiceTimer timer;

	// If they want to get the data from a file instead of the mic, use that.
	if (g_pMicInputFileData != NULL)
	{
		const double curtime (Globals::g_halfLifeEngine->GetSystemTime ());
		const int nShouldGet (static_cast <int> ((curtime - g_MicStartTime) * g_VoiceSampleFormat.nSamplesPerSec));

		gotten = Math::GetMinimumValueBetween <int> (sizeof (tempData) / BYTES_PER_SAMPLE, Math::GetMinimumValueBetween <int> (nShouldGet, (g_nMicInputFileBytes - g_CurMicInputFileByte) / BYTES_PER_SAMPLE));

		memcpy (tempData, g_pMicInputFileData + g_CurMicInputFileByte, gotten * BYTES_PER_SAMPLE);

		g_CurMicInputFileByte += gotten * BYTES_PER_SAMPLE;
		g_MicStartTime = curtime;
	}
/*	else
		gotten = g_pVoiceRecord == NULL ? 0 : g_pVoiceRecord->GetRecordedData (tempData, Math::GetMinimumValueBetween <int> (nCount/BYTES_PER_SAMPLE, sizeof (tempData)/BYTES_PER_SAMPLE));

	timer.Start ();
*/
	const unsigned short nCompressedBytes (static_cast <unsigned short> (g_pEncodeCodec->Compress ((char*)tempData, gotten, pchDest, nCount, bFinal)));

//	timer.End (g_CompressTime);

	// Write to our file buffers..
	if (g_pUncompressedFileData != NULL)
	{
		const int nToWrite (Math::GetMinimumValueBetween <int> (gotten * BYTES_PER_SAMPLE, MAX_WAVEFILEDATA_LEN - g_nUncompressedDataBytes));

		memcpy (g_pUncompressedFileData + g_nUncompressedDataBytes, tempData, nToWrite);

		g_nUncompressedDataBytes += nToWrite;
	}

	return nCompressedBytes;
}
/*
//------------------ Copyright (c) 1999 Valve, LLC. ----------------------------
// Purpose: Assigns a channel to an entity by searching for either a channel
//			already assigned to that entity or picking the least recently used
//			channel. If the LRU channel is picked, it is flushed and all other
//			channels are aged.
// Input  : nEntity - entity number to assign to a channel.
// Output : A channel index to which the entity has been assigned.
//------------------------------------------------------------------------------
int Voice_AssignChannel (int nEntity)
{
	if (g_bInTweakMode)
		return VOICE_CHANNEL_IN_TWEAK_MODE;

	// See if a channel already exists for this entity and if so, just return it.
	int iFree = VOICE_CHANNEL_ERROR;
	for (int i=0; i < VOICE_NUM_CHANNELS; i++)
	{
		CVoiceChannel *pChannel = &g_VoiceChannels[i];

		if (pChannel->m_iEntity == nEntity)
		{
			return i;
		}
		else if (pChannel->m_iEntity == -1 && pChannel->m_pVoiceCodec)
		{
			pChannel->m_pVoiceCodec->ResetState ();
			iFree = i;
			break;
		}
	}

	// If they're all used, then don't allow them to make a new channel.
	if (iFree == VOICE_CHANNEL_ERROR)
		return VOICE_CHANNEL_ERROR;

	CVoiceChannel *pChannel = &g_VoiceChannels[iFree];
	pChannel->Init (nEntity);
///	VoiceSE_StartOverdrive ();

	return iFree;
}

//------------------ Copyright (c) 1999 Valve, LLC. ----------------------------
// Purpose: Determines which channel has been assigened to a given entity.
// Input  : nEntity - entity number.
// Output : The index of the channel assigned to the entity, VOICE_CHANNEL_ERROR
//			if no channel is currently assigned to the given entity.
//------------------------------------------------------------------------------
int Voice_GetChannel (int nEntity)
{
	for (int i=0; i < VOICE_NUM_CHANNELS; i++)
		if (g_VoiceChannels[i].m_iEntity == nEntity)
			return i;

	return VOICE_CHANNEL_ERROR;
}

double UpsampleIntoBuffer
(
	const short *pSrc,
	int nSrcSamples,
	CCircularBuffer *pBuffer,
	double startFraction,
	double rate
)
{
	const double maxFraction (nSrcSamples - 1.0);

	while (startFraction < maxFraction)
	{
		const int iSample = static_cast <int> (startFraction);
		const double frac = startFraction - Math::floor (startFraction);
		const double val1 = pSrc[iSample];
		const double val2 = pSrc[iSample + 1];
		const short newSample = static_cast <short> (val1 + (val2 - val1) * frac);

		pBuffer->Write (&newSample, sizeof (newSample));

		startFraction += rate;
	}

	return startFraction - Math::floor (startFraction);
}

//------------------ Copyright (c) 1999 Valve, LLC. ----------------------------
// Purpose: Adds received voice data to
// Input  :
// Output :
//------------------------------------------------------------------------------
int Voice_AddIncomingData (int nChannel, const char *pchData, int nCount, int iSequenceNumber)
{
	// If in tweak mode, we call this during Idle with -1 as the channel, so any channel data from the network
	// gets rejected.
	if (g_bInTweakMode)
	{
		dword_24A96FC = 0;

		if (nChannel != TWEAKMODE_CHANNELINDEX)
			return 0;

		nChannel = 0;
	}

	CVoiceChannel *pChannel;

	if ((pChannel = GetVoiceChannel (nChannel)) == NULL || pChannel->m_pVoiceCodec == NULL)
		return 0;

	pChannel->m_bStarved = false;	// This only really matters if you call Voice_AddIncomingData between the time the mixer
									// asks for data and Voice_Idle is called.

	CVoiceTimer timer;

	timer.Start ();

	// Decompress.
	char decompressed[8192];
	int nDecompressed = pChannel->m_pVoiceCodec->Decompress (pchData, nCount, decompressed, sizeof (decompressed));

	if (g_bInTweakMode)
	{
		int v9;

		if (nDecompressed <= 0)
		{
			v9 = dword_24A96FC;
		}
		else
		{
			v9 = dword_24A96FC;
			short *decompressedPointer = (short *)decompressed;
			int count (nDecompressed);

			do
			{
				const int val (abs (*decompressedPointer));

				if (val > v9)
					v9 = val;

				++decompressedPointer;
			} while (--count > 0);
		}

		dword_24A96FC = v9 & 0xFE00;
	}

	timer.End (g_DecompressTime);
	timer.Start ();

	pChannel->m_AutoGain.ProcessSamples ((short*)decompressed, nDecompressed);

	timer.End (g_GainTime);
	timer.Start ();

	// Upsample into the dest buffer. We could do this in a mixer but it complicates the mixer.
	pChannel->m_LastFraction = UpsampleIntoBuffer
	(
		(short*)decompressed,
		nDecompressed,
		&pChannel->m_Buffer,
		pChannel->m_LastFraction,
		(double)g_VoiceSampleFormat.nSamplesPerSec/VOICE_OUTPUT_SAMPLE_RATE
	);
	pChannel->m_LastSample = decompressed[nDecompressed];

	timer.End (g_UpsampleTime);

	// Write to our file buffer..
	if (g_pDecompressedFileData != NULL)
	{
		int nToWrite = Math::GetMinimumValueBetween <int> (nDecompressed*2, MAX_WAVEFILEDATA_LEN - g_nDecompressedDataBytes);
		memcpy (g_pDecompressedFileData + g_nDecompressedDataBytes, decompressed, nToWrite);
		g_nDecompressedDataBytes += nToWrite;
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_showincoming)->GetValue <bool> ())
		Globals::g_halfLifeEngine->ServerPrintFormat ("Voice - %d incoming samples added to channel %d\n", nDecompressed, nChannel);

	return nChannel;
}

#if DEAD
//------------------ Copyright (c) 1999 Valve, LLC. ----------------------------
// Purpose: Flushes a given receive channel.
// Input  : nChannel - index of channel to flush.
//------------------------------------------------------------------------------
void Voice_FlushChannel (int nChannel)
{
	if ((nChannel < 0) || (nChannel >= VOICE_NUM_CHANNELS))
	{
		InternalAssert (false);
		return;
	}

	g_VoiceChannels[nChannel].m_Buffer.Flush ();
}
#endif


//------------------------------------------------------------------------------
// IVoiceTweak implementation.
//------------------------------------------------------------------------------

int VoiceTweak_StartVoiceTweakMode (void)
{
	// If we're already in voice tweak mode, return an error.
	if (g_bInTweakMode)
	{
		InternalAssert (!"VoiceTweak_StartVoiceTweakMode called while already in tweak mode.");
		return 0;
	}

	if (g_pMixerControls == NULL)
		return 0;

	Voice_EndAllChannels ();
	Voice_RecordStart (NULL, NULL, NULL);
	Voice_AssignChannel (TWEAKMODE_ENTITYINDEX);
	g_bInTweakMode = true;
//	g_pMixerControls = GetMixerControls ();	// NOT EXISTS IN HL1, AND IT IS LOGICAL :)

	return 1;
}

void VoiceTweak_EndVoiceTweakMode (void)
{
	if (!g_bInTweakMode)
	{
		InternalAssert (!"VoiceTweak_EndVoiceTweakMode called when not in tweak mode.");
		return;
	}

	g_bInTweakMode = false;
	Voice_RecordStop ();
}

void VoiceTweak_SetControlFloat (Classes::Interface::Voice::VoiceTweakControl iControl, float value)
{
	if (!g_pMixerControls)
		return;

	switch (iControl)
	{
		case MicrophoneVolume:
			g_pMixerControls->SetValue_Float (IMixerControls::MicVolume, value);

			break;

		case OtherSpeakerScale:
			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_scale)->SetValue (value);

			break;

		case MicBoost:
			g_pMixerControls->SetValue_Float (IMixerControls::MicBoost, value);

			break;
	}
}

float VoiceTweak_GetControlFloat (Classes::Interface::Voice::VoiceTweakControl iControl)
{
	if (!g_pMixerControls)
		return 0.0f;

	switch (iControl)
	{
		case MicrophoneVolume:
		{
			float value (1.0f);

			g_pMixerControls->GetValue_Float (IMixerControls::MicVolume, value);

			return value;
		}

		case OtherSpeakerScale:
			return Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_scale)->GetValue <float> ();

		case MicBoost:
		{
			float value (1.0f);

			g_pMixerControls->GetValue_Float (IMixerControls::MicBoost, value);

			return value;
		}

		default:
			return 1.0f;
	}
}

int VoiceTweak_sub_1DDA400 (void)
{
  return dword_24A96FC;
}

const Classes::Interface::Voice::IVoiceTweak g_VoiceTweakAPI =
{
	VoiceTweak_StartVoiceTweakMode,
	VoiceTweak_EndVoiceTweakMode,
	VoiceTweak_SetControlFloat,
	VoiceTweak_GetControlFloat,
	VoiceTweak_sub_1DDA400
};*/
		}
	}
}