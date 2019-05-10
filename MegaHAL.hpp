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
// MegaHAL.hpp
//
// Version: $ID:$
//

#ifndef MEGAHAL_INCLUDED
#define MEGAHAL_INCLUDED

#define BOT_CHAT_MESSAGE_LENGTH 128u

// HAL-related structure definitions
struct HAL_STRING
{
	unsigned char length;	// length of word (for quick access)
	char *word;	// the string itself
};


struct HAL_DICTIONARY
{
	unsigned long size;	// size of dictionary (for quick access)
	HAL_STRING *entry;	// string entry at this position of the dictionary
	unsigned short *index;	// pointer to index
};


struct HAL_SWAP
{
	unsigned short size;	// size of the word swap structure
	HAL_STRING *from;	// word to be changed into the next
	HAL_STRING *to;	// word for replacing the previous
};


typedef struct HAL_NODE
{
	unsigned short symbol;	// symbol ID
	unsigned long usage;	// usage information
	unsigned short count;	// usage count (?)
	unsigned short branch;	// branch of the tree
	struct HAL_NODE **tree;	// pointer to tree node pointer
} HAL_TREE;


struct HAL_MODEL
{
	unsigned char order;	// model order (complexity)
	HAL_TREE *forward;	// pointer to the model's forward tree
	HAL_TREE *backward;	// pointer to the model's backwards tree
	HAL_TREE **context;	// pointer to context tree pointer
	HAL_DICTIONARY *dictionary;	// pointer to the model's dictionary
};

struct HAL_bot_t
{
	HAL_DICTIONARY *banned_keywords;	// dictionary of words that must never be used as keywords
	HAL_DICTIONARY *auxiliary_keywords;	// dictionary of auxiliary keywords
	HAL_SWAP *swappable_keywords;	// array of swappable keywords with their equivalences
	HAL_MODEL *bot_model;	// Markov model of the bot
	HAL_DICTIONARY *input_words;	// global chat's dictionary of words
//	HAL_DICTIONARY *bot_words;	// bot's own dictionary of words
	bool keyword_is_used;
};

// bot HAL Markov model order
#define BOT_HAL_MODEL_ORDER 5

void HumanizeString (char *string);
void RemoveNameTags (const char *in_string, char *out_string);

void BotChat (YaPB *pBot);
void BotSayText (YaPB *pBot);
void BotSayAudio (YaPB *pBot);
void BotTalk (YaPB *pBot, char *sound_path);

void HAL_LoadTree (FILE *file, HAL_TREE *node);
void HAL_LoadDictionary (FILE *file, HAL_DICTIONARY *dictionary);
void HAL_SaveTree (FILE *file, HAL_TREE *node);
void HAL_SaveDictionary (FILE *file, HAL_DICTIONARY *dictionary);
void HAL_Learn (HAL_MODEL *model, HAL_DICTIONARY *words);
unsigned short HAL_AddWord (HAL_DICTIONARY *dictionary, HAL_STRING word);
int HAL_SearchDictionary (HAL_DICTIONARY *dictionary, HAL_STRING word, bool &find);
unsigned short HAL_FindWord (HAL_DICTIONARY *dictionary, HAL_STRING word);
int HAL_CompareWords (HAL_STRING word1, HAL_STRING word2);
void HAL_InitializeDictionary (HAL_DICTIONARY *dictionary);
HAL_DICTIONARY *HAL_NewDictionary (void);
HAL_TREE *HAL_NewNode (void);
HAL_MODEL *HAL_NewModel (int order);
void HAL_UpdateModel (HAL_MODEL *model, int symbol);
void HAL_UpdateContext (HAL_MODEL *model, int symbol);
HAL_TREE *HAL_AddSymbol (HAL_TREE *tree, unsigned short symbol);
HAL_TREE *HAL_FindSymbol (HAL_TREE *node, int symbol);
HAL_TREE *HAL_FindSymbolAdd (HAL_TREE *node, int symbol);
void HAL_AddNode (HAL_TREE *tree, HAL_TREE *node, int position);
int HAL_SearchNode (HAL_TREE *node, int symbol, bool *found_symbol);
void HAL_InitializeContext (HAL_MODEL *model);
void BotHALTrainModel (YaPB *pBot, HAL_MODEL *model);
void HAL_ShowDictionary (HAL_DICTIONARY *dictionary);
void HAL_MakeWords (char *input, HAL_DICTIONARY *words);
void BotHALGenerateReply (YaPB *pBot, char *output);
bool HAL_BoundaryExists (const char *string, int position);
void BotChatReply (YaPB *pBot, char *szMsg, HalfLifeEngine::SDK::Classes::Edict *pSender, char *szReplyMsg);
bool HAL_DictionariesDiffer (HAL_DICTIONARY *words1, HAL_DICTIONARY *words2);
HAL_DICTIONARY *BotHALMakeKeywords (YaPB *pBot, HAL_DICTIONARY *words);
void BotHALAddKeyword (YaPB *pBot, HAL_DICTIONARY *keys, HAL_STRING word);
void BotHALAddAuxiliaryKeyword (YaPB *pBot, HAL_DICTIONARY *keys, HAL_STRING word);
HAL_DICTIONARY *BotHALBuildReplyDictionary (YaPB *pBot, HAL_DICTIONARY *keys);
int BotHALBabble (YaPB *pBot, HAL_DICTIONARY *keys, HAL_DICTIONARY *words);
bool HAL_WordExists (HAL_DICTIONARY *dictionary, HAL_STRING word);
int BotHALSeedReply (YaPB *pBot, HAL_DICTIONARY *keys);
HAL_SWAP *HAL_NewSwap (void);
void HAL_AddSwap (HAL_SWAP *list, char *s, char *d);
HAL_SWAP *HAL_InitializeSwap (char *filename);
HAL_DICTIONARY *HAL_InitializeList (char *filename);
void HAL_EmptyDictionary (HAL_DICTIONARY *dictionary);
void HAL_FreeModel (HAL_MODEL *model);
void HAL_FreeTree (HAL_TREE *tree);
void HAL_FreeSwap (HAL_SWAP *swap);

