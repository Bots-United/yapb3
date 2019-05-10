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
// Human.hpp
//
// Class: Human
//
// Description: Contains the 'Human' class Function Prototypes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HUMAN_INCLUDED
#define HUMAN_INCLUDED

class Human : public Client
{
	friend ClientManager;
	friend MenuManager;

	DECLARE_CLASS_WITH_BASE (Client, Human);

	public:
		enum VoiceRecieverListenType_t
		{
			VoiceRecieverListenType_Default,	// Keep all as it is....

			VoiceRecieverListenType_Loopback,	// No matter what send voice data packed back to real client....
			VoiceRecieverListenType_Ignore		// No matter what ignore voice data packed of the real client....
		};

	//
	// Group: Private members.
	//
	private:
		struct OpenedYaPBMenuInformation_t
		{
			struct PrevoiusMenu_t
			{
				MenuID_t                                          id;	// index to opened YaPB menu
				HalfLifeEngine::SDK::Constants::Menu::PageIndex_t page;	// page index to opened YaPB menu
			};
			typedef Stack <PrevoiusMenu_t, unsigned char> PrevoiusMenuStack_t;

			MenuID_t                                          id;				// index to opened YaPB menu
			HalfLifeEngine::SDK::Constants::Menu::PageIndex_t page;				// page index to opened YaPB menu
			HalfLifeEngine::SDK::Constants::Menu::KeyBit_t    keys;				// valid key bits to opened YaPB menu
//			float                                             shutoffTime;

			PrevoiusMenuStack_t                               previousMenus;	// indexes to previous opened YaPB menus

			float                                             updateTime;

			inline OpenedYaPBMenuInformation_t (void) :
				id (MenuID_None),
				page (0u),
				keys (HalfLifeEngine::SDK::Constants::Menu::KeyBit_None),
//				shutoffTime (-1.0f),

				updateTime (0.0f)
			{ /* VOID */ }

			inline void Reset (void)
			{
				id = MenuID_None;
				page = 0u;	// Reset current opened page to first one.
			}
			inline void SetNewID (const MenuID_t newID)
			{
				// Reliability check.
				InternalAssert (id != newID);

				if (id == MenuID_None)
					previousMenus.RemoveAll ();
				else
					AddPreviousMenu ();

				id = newID;
				page = 0u;
				keys = HalfLifeEngine::SDK::Constants::Menu::KeyBit_None;
			}
			inline void SetPreviousMenu (void)
			{
				// Reliability check.
				InternalAssert (!previousMenus.IsEmpty ());

				const PrevoiusMenu_t previousMenu (previousMenus.Pop ());

				// Reliability check.
				InternalAssert (id != previousMenu.id || page != previousMenu.page);

				id = previousMenu.id;
				page = previousMenu.page;
				keys = HalfLifeEngine::SDK::Constants::Menu::KeyBit_None;
			}
			inline void SetNewPage (const HalfLifeEngine::SDK::Constants::Menu::PageIndex_t newPage)
			{
				// Reliability checks.
				InternalAssert (id != MenuID_None);
				InternalAssert (page != newPage);

				AddPreviousMenu ();	/// @todo MAKE AddPreviousMenu() CALLED ONLY WHEN 'newPage' IS BIGGER THAN 'page'?!?!?!?!?!?!?!?!?!?!?!?!??!

				page = newPage;
				keys = HalfLifeEngine::SDK::Constants::Menu::KeyBit_None;
			}
			inline void SetPreviousPage (void)
			{
				// Reliability check.
				InternalAssert (page > 0u);

				SetNewPage (page - 1u);
			}
			inline void SetNextPage (void)
			{
				SetNewPage (page + 1u);
			}

			private:
				inline void AddPreviousMenu (void)
				{
//					previousMenus += PrevoiusMenu_t (id, page);
					const PrevoiusMenu_t newPreviousMenu = {id, page};

					previousMenus += newPreviousMenu;
				}
		} m_openedYaPBMenu;
		struct LastStateInformation_t
		{
			Math::Vector3D                               origin;
			Math::Angles2D                               viewAngles;
			HalfLifeEngine::SDK::Constants::ButtonType_t pressedButtons;
//			float                                        activityTime;

			inline LastStateInformation_t (void) :
				origin (Math::Vector3D::ZeroValue),
				viewAngles (Math::Angles2D::ZeroValue),
				pressedButtons (HalfLifeEngine::SDK::Constants::ButtonType_None)/*,
				activityTime (0.0f)*/
			{ /* VOID */ }
		}     m_lastStateInformation;	// Information for AFK detection....
		float m_AFKCheckTimestamp;		// AFK check interval (in seconds)
		float m_AFKTime;				// Time human was AFK (in seconds, Note: not world time!)

		Localizer                 *m_localizer;	/// @todo INVOLVE ME!!!

