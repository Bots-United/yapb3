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
// MegaHAL.cpp
//
// Version: $ID:$
//

/*
  MegaHAL Stuff

  code from PM's RACC Bot (http://racc.bots-united.com) Check it out!
  adapted to rcbot code

  MegaHAL - http://megahal.sourceforge.net
*/

// RACC - AI development project for first-person shooter games derivated from Valve's Half-Life
// (http://www.racc-ai.com/)
//
// The game to engine interfacing code is based on the work done by Jeffrey 'Botman' Broome
// (http://planethalflife.com/botman/)
//
// This project is partially based on the work done by Eric Bieschke in his BSDbot
// (http://gamershomepage.com/csbot/)
//
// This project is partially based on the work done by Brendan 'Spyro' McCarthy in his ODD Bot
// (http://oddbot.hlfusion.com/)
//
// This project is partially based on the work done by Alistair 'eLiTe' Stewart in his TEAMbot
// (http://www.planethalflife.com/teambot/)
//
// This project is partially based on the work done by Johannes '@$3.1415rin' Lampel in his JoeBot
// (http://www.joebot.net/)
//
// Rational Autonomous Cybernetic Commandos AI
//
// bot_chat.cpp
//

#include <Core.hpp>

const char *const name_in_msg ("%n");

void BotChatReply (YaPB *pBot, char *szMsg, Client *pSender, char *szReplyMsg)
{
	// the purpose of this function is to make the bot keep an eye on what's happening in the
	// chat room, and in case of new messages, think about a possible reply.

	// no message!
	if (!szMsg || !*szMsg)
		return;

	// is bot chat allowed AND this message is not from this bot itself?
	if (/*!gBotGlobals.IsConfigSettingOn (BOT_CONFIG_CHATTING) || */pSender == pBot)
		return;

	int iNameLength;
	char *szName;

	iNameLength = GetStringLength <unsigned char> (pBot->GetName ());
	szName = new char[iNameLength + 1];
	RemoveNameTags (pBot->GetName (), szName);
	szName[iNameLength] = '\0';

	//int i, j;

	bool bNameInMsg = false;
	char *szNamePos;

	StringMakeLowerCase (szMsg);
	StringMakeLowerCase (szName);

	szNamePos = strstr (szMsg, szName);
	bNameInMsg = (szNamePos != NULL);

	int iSenderNameLength = GetStringLength <unsigned char> (pSender->GetName ());
	char *szSenderName = new char[iSenderNameLength + 1];
	RemoveNameTags (pSender->GetName (), szSenderName);
	szSenderName[iSenderNameLength] = '\0';

	/*while (szNamePos != NULL)
	{
		i = 0;



		strstr (szNamePos, szName);
	}*/

	if (bNameInMsg)
		BotFunc_FillString (szMsg, szName, name_in_msg, GetStringLength <unsigned short> (szMsg));

	// break the new message into an array of words
	HAL_MakeWords (szMsg, pBot->GetProfile ()->m_HAL->input_words);

	unsigned char iRep (5u);//pBot->GetProfile ()->m_Rep.GetClientRep (pSender);

	// does the bot feel concerned? (more chances of replying if its name appears)
	// if real mode is on, then bot chat is affected by bots rep with sender
	// and depends on chat_reply_percent command
	if (33u/*gBotGlobals.m_iBotChatReplyPercent*/ > 0u && (bNameInMsg || /*gBotGlobals.m_Clients*/
		((/*!gBotGlobals.IsConfigSettingOn (BOT_CONFIG_REAL_MODE) || */ g_randomNumberGenerator.GetValueBetween <unsigned char> (BOT_MIN_REP, BOT_MAX_REP) < iRep) && g_randomNumberGenerator.GetValueBetween <unsigned char> (0, 100) < 33u/*gBotGlobals.m_iBotChatReplyPercent*/) || g_server->GetClientManager ()->GetClientsCount () == 2u))
	{
//UNUSED IN RCBot ALSO!!!		pBot->m_MegaHALTalkEdict = pSender;

			BotHALGenerateReply (pBot, szReplyMsg);	// generate the reply

//	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("BotChatReply(): CALLED! szMsg: \"%s\".\n", szMsg);

		if (szReplyMsg[0u] != '\0')
		{
			BotFunc_FillString (szReplyMsg, name_in_msg, szSenderName, BOT_CHAT_MESSAGE_LENGTH);

			if (strcmp (szReplyMsg, szMsg) != 0)	// not the exact same message? :-p
				StringMakeLowerCase (szReplyMsg);	// convert the output string to lowercase
			else
				szReplyMsg[0u] = '\0';

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("------------------------------------------------------>DONE!!\n");
		}
	}

	// if sender is not a bot then learn from it's message
	// do this after working out a reply so the bots dont just say the same thing back.
	if (/*!gBotGlobals.IsConfigSettingOn (BOT_CONFIG_CHAT_DONT_LEARN) && */!pSender->IsYaPB ())
		HAL_Learn (pBot->GetProfile ()->m_HAL->bot_model, pBot->GetProfile ()->m_HAL->input_words);	// only learn from humans

	delete [] szName;
	delete [] szSenderName;
}

// from old rcbot
void HumanizeString (char *string)
{
	const int drop_percent = 1;
	const int swap_percent = 1;
	const int capitalise_percent = 1;
	const int lower_percent = 2;

	int length = GetStringLength <unsigned short> (string);
	int i = 0;
	int n = 0;
	int rand;

	char temp;

	while (i < length)
	{
		if (((i + 1) < length) && (g_randomNumberGenerator.GetValueBetween <unsigned char> (0, 100) < swap_percent))
		{
			temp = string[i];

			string[i] = string[i + 1];

			string[i + 1] = temp;

			i+=2;
			continue;
		}

		if (((rand = g_randomNumberGenerator.GetValueBetween <unsigned char> (0, 100)) < drop_percent) ||
			(((string[n] < '0') && (string[n] > '9')) && (rand < drop_percent*2)))
		{
			n = i;

			while (n < (length - 1))
			{
				string[n] = string[n + 1];
				++n;
			}

			string[--length] = '\0';

			++i;
			continue;
		}

		if (!isupper (string[i]) && ((i == 0) || (string[i-1] == ' ')))
		{
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (0, 100) < (capitalise_percent * 2))
				string[i] = static_cast <char> (toupper (string[i]));
		}

		if (isupper (string[i]) && (g_randomNumberGenerator.GetValueBetween <unsigned char> (0, 100) < lower_percent))
			string[i] = static_cast <char> (tolower (string[i]));

		++i;
	}
}

