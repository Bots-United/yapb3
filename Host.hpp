//
// Copyright � 2003-2010, by YaPB Development Team. All rights reserved.
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
// Host.hpp
//
// Class: Host
//
// Description: Contains the 'Host' class Function Prototypes for listen server host edict.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HOST_INCLUDED
#define HOST_INCLUDED

class Host : public Human
{
	friend ClientManager;

	DECLARE_CLASS_WITH_BASE (Human, Host);

	//
	// Group: Private members.
	//
	private:
		struct WaypointEdit_t
		{
			bool isOn;			// is drawing waypoints ON for this player
			bool automatic;		// auto-waypointing stuff
			bool changed;		// is waypoints changed?
			bool withNoClip;	// is waypoint editing needed no clip cheat?

			inline WaypointEdit_t (void) : isOn (false), automatic (false), changed (true), withNoClip (false) { /* VOID */ }
		} m_waypointing;

		class PressedKeySet : public ElementSet <bool, HalfLifeEngine::SDK::Constants::FROM_SDL::SDLKey, unsigned short, HalfLifeEngine::SDK::Constants::FROM_SDL::SDLK_LAST>
		{
			public:
				inline PressedKeySet (void) { Reset (); }

			public:
				inline void Reset (void)
				{
					for (IndexType_t index (0u); index < ElementsNumber; ++index)
						m_set[index] = false;
				}
		};
		PressedKeySet m_pressedKeys;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Host (HalfLifeEngine::SDK::Classes::Edict *const host) : Human (host)
		{
			HalfLifeEngine::SDK::Structures::ScreenInformation_t screenInformation = {sizeof (HalfLifeEngine::SDK::Structures::ScreenInformation_t)/*, ...*/};

			(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnGetScreenInfo) (&screenInformation);

			// Convert structures....
			{
				m_screenInformation.width = static_cast <unsigned short> (screenInformation.width);
				m_screenInformation.height = static_cast <unsigned short> (screenInformation.height);
				m_screenInformation.charactersHeight = static_cast <unsigned char> (screenInformation.charactersHeight);

				for (unsigned short index (0u); index < MximumCharactersNumber; ++index)
					m_screenInformation.charactersWidths[index] = static_cast <unsigned char> (screenInformation.charactersWidths[index]);
			}
		}

		virtual inline ~Host (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_Host; }

class WelcomeSentencesManager
{
	public:
		#define DEFAULT_WELCOME_SENTENCES_FILENAME "/Addons/YaPB/Configs/Welcome sentences.db"

		typedef DynamicArray <DynamicString, unsigned char> WelcomeSentencesArray_t;

	private:
		WelcomeSentencesArray_t m_welcomeSentences;

	public:
		inline const WelcomeSentencesArray_t &GetArray (void) const { return m_welcomeSentences; }