		bool  m_isSpeaking;

		VoiceRecieverListenType_t  m_voiceRecieverListenType;
		YaPB                      *m_voiceRecieverBot;

	//
	// Group: Protected members.
	//
	protected:
		/// @note This values should equals real human screen resolution only for GetScreenPosition() function! (For GetNormalizedScreenPosition() this is unnecessary)
		struct ScreenInformation_t
		{
			// Resolution
			unsigned short width;	// Half is screen center X
			unsigned short height;	// Half is screen center Y

			// Characters size
			unsigned char  charactersHeight;
			unsigned char  charactersWidths[MximumCharactersNumber];
		} m_screenInformation;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Human (HalfLifeEngine::SDK::Classes::Edict *const human) :
			Client (human),

			m_AFKCheckTimestamp (0.0f),
			m_AFKTime (0.0f),

			m_localizer (g_localizerManager.TryAddLocalizerForLanguage (GetInitialLanguageName (human))),

			m_isSpeaking (false),

			m_voiceRecieverListenType (VoiceRecieverListenType_Default),
			m_voiceRecieverBot (NULL)
		{
			// Reliability check.
			InternalAssert (m_localizer != NULL);

			m_screenInformation.width = 1280u;
			m_screenInformation.height = 1024u;
			m_screenInformation.charactersHeight = HalfLifeEngine::SDK::Constants::charactersHeight;

			MemoryCopy (HalfLifeEngine::SDK::Constants::charactersWidths, m_screenInformation.charactersWidths, GET_STATIC_ARRAY_SIZE (m_screenInformation.charactersWidths));
		}

		virtual inline ~Human (void)
		{
			g_localizerManager.TryRemoveLocalizer (m_localizer, this);
		}

	//
	// Group: Private functions.
	//
	private:
		static inline const DynamicString GetInitialLanguageName (HalfLifeEngine::SDK::Classes::Edict *const human)
		{
			const DynamicString languageName (HalfLifeEngine::Globals::g_halfLifeEngine->GetEntityKeyValue (human, CLIENT_USER_INFO_LANGUAGE_KEY));

			return languageName.IsEmpty () || !languageName.IsAlpha () ? g_localizer.GetCurrentLanguageName () : languageName;
		}
		inline void ChangeLanguage (const DynamicString &newLanguage)
		{
//			AddLogEntry (true, LogLevel_Default, false, "Human::UserInfoChanged(): called for human \"%s\", current language \"%s\" is changed to: \"%s\".", GetName ().GetData (), GetCurrentLanguageName ().GetData (), newLanguage.GetData ());

			g_localizerManager.TryRemoveLocalizer (m_localizer, this);

			m_localizer = g_localizerManager.TryAddLocalizerForLanguage (newLanguage);
		}

		virtual inline const Type_t GetType (void) const { return Type_Human; }

		inline void CheckAFK (void)
		{
			const float AFKCheckInterval (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MaximumAFKTime)->GetValue <float> () * 0.1f);

			if (m_AFKCheckTimestamp > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;

			m_AFKCheckTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + AFKCheckInterval;

			if (m_lastStateInformation.origin == GetOrigin () && m_lastStateInformation.viewAngles == GetViewAngles () && m_lastStateInformation.pressedButtons == m_edict->variables.buttons)
			{
				// Human is AFK now....

				m_AFKTime += AFKCheckInterval;
/*
				#if defined _DEBUG
					if (IsAwayFromKeyboard ())
						PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Human \"%s\" is AFK now!", GetName ().GetData ());
				#endif	// if defined _DEBUG
*/			}
			else
			{
				// Human no more AFK now....

				m_AFKTime = 0.0f;

				m_lastStateInformation.origin = GetOrigin ();
				m_lastStateInformation.viewAngles = GetViewAngles ();
				m_lastStateInformation.pressedButtons = m_edict->variables.buttons;
			}
		}