inline const int strpos (const char *pos, const char *start)
{
	return ((int)pos - (int)start);
}

// Fill string, a neat function I made
// fills the points of a string with %l, %r, %whatever with readable text
inline const bool BotFunc_FillString (char *string, const char *fill_point, const char *fill_with, int max_len)
{
	// keep a big string to make sure everything fits
	static char temp[1024];
	int len = GetStringLength <unsigned short> (string) + 1;
	int start;
	int end;
	char *ptr = NULL;

	// store before and after strings, well put these in the final string
	char *before;
	char *after;

	if (fill_with == NULL)
		return false;

	ptr = string;

	// Keep searching for a point in the string
	while ((ptr = strstr (ptr, fill_point)) != NULL)
	{
		// found a point
		before = new char[len];
		after = new char[len];

		// initialize them to empty (don't need to null terminate at the right point)
		memset (before, '\0', len);
		memset (after, '\0', len);

		start = strpos (ptr, string);
		strncpy(before,string,start);

		// always null terminate the last possible character
		string[start] = '\0';

		end = start + GetStringLength <unsigned short> (fill_point);
		strncpy (after, string + end, len - end);
		after[len - end] = '\0';

		// fill in new string and..
		// update len (string size may have INCREASED
		// if by more than 2* normal string size, could have overwritten
		// some precious memory!)
		len = sprintf (temp, "%s%s%s", before, fill_with, after);

		// fill string now with maximum length for string
		strncpy (string, temp, max_len - 1);
		string[max_len - 1] = '\0';

		// MUST reset ptr incase new ptr is outside the string (if string size was reduced)
		ptr = string;

		// free memory used
		delete [] before;
		delete [] after;
/*
		int len = GetStringLength <unsigned short> (ptr + 2);

		temp = NULL;

		if (len > 0)
		{
			// allocate the string size, will use less space and wont crash if too big :p
			temp = new char[len + 1];
			strcpy (temp, ptr + 2);
		}

		string[strpos (ptr, string)] = '\0';

		strcat (string, fill_with);

		if (temp != NULL)
		{
			strcat (string, temp);
			delete [] temp;
		}*/
	}

	return true;	// no point found (nothing to add) return true (string can be used)
}

		//////////////////////////////////////////////////////////
		// REPUTATION DEFINITIONS
		#define BOT_MIN_REP  0
		#define BOT_LOW_REP  3
		#define BOT_MID_REP  5
		#define BOT_HIGH_REP 6
		#define BOT_MAX_REP  10