		inline const bool LoadFromFile (const BaseFile &file = STDIOFile (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + DEFAULT_WELCOME_SENTENCES_FILENAME))
		{
			RemoveAll ();

			// Reliability check.
			if (!file.IsValid ())
			{
				AddLogEntry (true, LogLevel_Warning, false, "Unable to open welcome sentences database file \"%s\"!", file.GetPathName ().GetData ());

				return false;
			}

			const unsigned char maximumWelcomeSentenceLength (128u);
			DynamicString line;
			unsigned short lineIndex (0u);

			// for each line in the file...
			while (file.ReadLine (line, maximumWelcomeSentenceLength))
			{
				++lineIndex;

				// trim all the blanks or linefeeds
				line.Trim ();

				// skip all commented or empty lines
				if (IsEmptyOrCommentedFileBufferLine (line))
					continue;

				if (m_welcomeSentences.IsMember (line.MakeLower ()))
				{
					AddLogEntry (true, LogLevel_Warning, false, "Duplicate welcome sentence \"%s\" in a file \"%s\", on a line %u!", line.GetData (), file.GetPathName ().GetData (), lineIndex);

					continue;
				}

				m_welcomeSentences += line;
			}

			return m_welcomeSentences.IsEmpty () == false;
		}
		inline const bool AppendFromFile (const BaseFile &file = STDIOFile (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + DEFAULT_WELCOME_SENTENCES_FILENAME))
		{
			// Reliability check.
			if (!file.IsValid ())
				return false;

			const unsigned char maximumWelcomeSentenceLength (128u);
			DynamicString line;
			const WelcomeSentencesArray_t::IndexType_t oldElementNumber (m_welcomeSentences.GetElementNumber ());

			// for each line in the file...
			while (file.ReadLine (line, maximumWelcomeSentenceLength))
			{
				// trim all the blanks or linefeeds
				line.Trim ();

				// skip all commented or empty lines
				if (IsEmptyOrCommentedFileBufferLine (line))
					continue;

				if (m_welcomeSentences.IsMember (line.MakeLower ()))
					continue;

				m_welcomeSentences += line;
			}

			return m_welcomeSentences.GetElementNumber () > oldElementNumber;
		}
		inline void RemoveAll (void) { m_welcomeSentences.RemoveAll (); }
		inline void SetDefault (void)
		{
			RemoveAll ();

			m_welcomeSentences += "hello user, communication is acquired";
			m_welcomeSentences += "your presence is acknowledged";
			m_welcomeSentences += "high man, your in command now";
			m_welcomeSentences += "blast your hostile for good";
			m_welcomeSentences += "high man, kill some idiot here";
			m_welcomeSentences += "is there a doctor in the area";
			m_welcomeSentences += "warning, experimental materials detected";
			m_welcomeSentences += "high amigo, shoot some but";
			m_welcomeSentences += "attention, mass hours of run detected";
			m_welcomeSentences += "time for some bad ass explosion";
			m_welcomeSentences += "bad ass son of a breach device activated";
			m_welcomeSentences += "high, do not question this great service";
			m_welcomeSentences += "engine is operative, hello and goodbye";
			m_welcomeSentences += "high amigo, your administration has been great last day";
			m_welcomeSentences += "attention, expect experimental armed hostile presence";
			m_welcomeSentences += "warning, medical attention required";
			m_welcomeSentences += "check check, test, mike check, talk device is activated";
			m_welcomeSentences += "good, day mister, your administration is now acknowledged";
			m_welcomeSentences += "hello pal, at your service";
			m_welcomeSentences += "high man, at your command";
			m_welcomeSentences += "all command access granted, over and out";
		}
};

