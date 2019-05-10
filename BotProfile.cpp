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
// BotProfile.cpp
//
// Class: Profile
//
// Description: YaPB profile item structure.
//
// Version: $ID:$
//

#include <Core.hpp>

// return true if need to pre-train
bool YaPB::Profile::PrepareHALBrainForPersonality (void)
{
	// this function prepares a HAL personality

	FILE *fp;
	char ban_filename[256];
	char aux_filename[256];
	char swp_filename[256];
	char brn_filename[256];

	char cookie[32];

	// Reliability check.
	InternalAssert (m_HAL == NULL);

	m_HAL = new HAL_bot_t ();

	if (m_HAL == NULL)
		return false;	// reliability check

	m_HAL->auxiliary_keywords = NULL;
	m_HAL->banned_keywords = NULL;
//	m_HAL->bot_words = NULL;
	m_HAL->input_words = NULL;
	m_HAL->swappable_keywords = NULL;
	m_HAL->keyword_is_used = false;
	m_HAL->bot_model = HAL_NewModel (BOT_HAL_MODEL_ORDER);	// create a language model of a certain order

	// build the file names

	// Build file name will set up the directoy for the filenames
	strcpy (ban_filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/");
	strcpy (aux_filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/");
	strcpy (swp_filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/");

	char brn_file[256];

	sprintf (brn_file, "%d_hal.brn", m_iProfileId);
	strcpy (brn_filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/" + brn_file);

	if (!m_szHAL_BanFile.IsEmpty ())
		strcat (ban_filename, m_szHAL_BanFile);
	else
	{
		char szFilename[16];

		sprintf (szFilename, "%d_hal.ban", m_iProfileId);
		strcat (ban_filename, szFilename);
	}

	if (!m_szHAL_AuxFile.IsEmpty ())
		strcat (aux_filename, m_szHAL_AuxFile);
	else
	{
		char szFilename[16];

		sprintf (szFilename, "%d_hal.aux", m_iProfileId);
		strcat (aux_filename, szFilename);
	}

	if (!m_szHAL_SwapFile.IsEmpty ())
		strcat (swp_filename, m_szHAL_SwapFile);
	else
	{
		char szFilename[16];

		sprintf (szFilename, "%d_hal.swp", m_iProfileId);
		strcat (swp_filename, szFilename);
	}

	// read dictionaries containing banned keywords, auxiliary keywords and swap keywords
	m_HAL->banned_keywords = HAL_InitializeList (ban_filename);
	m_HAL->auxiliary_keywords = HAL_InitializeList (aux_filename);
	m_HAL->swappable_keywords = HAL_InitializeSwap (swp_filename);

	// check if the brain exists, try to open it
	fp = fopen (brn_filename, "rb");
	if (fp != NULL)
	{
		fseek (fp, 0, SEEK_SET);	// seek at start of file
		fread (cookie, sizeof ("RCBOTHAL"), 1, fp);	// read the brain signature
		fclose (fp);	// close the brain (we just wanted the signature)

		// check for brain file validity
		if (strcmp (cookie, "RCBOTHAL") == 0)
			return false;	// ok, brain is valid
	}

	// there is a problem with the brain, infer a brand new one
	AddLogEntry (true, LogLevel_Warning, false, "bot profile (%d) HAL brain damaged!", m_iProfileId);
	AddLogEntry (true, LogLevel_Warning, false, "inferring a new HAL brain to profile (%d)", m_iProfileId);

	// create the new brain (i.e, save a void one in the brain file)
	fp = fopen (brn_filename, "wb");

	if (fp == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "PrepareHALBrainForPersonality(): writing permissions not allowed on profile (%d) HAL brain! (brn_filename: \"%s\")", m_iProfileId, brn_filename);

		return false;
	}

	fwrite ("RCBOTHAL", sizeof ("RCBOTHAL"), 1, fp);
	fwrite (&m_HAL->bot_model->order, sizeof (unsigned char), 1, fp);
	HAL_SaveTree (fp, m_HAL->bot_model->forward);
	HAL_SaveTree (fp, m_HAL->bot_model->backward);
	HAL_SaveDictionary (fp, m_HAL->bot_model->dictionary);
	fclose (fp);	// everything is saved, close the file

	return true;	// ok, now it is guarantee that this personality has an associated brain
}

bool YaPB::Profile::LoadHALBrainForPersonality (const bool bPreTrain)
{
	// this function loads a HAL brain

	FILE *fp;
	char filename[512];
	char file[256];
	char cookie[8];

	//int iNameLength;
	//char *szName;

	//iNameLength = GetStringLength <unsigned char> (name);
	//szName = new char[iNameLength + 1];
	//RemoveNameTags (name, szName);
	//szName[iNameLength] = '\0';

	sprintf (file, "%d_hal.brn", m_iProfileId);

	strcpy (filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/" + file);

	// check if the brain exists, try to open it
	fp = fopen (filename, "rb");
	if (fp == NULL)
	{
		//delete [] szName;
		AddLogEntry (true, LogLevel_Warning, false, "LoadHALBrainForPersonality(): %s's HAL brain refuses to wake up! (file does not exist)", name.GetData ());
		return true;	// there was an error, return true
	}

	// check for brain file validity
	fread (cookie, sizeof ("RCBOTHAL"), 1, fp);	// read the brain signature

	if (strcmp (cookie, "RCBOTHAL") != 0)
	{
		// delete [] szName;
		AddLogEntry (true, LogLevel_Warning, false, "LoadHALBrainForPersonality(): %s's HAL brain damaged!", name.GetData ());	// bad brain
		AddLogEntry (true, LogLevel_Error, false, "damanged bot file, delete %s/%d.brn file", BOT_PROFILES_FOLDER, m_iProfileId);	// bad brain
		fclose (fp);	// close file
		return true;	// there was an error, return true
	}

	AddLogEntry (true, LogLevel_Warning, false, "HAL: restoring brain to %s\n", name.GetData ());

	fread (&m_HAL->bot_model->order, 1, 1, fp);
	HAL_LoadTree (fp, m_HAL->bot_model->forward);
	HAL_LoadTree (fp, m_HAL->bot_model->backward);
	HAL_LoadDictionary (fp, m_HAL->bot_model->dictionary);
	fclose (fp);

	m_HAL->input_words = HAL_NewDictionary ();	// create the global chat dictionary

	m_HAL->keyword_is_used = false;

	//HAL_AddSwap (m_HAL->swappable_keywords, szName, name_in_msg);

	if (bPreTrain)
	{
		char trn_filename[256];

		strcpy (trn_filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/");

		if (!m_szHAL_PreTrainFile.IsEmpty ())
		{
//			strcat (trn_filename, "/");
			strcat (trn_filename, m_szHAL_PreTrainFile);
		}
		else
		{
			char szFilename[16];

			sprintf (szFilename, "%d_hal.trn", m_iProfileId);
			strcat (trn_filename, szFilename);
		}

		// see if there is a training file
		fp = fopen (trn_filename, "r");

		if (fp != NULL)
		{
			char szBuffer[256];
			int iLen;

			while (fgets (szBuffer, 255, fp) != NULL)
			{
				szBuffer[255] = '\0';

				if (szBuffer[0] == '#')
					continue;	// a comment
				if (szBuffer[0] == '\0')
					continue;	// nothing on this line

				iLen = GetStringLength <unsigned short> (szBuffer);

				if (szBuffer[iLen-1] == '\n')
					szBuffer[iLen-1] = '\0';

				HAL_MakeWords (szBuffer, m_HAL->input_words);
				HAL_Learn (m_HAL->bot_model, m_HAL->input_words);
			}

			fclose (fp);
		}
	}

	return false;	// no error, return false
}

void YaPB::Profile::SaveHALBrainForPersonality (void)
{
	// this function saves the current state to a HAL brain file

	FILE *fp;
	char file[256];
	char filename[256];

	sprintf (file, "%d_hal.brn", m_iProfileId);

	strcpy (filename, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + g_YaPBFolder + "/" BOT_PROFILES_FOLDER "/" + file);

	fp = fopen (filename, "wb");
	if (fp == NULL)
	{
		AddLogEntry (true, LogLevel_Warning, false, "Unable to save profile %d's HAL brain to %s\n", m_iProfileId, filename);
		return;
	}

	// dump the HAL brain to disk
	fwrite ("RCBOTHAL", sizeof ("RCBOTHAL"), 1, fp);
	fwrite (&m_HAL->bot_model->order, sizeof (unsigned char), 1, fp);
	HAL_SaveTree (fp, m_HAL->bot_model->forward);
	HAL_SaveTree (fp, m_HAL->bot_model->backward);
	HAL_SaveDictionary (fp, m_HAL->bot_model->dictionary);

	fclose (fp);
}

void YaPB::Profile::FreeHALBrain (void)
{
	if (m_HAL == NULL)
		return;

	// free every word in their global chat dictionary
	if (m_HAL->bot_model->dictionary != NULL)
	{
		for (unsigned int i (0u); i < m_HAL->bot_model->dictionary->size; ++i)
		{
			if (m_HAL->bot_model->dictionary->entry[i].word != NULL)
				free (m_HAL->bot_model->dictionary->entry[i].word);	// free the word

			m_HAL->bot_model->dictionary->entry[i].word = NULL;	// and fools the pointer
		}

		HAL_EmptyDictionary (m_HAL->bot_model->dictionary);	// empty that dictionary itself

		free (m_HAL->bot_model->dictionary);	// now frees the dictionary

		m_HAL->bot_model->dictionary = NULL;	// and fools the pointer
	}

	HAL_EmptyDictionary (m_HAL->input_words);
//	free (m_HAL->input_words);	/// @note ADDED BY EDWARD!!!!!!!!!!!!!!!!!!!!
	m_HAL->input_words = NULL;
	HAL_EmptyDictionary (m_HAL->banned_keywords);
//	free (m_HAL->banned_keywords);	/// @note ADDED BY EDWARD!!!!!!!!!!!!!!!!!!!!
	m_HAL->banned_keywords = NULL;
	HAL_EmptyDictionary (m_HAL->auxiliary_keywords);
//	free (m_HAL->auxiliary_keywords);	/// @note ADDED BY EDWARD!!!!!!!!!!!!!!!!!!!!
	m_HAL->auxiliary_keywords = NULL;
	//HAL_EmptyDictionary (m_HAL->bot_words);

	HAL_FreeModel (m_HAL->bot_model);
	m_HAL->bot_model = NULL;
	HAL_FreeSwap (m_HAL->swappable_keywords);
	m_HAL->swappable_keywords = NULL;
}