#if 0
/////////////////////////////////////////////
// REPUTATION  STORAGE  CLASSES
class CBotReputation
{
	public:
		//////////////////////////////////////////////////////////
		// REPUTATION DEFINITIONS
		#define BOT_MIN_REP  0
		#define BOT_LOW_REP  3
		#define BOT_MID_REP  5
		#define BOT_HIGH_REP 6
		#define BOT_MAX_REP  10

	private:
		int m_iPlayerRepId;
		int m_iRep;

	public:
		CBotReputation()
		{
			m_iRep = BOT_MID_REP;
			m_iPlayerRepId = -1;
		}

		CBotReputation( int iPlayerRepId, int iRep )
		{
			m_iPlayerRepId = iPlayerRepId;
			m_iRep = iRep;
		}

	public:
		bool operator == (CBotReputation Rep)
		{
			return Rep.IsForPlayer(m_iPlayerRepId);
		}

	public:
		inline void UpdateRep(int iRep)
		{
			m_iRep = iRep;
		}

		inline int CurrentRep ( void )
		{
			return m_iRep;
		}

		inline int GetRepId ( void )
		{
			return m_iPlayerRepId;
		}

		inline bool IsForPlayer ( int iPlayerRepId )
		{
			return m_iPlayerRepId == iPlayerRepId;
		}

