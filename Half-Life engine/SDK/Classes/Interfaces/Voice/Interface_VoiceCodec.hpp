//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: Define the IVoiceCodec interface.
//
// $NoKeywords: $
//=============================================================================

#ifndef IVOICECODEC_H
#define IVOICECODEC_H
#pragma once

#define BYTES_PER_SAMPLE 2

// This interface is for voice codecs to implement.

// Codecs are guaranteed to be called with the exact output from Compress into Decompress (ie:
// data won't be stuck together and sent to Decompress).

// Decompress is not guaranteed to be called in any specific order relative to Compress, but 
// Codecs maintain state between calls, so it is best to call Compress with consecutive voice data
// and decompress likewise. If you call it out of order, it will sound wierd. 

// In the same vein, calling Decompress twice with the same data is a bad idea since the state will be
// expecting the next block of data, not the same block.

abstract_class IVoiceCodec : public Base
{
	public:
		// Initialize the object. The uncompressed format is always 8-bit signed mono.
/*! off=4(1) */		virtual const bool Init (const unsigned int quality) = 0;

		// Use this to delete the object.
/*! off=8(2) */		virtual void Release (void) = 0;

		// Compress the voice data.
		// pUncompressed		-	16-bit signed mono voice data.
		// maxCompressedBytes	-	The length of the pCompressed buffer. Don't exceed this.
		// bFinal				-	Set to true on the last call to Compress (the user stopped talking).
		//							Some codecs like big block sizes and will hang onto data you give them in Compress calls.
		//							When you call with bFinal, the codec will give you compressed data no matter what.
		// Return the number of bytes you filled into pCompressed.
/*! off=12(3) */		virtual const int Compress (const char *const pUncompressed, const int nSamples, char *const pCompressed, const int maxCompressedBytes, const bool bFinal) = 0;

		// Decompress voice data. pUncompressed is 16-bit signed mono.
/*! off=16(4) */		virtual const int Decompress (const char *const pCompressed, const int compressedBytes, char *const pUncompressed, const int maxUncompressedBytes) = 0;

		// Some codecs maintain state between Compress and Decompress calls. This should clear that state.
/*! off=20(5) */		virtual const bool ResetState (void) = 0;
};

#endif // IVOICECODEC_H