		inline void PrintWelcomeMessage (void)
		{
			// The purpose of this function, is to send quick welcome message, to the listen server entity.

			// this function sends a welcome message to incoming clients, under the form of a white text
			// that fades in, holds a few seconds and then fades out in the center of the screen. A test
			// is made that cancels the function if the client to send the message to is a bot, as sending
			// such messages (HUD text) to bots would crash the server, the bots having no client DLL. I
			// know we're supposed to have the checks that fit well in engine.cpp for this, but never
			// trust anyone but yourself, mama said. Ah also, a little sound is played on the client side
			// along with the message, and BTW, this function behaves a little particularly when it's my
			// birthday... :) :) :)

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Synth)->GetValue <bool> ())
			{
				// play welcome sound on this client

				WelcomeSentencesManager welcomeSentences;

				welcomeSentences.SetDefault ();
				welcomeSentences.AppendFromFile ();

				// scare host client even more with a spoken message :P
				HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommandFormat ("speak \"%s\"\n", welcomeSentences.GetArray ().GetRandomElement ().GetData ());
			}

			// is today my birthday? :D
			if (IsMyBirthday ())
			{
				// w00t, it's my birthday!!!

				// send the "special birthday" welcome message to this client
				HudMessage
				(
					Math::Vector2D (-1.0f, -1.0f),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u)),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 5.0f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

					"+------------------------------------------------------------------------------------------------------------------------+\n"
						"\tServer is running with " PRODUCT_SHORT_NAME " v" PRODUCT_VERSION " (Build: " PRODUCT_BUILD "), {" PRODUCT_DATE "}, © " PRODUCT_COPYRIGHT_YEAR ".\n"
						"\tDeveloped by " PRODUCT_AUTHOR "\n"
						"\n"
						"\n"
						"Today is 30 october - it's the author's birthday!\n"
						"Happy Birthday Immor†al*BLG!!!\n"
					"+------------------------------------------------------------------------------------------------------------------------+"
				);
			}
			else if (IsNewYear ())
			{
				// send the welcome message to this client
				HudMessage
				(
					Math::Vector2D (-1.0f, -1.0f),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u)),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 5.0f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

					"+------------------------------------------------------------------------------------------------------------------------+\n"
						"\tServer is running with " PRODUCT_SHORT_NAME " v" PRODUCT_VERSION " (Build: " PRODUCT_BUILD "), {" PRODUCT_DATE "}, © " PRODUCT_COPYRIGHT_YEAR ".\n"
						"\tDeveloped by " PRODUCT_AUTHOR "\n"
						"\n"
						"\n"
						"Happy New Year!!!\n"
					"+------------------------------------------------------------------------------------------------------------------------+"
				);
			}
			else
			{
				// looks like it's just a normal, boring day

				HudMessage
				(
					Math::Vector2D (-1.0f, -1.0f),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u)),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 5.0f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

					"+------------------------------------------------------------------------------------------------------------------------+\n"
						"\tServer is running with " PRODUCT_SHORT_NAME " v" PRODUCT_VERSION " (Build: " PRODUCT_BUILD "), {" PRODUCT_DATE "}, © " PRODUCT_COPYRIGHT_YEAR ".\n"
						"\tDeveloped by " PRODUCT_AUTHOR "\n"
					"+------------------------------------------------------------------------------------------------------------------------+"
				);
			}
		}
		inline void CheckWelcomeMessage (void)
		{
			static float welcomeMessageReceiveTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (2.5f, 6.5f));

			// Receive welcome message in four seconds after game has commencing
			if (welcomeMessageReceiveTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				if (IsAwayFromKeyboard ())
					welcomeMessageReceiveTime = GetAFKCheckTimestamp ();
				else
				{
					PrintWelcomeMessage ();

					welcomeMessageReceiveTime = Math::MaximumFloatValue;
				}
			}
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsHost
		//
		// Description: Checks whether client is host.
		//
		// Returns: True if client is host, false otherwise.
		//
		inline const bool          IsHost           (void) const { return true; }

		//
		// Function: IsPlayer
		//
		// Description: Checks whether client is player.
		//
		// Returns: True if client is player, false otherwise.
		//
		inline const bool          IsPlayer         (void) const { return false; }

		//
		// Function: IsAdmin
		//
		// Description: Checks whether client is YaPB admin.
		//
		// Returns: True if client is YaPB admin, false otherwise.
		//
		inline const bool          IsAdmin          (void) const { return true; }

		#if defined MULTIPLE_DRAW_METHODS
			inline void                         DrawLine                  (const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const ColorWithAlpha <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration, const bool depthTest = true) const	// Purpose: Draws a line from one position to another.
			{
				// This function draws a line visible from the client side of the player whose player entity
				// is pointed to by 'client', from the vector location 'source' to the vector location 'destination',
				// which is supposed to last life tenths seconds, and having the color defined by RGB.

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawLine (source, destination, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawLine (source, destination, spriteIndex, width, noise, color, brightness, speed, duration, depthTest);
			}
			inline void                         DrawCross2D               (const Math::Vector3D &position, const float length, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawCross2D (position, length, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawCross2D (position, length, color, duration, brightness, width, depthTest);
			}
			inline void                         DrawCross3D               (const Math::Vector3D &position, const Math::Vector3D &mins, const Math::Vector3D &maxs, const Color <> &color, const unsigned char duration) const
			{
				// Purpose: Draw a colored 3D cross of the given hull size at the given position.

//				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
//					/// @todo !!!
//				else
					// Call this function in base class....
					BaseClass::DrawCross3D (position, mins, maxs, color, duration);
			}
			inline void                         DrawCross3D               (const Math::Vector3D &position, const float length, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				// Purpose: Draw a colored 3D cross of the given size at the given position.

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawCross3D (position, length, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawCross3D (position, length, color, duration, brightness, width, depthTest);
			}
			inline void                         DrawCross3DWithoutNegativeZ (const Math::Vector3D &position, const float length, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				// Purpose: Draw a colored 3D cross of the given size at the given position.

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawCross3DWithoutNegativeZ (position, length, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawCross3DWithoutNegativeZ (position, length, color, duration, brightness, width, depthTest);
			}
			inline void                         DrawTriangle              (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const ColorWithAlpha <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				// Purpose: Draw triangle.

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawTriangle (point1, point2, point3, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawTriangle (point1, point2, point3, color, duration, brightness, width, depthTest);
			}
			inline void                         DrawQuadrangle            (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const Math::Vector3D &point4, const ColorWithAlpha <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				// Purpose: Draw quadrangle.

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawQuadrangle (point1, point2, point3, point4, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawQuadrangle (point1, point2, point3, point4, color, duration);
			}
			inline void                         DrawHorizontalArrow       (const Math::Vector3D &source, const Math::Vector3D &destination, const ColorWithAlpha <> &color, const unsigned char width, const unsigned char duration, const bool depthTest = true) const
			{
				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawHorizontalArrow (source, destination, color, 255u, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawHorizontalArrow (source, destination, color, width, duration, depthTest);
			}
			inline void                         DrawDashedLine            (const Math::Vector3D &source, const Math::Vector3D &destination, const Color <> &color, const unsigned char duration) const
			{
//				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
//					/// @todo !!!
//				else
					// Call this function in base class....
					BaseClass::DrawDashedLine (source, destination, color, duration);
			}
			inline void                         DrawBox                   (const Math::Vector3D &absoluteMins, const Math::Vector3D &absoluteMaxs, const Color <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				// This function draws an axial box (i.e, a box whose faces are parallel to the map's axises)
				// visible from the client side of the player whose player entity is pointed to by 'client',
				// encompassing the bounding box defined by the two vector locations 'absoluteMins' and 'absoluteMaxs', for a
				// duration of life tenths seconds, and having the color defined by RGB.

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawBox (absoluteMins, absoluteMaxs, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawBox (absoluteMins, absoluteMaxs, color, duration, brightness, width, depthTest);
			}
			inline void                         DrawBox                   (const Math::Vector3D points[8u], const ColorWithAlpha <> &color, const unsigned char duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) const
			{
				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
					g_drawInterface->DrawBox (points, color, brightness, width, depthTest);
				else
					// Call this function in base class....
					BaseClass::DrawBox (points, color, duration, brightness, width, depthTest);
			}
		#endif	// if defined MULTIPLE_DRAW_METHODS

		inline void OnKeyEvent (const HalfLifeEngine::SDK::Constants::FROM_SDL::SDLKey key, const bool down) { m_pressedKeys[key] = down; }
		inline const PressedKeySet &GetPressedKeys (void) const { return m_pressedKeys; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void AliveThink (void)
		{
			// is this player alive?
			if (m_waypointing.withNoClip)
				m_edict->variables.moveType = HalfLifeEngine::SDK::Constants::MOVETYPE_NOCLIP;	// keep the clipping mode enabled, or it can be turned off after new round has started

			// Call this function in base class....
			Human::AliveThink ();

			CheckWelcomeMessage ();
/*
		if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
		{
			static float m_frameIntervalTime, m_lastThinkTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
//			static YaPB::Msec m_msec;
			static unsigned char m_msec;
			{
				m_frameIntervalTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - m_lastThinkTime;	// calculate frame interval time
				m_lastThinkTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();	// update last think interval for last calculation method

//				m_msec.Think (m_frameIntervalTime);	// Calculate the msec value.
				m_msec = static_cast <unsigned char> (m_frameIntervalTime * 1000.0f);	// Calculate the msec value.
			}

			// ask the engine to do the fake client movement on server
			m_edict->RunPlayerMove (m_edict->variables.modelAngles, 250.0f, 0.0f, m_msec);
		}
*/		}
};

inline       Host *const Client::GetHostPointer (void)
{
	// Reliability check.
	InternalAssert (IsHost ());

	return static_cast <Host *const> (this);
}
inline const Host *const Client::GetHostPointer (void) const
{
	// Reliability check.
	InternalAssert (IsHost ());

	return static_cast <const Host *const> (this);
}

#endif	// ifndef HOST_INCLUDED