// from old rcbot
void RemoveNameTags (const char *in_string, char *out_string)
{
	int i = 0;	// index of in_string
	int n = 0;	// length of out_string

	out_string[0] = '\0';

	int length;

	char current_char;

	char tag_start;
	int tag_size = 0;
	bool space_allowed = false;
	bool inside_tag;

	if (in_string == NULL)
		return;

	length = GetStringLength <unsigned short> (in_string);

	InternalAssert (length <= 127);
/*	if (length > 127)
	{
		if (gBotGlobals.m_iDebugLevels & BOT_DEBUG_THINK_LEVEL)
			ALERT (at_console, "Error : RemoveNameTags (): Input netname is too long!\n");

		return;
	}
*/
	inside_tag = false;

	while (i < length)
	{
		current_char = 0;

		if (inside_tag)
		{
			if (((i + 1) < length) && (in_string[i] == '=') && (in_string[i+i] == '-'))
			{
				inside_tag = false;
				i += 2;
				continue;
			}
			else if ((in_string[i] == ')') || (in_string[i] == ']') || (in_string[i] == '}'))
			{
				//char temp = in_string[i];

				inside_tag = false;

				//if (tag_size == 0)
				//{
				//	out_string[n++] = tag_start;
				//	out_string[n++] = temp;
				//}

				++i;

				continue;
			}
			else
			{
				++tag_size;
				++i;
				continue;
			}
		}

		if (isalnum (in_string[i]))
		{
			current_char = in_string[i];

			space_allowed = true;
		}
		else if ((in_string[i] == ' (') || (in_string[i] == '[') || (in_string[i] == '{'))
		{
			inside_tag = true;
			tag_start = in_string[i];
			tag_size=0;
			++i;
			continue;
		}
		else if (((i + 1) < length) && (in_string[i] == '-') && (in_string[i+i] == '='))
		{
			inside_tag = true;

			i += 2;
			continue;
		}
		else
		{
			if (space_allowed)
			{
				current_char = ' ';
				space_allowed = false;
			}
			else
			{
				++i;
				continue;
			}
		}

		// l33t to normal
		switch (current_char)
		{
			case '5':
				current_char = 's';
				break;
			case '0':
				current_char = 'o';
				break;
			case '7':
				current_char = 't';
				break;
			case '3':
				current_char = 'e';
				break;
			case '1':
				current_char = 'i';
				break;
			case '4':
				current_char = 'a';
				break;
		}

		tag_size=0;
		out_string[n] = current_char;
		++n;
		++i;
	}

	if (out_string[0] != 0)
	{
		out_string[n] = '\0';

		--n;

		while ((n > 0) && (out_string[n] == ' '))
			out_string[n--] = '\0';
	}

	if (out_string[0] == 0)
		strcpy (out_string, in_string);

	StringMakeLowerCase (out_string);
/*
	// more 'l33t stuff
	// "l33t" R
	BotFunc_FillString (out_string, "|2", "r", length);
	// fat "l33ty" P's I's and M's
	BotFunc_FillString (out_string, "[]D", "p", length);
	BotFunc_FillString (out_string, "[]V[]", "m", length);
	BotFunc_FillString (out_string, "[]", "i", length);

	// http://www.bbc.co.uk/dna/h2g2/A787917
	BotFunc_FillString (out_string, " ()", "o", length);
	BotFunc_FillString (out_string, "|_|", "u", length);
	BotFunc_FillString (out_string, "|)", "d", length);
	BotFunc_FillString (out_string, "\\/\\/", "w", length);
	BotFunc_FillString (out_string, "$", "s", length);
*/
}

void BotHALGenerateReply (YaPB *pBot, char *output)
{
	// this function takes a string of user input and return a string of output which may
	// vaguely be construed as containing a reply to whatever is in the input string.
	// Create an array of keywords from the words in the user's input...

	HAL_DICTIONARY *keywords, *replywords;
	int tries_count, last_entry, last_character, length = 1;
	unsigned int i, j;

	keywords = BotHALMakeKeywords (pBot, pBot->GetProfile ()->m_HAL->input_words);
	replywords = BotHALBuildReplyDictionary (pBot, keywords);

	last_entry = pBot->GetProfile ()->m_HAL->input_words->size - 1;
	last_character = pBot->GetProfile ()->m_HAL->input_words->entry[last_entry].length - 1;

	// was it a question (i.e. was the last word in the general chat record a question mark?)
	if (pBot->GetProfile ()->m_HAL->input_words->entry[last_entry].word[last_character] == '?')
	{
		// try ten times to answer something relevant
		for (tries_count = 0; tries_count < 10; ++tries_count)
		{
			if (HAL_DictionariesDiffer (pBot->GetProfile ()->m_HAL->input_words, replywords))
				break;	// stop as soon as we've got something to say

			replywords = BotHALBuildReplyDictionary (pBot, keywords);	// else think again
		}

		// if we've finally found something to say, generate the reply
		if (tries_count < 10)
		{
			// if no words in the reply dictionary...
			if (replywords->size == 0)
			{
				// the HAL is too young to talk yet
				output[0] = '\0';
				return;
			}

			for (i = 0; i < replywords->size; ++i)
				length += replywords->entry[i].length;

			char *const output_template (new char[length]);
			if (output_template == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_MakeOutput() unable to allocate output\n");

				return;
			}

			output_template[0] = '\0';	// first reset the reply string

			length = 0;

			for (i = 0; i < replywords->size; ++i)
				for (j = 0; j < replywords->entry[i].length; ++j)
					output_template[length++] = replywords->entry[i].word[j];

			if (length >= BOT_CHAT_MESSAGE_LENGTH)
				output_template[BOT_CHAT_MESSAGE_LENGTH - 1u] = '\0';	// disallow strings to be longer than BOT_CHAT_MESSAGE_LENGTH chars
			else
				output_template[length] = '\0';	// terminate the string

			strcpy (output, output_template);	// then copy the answer

			delete [] output_template;

			return;
		}
	}
	// else if we are not paraphrazing, generate a string from the dictionary of reply words
	else if (HAL_DictionariesDiffer (pBot->GetProfile ()->m_HAL->input_words, replywords))
	{
		// if no words in the reply dictionary...
		if (replywords->size == 0)
		{
			// the HAL is too young to talk yet
			output[0] = '\0';
			return;
		}

		for (i = 0; i < replywords->size; ++i)
			length += replywords->entry[i].length;

		char *const output_template (new char[length]);
		if (output_template == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_MakeOutput() unable to allocate output\n");

			return;
		}

		output_template[0] = '\0';	// first reset the reply string

		length = 0;

		for (i = 0; i < replywords->size; ++i)
			for (j = 0; j < replywords->entry[i].length; ++j)
				output_template[length++] = replywords->entry[i].word[j];

		if (length >= BOT_CHAT_MESSAGE_LENGTH)
			output_template[BOT_CHAT_MESSAGE_LENGTH - 1u] = '\0';	// disallow strings to be longer than 128 chars
		else
			output_template[length] = '\0';	// terminate the string

		strcpy (output, output_template);	// then copy the answer

		delete [] output_template;

		return;
	}
}

