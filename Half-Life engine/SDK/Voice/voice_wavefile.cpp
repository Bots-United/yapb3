//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include <Core.hpp>

namespace HalfLifeEngine
{
	namespace SDK
	{
		namespace Voice
		{
static unsigned long ReadDWord(Classes::Interface::FileSystem::FileHandle_t *const fp)
{
	unsigned long ret;
	Globals::g_halfLifeEngine->GetFileSystem ()->Read(&ret, sizeof (ret), fp);
	return ret;
}
static unsigned short ReadWord(Classes::Interface::FileSystem::FileHandle_t *const fp)
{
	unsigned short ret;
	Globals::g_halfLifeEngine->GetFileSystem ()->Read(&ret, sizeof (ret), fp);
	return ret;
}
static void WriteDWord(Classes::Interface::FileSystem::FileHandle_t *const fp, unsigned long val) 
{
	Globals::g_halfLifeEngine->GetFileSystem ()->Write(&val, sizeof (val), fp);
}
static void WriteWord(Classes::Interface::FileSystem::FileHandle_t *const fp, unsigned short val) 
{
	Globals::g_halfLifeEngine->GetFileSystem ()->Write(&val, sizeof (val), fp);
}

bool ReadWaveFile
(
	const char *const pFilename,
	char *&pData,
	unsigned int &nDataBytes,
	unsigned short &wBitsPerSample,
	unsigned short &nChannels,
	unsigned int &nSamplesPerSec
)
{
	Classes::Interface::FileSystem::FileHandle_t *const fp (Globals::g_halfLifeEngine->GetFileSystem ()->Open(pFilename, "rb"));
	if(fp == NULL)
		return false;

	unsigned char seekPosition (sizeof (char[GET_STATIC_STRING_LENGTH ("RIFF")]) + sizeof (unsigned long) + sizeof (char[GET_STATIC_STRING_LENGTH ("WAVE")]) + sizeof (char[GET_STATIC_STRING_LENGTH ("fmt ")]) + sizeof (unsigned long) + sizeof (unsigned short));

	Globals::g_halfLifeEngine->GetFileSystem ()->Seek(fp, seekPosition, Classes::Interface::FILESYSTEM_SEEK_HEAD);
	
	nChannels = ReadWord(fp);
	nSamplesPerSec = ReadDWord(fp);

	seekPosition += sizeof (nChannels) + sizeof (nSamplesPerSec) + sizeof (unsigned long) + sizeof (unsigned short);

	Globals::g_halfLifeEngine->GetFileSystem ()->Seek(fp, seekPosition, Classes::Interface::FILESYSTEM_SEEK_HEAD);
	wBitsPerSample = ReadWord(fp);

	seekPosition += sizeof (wBitsPerSample) + sizeof (char[GET_STATIC_STRING_LENGTH ("data")]);

	Globals::g_halfLifeEngine->GetFileSystem ()->Seek(fp, seekPosition, Classes::Interface::FILESYSTEM_SEEK_HEAD);
	nDataBytes = ReadDWord(fp);

	if (nDataBytes == 0u)
	{
		Globals::g_halfLifeEngine->GetFileSystem ()->Close (fp);

		return false;
	}

	pData = new char[nDataBytes];

	if (pData == NULL)
	{
		Globals::g_halfLifeEngine->GetFileSystem ()->Close (fp);

		return false;
	}
/*!
	seekPosition += sizeof (nDataBytes) + sizeof (unsigned long);	/// @warning WHY WE SKIP FIRST FOUR BYTES OF DATA?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!??!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!??!?!?!

	Globals::g_halfLifeEngine->GetFileSystem ()->Seek(fp, seekPosition, Classes::Interface::FILESYSTEM_SEEK_HEAD);
*/
	Globals::g_halfLifeEngine->GetFileSystem ()->Read(pData, nDataBytes, fp);

	Globals::g_halfLifeEngine->GetFileSystem ()->Close (fp);

	return true;
}

bool WriteWaveFile(
	const char *const pFilename, 
	const char *const pData, 
	const unsigned int nDataBytes, 
	const unsigned short wBitsPerSample, 
	const unsigned short nChannels, 
	const unsigned int nSamplesPerSec)
{
	Classes::Interface::FileSystem::FileHandle_t *const fp (Globals::g_halfLifeEngine->GetFileSystem ()->Open(pFilename, "wb"));
	if(fp == NULL)
		return false;

	// Write the RIFF chunk.
	Globals::g_halfLifeEngine->GetFileSystem ()->Write("RIFF", GET_STATIC_STRING_LENGTH ("RIFF"), fp);
	WriteDWord(fp, 0);
	Globals::g_halfLifeEngine->GetFileSystem ()->Write("WAVE", GET_STATIC_STRING_LENGTH ("WAVE"), fp);

	// Write the FORMAT chunk.
	Globals::g_halfLifeEngine->GetFileSystem ()->Write("fmt ", GET_STATIC_STRING_LENGTH ("fmt "), fp);
	
	WriteDWord(fp, 16);
	WriteWord(fp, WAVE_FORMAT_PCM);
	WriteWord(fp, nChannels);
	WriteDWord(fp, nSamplesPerSec);
	WriteDWord(fp, (unsigned long)((wBitsPerSample / 8) * nChannels * nSamplesPerSec));
	WriteWord(fp, (unsigned short)((wBitsPerSample / 8) * nChannels));
	WriteWord(fp, wBitsPerSample);

	// Write the DATA chunk.
	Globals::g_halfLifeEngine->GetFileSystem ()->Write("data", GET_STATIC_STRING_LENGTH ("data"), fp);
	WriteDWord(fp, nDataBytes);
	Globals::g_halfLifeEngine->GetFileSystem ()->Write(pData, nDataBytes, fp);

	// Go back and write the length of the riff file.
	const unsigned long dwVal (Globals::g_halfLifeEngine->GetFileSystem ()->Tell(fp) - 8);
	Globals::g_halfLifeEngine->GetFileSystem ()->Seek(fp, sizeof (unsigned long), Classes::Interface::FILESYSTEM_SEEK_HEAD);
	WriteDWord(fp, dwVal);

	Globals::g_halfLifeEngine->GetFileSystem ()->Close(fp);
	return true;
}
		}
	}
}