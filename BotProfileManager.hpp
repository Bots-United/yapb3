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
// BotProfileManager.hpp
//
// Class: ProfileManager
//
// Description: YaPB profile manager.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_PROFILE_MANAGER_INCLUDED
#define BOT_PROFILE_MANAGER_INCLUDED

class ProfileManager
{
	//
	// Group: Type definitions.
	//
	public:
		enum { MaximumProfilesNumber = 100u };

		typedef StaticArray <YaPB::Profile, unsigned char, MaximumProfilesNumber> ProfilesArray_t;

	//
	// Group: Private members.
	//
	private:
		#define PROFILE_MANAGER_CONFIG_FILENAME                "profiles"
		#define PROFILE_MANAGER_CONFIG_FILENAME_EXTANTION      "cfg"
		#define PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION PROFILE_MANAGER_CONFIG_FILENAME "." PROFILE_MANAGER_CONFIG_FILENAME_EXTANTION

		YaPB::Profile   m_defaultProfile;	// Default bot profile in case configuration file missed, wrong, or empty.

		ProfilesArray_t m_profiles;			// Array of known bot profiles, loaded from configuration file.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ProfileManager (void)
		{
			// Load bot profiles from the disk....

			LoadBotProfiles ();
		}

	//
	// Group: Private functions.
	//
	private:
		inline const bool IsBotNameExists (const DynamicString &name) const
		{
			// Reliability check.
			if (m_profiles.IsEmpty ())
				return false;	// No match.

			for (ProfilesArray_t::IndexType_t index (0u); index < m_profiles.GetElementNumber (); ++index)
				if (m_profiles[index].name == name)
					return true;	// Found a match.

			return false;	// No match.
		}

		inline void AppendDefaultProfile (void)
		{
			m_profiles += m_defaultProfile;

			YaPB::Profile &profile (m_profiles.GetLastElement ());

			InternalAssert (profile.m_iProfileId == 0u);
			profile.m_iProfileId = m_profiles.GetElementNumber ();

			InternalAssert (profile.m_HAL == NULL);
//			profile.m_HAL = NULL;	// Reset HAL pointer....
		}

	//
	// Group: Functions.
	//
	public:
		void        LoadBotProfiles (void);
		inline void FreeBotProfiles (void)
		{
			// Free all the memory allocated for bot profiles.

			m_profiles.RemoveAll ();
		}

		inline const ProfilesArray_t &GetProfiles (void) const { return m_profiles; }

		YaPB::Profile *const PickBotProfile (const unsigned char skill = 0u);
		YaPB::Profile *const PickBotProfile (const DynamicString &botName);

		void PrintBotProfiles (void) const;

		#if defined _DEBUG
			inline const bool IsValidProfile (const YaPB::Profile *const profile) const
			{
				return profile != NULL && m_profiles.IsInside (profile);
			}
		#endif	// if defined _DEBUG
};

#endif	// ifndef BOT_PROFILE_MANAGER_INCLUDED