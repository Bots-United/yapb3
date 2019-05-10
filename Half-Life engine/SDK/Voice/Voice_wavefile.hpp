//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VOICE_WAVEFILE_H
#define VOICE_WAVEFILE_H
#pragma once


// Load in a wave file. This isn't very flexible and is only guaranteed to work with files
// saved with WriteWaveFile.
bool ReadWaveFile(
	const char *const pFilename,
	char *&pData,
	unsigned int &nDataBytes,
	unsigned short &wBitsPerSample,
	unsigned short &nChannels,
	unsigned int &nSamplesPerSec);


// Write out a wave file.
bool WriteWaveFile(
	const char *const pFilename, 
	const char *const pData, 
	const unsigned int nDataBytes, 
	const unsigned short wBitsPerSample, 
	const unsigned short nChannels, 
	const unsigned int nSamplesPerSec);


#endif // VOICE_WAVEFILE_H