unsigned short HAL_AddWord (HAL_DICTIONARY *dictionary, HAL_STRING word)
{
	// this function adds a word to a dictionary, and return the identifier assigned to the
	// word. If the word already exists in the dictionary, then return its current identifier
	// without adding it again.

	int i;
	int position;
	bool found;

	// if the word's already in the dictionary, there is no need to add it
	position = HAL_SearchDictionary (dictionary, word, found);
	if (found)
		return dictionary->index[position];

	// increase the number of words in the dictionary
	++dictionary->size;

	// allocate one more entry for the word index
	if (dictionary->index == NULL)
		dictionary->index = static_cast <unsigned short *> (malloc (sizeof (unsigned short) * (dictionary->size)));
	else
		dictionary->index = static_cast <unsigned short *> (realloc (dictionary->index, sizeof (unsigned short) * dictionary->size));

	if (dictionary->index == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddWord() unable to reallocate the dictionary index\n");

	// allocate one more entry for the word array
	if (dictionary->entry == NULL)
		dictionary->entry = static_cast <HAL_STRING *> (malloc (sizeof (HAL_STRING) * (dictionary->size)));
	else
		dictionary->entry = static_cast <HAL_STRING *> (realloc (dictionary->entry, sizeof (HAL_STRING) * dictionary->size));

	if (dictionary->entry == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddWord() unable to reallocate the dictionary to %d elements\n", dictionary->size);

	// copy the new word into the word array
	dictionary->entry[dictionary->size - 1u].length = word.length;
	dictionary->entry[dictionary->size - 1u].word = static_cast <char *> (malloc (sizeof (char) * (word.length + 1u)));
	if (dictionary->entry[dictionary->size - 1u].word == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddWord() unable to allocate the word\n");

	for (i = 0; i < word.length; ++i)
		dictionary->entry[dictionary->size - 1u].word[i] = word.word[i];
	dictionary->entry[dictionary->size - 1u].word[word.length] = '\0';

	// shuffle the word index to keep it sorted alphabetically
	for (i = dictionary->size - 1; i > position; --i)
		dictionary->index[i] = dictionary->index[i - 1];

	// copy the new symbol identifier into the word index
	dictionary->index[position] = static_cast <unsigned short> (dictionary->size - 1u);

	return dictionary->index[position];
}

int HAL_SearchDictionary (HAL_DICTIONARY *dictionary, HAL_STRING word, bool &find)
{
	// search the dictionary for the specified word, returning its position in the index if
	// found, or the position where it should be inserted otherwise

	int imin;
	int imax;
	int middle;
	int compar;

	// if the dictionary is empty, then obviously the word won't be found
	if (dictionary->size == 0)
	{
		find = false;

		return 0;
	}

	// initialize the lower and upper bounds of the search
	imin = 0;
	imax = dictionary->size - 1;

	//bool bDone = false;
	//int iFound = 0;

	// search repeatedly, halving the search space each time, until either the entry is found,
	// or the search space becomes empty
	for (;;)
	{
		// see whether the middle element of the search space is greater than, equal to, or
		// less than the element being searched for.
		middle = (imin + imax) / 2;
		compar = HAL_CompareWords (word, dictionary->entry[dictionary->index[middle]]);

		// if equal then we have found the element. Otherwise halve the search space accordingly
		if (compar == 0)
		{
			find = true;
			return middle;
		}
		else if (compar > 0)
		{
			if (imax == middle)
			{
				find = false;

				return middle + 1;
			}

			imin = middle + 1;
		}
		else
		{
			if (imin == middle)
			{
				find = false;

				return middle;
			}

			imax = middle - 1;
		}
	}
}

unsigned short HAL_FindWord (HAL_DICTIONARY *dictionary, HAL_STRING word)
{
	// this function returns the symbol corresponding to the word specified. We assume that
	// the word with index zero is equal to a NULL word, indicating an error condition.

	bool found;
	int position = HAL_SearchDictionary (dictionary, word, found);

	if (found == true)
		return dictionary->index[position];

	return 0;
}

int HAL_CompareWords (HAL_STRING word1, HAL_STRING word2)
{
	// this function compares two words, and return an integer indicating whether the first
	// word is less than, equal to or greater than the second word

	int i;
	int bound;

	try
	{
		bound = min (word1.length, word2.length);

		for (i = 0; i < bound; ++i)
			if (toupper (word1.word[i]) != toupper (word2.word[i]))
				return static_cast <int> (toupper (word1.word[i]) - toupper (word2.word[i]));
	}
	catch (...)
	{
		AddLogEntry (true, LogLevel_Error, false, "First chance exception in HAL_CompareWords ()\r\n (May be a problem with HAL Files)");
	}

	if (word1.length < word2.length)
		return -1;
	else if (word1.length > word2.length)
		return 1;

	return 0;
}

void HAL_InitializeDictionary (HAL_DICTIONARY *dictionary)
{
	// this function adds dummy words to the dictionary

	HAL_STRING word = {GET_STATIC_STRING_LENGTH ("<ERROR>"), "<ERROR>"};
	HAL_STRING end = {GET_STATIC_STRING_LENGTH ("<FIN>"), "<FIN>"};

	HAL_AddWord (dictionary, word);
	HAL_AddWord (dictionary, end);
}

HAL_DICTIONARY *HAL_NewDictionary (void)
{
	// this function allocates room for a new dictionary

	HAL_DICTIONARY *dictionary = static_cast <HAL_DICTIONARY *> (malloc (sizeof (HAL_DICTIONARY)));
	if (dictionary == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_NewDictionary() unable to allocate dictionary\n");

		return NULL;
	}

	dictionary->size = 0;
	dictionary->index = NULL;
	dictionary->entry = NULL;

	return dictionary;
}

void HAL_SaveDictionary (FILE *file, HAL_DICTIONARY *dictionary)
{
	// this function saves a dictionary to the specified file

	unsigned int i;
	unsigned char j;

	fwrite (&dictionary->size, sizeof (unsigned long), 1, file);

	// save each word to the file
	for (i = 0u; i < dictionary->size; ++i)
	{
		fwrite (&dictionary->entry[i].length, sizeof (unsigned char), 1, file);
		for (j = 0u; j < dictionary->entry[i].length; ++j)
			fwrite (&dictionary->entry[i].word[j], sizeof (char), 1, file);
	}
}

void HAL_LoadDictionary (FILE *file, HAL_DICTIONARY *dictionary)
{
	// this function loads a dictionary from the specified file

	int i;
	int size;
	HAL_STRING word;

	if (file == NULL)
		return;

	fread (&size, sizeof (unsigned long), 1, file);

	// load each dictionary word from the file
	for (i = 0; i < size; ++i)
	{
		fread (&word.length, sizeof (unsigned char), 1, file);

		word.word = new char[word.length + 1];

		if (word.word == NULL)
			AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_LoadDictionary() unable to allocate word\n");

		//for (j = 0; j < word.length; ++j)
		fread (word.word, sizeof (char), word.length, file);

		word.word[word.length] = '\0';

		HAL_AddWord (dictionary, word);

		delete [] word.word;
	}
}

HAL_TREE *HAL_NewNode (void)
{
	// allocate a new node for the n-gram tree, and initialise its contents to sensible values

	// allocate memory for the new node
	HAL_TREE *const node (static_cast <HAL_TREE *> (malloc (sizeof (HAL_TREE))));
	if (node == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_NewNode() unable to allocate node\n");

		return NULL;
	}

	// initialise the contents of the node
	node->symbol = 0;
	node->usage = 0;
	node->count = 0;
	node->branch = 0;
	node->tree = NULL;

	return node;
}

HAL_MODEL *HAL_NewModel (int order)
{
	// this function creates and initializes a new ngram model

	HAL_MODEL *model (new HAL_MODEL ());
	if (model == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_NewModel() unable to allocate model\n");

		return NULL;
	}

	model->order = (unsigned char) order;
	model->forward = HAL_NewNode ();
	model->backward = HAL_NewNode ();
	model->context = static_cast <HAL_TREE **> (malloc (sizeof (HAL_TREE *) * (order + 2)));
	if (model->context == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_NewModel() unable to allocate context array\n");

		return NULL;
	}

	HAL_InitializeContext (model);
	model->dictionary = HAL_NewDictionary ();
	HAL_InitializeDictionary (model->dictionary);

	return model;
}

void HAL_UpdateModel (HAL_MODEL *model, int symbol)
{
	// this function uppdates the model with the specified symbol

	int i;

	// update all of the models in the current context with the specified symbol
	for (i = model->order + 1; i > 0; --i)
		if (model->context[i - 1] != NULL)
			model->context[i] = HAL_AddSymbol (model->context[i - 1], (unsigned short) symbol);

	return;
}

void HAL_UpdateContext (HAL_MODEL *model, int symbol)
{
	// this function updates the context of the model without adding the symbol

	int i;

	for (i = model->order + 1; i > 0; --i)
		if (model->context[i - 1] != NULL)
			model->context[i] = HAL_FindSymbol (model->context[i - 1], symbol);
}

HAL_TREE *HAL_AddSymbol (HAL_TREE *tree, unsigned short symbol)
{
	// this function updates the statistics of the specified tree with the specified symbol,
	// which may mean growing the tree if the symbol hasn't been seen in this context before

	// search for the symbol in the subtree of the tree node
	HAL_TREE *node (HAL_FindSymbolAdd (tree, symbol));

	// increment the symbol counts
	if (node->count < 65535)
	{
		++node->count;
		++tree->usage;
	}

	return node;
}

HAL_TREE *HAL_FindSymbol (HAL_TREE *node, int symbol)
{
	// this function returns a pointer to the child node, if one exists, which contains symbol

	int i;
	HAL_TREE *found = NULL;
	bool found_symbol = false;

	// perform a binary search for the symbol
	i = HAL_SearchNode (node, symbol, &found_symbol);
	if (found_symbol)
		found = node->tree[i];

	return found;
}

HAL_TREE *HAL_FindSymbolAdd (HAL_TREE *node, int symbol)
{
	// this function is conceptually similar to HAL_FindSymbol, apart from the fact that if the
	// symbol is not found, a new node is automatically allocated and added to the tree

	int i;
	HAL_TREE *found = NULL;
	bool found_symbol = false;

	// perform a binary search for the symbol. If the symbol isn't found, attach a new sub-node
	// to the tree node so that it remains sorted.
	i = HAL_SearchNode (node, symbol, &found_symbol);

	if (found_symbol)
		found = node->tree[i];
	else
	{
		found = HAL_NewNode ();
		found->symbol = (unsigned short) symbol;
		HAL_AddNode (node, found, i);
	}

	return found;
}

void HAL_AddNode (HAL_TREE *tree, HAL_TREE *node, int position)
{
	// this function attachs a new child node to the sub-tree of the tree specified

	int i;

	// allocate room for one more child node, which may mean allocating the sub-tree from scratch
	if (tree->tree == NULL)
		tree->tree = static_cast <HAL_TREE **> (malloc (sizeof (HAL_TREE *) * (tree->branch+1)));
	else
	{
		tree->tree = static_cast <HAL_TREE **> (realloc (tree->tree, sizeof (HAL_TREE *) * (tree->branch + 1)));
	}

	if (tree->tree == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddNode() unable to reallocate subtree\n");

	// shuffle nodes down so that we can insert new node at subtree index given by position
	for (i = tree->branch; i > position; --i)
		tree->tree[i] = tree->tree[i - 1];

	// add the new node to the sub-tree
	tree->tree[position] = node;
	++tree->branch;
}

int HAL_SearchNode (HAL_TREE *node, int symbol, bool *found_symbol)
{
	// this function performs a binary search for the specified symbol on the subtree of the
	// given node. Return the position of the child node in the subtree if the symbol was found,
	// or the position where it should be inserted to keep the subtree sorted if it wasn't

	int imin;
	int imax;
	int middle;
	int compar;

	// handle the special case where the subtree is empty
	if (node->branch == 0)
	{
		*found_symbol = false;
		return 0;
	}

	// perform a binary search on the subtree
	imin = 0;
	imax = node->branch - 1;

	for (;;)
	{
		middle = (imin + imax) / 2;
		compar = symbol-node->tree[middle]->symbol;

		if (compar == 0)
		{
			*found_symbol = true;
			return middle;
		}
		else if (compar > 0)
		{
			if (imax == middle)
			{
				*found_symbol = false;
				return middle + 1;
			}

			imin = middle + 1;
		}
		else
		{
			if (imin == middle)
			{
				*found_symbol = false;
				return middle;
			}

			imax = middle - 1;
		}
	}
}

void HAL_InitializeContext (HAL_MODEL *model)
{
	// this function sets the context of the model to a default value

	int i;

	for (i = 0; i <= model->order; ++i)
		model->context[i] = NULL;	// reset all the context elements
}

void HAL_Learn (HAL_MODEL *model, HAL_DICTIONARY *words)
{
	// this function learns from the user's input

	unsigned int i;
	unsigned short symbol;

	if (words->size <= model->order)
		return;	// only learn from inputs which are long enough

	// train the model in the forward direction. Start by initializing the context of the model
	HAL_InitializeContext (model);
	model->context[0] = model->forward;

	for (i = 0; i < words->size; ++i)
	{
		// add the symbol to the model's dictionary if necessary, and update the model accordingly
		symbol = HAL_AddWord (model->dictionary, words->entry[i]);
		HAL_UpdateModel (model, symbol);
	}

	// add the sentence-terminating symbol
	HAL_UpdateModel (model, 1);

	// train the model in the backwards direction. Start by initializing the context of the model
	HAL_InitializeContext (model);
	model->context[0] = model->backward;

	for (i = words->size - 1; static_cast <int> (i) >= 0; --i)
	{
		// find the symbol in the model's dictionary, and update the backward model accordingly
		symbol = HAL_FindWord (model->dictionary, words->entry[i]);
		HAL_UpdateModel (model, symbol);
	}

	// add the sentence-terminating symbol
	HAL_UpdateModel (model, 1);

	return;
}

void HAL_SaveTree (FILE *file, HAL_TREE *node)
{
	// this function saves a tree structure to the specified file

	int i;

	if (node)
	{
		fwrite (&node->symbol, sizeof (unsigned short), 1, file);
		fwrite (&node->usage, sizeof (unsigned long), 1, file);
		fwrite (&node->count, sizeof (unsigned short), 1, file);
		fwrite (&node->branch, sizeof (unsigned short), 1, file);

		for (i = 0; i < node->branch; ++i)
			HAL_SaveTree (file, node->tree[i]);
	}
}

void HAL_LoadTree (FILE *file, HAL_TREE *node)
{
	// this function loads a tree structure from the specified file

	int i;

	fread (&node->symbol, sizeof (unsigned short), 1, file);
	fread (&node->usage, sizeof (unsigned long), 1, file);
	fread (&node->count, sizeof (unsigned short), 1, file);
	fread (&node->branch, sizeof (unsigned short), 1, file);

	if (node->branch == 0)
		return;	// reliability check

	node->tree = static_cast <HAL_TREE **> (malloc (sizeof (HAL_TREE *) * node->branch));
	if (node->tree == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_LoadTree() unable to allocate subtree\n");

	for (i = 0; i < node->branch; ++i)
	{
		node->tree[i] = HAL_NewNode ();
		HAL_LoadTree (file, node->tree[i]);
	}
}

void HAL_MakeWords (char *input, HAL_DICTIONARY *words)
{
	// this function breaks a string into an array of words
	int offset = 0;
	int iLen;

	if (!input || !*input)
		return;	// if void, return

	// re-written

	iLen = GetStringLength <unsigned short> (input);

	// clear the entries in the dictionary
	HAL_EmptyDictionary (words);

	// loop forever
	for (;;)
	{
		// if the current character is of the same type as the previous character, then include
		// it in the word. Otherwise, terminate the current word.
		if (HAL_BoundaryExists (input, offset))
		{
			// add the word to the dictionary
			if (words->entry == NULL)
				words->entry = static_cast <HAL_STRING *> (malloc ((words->size + 1) * sizeof (HAL_STRING)));
			else
				words->entry = static_cast <HAL_STRING *> (realloc (words->entry, (words->size + 1) * sizeof (HAL_STRING)));

			if (words->entry == NULL)
				AddLogEntry (true, LogLevel_Error, false, "HAL_MakeWords() unable to reallocate dictionary\n");

			words->entry[words->size].length = static_cast <unsigned char> (offset);
			words->entry[words->size].word = input;
			++words->size;

			if (offset == GetStringLength <unsigned short> (input))
				break;

			input += offset;
			offset = 0;
		}
		else
			++offset;
	}

	// finished, no need to add punctuation (it's an ACTION game, woohoo!)
}
/*
void HAL_MakeWords (char *input, HAL_DICTIONARY *words)
{
	// this function breaks a string into an array of words
	int iLen;
	int iStart;
	int iEnd;

	char *szNewWord;
	int iNewWordLength;

	HAL_STRING *pNewWord;

	//int offset = 0;

	// clear the entries in the dictionary
	HAL_EmptyDictionary (words);

	if (!input || !*input)
		return;	// if void, return

	// re-written

	iLen = GetStringLength <unsigned short> (input);
	iStart = 0;
	iEnd = 0;

	while (iStart < iLen)
	{
		if (HAL_BoundaryExists (input, iEnd) || (iEnd == iLen))
			// If there is a new word to take in or at the end of the string
			// then add the word to the dictionary
		{
			// add the word to the dictionary
			if (words->entry == NULL)
				words->entry = static_cast <HAL_STRING *> (malloc ((words->size + 1) * sizeof (HAL_STRING)));
			else
				words->entry = static_cast <HAL_STRING *> (realloc (words->entry, (words->size + 1) * sizeof (HAL_STRING)));

			// get pointer to new word for quick access
			pNewWord = &words->entry[words->size];
			// work out new word length for new string
			iNewWordLength = iEnd - iStart;

			szNewWord = static_cast <char *> (malloc (sizeof (char) * (iNewWordLength + 1)));

			// copy word into string szNewWord
			// word starts from position iStart until iEnd
			// (goes on for iNewWordLength)
			strncpy (szNewWord, &input[iStart], iNewWordLength);
			szNewWord[iNewWordLength] = '\0';

			// update new word store
			pNewWord->length = iNewWordLength;
			pNewWord->word = szNewWord;

			// increment number of words stored
			++words->size;

			iStart = iEnd;
		}

		++iEnd;
	}
}*/

bool HAL_BoundaryExists (const char *string, int position)
{
	// this function returns whether or not a word boundary exists in a string at the
	// specified location

	if (position == 0)
		return false;

	if (position == GetStringLength <unsigned short> (string))
		return true;

	if ((string[position] == '\'') && (IsAlphaCharacter (string[position - 1]) != 0)
		&& (IsAlphaCharacter (string[position + 1]) != 0))
		return false;

	if ((position > 1) && (string[position - 1] == '\'')
		&& (IsAlphaCharacter (string[position - 2]) != 0) && (IsAlphaCharacter (string[position]) != 0))
		return false;

	if ((IsAlphaCharacter (string[position]) != 0) && (IsAlphaCharacter (string[position - 1]) == 0))
		return true;

	if ((IsAlphaCharacter (string[position]) == 0) && (IsAlphaCharacter (string[position - 1]) != 0))
		return true;

	if (IsDigitCharacter (string[position]) != IsDigitCharacter (string[position - 1]))
		return true;

	return false;
}

bool HAL_DictionariesDiffer (HAL_DICTIONARY *words1, HAL_DICTIONARY *words2)
{
	// this function returns true if the dictionaries are NOT the same or false if not

	unsigned int i;

	if (words1->size != words2->size)
		return true;	// if they haven't the same size, obviously they aren't the same

	// for each word of the first dictionary...
	for (i = 0; i < words1->size; ++i)
		if (HAL_CompareWords (words1->entry[i], words2->entry[i]) != 0)
			return true;	// compare it with the second and break at the first difference

	return false;	// looks like those dictionaries are identical
}

HAL_DICTIONARY *BotHALMakeKeywords (YaPB *pBot, HAL_DICTIONARY *words)
{
	// this function puts all the interesting words from the user's input into a keywords
	// dictionary, which will be used when generating a reply

	static HAL_DICTIONARY *keys = NULL;
	unsigned int i;
	unsigned short j;
	unsigned short c;

	// Reliability check.
	InternalAssert (pBot->IsValid ());

	if (keys == NULL)
		keys = HAL_NewDictionary ();

	for (i = 0; i < keys->size; ++i)
	{
		if (keys->entry[i].word != NULL)
			free (keys->entry[i].word);

		keys->entry[i].word = NULL;
	}

	HAL_EmptyDictionary (keys);

	for (i = 0; i < words->size; ++i)
	{
		// find the symbol ID of the word. If it doesn't exist in the model, or if it begins
		// with a non-alphanumeric character, or if it is in the exclusion array, then skip it

		c = 0;

		for (j = 0; j < pBot->GetProfile ()->m_HAL->swappable_keywords->size; ++j)
			if (HAL_CompareWords (pBot->GetProfile ()->m_HAL->swappable_keywords->from[j], words->entry[i]) == 0)
			{
				BotHALAddKeyword (pBot, keys, pBot->GetProfile ()->m_HAL->swappable_keywords->to[j]);
				++c;
			}

		if (c == 0)
			BotHALAddKeyword (pBot, keys, words->entry[i]);
	}

	if (keys->size > 0)
		for (i = 0; i < words->size; ++i)
		{
			c = 0;

			for (j = 0; j < pBot->GetProfile ()->m_HAL->swappable_keywords->size; ++j)
				if (HAL_CompareWords (pBot->GetProfile ()->m_HAL->swappable_keywords->from[j], words->entry[i]) == 0)
				{
					BotHALAddAuxiliaryKeyword (pBot, keys, pBot->GetProfile ()->m_HAL->swappable_keywords->to[j]);
					++c;
				}

			if (c == 0)
				BotHALAddAuxiliaryKeyword (pBot, keys, words->entry[i]);
		}

	return keys;
}

void FillStringArea (char *string, int maxstring, const char *fill, int/* maxfill*/, int start, int end)
{
	const int size (maxstring + 1);
	char *const before (new char[size]);
	char *const after (new char[size]);

	memset (before, '\0', size);
	memset (after, '\0', size);

	strncpy (before, string, start);
	strncpy (after, string + end, maxstring - end);

	sprintf (string, "%s%s%s", before, fill, after);

	delete [] before;
	delete [] after;
}

void BotHALAddKeyword (YaPB *pBot, HAL_DICTIONARY *keys, HAL_STRING word)
{
	// this function adds a word to the keyword dictionary

	int symbol;

	// Reliability check.
	InternalAssert (pBot->IsValid ());

	symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->bot_model->dictionary, word);

	if (symbol == 0)
		return;	// if void, return

	if (isalnum (word.word[0]) == 0)
		return;	// if not alphanumeric, return

	symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->banned_keywords, word);	// is this word in the banned dictionary?

	if (symbol != 0)
		return;	// if so, return

	/*if (word.word)
	{
		char *pName = StringDuplicate (pBot->GetName ());

		if (pName)
		{
			int len;

			RemoveNameTags (pBot->GetName (), pName);

			len = GetStringLength <unsigned char> (pName) + 3;

			char *pCheck = new char[len];

			if (pCheck)
			{
				char *strsearch;
				int namelen;
				int wordlen;
				char *theword;

				int start;
				int end;

				theword = word.word;

				sprintf (pCheck, pName);

				StringMakeLowerCase (pCheck);

				namelen = GetStringLength <unsigned char> (pName);
				wordlen = GetStringLength <unsigned short> (theword);

				strsearch = theword;

				while ((strsearch = strstr (strsearch, pCheck)) != NULL)
				{
					start = strpos (strsearch, theword);
					end = start+namelen;

					if (start == 0)
					{
						if (end < wordlen)
						{
							if (!isalnum (theword[end]))
								FillStringArea (theword, wordlen, "%n", 2, start, end);
						}
						else
							FillStringArea (theword, wordlen, "%n", 2, start, end);
					}
					else
					{
						if (end < wordlen)
						{
							if (!isalnum (theword[start-1]) && !isalnum (theword[end]))
								FillStringArea (theword, wordlen, "%n", 2, start, end);
						}
						else
							FillStringArea (theword, wordlen, "%n", 2, start, end);
					}
				}

				// name match, don't use name as keyword?
				if (strstr (word.word, pCheck) != NULL)
				{
					delete [] pCheck;
					delete [] pName;

					return;
				}
				else if (strncmp (word.word, pCheck + 1, len-2) == 0)
				{
					delete [] pCheck;
					delete [] pName;

					return;
				}

				delete [] pCheck;
			}

			delete [] pName;
		}
	}
*/
	symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->auxiliary_keywords, word);	// is this word in the auxiliary dictionary?

	if (symbol != 0)
		return;	// if so, return

	HAL_AddWord (keys, word);	// once we are sure this word isn't known yet, we can add it
}

void BotHALAddAuxiliaryKeyword (YaPB *pBot, HAL_DICTIONARY *keys, HAL_STRING word)
{
	// this function adds an auxilliary keyword to the keyword dictionary

	int symbol;

	// Reliability check.
	InternalAssert (pBot->IsValid ());

	symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->bot_model->dictionary, word);

	if (symbol == 0)
		return;	// if void, return

	if (isalnum (word.word[0]) == 0)
		return;	// if not alphanumeric, return

	symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->auxiliary_keywords, word);	// is it already in the dictionary?
	if (symbol == 0)
		return;	// if already in dictionary, return

	HAL_AddWord (keys, word);	// add this word to the keywords dictionary
}