		inline void CheckWalk (void)
		{
			// Detect if human pushes a walk button....
			if (!IsDucking () && IsOnFloor () ? m_edict->GetSpeedSquared2D () <= HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared : IsOnLadder () && m_edict->GetSpeedSquared () <= HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared)
				m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_RUN;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsFakeClient
		//
		// Description: Checks whether client is bot (fake client).
		//
		// Returns: True if client is bot (fake client), false otherwise.
		//
		inline const bool                   IsFakeClient              (void) const { return false; }

		//
		// Function: IsYaPB
		//
		// Description: Checks whether client is YaPB.
		//
		// Returns: True if client is YaPB, false otherwise.
		//
		inline const bool                   IsYaPB                    (void) const { return false; }

		//
		// Function: IsHuman
		//
		// Description: Checks whether client is human, not a bot.
		//
		// Returns: True if client is human, false otherwise.
		//
		inline const bool                   IsHuman                   (void) const { return true; }

		//
		// Function: IsOtherBot
		//
		// Description: Checks whether client is other bot, not a YaPB.
		//
		// Returns: True if client is other bot, false otherwise.
		//
		inline const bool                   IsOtherBot                (void) const { return false; }

		inline void                         SetSpeaking               (const bool set)       { m_isSpeaking = set; }
		virtual inline const bool           IsSpeaking                (void)           const { return m_isSpeaking; }

		inline const OpenedYaPBMenuInformation_t &GetOpenedYaPBMenu         (void) const { return m_openedYaPBMenu; }

		inline const float                  GetAFKCheckTimestamp      (void) const { return m_AFKCheckTimestamp; }

		//
		// Function: IsAwayFromKeyboard
		//
		// Description: Checks whether human is away from keyboard (AFK).
		//
		// Returns: True if human is away from keyboard (AFK), false otherwise.
		//
		inline const bool                   IsAwayFromKeyboard        (void) const { return GetAwayFromKeyboardTime () >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MaximumAFKTime)->GetValue <float> (); }
		inline const float                  GetAwayFromKeyboardTime   (void) const { return m_AFKTime; }

		inline void                         ExecuteCommand            (const DynamicString &command) const
		{
			// Reliability check.
			InternalAssert (!command.IsEmpty () && command.GetElementNumber () <= HalfLifeEngine::SDK::Constants::MaximumClientCommandLength);

			HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteClientCommand (m_edict, command);
		}

		inline const Math::Angles2D GetModifiedPunchAngles (void) const { return m_edict->GetHumanModifiedPunchAngles (); }

		//
		// Function: IsAdmin
		//
		// Description: Checks whether client is YaPB admin.
		//
		// Returns: True if client is YaPB admin, false otherwise.
		//
		virtual inline const bool           IsAdmin                   (void) const =/*>*/ 0;

		/**
		* @brief Returns the client's language name.
		*
		* @return		Language name.
		*/
		inline const Localizer::LanguageName_t &GetCurrentLanguageName (void) const { return m_localizer->GetCurrentLanguageName (); }
		/**
		* @brief Sets the client's language name.
		*/
//		inline void SetCurrentLanguageName (const Localizer::LanguageName_t &newLanguageName) { ...; }
		inline const Localizer *const GetLocalizer (void) const { return m_localizer; }
/** FROM SOURCE ENGINE
//-----------------------------------------------------------------------------
// Purpose: Convert a damage position in world units to the screen's units
//-----------------------------------------------------------------------------
void CHudDamageIndicator::GetDamagePosition( const Vector &vecDelta, float *flRotation )
{
	float flRadius = 360.0f;

	// Player Data
	Vector playerPosition = MainViewOrigin();
	QAngle playerAngles = MainViewAngles();

	/// VectorVectors(): ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
	Vector forward, right, up(0,0,1);
	AngleVectors (playerAngles, &forward, NULL, NULL );
	forward.z = 0;
	VectorNormalize(forward);
	CrossProduct( up, forward, right );
	float front = DotProduct(vecDelta, forward);
	float side = DotProduct(vecDelta, right);
	float xpos = flRadius * -side;
	float ypos = flRadius * -front;

	// Get the rotation (yaw)
	*flRotation = atan2(xpos, ypos) + M_PI;
	*flRotation *= 180 / M_PI;

	float yawRadians = -(*flRotation) * M_PI / 180.0f;
	float ca = cos( yawRadians );
	float sa = sin( yawRadians );

	// Rotate it around the circle
	xpos = (int)((ScreenWidth() / 2) + (flRadius * sa));
	ypos = (int)((ScreenHeight() / 2) - (flRadius * ca));
}*/
		inline const bool GetScreenPosition (const Math::Vector3D &origin, Math::Vector2D &screenPosition, const float fieldOfViewMultiplier = 1.0f / 1.8f) const
		{
			// Converts world 3D vector to human screen position. (in pixels)
			/// @warning 'm_screenInformation.width' and 'm_screenInformation.height' is not equals to real human screen resolution!

			// Reliability check. (If human is dead...)
			const float fieldOfView (GetSafeFieldOfView ());

			Math::Vector3D aimDirection (origin - GetEyePosition ());

			// Check field of view....
			{
				GetViewAngles ().MakeForwardVector ();

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.GetAngleBetween (aimDirection) > fieldOfView * fieldOfViewMultiplier)
					return false;	// origin is not in human field of view!
			}

			// Pitch, yaw, but without roll
			const Math::Vector3D &newAim (aimDirection.Rotate (-GetViewAngles ().AsAngles2D ()/* - Ignore roll component, since it always equal to 0.0 */));

			if (newAim.x <= 0.0f)
				return false;	// origin is behind us!

			const float screenHalfWidth (m_screenInformation.width * 0.5f), screenHalfHeight (m_screenInformation.height * 0.5f);
			const float number (screenHalfWidth / newAim.x * (120.0f / fieldOfView - 1.0f / 3.0f));