		void printRep ( YaPB *forBot, HalfLifeEngine::SDK::Classes::Edict *pPrintTo )
		{
			Client *pClient = gBotGlobals.m_Clients.GetClientByRepId(m_iPlayerRepId);

			switch ( m_iRep )
			{
				case 0:
					BotMessage(pPrintTo,0,"%s really hates %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 1:
					BotMessage(pPrintTo,0,"%s hates %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 2:
					BotMessage(pPrintTo,0,"%s kind of hates %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 3:
					BotMessage(pPrintTo,0,"%s doesn't like %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 4:
				case 5:
				case 6:
					BotMessage(pPrintTo,0,"%s likes %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 7:
					BotMessage(pPrintTo,0,"%s really likes %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 8:
					BotMessage(pPrintTo,0,"%s loves %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 9:
					BotMessage(pPrintTo,0,"%s really loves %s (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
				case 10:
					BotMessage(pPrintTo,0,"%s LURRVESS %s !!! (%d)",forBot->GetName ().GetData (),pClient->GetName ().GetData (),m_iRep);
					break;
			}
		}
};

class CBotReputations
{
	private:
		dataStack<CBotReputation> m_RepList;

	public:
		void printReps ( YaPB *forBot, HalfLifeEngine::SDK::Classes::Edict *pPrintTo )
		{
			dataStack<CBotReputation> tempStack = m_RepList;
			CBotReputation *pRep;

			while ( !tempStack.IsEmpty() )
			{
				pRep = tempStack.ChoosePointerFromStack();

				pRep->printRep(forBot,pPrintTo);
			}
		}

		void Destroy ( void )
		{
			m_RepList.Destroy();
		}

		Client *GetRandomClient ( int iRep )
		// return a random client that conforms to the iRep (reputation)
		// -1 will return a random bad client
		// 0 : a random neutral client
		// 1 : a random friendly client
		{
			int iGotRep;

			dataStack <CBotReputation> tempStack = m_RepList;
			CBotReputation *pRep;
			dataUnconstArray<CBotReputation*> iIdList;

			iIdList.Init();

			while ( !tempStack.IsEmpty() )
			{
				pRep = tempStack.ChoosePointerFromStack();
				iGotRep = pRep->CurrentRep();

				switch ( iRep )
				{
				case -1:	// bad
					if ( iGotRep <= 3 )
						iIdList.Add(pRep);
					break;
				case 0:	// neutral
					if (( iGotRep > 3 ) && ( iGotRep < 7 ))
						iIdList.Add(pRep);
					break;
				case 1:	// good
					if ( iGotRep >= 7 )
						iIdList.Add(pRep);
					break;
				}
			}

			tempStack.Init();

			if ( iIdList.IsEmpty() )
				return NULL;

			assert ( iIdList.Size() > 0 );

			pRep = iIdList.Random();

			iIdList.Clear();

			if ( pRep == NULL )
				return NULL;

			return gBotGlobals.m_Clients.GetClientByRepId(pRep->GetRepId());
		}

		void WriteToFile (int iBotProfile, CBotReputation *pRep)
		{
			FILE *fp;

			char filename[256];
			char repfile[16];

			CBotReputation Rep;

			bool bChanged = false;

			if ( pRep == NULL )	// error
				return;

			sprintf(repfile,"%d.rcr",iBotProfile);

			UTIL_BuildFileName(filename,BOT_PROFILES_FOLDER,repfile);

			fp = fopen (filename, "rb+");	// open the file in ascii read/write mode

			if ( fp == NULL )
			{
				fp = fopen (filename, "wb");

				if ( fp == NULL )
				{
					BotMessage(NULL,0,"could not save Bot id %d's rep file", iBotProfile);
					return;
				}

				fwrite(pRep,sizeof(CBotReputation),1,fp);
				fclose(fp);

				return;
			}

			fseek(fp,0,SEEK_END);	// move pos to end of file
			long fPos = ftell(fp);	// get length of file

			// do some error checking - verify the file is not corrupt
			if (fPos%sizeof(CBotReputation)!=0)
				return;

			// get the count of items in the file
			long count = fPos/sizeof(CBotReputation);

			fseek(fp,0,SEEK_SET);	// move pos back to beginning

			while (bChanged == false && !feof(fp) && count>0)
			{
				fread(&Rep,sizeof(CBotReputation),1,fp);

				if ( Rep == *pRep )
				{
					fPos = ftell(fp);	// get the current position
					// move marker back to start of this record
					fseek(fp,0-sizeof(CBotReputation),SEEK_CUR);
					fwrite(pRep,sizeof(CBotReputation),1,fp);
					fflush(fp);
					// now reset the marker to same as before we
					// deleted this record so that everything continues
					// as normal
					fseek(fp,fPos,SEEK_SET);

					bChanged = true;
				}
				--count;
			}
			fclose(fp);

			if ( bChanged == false )
			{
				fp = fopen(filename,"ab");	// append binary

				if ( fp == NULL )
				{
					BotMessage(NULL,0,"Can't open reputation file \"%s\" for appending",filename);
					return;	// error
				}
				else
				{
					fwrite(pRep,sizeof(CBotReputation),1,fp);

					fclose(fp);
				}
			}
		}

		// Saves ONE rep and removes it
		void RemoveSaveRep ( int iBotProfile, int iPlayerRepId )
		{
			CBotReputation *pRep = GetRep(iPlayerRepId);

			if ( pRep )
			{
				WriteToFile(iBotProfile,pRep);
			}

			m_RepList.RemoveByPointer(pRep);
		}

		// Saves ALL rep
		void SaveAllRep ( int iBotProfile )
		{
			dataStack <CBotReputation> tempStack = m_RepList;
			CBotReputation *pRep;

			while ( !tempStack.IsEmpty() )
			{
				pRep = tempStack.ChoosePointerFromStack();

				if ( pRep )
					WriteToFile(iBotProfile,pRep);
			}

			return;
		}

		void AddLoadRep ( int iBotProfile, int iPlayerRepId )
		{
			FILE *fp;

			char filename[256];
			char repfile[16];

			CBotReputation Rep;

			bool bFound = false;

			sprintf(repfile,"%d.rcr",iBotProfile);

			UTIL_BuildFileName(filename,BOT_PROFILES_FOLDER,repfile);

			fp = fopen (filename, "rb");	// open the file in ascii read/write mode

			if ( fp == NULL )
			{
				m_RepList.Push(CBotReputation(iPlayerRepId,BOT_MID_REP));

				WriteToFile(iBotProfile,m_RepList.GetHeadInfoPointer());

				return;
			}

			fseek(fp,0,SEEK_END);	// move pos to end of file
			long fPos = ftell(fp);	// get length of file

			// do some error checking - verify the file is not corrupt
			if (fPos%sizeof(CBotReputation)!=0) return;

			// get the count of items in the file
			long count = fPos/sizeof(CBotReputation);

			fseek(fp,0,SEEK_SET);	// move pos back to beginning

			while ((bFound == false) && !feof(fp) && count>0)
			{
				fread(&Rep,sizeof(CBotReputation),1,fp);

				// is for the player input into function (player id)?
				if ( Rep.IsForPlayer(iPlayerRepId) )
				{
					m_RepList.Push(Rep);
					bFound = true;
				}

				--count;
			}

			fclose(fp);

			if ( bFound == false )
			{
				m_RepList.Push(CBotReputation(iPlayerRepId,BOT_MID_REP));

				// save new
				WriteToFile(iBotProfile,m_RepList.GetHeadInfoPointer());
			}

			return;

		}

		void NewRep (int iPlayerRepId)
			// Create a NEW Reputation value for player name szPlayerName
			// With a default Rep of a Mid way rep.
		{
			AddRep(iPlayerRepId,BOT_MID_REP);
		}

		int AverageRepOnServer ( void )
		{
			int iTotal = 0;
			int iNum = 0;

			if ( m_RepList.IsEmpty() )
				return 0;

			dataStack<CBotReputation> tempStack = m_RepList;
			CBotReputation *pRep;

			while ( !tempStack.IsEmpty() )
			{
				pRep = tempStack.ChoosePointerFromStack();

				// bug fixed... didn't add :-P
				iTotal += pRep->CurrentRep ();
				++iNum;
			}

			return iTotal / iNum;
		}

		void AddRep ( int iPlayerRepId ,int iRep)
			// Add a NEW Reputation WITH a reputation value
		{
			CBotReputation *l_Rep;

			l_Rep = GetRep(iPlayerRepId);

			if ( l_Rep == NULL )	// Don't want to add a duplicate
				m_RepList.Push(CBotReputation(iPlayerRepId,iRep));
		}

		CBotReputation *GetCreateRep ( int iPlayerRepId )
		{
			dataStack<CBotReputation> tempStack = m_RepList;
			CBotReputation *l_Rep;

			while ( !tempStack.IsEmpty() )
			{
				l_Rep = tempStack.ChoosePointerFromStack();

				if ( l_Rep->IsForPlayer(iPlayerRepId) )
				{
					// Fool pointer, dont free memory
					tempStack.Init ();

					return l_Rep;
				}
			}

			m_RepList.Push(CBotReputation(iPlayerRepId,BOT_MID_REP));

			return m_RepList.GetHeadInfoPointer();
		}

		CBotReputation *GetRep ( int iPlayerRepId )
		{
			dataStack<CBotReputation> tempStack = m_RepList;
			CBotReputation *l_Rep;

			while ( !tempStack.IsEmpty() )
			{
				l_Rep = tempStack.ChoosePointerFromStack();

				if ( l_Rep->IsForPlayer(iPlayerRepId) )
				{
					// Fool pointer, dont free memory
					tempStack.Init();

					return l_Rep;
				}
			}

			return NULL;
		}

		int GetClientRep ( Client *pClient )
		{
			if ( pClient == NULL )
			{
				BotMessage(NULL,0,"warning: GetClientRep(): Bad pClient recieved (NULL) (returning default)");
				return BOT_MID_REP;
			}

			int iRepId = pClient->GetPlayerRepId();

			if ( iRepId == -1 )
				return BOT_MID_REP;

			CBotReputation *pRep = GetRep(iRepId);

			if ( pRep == NULL )
			{
				BotMessage(NULL,0,"warning: GetClientRep(): No Rep for pClient (id: %d) (returning default)",pClient->GetPlayerRepId());
				return BOT_MID_REP;
			}

			return pRep->CurrentRep();
		}

		void IncreaseRep( int iPlayerRepId )
		{
			if ( iPlayerRepId == -1 )
				return;

			CBotReputation *l_Rep = GetRep(iPlayerRepId);

			if ( l_Rep )
			{
				int iNewRep = l_Rep->CurrentRep()+1;

				if ( iNewRep > BOT_MAX_REP )
					return;

				l_Rep->UpdateRep(iNewRep);
			}
			else
			{
				// oh crap wth?

				BugMessage(NULL,"bad rep data... (try deleting all " BOT_PROFILES_FOLDER "/*.rcr files :-( )");
			}
		}

		void DecreaseRep( int iPlayerRepId )
		{
			if ( iPlayerRepId == -1 )
				return;

			CBotReputation *l_Rep = GetRep(iPlayerRepId);

			if ( l_Rep )
			{
				int iNewRep = l_Rep->CurrentRep()-1;

				if ( iNewRep < BOT_MIN_REP )
					return;

				l_Rep->UpdateRep(iNewRep);
			}
			else
			{
				// oh crap wth?

				BugMessage(NULL,"bad rep data... (try deleting all " BOT_PROFILES_FOLDER "/*.rcr files :-( )");
			}
		}

		void Init ( void )
		{
			m_RepList.Init();
		}
};
#endif	// if 0
void ReplyToMessage (YaPB *const bot, char *szMessage, Client *pSender, const bool iTeamOnly);

#endif	// ifndef MEGAHAL_INCLUDED