HAL_DICTIONARY *BotHALBuildReplyDictionary (YaPB *pBot, HAL_DICTIONARY *keys)
{
	// this function generates a dictionary of reply words relevant to the dictionary of keywords

	static HAL_DICTIONARY *replies = NULL;
	int i;
	int symbol;
	bool start = true;

	// Reliability check.
	InternalAssert (pBot->IsValid ());

	if (replies == NULL)
		replies = HAL_NewDictionary ();

	HAL_EmptyDictionary (replies);

	// start off by making sure that the model's context is empty
	HAL_InitializeContext (pBot->GetProfile ()->m_HAL->bot_model);
	pBot->GetProfile ()->m_HAL->bot_model->context[0] = pBot->GetProfile ()->m_HAL->bot_model->forward;
	pBot->GetProfile ()->m_HAL->keyword_is_used = false;

	// generate the reply in the forward direction
	for (;;)
	{
		// get a random symbol from the current context
		if (start == true)
			symbol = BotHALSeedReply (pBot, keys);
		else
			symbol = BotHALBabble (pBot, keys, replies);

		if ((symbol == 0) || (symbol == 1))
			break;

		start = false;

		// append the symbol to the reply dictionary
		if (replies->entry == NULL)
			replies->entry = static_cast <HAL_STRING *> (malloc ((replies->size + 1) * sizeof (HAL_STRING)));
		else
			replies->entry = static_cast <HAL_STRING *> (realloc (replies->entry, (replies->size + 1) * sizeof (HAL_STRING)));

		if (replies->entry == NULL)
			AddLogEntry (true, LogLevel_Error, false, "HAL: BotHALBuildReplyDictionary() unable to reallocate dictionary\n");

		replies->entry[replies->size].length = pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol].length;
		replies->entry[replies->size].word = pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol].word;
		++replies->size;

		// extend the current context of the model with the current symbol
		HAL_UpdateContext (pBot->GetProfile ()->m_HAL->bot_model, symbol);
	}

	// start off by making sure that the model's context is empty
	HAL_InitializeContext (pBot->GetProfile ()->m_HAL->bot_model);
	pBot->GetProfile ()->m_HAL->bot_model->context[0] = pBot->GetProfile ()->m_HAL->bot_model->backward;

	// re-create the context of the model from the current reply dictionary so that we can
	// generate backwards to reach the beginning of the string.
	if (replies->size > 0)
		for (i = min (replies->size - 1, pBot->GetProfile ()->m_HAL->bot_model->order); i >= 0; --i)
		{
			symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->bot_model->dictionary, replies->entry[i]);
			HAL_UpdateContext (pBot->GetProfile ()->m_HAL->bot_model, symbol);
		}

	// generate the reply in the backward direction
	for (;;)
	{
		// get a random symbol from the current context
		symbol = BotHALBabble (pBot, keys, replies);
		if ((symbol == 0) || (symbol == 1))
			break;

		// prepend the symbol to the reply dictionary
		if (replies->entry == NULL)
			replies->entry = static_cast <HAL_STRING *> (malloc ((replies->size + 1) * sizeof (HAL_STRING)));
		else
			replies->entry = static_cast <HAL_STRING *> (realloc (replies->entry, (replies->size + 1) * sizeof (HAL_STRING)));

		if (replies->entry == NULL)
			AddLogEntry (true, LogLevel_Error, false, "HAL: BotHALBuildReplyDictionary() unable to reallocate dictionary\n");

		// shuffle everything up for the prepend
		for (i = replies->size; i > 0; --i)
		{
			replies->entry[i].length = replies->entry[i - 1].length;
			replies->entry[i].word = replies->entry[i - 1].word;
		}

		replies->entry[0].length = pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol].length;
		replies->entry[0].word = pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol].word;
		++replies->size;

		// extend the current context of the model with the current symbol
		HAL_UpdateContext (pBot->GetProfile ()->m_HAL->bot_model, symbol);
	}

	return replies;
}