			screenPosition.x = Math::GetClampedValueInRange <float> (screenHalfWidth - number * newAim.y, 0.0f, m_screenInformation.width);
			screenPosition.y = Math::GetClampedValueInRange <float> (screenHalfHeight - number * newAim.z, 0.0f, m_screenInformation.height);

			return true;
		}
		inline const bool GetNormalizedScreenPosition (const Math::Vector3D &origin, Math::Vector2D &screenPosition, const float fieldOfViewMultiplier = 1.0f / 1.8f) const
		{
			// Converts world 3D vector to human screen position. (in range 0.0, ... 1.0)
			// This function is used for HUD messages.

			if (!GetScreenPosition (origin, screenPosition, fieldOfViewMultiplier))
				return false;

			screenPosition.x /= m_screenInformation.width;
			screenPosition.y /= m_screenInformation.height;

			return true;
		}
#if 0
void DrawHudString (int x, int y, bool confont, bool center, const Color <> &color, const char *text)
{
	/// @warning NOT WORK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (confont/*cvar->GetCVarInt(L"confont")*/)
	{
		if (center)
		{
			int length, height;
			(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnDrawConsoleStringLen) (text, &length, &height );
			x -= length / 2;
		}

		(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnDrawSetTextColor) (color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f);

		DrawConsoleText (x, y, text);
	}
	else
	{
		int borderY = m_screenInformation.height - 18;

		if (y < 0 || y > borderY)
			return;

		int drawLen = HalfLifeEngine::Utilities::GetDrawStringLength <unsigned short> (text);

		if (center)
			x -= drawLen / 2;
/*
		if (cvar.text_background)
			tintArea (x-2,y+2,drawLen+4,18,colorList.get(24));
*/
		int borderX = m_screenInformation.width - 11;
		int minX = x;
		int maxX = x + drawLen;
		bool needSingleCheck = (minX < 1 || maxX > borderX);

		if (needSingleCheck)
		{
			for (/* Empty */; *text != '\0'; ++text)
			{
				int next = x + HalfLifeEngine::SDK::Constants::charactersWidths[*text];

				// IMPORTANT NOTE: when drawing admin-mod style charactters
				//                 you MAY NOT provide x/y coordinates that cause drawing
				//                 off screen. This causes HL to crash or just quit
				if( x > 0 && x < borderX )
					(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnDrawCharacter) (x, y, *text, color.red, color.green, color.blue);

				x = next;
			}
		}
		else
		{
			for (/* Empty */; *text != '\0'; ++text)
			{
				int next = x + HalfLifeEngine::SDK::Constants::charactersWidths[*text];

				// IMPORTANT NOTE: when drawing admin-mod style charactters
				//                 you MAY NOT provide x/y coordinates that cause drawing
				//                 off screen. This causes HL to crash or just quit
				(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnDrawCharacter) (x, y, *text, color.red, color.green, color.blue);

				x = next;
			}
		}
	}
}
static void DrawConsoleText (int x, int y, const char *text)
{
	/// @warning NOT WORK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Super handy utility function ;)
	//
	// Will display the 'managed' string on the screen at the given location.
	(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnDrawConsoleString) (x, y, text);
}
#endif	// if 0
		inline void                         Print                     (const HalfLifeEngine::SDK::Constants::PrintType_t type, const char *const message) const
		{
			// Print a text message to this client.

			HalfLifeEngine::Globals::g_halfLifeEngine->ClientPrint (m_edict, type, message);
		}
		inline void                         PrintFormat               (const HalfLifeEngine::SDK::Constants::PrintType_t type, const char *const format, ...) const
		{
			// Print a text message to this client.

			char message[HalfLifeEngine::SDK::Constants::MaximumClientMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			HalfLifeEngine::Globals::g_halfLifeEngine->ClientPrint (m_edict, type, message);
		}
		inline void                         Print                     (const HalfLifeEngine::SDK::Constants::HUDPrintType_t type, const char *const message) const
		{
			// Print a text message to this client.

			HalfLifeEngine::Globals::g_halfLifeEngine->Print (m_edict, type, message);
		}
		inline void                         PrintFormat               (const HalfLifeEngine::SDK::Constants::HUDPrintType_t type, const char *const format, ...) const
		{
			// Print a text message to this client.

			char message[HalfLifeEngine::SDK::Constants::MaximumTextMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			Print (type, message);
		}
		inline void                         DirectorHudMessage        (const Math::Vector2D &coordinates, const Color <> &color, const HalfLifeEngine::SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const message) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DirectorHudMessage (m_edict, coordinates, color, effect, fxTime, holdTime, fadeInTime, fadeOutTime, message);
		}
		inline void                         DirectorHudMessageFormat  (const Math::Vector2D &coordinates, const Color <> &color, const HalfLifeEngine::SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const format, ...) const
		{
			char message[HalfLifeEngine::SDK::Constants::MaximumClientHudMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			DirectorHudMessage (coordinates, color, effect, fxTime, holdTime, fadeInTime, fadeOutTime, message);
		}
		inline void                         HudMessage                (const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const HalfLifeEngine::SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const HalfLifeEngine::SDK::Constants::HUDMessageChannel_t channel, const char *const message) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->HudMessage (m_edict, coordinates, color, effectColor, effect, fxTime, holdTime, fadeInTime, fadeOutTime, channel, message);
		}
		inline void                         HudMessageFormat          (const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const HalfLifeEngine::SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const HalfLifeEngine::SDK::Constants::HUDMessageChannel_t channel, const char *const format, ...) const
		{
			char message[HalfLifeEngine::SDK::Constants::MaximumClientHudMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			HudMessage (coordinates, color, effectColor, effect, fxTime, holdTime, fadeInTime, fadeOutTime, channel, message);
		}
		inline void                HudMessageCentered       (const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const HalfLifeEngine::SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const HalfLifeEngine::SDK::Constants::HUDMessageChannel_t channel, const char *message) const
		{
			/// @todo OPTIMIZE THIS FUNCTION!!!

			unsigned short lineStartIndex (0u), lineEndIndex, biggestLineLength (0u), biggestLineStartIndex (0u), biggestLineEndIndex (0u);
			unsigned char linesNumber (0u);

			for (unsigned short index (0u); message[index] != '\0'; ++index)
			{
				if (message[index] != '\n')
					continue;

				lineEndIndex = index - 1u;	/// @note Ignore current '\n' new line character.

				const unsigned short lineLength (lineEndIndex - lineStartIndex);

				if (biggestLineLength < lineLength)
				{
					biggestLineStartIndex = lineStartIndex;
					biggestLineEndIndex = lineEndIndex;
					biggestLineLength = lineLength;
				}

				lineStartIndex = index + 1u;	/// @note Ignore current '\n' new line character.
			}

			if (linesNumber == 0u)
			{
				linesNumber = 1u;

				biggestLineStartIndex = 0u;
				biggestLineEndIndex = GetStringLength <unsigned short> (message);
//				biggestLineLength = biggestLineEndIndex - biggestLineStartIndex;
			}

			unsigned short drawHalfLength (0u);

			while (++biggestLineStartIndex <= biggestLineEndIndex)
				drawHalfLength += HalfLifeEngine::SDK::Constants::charactersWidths[message[biggestLineStartIndex]];

			HudMessage (Math::Vector2D (Math::GetClampedValueInRange (coordinates.x - drawHalfLength / static_cast <float> (m_screenInformation.width) * 0.5f, 0.0f, 1.0f), Math::GetClampedValueInRange (coordinates.y - HalfLifeEngine::SDK::Constants::charactersHeight * linesNumber / static_cast <float> (m_screenInformation.height) * 0.5f, 0.0f, 1.0f)), color, effectColor, effect, fxTime, holdTime, fadeInTime, fadeOutTime, channel, message);
		}
		inline void                HudMessageCenteredFormat (const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const HalfLifeEngine::SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const HalfLifeEngine::SDK::Constants::HUDMessageChannel_t channel, const char *const format, ...) const
		{
			char message[HalfLifeEngine::SDK::Constants::MaximumClientHudMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			HudMessageCentered (coordinates, color, effectColor, effect, fxTime, holdTime, fadeInTime, fadeOutTime, channel, message);
		}

		#if defined MULTIPLE_DRAW_METHODS
		#	define TYPE virtual
		#else	// if defined MULTIPLE_DRAW_METHODS
		#	define TYPE /* None */
		#endif	// if !defined MULTIPLE_DRAW_METHODS

		TYPE inline void                         DrawLine                  (const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const ColorWithAlpha <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration, const bool depthTest = true) const	// Purpose: Draws a line from one position to another.
		{
			// This function draws a line visible from the client side of the player whose player entity
			// is pointed to by 'client', from the vector location 'source' to the vector location 'destination',
			// which is supposed to last life tenths seconds, and having the color defined by RGB.

			// send this client a packet telling him to draw a beam using the specified parameters
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (m_edict, source, destination, spriteIndex, width, noise, color, brightness, speed, duration);
		}
		TYPE inline void                         DrawCross2D               (const Math::Vector3D &position, const float length, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross2D (m_edict, position, length, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawCross3D               (const Math::Vector3D &position, const Math::Vector3D &mins, const Math::Vector3D &maxs, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given hull size at the given position.

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (m_edict, position, mins, maxs, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawCross3D               (const Math::Vector3D &position, const float length, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (m_edict, position, length, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawCross3DWithoutNegativeZ (const Math::Vector3D &position, const float length, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3DWithoutNegativeZ (m_edict, position, length, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawTriangle              (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const ColorWithAlpha <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			// Purpose: Draw triangle.

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawTriangle (m_edict, point1, point2, point3, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawQuadrangle            (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const Math::Vector3D &point4, const ColorWithAlpha <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			// Purpose: Draw quadrangle.

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawQuadrangle (m_edict, point1, point2, point3, point4, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawHorizontalArrow       (const Math::Vector3D &source, const Math::Vector3D &destination, const ColorWithAlpha <> &color, const unsigned char width, const unsigned char duration, const bool depthTest = true) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawHorizontalArrow (m_edict, source, destination, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), width, 0u, color, 255u, 0u, duration);
		}
		TYPE inline void                         DrawDashedLine            (const Math::Vector3D &source, const Math::Vector3D &destination, const Color <> &color, const unsigned char duration) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawDashedLine (m_edict, source, destination, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawBox                   (const Math::Vector3D &absoluteMins, const Math::Vector3D &absoluteMaxs, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			// This function draws an axial box (i.e, a box whose faces are parallel to the map's axises)
			// visible from the client side of the player whose player entity is pointed to by 'client',
			// encompassing the bounding box defined by the two vector locations 'absoluteMins' and 'absoluteMaxs', for a
			// duration of life tenths seconds, and having the color defined by RGB.

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (m_edict, absoluteMins, absoluteMaxs, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}
		TYPE inline void                         DrawBox                   (const Math::Vector3D points[8u], const ColorWithAlpha <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (m_edict, points, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color, duration);
		}

		#undef TYPE

		inline void                         DisplayMenu               (const char *const text, const HalfLifeEngine::SDK::Constants::Menu::KeyBit_t validSlots) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DisplayMenu (m_edict, text, validSlots);
		}
		void                                DisplayMenu               (const MenuID_t menuID);
		inline void                         UpdateMenu                (const MenuID_t menuID)
		{
			DisplayMenu (menuID);	/// @note THIS IS TEMPORAILY!!!!!

			g_menusManager.Think (this);
		}
		inline void                         CloseMenu                 (void)
		{
			// Reliability check.
			InternalAssert (GetOpenedYaPBMenu ().id != MenuID_None || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹2 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹3 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹4);

			// Reset the currently displaying menu.
			HalfLifeEngine::Globals::g_halfLifeEngine->CloseMenu (m_edict);

			ResetMenu ();
		}
		inline void                         ResetMenu                 (void)
		{
			// Reliability check.
			InternalAssert (GetOpenedYaPBMenu ().id != MenuID_None || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹2 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹3 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹4);

			m_openedYaPBMenu.Reset ();
		}
		inline void                         UpdateOpenedMenu          (void)
		{
			// Update current displaying menu. For example: if human lost his admin rights....

			UpdateMenu (GetOpenedYaPBMenu ().id);
		}
		inline void                         TryUpdateOpenedMenu       (void)
		{
			if (GetOpenedYaPBMenu ().id == MenuID_None || GetOpenedYaPBMenu ().updateTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;

			UpdateOpenedMenu ();

			// Add some delay, - prevent spamming by "ShowMenu" messages for human.
			m_openedYaPBMenu.updateTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f;
		}
		inline void                         DisplayMOTD               (const DynamicString &name, const char *text) const { HalfLifeEngine::Globals::g_halfLifeEngine->DisplayMOTD (m_edict, name, text); }

//		const BotConsoleCommandReturnType_t BotCommandHandler         (const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5, const DynamicString &argument6);

		inline void                             SetVoiceRecieverListenType (const VoiceRecieverListenType_t newType)       { m_voiceRecieverListenType = newType; }
		inline const VoiceRecieverListenType_t &GetVoiceRecieverListenType (void)                                    const { return m_voiceRecieverListenType; }
		inline void                             SetVoiceRecieverBot        (YaPB *const bot)                               { m_voiceRecieverBot = bot; }
		inline       YaPB *const                GetVoiceRecieverBot        (void)                                    const { return m_voiceRecieverBot; }

		inline void DrawCrosshair (void) const { GetEdict ()->privateData->GetBasePlayerPointer ()->m_iHideHUD &= ~HalfLifeEngine::SDK::Constants::HIDEHUD_CROSSHAIR; }
		inline void HideCrosshair (void) const { GetEdict ()->privateData->GetBasePlayerPointer ()->m_iHideHUD |= HalfLifeEngine::SDK::Constants::HIDEHUD_CROSSHAIR; }

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline void Think                 (void)
		{
			SetSpeaking (false);

			TryUpdateOpenedMenu ();

			// Call this function in base class....
			Client::Think ();
		}
		virtual inline void AliveThink            (void)
		{
			CheckAFK ();

			// Call this function in base class....
			Client::AliveThink ();
		}
		virtual inline void Killed                (const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			// Is called when the client is killed.

			if (GetOpenedYaPBMenu ().id == MenuID_BotCommand)
				CloseMenu ();	// reset menu displayed

			// Call this function in base class....
			Client::Killed (killer);
		}
		inline void                                 UpdateData            (void) const
		{
			/// @todo DO SOMETHING HERE!
		}
		virtual inline void                         CurrentWeaponChanged  (const bool state, const HalfLifeEngine::SDK::Constants::WeaponID_t newWeaponIndex, const char newAmmoInClip)
		{
			// Call this function in base class....
			Client::CurrentWeaponChanged (state, newWeaponIndex, newAmmoInClip);

			// Hide crosshair again....
			if (g_server->GetNavigationMeshManager ().IsEditing ()/*! && g_server->GetNavigationMeshManager ().GetEditor () == this / @todo !!!*/)
				HideCrosshair ();
		}
		virtual const HandleExecutedCommandResult_t HandleExecutedCommand (const DynamicString &command, const DynamicString &argument1);
		void                                        HandleYaPBMenus       (const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection);
		virtual inline void OnDisplayMenu (const HalfLifeEngine::SDK::Constants::Menu::KeyBit_t validSlots, const char lifeTime, const DynamicString &string)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\"%s\"::OnDisplayMenu(): take menu, m_information: [validSlots=%u, lifeTime=%d, string=\"%s\"].\n", GetName ().GetData (), validSlots, lifeTime, string.GetData ());
		}
		virtual inline void OnCloseMenu (void)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\"%s\"::OnCloseMenu(): close menu.\n", GetName ().GetData ());
		}
		virtual inline void UserInfoChanged (const DynamicString &infoBuffer)
		{
			// Called on client user info changed.

			const DynamicString potentionalNewLanguage (HalfLifeEngine::Globals::g_halfLifeEngine->GetInfoKeyValue (infoBuffer, CLIENT_USER_INFO_LANGUAGE_KEY));

//			AddLogEntry (true, LogLevel_Default, false, "Human::UserInfoChanged(): called for human \"%s\", infoBuffer: \"%s\", potentionalNewLanguage: \"%s\".", GetName ().GetData (), infoBuffer.GetData (), potentionalNewLanguage.GetData ());

			if (potentionalNewLanguage.IsEmpty () || !potentionalNewLanguage.IsAlpha ())
				return;

			if (GetCurrentLanguageName ().CompareWithoutCaseCheck (potentionalNewLanguage) != 0)
				ChangeLanguage (potentionalNewLanguage);
		}
};

inline       Human *const Client::GetHumanPointer (void)
{
	// Reliability check.
	InternalAssert (IsHuman ());

	return static_cast <Human *const> (this);
}
inline const Human *const Client::GetHumanPointer (void) const
{
	// Reliability check.
	InternalAssert (IsHuman ());

	return static_cast <const Human *const> (this);
}

/*virtual */inline void MenuManager::Menu::Item::AppendToMenu (Human *const human, DynamicString &menuText, const ItemID_t index) const
{
	if (HasAccess (human))
	{
		menuText.AppendFormat (("\n%s%u\\w. " + human->GetLocalizer ()->GetLocalizedString (GetLocalizedTextID ())).GetData (), HalfLifeEngine::SDK::Constants::Menu::colors[GetColorID ()], (index + 1u) % ItemID_Total);

		human->m_openedYaPBMenu.keys |= BIT (index);
	}
	else
		menuText.AppendFormat ("\n\\d%u\\w. \\d" + human->GetLocalizer ()->GetLocalizedString (GetLocalizedTextID ()), (index + 1u) % ItemID_Total);
}
/*virtual */inline void MenuManager::Menu::Item::PlaySelectionSound (Human *const human) const
{
	// Stops others from hearing menu sounds....
	human->ExecuteCommand ("speak player/geiger1\n");
}
/*virtual */inline void MenuManager::Menu::Item::CloseMenu (Human *const human) { human->CloseMenu (); }
/*virtual */inline void MenuManager::Menu::Item::RedrawMenu (Human *const human)
{
	human->m_openedYaPBMenu.keys = HalfLifeEngine::SDK::Constants::Menu::KeyBit_None;	// Reset only keys....

	g_menusManager.Draw (human);	// Redraw menu again....
}

inline void MenuManager::Menu::Item_Next::HandleSelection (Human *const human)
{
	// Reliability check.
	InternalAssert (human->GetOpenedYaPBMenu ().page + 1u < g_menusManager.GetMenu (human->GetOpenedYaPBMenu ().id)->m_pages.GetElementNumber ());

	human->m_openedYaPBMenu.SetNextPage ();

	g_menusManager.Draw (human);
}

inline void MenuManager::Menu::Item_Previous::HandleSelection (Human *const human)
{
	human->m_openedYaPBMenu.SetPreviousMenu ();

	g_menusManager.Draw (human);
}

/*virtual */inline void MenuManager::Menu::Draw (Human *const human)
{
	// Reliability check.
	InternalAssert (!m_pages.IsEmpty ());

	// Update current page and previous menus. Actual for kick menu.
	for (/* Empty */; human->GetOpenedYaPBMenu ().page >= m_pages.GetElementNumber (); --human->m_openedYaPBMenu.page)
		human->m_openedYaPBMenu.previousMenus.Pop ();

	const PageID_t page (static_cast <PageID_t> (human->GetOpenedYaPBMenu ().page));

	// Reliability check.
	InternalAssert (page < m_pages.GetElementNumber ());

	DynamicString text;

	// Title.
	if (m_pages.GetElementNumber () == 1u)
		text = human->GetLocalizer ()->GetLocalizedString (GetTitleLocalizedTextID ()) + ":\n";	// Menu without pages.
	else
		text.AssignFormat (human->GetLocalizer ()->GetLocalizedString (GetTitleLocalizedTextID ()) + " (%u/%u):\n", page + 1u, m_pages.GetElementNumber ());

	// Items.
	for (ItemID_t itemIndex (0u); itemIndex < m_pages[page].GetElementNumber (); ++itemIndex)
	{
		/// @note I KNOW THIS IS PRETTY UGLY BUT....
		{
			const Localizer::LocalizedStringID_t localizedStringID (m_pages[page][itemIndex]->GetLocalizedTextID ());
			const bool isPreviousItemWasEmpty (text[text.GetElementNumber () - GET_STATIC_STRING_LENGTH ("\n\n")] == '\n');

			if (!isPreviousItemWasEmpty && (localizedStringID == Localizer::LocalizedStringID_Menu_Item_Next || (localizedStringID == Localizer::LocalizedStringID_Menu_Item_Previous && itemIndex > 0u && m_pages[page][itemIndex - 1u]->GetLocalizedTextID () != Localizer::LocalizedStringID_Menu_Item_Next) || localizedStringID == Localizer::LocalizedStringID_Menu_Item_Exit))
				text += '\n';	// Separate next|previous and exit items out of other items.
		}

		m_pages[page][itemIndex]->AppendToMenu (human, text, itemIndex);
//		text.AppendFormat ("\n%s%u\\w. " + human->GetLocalizer ()->GetLocalizedString (m_pages[page][itemIndex]->GetLocalizedTextID ()), HalfLifeEngine::SDK::Constants::Menu::colors[m_pages[page][itemIndex]->GetColorID ()], itemIndex + 1u);
	}

	human->DisplayMenu (text, human->GetOpenedYaPBMenu ().keys);
}

/*virtual */inline void MenuManager::Menu::Close (Human *const human) const
{
	human->CloseMenu ();
}

/*virtual */inline void MenuManager::Menu::HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection)
{
	const PageID_t page (static_cast <PageID_t> (human->GetOpenedYaPBMenu ().page));

	// Reliability checks.
	InternalAssert (page < m_pages.GetElementNumber ());
	InternalAssert (selection < m_pages[page].GetElementNumber ());

	m_pages[page][selection]->PlaySelectionSound (human);	// Stops others from hearing menu sounds....
	m_pages[page][selection]->HandleSelection (human);
}

inline void MenuManager::Menu_YaPBMain::Item_Control::HandleSelection (Human *const human)
{
//	fillCommand = false;

	human->DisplayMenu (MenuID_BotControl);
}
/*
inline void MenuManager::Menu_YaPBMain::Draw (Human *const human)
{
	human->DisplayMenu
	(
		human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Bot_Main_Title) + ":\n\n" +	// Title.

		"\\r1\\w. " + human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Bot_Main_Item_Control) + '\n' +		// First item.
		"\\r2\\w. " + human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Bot_Main_Item_Features) + "\n\n" +	// Second item.

		"\\r3\\w. " + human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Bot_Main_Item_FillServer) + '\n' +	// Third item.
		"\\r4\\w. " + human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Bot_Main_Item_EndRound) + "\n\n" +	// Fourth item.

		"\\r0\\w. " + human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Item_Exit),							// Fifth item.

		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0
	);
}
*/
inline void MenuManager::Draw (Human *const human) { GetMenu (human->GetOpenedYaPBMenu ().id)->Draw (human); }
inline void MenuManager::HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection)
{
	GetMenu (human->GetOpenedYaPBMenu ().id)->HandleSelection (human, selection);
}
inline void MenuManager::Think (Human *const human)
{
	if (human->GetOpenedYaPBMenu ().id != MenuID_None)
		GetMenu (human->GetOpenedYaPBMenu ().id)->Think (human);
}

#endif	// ifndef HUMAN_INCLUDED