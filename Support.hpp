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
// Support.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED

extern const bool  GetWaveSoundInformation   (const DynamicString &filename, DynamicString &output);
extern const float GetWaveSoundPlayDuration  (const DynamicString &filename);
extern const bool  OpenConfigurationFile     (const DynamicString &filename, const DynamicString &errorIfNotExists, BaseFile &outFile);
extern void        FreeAllMemory             (void);
extern void        RestoreAllHookedFunctions (void);
extern void        InternalAttach            (void);
extern void        InternalDetach            (void);

extern inline const bool IsMyBirthday (void)
{
	// this function returns true, if today is my birthday, false otherwise.

	// get current local time
	const tm *const localTime (GetLocalTime ());

	// is it my birthday?
	return localTime->tm_mon == 9u && localTime->tm_mday == 30u;	// w0ot, today is 30 october, it's my birthday! ={P
}
extern inline const bool IsNewYear (void)
{
	// this function returns true, if today is new year, false otherwise.

	// get current local time
	const tm *const localTime (GetLocalTime ());

	// is it my birthday?
	return localTime->tm_mon == 11u && localTime->tm_mday == 31u;	// w0ot, today is 31 december, it's new year! ={P
}

namespace XASH
{
/*
========================================================================

internal sound format

typically expanded to wav buffer
========================================================================
*/
typedef enum
{
	WF_UNKNOWN = 0,
	WF_PCMDATA,
	WF_MPGDATA,
	WF_TOTALCOUNT,	// must be last
} sndformat_t;

// wavdata output flags
typedef enum
{
	// wavdata->flags
	SOUND_LOOPED	= BIT( 0 ),	// this is looped sound (contain cue markers)
	SOUND_STREAM	= BIT( 1 ),	// this is a streaminfo, not a real sound

	// Sound_Process manipulation flags
	SOUND_RESAMPLE	= BIT(12),	// resample sound to specified rate
	SOUND_CONVERT16BIT	= BIT(13),	// change sound resolution from 8 bit to 16
} sndFlags_t;

typedef struct
{
	short	rate;		// num samples per second (e.g. 11025 - 11 khz)
	unsigned char	width;		// resolution - bum bits divided by 8 (8 bit is 1, 16 bit is 2)
	unsigned char	channels;		// num channels (1 - mono, 2 - stereo)
	int	loopStart;	// offset at this point sound will be looping while playing more than only once
	int	samples;		// total samplecount in wav
	unsigned int	type;		// compression type
	unsigned int	flags;		// misc sound flags
	unsigned char	*buffer;		// sound buffer
	size_t	size;		// for bounds checking
} wavdata_t;

typedef struct sndlib_s
{
//	const loadwavformat_t	*loadformats;
//	const streamformat_t	*streamformat;	// music stream

	// current sound state
	int		type;		// sound type
	int		rate;		// num samples per second (e.g. 11025 - 11 khz)
	int		width;		// resolution - bum bits divided by 8 (8 bit is 1, 16 bit is 2)
	int		channels;		// num channels (1 - mono, 2 - stereo)
	int		loopstart;	// start looping from
	unsigned int		samples;		// total samplecount in sound
	unsigned int		flags;		// additional sound flags
	size_t		size;		// sound unpacked size (for bounds checking)
	unsigned char		*wav;		// sound pointer (see sound_type for details)

	unsigned char		*tempbuffer;	// for convert operations
	int		cmd_flags;
} sndlib_t;

static sndlib_t sound;

static const unsigned char *iff_data;
static const unsigned char *iff_dataPtr;
static const unsigned char *iff_end;
static const unsigned char *iff_lastChunk;
static int iff_chunkLen;

/*
=================
GetLittleShort
=================
*/
static short GetLittleShort( void )
{
	short	val = 0;

	val += (*(iff_dataPtr+0) << 0);
	val += (*(iff_dataPtr+1) << 8);
	iff_dataPtr += 2;

	return val;
}

/*
=================
GetLittleLong
=================
*/
static int GetLittleLong( void )
{
	int	val = 0;

	val += (*(iff_dataPtr+0) << 0);
	val += (*(iff_dataPtr+1) << 8);
	val += (*(iff_dataPtr+2) <<16);
	val += (*(iff_dataPtr+3) <<24);
	iff_dataPtr += 4;

	return val;
}

/*
=================
FindNextChunk
=================
*/
static void FindNextChunk( const char *name )
{
	for (;;)
	{
		iff_dataPtr = iff_lastChunk;

		if( iff_dataPtr >= iff_end )
		{
			// didn't find the chunk
			iff_dataPtr = NULL;
			return;
		}
		
		iff_dataPtr += 4;
		iff_chunkLen = GetLittleLong();

		if( iff_chunkLen < 0 )
		{
			iff_dataPtr = NULL;
			return;
		}

		iff_dataPtr -= 8;
		iff_lastChunk = iff_dataPtr + 8 + ((iff_chunkLen + 1) & ~1);

		if(!strncmp( (char*)iff_dataPtr, name, 4 ))
			return;
	}
}

/*
============
StreamFindNextChunk
============
*//*
bool StreamFindNextChunk( FILE *file, const char *name, int *last_chunk )
{
	char	chunkName[4];
	int	iff_chunk_len;

	while( 1 )
	{
		fseek( file, *last_chunk, SEEK_SET );

		if( feof( file ))
			return false;	// didn't find the chunk

		fseek( file, 4, SEEK_CUR );
		fread( file, &iff_chunk_len, sizeof( iff_chunk_len ));
		if( iff_chunk_len < 0 )
			return false;	// didn't find the chunk

		fseek( file, -8, SEEK_CUR );
		*last_chunk = ftell( file ) + 8 + (( iff_chunk_len + 1 ) & ~1 );
		fread( file, chunkName, 4 );
		if( !strncmp( chunkName, name, 4 ))
			return true;
	}
	return false;
}
*/
/*
=================
FindChunk
=================
*/
static void FindChunk( const char *name )
{
	iff_lastChunk = iff_data;
	FindNextChunk( name );
}

/*
=============
Sound_LoadWAV
=============
*/
static bool Sound_LoadWAV( const char *name, const unsigned char *buffer, size_t filesize )
{
	int	samples;

	if( !buffer || filesize <= 0 ) return false;

	iff_data = buffer;
	iff_end = buffer + filesize;

	// dind "RIFF" chunk
	FindChunk( "RIFF" );
	if( !( iff_dataPtr && !strncmp( (char*)(iff_dataPtr + 8), "WAVE", 4 )))
	{
		AddLogEntry(true, LogLevel_Error, false, "Sound_LoadWAV: %s missing 'RIFF/WAVE' chunks\n", name );
		return false;
	}

	// get "fmt " chunk
	iff_data = iff_dataPtr + 12;
	FindChunk( "fmt " );
	if( !iff_dataPtr )
	{
		AddLogEntry(true, LogLevel_Error, false, "Sound_LoadWAV: %s missing 'fmt ' chunk\n", name );
		return false;
	}

	iff_dataPtr += 8;
	if( GetLittleShort() != 1 )
	{
		AddLogEntry(true, LogLevel_Error, false, "Sound_LoadWAV: %s not a microsoft PCM format\n", name );
		return false;
	}

	sound.channels = GetLittleShort();
	if( sound.channels != 1 && sound.channels != 2 )
	{
		AddLogEntry(true, LogLevel_Error, false, "Sound_LoadWAV: only mono and stereo WAV files supported (%s)\n", name );
		return false;
	}

	sound.rate = GetLittleLong();
	iff_dataPtr += 6;

	sound.width = GetLittleShort() / 8;
	if( sound.width != 1 && sound.width != 2 )
	{
		AddLogEntry(true, LogLevel_Warning, false, "Sound_LoadWAV: only 8 and 16 bit WAV files supported (%s)\n", name );
		return false;
	}

	// get cue chunk
	FindChunk( "cue " );

	if( iff_dataPtr )
	{
		iff_dataPtr += 32;
		sound.loopstart = GetLittleLong();
		FindNextChunk( "LIST" ); // if the next chunk is a LIST chunk, look for a cue length marker

		if( iff_dataPtr )
		{
			if( !strncmp( (char*)(iff_dataPtr + 28), "mark", 4 ))
			{	
				// this is not a proper parse, but it works with CoolEdit...
				iff_dataPtr += 24;
				sound.samples = sound.loopstart + GetLittleLong(); // samples in loop
			}
		}
	}
	else 
	{
		sound.loopstart = -1;
		sound.samples = 0;
	}

	// find data chunk
	FindChunk( "data" );
	if( !iff_dataPtr )
	{
		AddLogEntry(true, LogLevel_Warning, false, "Sound_LoadWAV: %s missing 'data' chunk\n", name );
		return false;
	}

	iff_dataPtr += 4;
	samples = GetLittleLong() / sound.width;

	if( sound.samples )
	{
		if( (unsigned int)samples < sound.samples )
		{
			AddLogEntry(true, LogLevel_Error, false, "Sound_LoadWAV: %s has a bad loop length\n", name );
			return false;
		}
	}
	else sound.samples = samples;

	if( sound.samples <= 0 )
	{
		AddLogEntry(true, LogLevel_Error, false, "Sound_LoadWAV: file with %i samples (%s)\n", sound.samples, name );
		return false;
	}

	sound.type = WF_PCMDATA;
	sound.samples /= sound.channels;

	// Load the data
	sound.size = sound.samples * sound.width * sound.channels;
	sound.wav = new unsigned char[sound.size];//Mem_Alloc( host.soundpool, sound.size );

	memcpy( sound.wav, /*buffer + (iff_dataPtr - buffer)*/iff_dataPtr, sound.size );

	// now convert 8-bit sounds to signed
	if( sound.width == 1 )
	{
		char *pData = (char*)sound.wav;
		const char *const pDataEnd (pData + sound.size);

		for (/* Empty */; pData < pDataEnd; ++pData)
			*pData = (unsigned char)((int)((unsigned char)*pData) - 128 );
	}

	return true;
}

/*
================
Sound_ConvertToSigned

Convert unsigned data to signed
================
*/
static void Sound_ConvertToSigned( const unsigned char *data, int channels, int samples )
{
	int	i;

	if( channels == 2 )
	{
		for (i = 0; i < samples; ++i)
		{
			((signed char *)sound.tempbuffer)[i*2+0] = (int)((unsigned char)(data[i*2+0]) - 128);
			((signed char *)sound.tempbuffer)[i*2+1] = (int)((unsigned char)(data[i*2+1]) - 128);
		}
	}
	else for (i = 0; i < samples; ++i)
		((signed char *)sound.tempbuffer)[i] = (int)((unsigned char)(data[i]) - 128);
}

/*
================
Sound_ResampleInternal

We need convert sound to signed even if nothing to resample
================
*/
static bool Sound_ResampleInternal(/* wavdata_t *sc, */int inrate, int inwidth, int outrate, int outwidth )
{
	float	stepscale;
	int	outcount, srcsample;
	int	i, sample, sample2, samplefrac, fracstep;
	unsigned char	*data;

	data = /*sc->buffer*/sound.wav;
	stepscale = (float)inrate / outrate;	// this is usually 0.5, 1, or 2
	outcount = (int)(/*sc->samples*/sound.samples / stepscale);
	/*sc->size*/sound.size = outcount * outwidth * /*sc->channels*/sound.channels;

	sound.tempbuffer = new unsigned char[sound.size];//(unsigned char *)Mem_Realloc( host.soundpool, sound.tempbuffer, /*sc->size*/sound.size );

	/*sc->samples*/sound.samples = outcount;
	if( /*sc->loopStart*/sound.loopstart != -1 )
		/*sc->loopStart*/sound.loopstart = (int)(/*sc->loopStart*/sound.loopstart / stepscale);

	// resample / decimate to the current source rate
	if( stepscale == 1.0f && inwidth == 1 && outwidth == 1 )
	{
		// fast special case
		Sound_ConvertToSigned( data, /*sc->channels*/sound.channels, outcount );
	}
	else
	{
		// general case
		samplefrac = 0;
		fracstep = (int)(stepscale * 256);

		if( /*sc->channels*/sound.channels == 2 )
		{
			for (i = 0; i < outcount; ++i)
			{
				srcsample = samplefrac >> 8;
				samplefrac += fracstep;

				if( inwidth == 2 )
				{
					sample = ((short *)data)[srcsample*2+0];
					sample2 = ((short *)data)[srcsample*2+1];
				}
				else
				{
					sample = (int)((char)(data[srcsample*2+0])) << 8;
					sample2 = (int)((char)(data[srcsample*2+1])) << 8;
				}

				if( outwidth == 2 )
				{
					((short *)sound.tempbuffer)[i*2+0] = (short)sample;
					((short *)sound.tempbuffer)[i*2+1] = (short)sample2;
				}
				else
				{
					((signed char *)sound.tempbuffer)[i*2+0] = (char)(sample >> 8);
					((signed char *)sound.tempbuffer)[i*2+1] = (char)(sample2 >> 8);
				}
			}
		}
		else
		{
			for (i = 0; i < outcount; ++i)
			{
				srcsample = samplefrac >> 8;
				samplefrac += fracstep;

				if( inwidth == 2 ) sample = ((short *)data)[srcsample];
				else sample = (int)( (char)(data[srcsample])) << 8;

				if( outwidth == 2 ) ((short *)sound.tempbuffer)[i] = (short)sample;
				else ((signed char *)sound.tempbuffer)[i] = (char)(sample >> 8);
			}
		}

		AddLogEntry(true, LogLevel_Default, false, "Sound_ResampleInternal: from[%d bit %d hz] to [%d bit %d hz]", inwidth * 8, inrate, outwidth * 8, outrate );
	}

	/*sc->rate*/sound.rate = outrate;
	/*sc->width*/sound.width = outwidth;

	return true;
}

inline void CONVERT_SOUND (const char *const path, const unsigned short outrate = 8000u, const unsigned char outwidth = 16u)
{
	STDIOFile fp (path, "rb");

	if (!fp.IsValid ())
	{
		AddLogEntry(true, LogLevel_Error, true, "CONVERT_SOUND(): File \"%s\" is invalid!!!", path);
		return;
	}

	unsigned int size (fp.GetSize ());
	unsigned char *data (new unsigned char[size]);

	if (!fp.Read (data, size) || !Sound_LoadWAV (path, data, size))
	{
		delete [] data;
		return;
	}

	Sound_ResampleInternal (sound.rate, sound.width, outrate, outwidth / 8u);

	STDIOFile fp2 ("C://Users//Эдуард//Desktop//MODIFIED.WAV", "wb");

	if (!fp2.IsValid ())
	{
		AddLogEntry(true, LogLevel_Error, true, "CONVERT_SOUND(): File \"MODIFIED.WAV\" is invalid!!!");
		return;
	}

	fp2.Write (data, iff_dataPtr - data);
	fp2.Write (sound.tempbuffer, sound.size);

	delete [] sound.tempbuffer;
	delete [] data;
}

inline void CONVERT_SOUND2 (const char *const path, unsigned char *&m_pMicInputFileData, unsigned int &m_nMicInputFileBytes, const unsigned short outrate = 8000u, const unsigned char outwidth = 16u)
{
	STDIOFile fp (path, "rb");

	if (!fp.IsValid ())
	{
		AddLogEntry(true, LogLevel_Error, true, "CONVERT_SOUND2(): File \"%s\" is invalid!!!", path);
		return;
	}

	unsigned int size (fp.GetSize ());
	unsigned char *data (new unsigned char[size]);

	if (!fp.Read (data, size) || !Sound_LoadWAV (path, data, size))
	{
		delete [] data;
		return;
	}

	Sound_ResampleInternal (sound.rate, sound.width, outrate, outwidth / 8u);

	delete [] m_pMicInputFileData;
	m_pMicInputFileData = sound.tempbuffer;
	m_nMicInputFileBytes = sound.size;

	delete [] data;
}
}

#endif	// ifndef SUPPORT_INCLUDED