int BotHALBabble (YaPB *pBot, HAL_DICTIONARY *keys, HAL_DICTIONARY *words)
{
	// this function returns a random symbol from the current context, or a zero symbol
	// identifier if we've reached either the start or end of the sentence. Selection of the
	// symbol is based on probabilities, favouring keywords. In all cases, use the longest
	// available context to choose the symbol

	HAL_TREE *node = NULL;
	int i;
	int count;
	int symbol = 0;

	// Reliability check.
	InternalAssert (pBot->IsValid ());

	// select the longest available context
	for (i = 0; i <= pBot->GetProfile ()->m_HAL->bot_model->order; ++i)
		if (pBot->GetProfile ()->m_HAL->bot_model->context[i] != NULL)
			node = pBot->GetProfile ()->m_HAL->bot_model->context[i];

	if (node->branch == 0)
		return 0;

	// choose a symbol at random from this context
	i = g_randomNumberGenerator.GetValueBetween <int> (0, node->branch - 1);
	count = g_randomNumberGenerator.GetValueBetween <int> (0, node->usage - 1);

	while (count >= 0)
	{
		// if the symbol occurs as a keyword, then use it. Only use an auxilliary keyword if
		// a normal keyword has already been used.
		symbol = node->tree[i]->symbol;

		if ((HAL_FindWord (keys, pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol]) != 0)
			&& (pBot->GetProfile ()->m_HAL->keyword_is_used || (HAL_FindWord (pBot->GetProfile ()->m_HAL->auxiliary_keywords, pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol]) == 0))
			&& !HAL_WordExists (words, pBot->GetProfile ()->m_HAL->bot_model->dictionary->entry[symbol]))
		{
			pBot->GetProfile ()->m_HAL->keyword_is_used = true;
			break;
		}

		count -= node->tree[i]->count;

		i = (i >= node->branch - 1) ? 0 : i + 1;
	}

	return symbol;
}

bool HAL_WordExists (HAL_DICTIONARY *dictionary, HAL_STRING word)
{
	// here's a silly brute-force searcher for the reply string

	unsigned int i;

	// for each element of the dictionary, compare word with it...
	for (i = 0; i < dictionary->size; ++i)
		if (HAL_CompareWords (dictionary->entry[i], word) == 0)
			return true;	// word was found

	return false;	// word was not found
}

int BotHALSeedReply (YaPB *pBot, HAL_DICTIONARY *keys)
{
	// this function seeds the reply by guaranteeing that it contains a keyword, if one exists

	unsigned int i;
	int symbol;
	unsigned int stop;

	// be aware of the special case where the tree is empty
	if (pBot->GetProfile ()->m_HAL->bot_model->context[0]->branch == 0)
		symbol = 0;
	else
		symbol = pBot->GetProfile ()->m_HAL->bot_model->context[0]->tree[g_randomNumberGenerator.GetValueBetween <int> (0, pBot->GetProfile ()->m_HAL->bot_model->context[0]->branch - 1)]->symbol;

	if (keys->size > 0)
	{
		i = g_randomNumberGenerator.GetValueBetween <unsigned int> (0, keys->size - 1);
		stop = i;

		for (;;)
		{
			if ((HAL_FindWord (pBot->GetProfile ()->m_HAL->bot_model->dictionary, keys->entry[i]) != 0) && (HAL_FindWord (pBot->GetProfile ()->m_HAL->auxiliary_keywords, keys->entry[i]) == 0))
			{
				symbol = HAL_FindWord (pBot->GetProfile ()->m_HAL->bot_model->dictionary, keys->entry[i]);
				return symbol;
			}

			++i;

			if (i == keys->size)
				i = 0;

			if (i == stop)
				return symbol;
		}
	}

	return symbol;
}

HAL_SWAP *HAL_NewSwap (void)
{
	// allocate a new swap structure.

	HAL_SWAP *const list (new HAL_SWAP ());
	if (list == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_NewSwap() unable to allocate swap\n");

	list->size = 0;	// initialize to defaults
	list->from = NULL;
	list->to = NULL;

	return list;	// return the fresh new swap
}

void HAL_AddSwap (HAL_SWAP *list, char *s, char *d)
{
	// this function adds a new entry to the swap structure.

	if (list->from == NULL)
	{
		list->from = static_cast <HAL_STRING *> (malloc (sizeof (HAL_STRING)));
		if (list->from == NULL)
			AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddSwap() unable to allocate list->from\n");
	}

	if (list->to == NULL)
	{
		list->to = static_cast <HAL_STRING *> (malloc (sizeof (HAL_STRING)));
		if (list->to == NULL)
			AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddSwap() unable to allocate list->to\n");
	}

	list->from = static_cast <HAL_STRING *> (realloc (list->from, sizeof (HAL_STRING) * (list->size + 1)));
	if (list->from == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddSwap() unable to reallocate from\n");

	list->to = static_cast <HAL_STRING *> (realloc (list->to, sizeof (HAL_STRING) * (list->size + 1)));
	if (list->to == NULL)
		AddLogEntry (true, LogLevel_Error, false, "HAL: HAL_AddSwap() unable to reallocate to\n");

	list->from[list->size].length = GetStringLength <unsigned char> (s);
	list->from[list->size].word = StringDuplicate (s);
	list->to[list->size].length = GetStringLength <unsigned char> (d);
	list->to[list->size].word = StringDuplicate (d);
	++list->size;
}

HAL_SWAP *HAL_InitializeSwap (char *filename)
{
	// this function reads a swap structure from a file.

	HAL_SWAP *list;
	FILE *fp;
	char buffer[1024];
	char *from;
	char *to;

	list = HAL_NewSwap ();

	if (filename == NULL)
		return list;

	fp = fopen (filename, "r");
	if (fp == NULL)
		return list;

	while (!feof (fp))
	{
		if (fgets (buffer, 1024, fp) == NULL)
			break;

		if (buffer[0] == '#' || buffer[0] == '\n')
			continue;

		from = strtok (buffer, "\t");
		to = strtok (NULL, "\t\n#");

		HAL_AddSwap (list, from, to);
	}

	fclose (fp);
	return list;
}

HAL_DICTIONARY *HAL_InitializeList (char *filename)
{
	// this function reads a dictionary from a file

	HAL_DICTIONARY *list;
	FILE *fp;
	HAL_STRING word;
	char *string;
	char buffer[1024];

	list = HAL_NewDictionary ();

	if (filename == NULL)
		return list;

	fp = fopen (filename, "r");
	if (fp == NULL)
		return list;

	while (!feof (fp))
	{
		if (fgets (buffer, 1024, fp) == NULL)
			break;

		if (buffer[0] == '#' || buffer[0] == '\n')
			continue;

		string = strtok (buffer, "\t\n#");

		if (!IsNullOrEmptyString (string))
		{
			word.length = GetStringLength <unsigned char> (string);
			word.word = StringDuplicate (buffer);	// StringDuplicate() - duplicates string
			HAL_AddWord (list, word);
		}
	}

	fclose (fp);
	return list;
}

void HAL_EmptyDictionary (HAL_DICTIONARY *dictionary)
{
	// this function empties the memory space used by a dictionary, cutting it down to zero.
	// NOTE THAT IT DOES NOT FREE THE DICTIONARY MEMORY SPACE

	if (dictionary == NULL)
		return;	// reliability check

	if (dictionary->entry != NULL)
	{
/*		/// @note ADDED BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		for (unsigned int index (0u); index < dictionary->size; ++index)
			free (dictionary->entry[index].word);
*/
		free (dictionary->entry);

		dictionary->entry = NULL;
	}

	if (dictionary->index != NULL)
	{
		free (dictionary->index);

		dictionary->index = NULL;
	}

	dictionary->size = 0;
}

void HAL_FreeModel (HAL_MODEL *model)
{
	// this function frees the memory space used by a model

	if (model == NULL)
		return;	// reliability check

	if (model->forward != NULL)
		HAL_FreeTree (model->forward);

	model->forward = NULL;

	if (model->backward != NULL)
		HAL_FreeTree (model->backward);

	model->backward = NULL;

	if (model->context != NULL)
		free (model->context);

	model->context = NULL;

	if (model->dictionary != NULL)
	{
		HAL_EmptyDictionary (model->dictionary);
		free (model->dictionary);
	}

	model->dictionary = NULL;

	delete model;
}

void HAL_FreeTree (HAL_TREE *tree)
{
	// this function frees the memory space used by a model tree

	int i;

	if (tree == NULL)
		return;	// reliability check

	if (tree->tree != NULL)
	{
		for (i = 0; i < tree->branch; ++i)
			HAL_FreeTree (tree->tree[i]);

		free (tree->tree);
		tree->tree = NULL;
	}

	free (tree);
}

void HAL_FreeSwap (HAL_SWAP *swap)
{
	// this function frees the memory space used in a swap structure

	int i;

	if (swap == NULL)
		return;	// if already freed, return

	// for each element of the swap structure...
	for (i = 0; i < swap->size; ++i)
	{
		if (swap->from[i].word != NULL)
			delete [] swap->from[i].word;	// free the "from" word

		swap->from[i].word = NULL;

		// free TO

		if (swap->to[i].word != NULL)
			delete [] swap->to[i].word;	// free the "to" word

		swap->to[i].word = NULL;
	}

	if (swap->from != NULL)
		free (swap->from);	// free the "from" array

	swap->from = NULL;

	if (swap->to != NULL)
		free (swap->to);	// free the "to" array

	swap->to = NULL;

	delete swap;	// free the swap structure itself
}

void ReplyToMessage (YaPB *const bot, char *szMessage, Client *pSender, const bool/* iTeamOnly*/)
{
	// make a reply using megaHAL from a message by another player

	char tempMessage[BOT_CHAT_MESSAGE_LENGTH];
/*
	if ( m_Tasks.HasTask(BOT_TASK_TYPE_MESSAGE) )
		return;
*/
	tempMessage[0u] = '\0';

	BotChatReply (bot, szMessage, pSender, tempMessage);

	// got a reply?
	if (tempMessage[0u] != '\0')
	{
//		strcpy (m_szChatString, tempMessage);
//		AddPriorityTask (CBotTask (BOT_TASK_TYPE_MESSAGE, 0, pSender, iTeamOnly));
		bot->ExecuteCommandFormat ("say \"%s\"", tempMessage);
	}
}