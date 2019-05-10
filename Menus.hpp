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
// Menus.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MENUS_INCLUDED
#define MENUS_INCLUDED

/*class MenuManager;
{
	//
	// Group: Constants/Definitions.
	//
	public:*/
		// YaPB menu ID's
		enum MenuID_t
		{
			MenuID_None = -1,

			// Bot ----------------------------------------------------------------------------------
			MenuID_YaPBMain,							// main YaPB menu
			MenuID_BotFeatures,							// bot features menu
			MenuID_BotControl,							// bot control menu
			MenuID_BotWeaponModeSelect,					// bot weapon mode select menu
			MenuID_BotPersonalitySelect,				// bot personality select menu
			MenuID_BotSkillSelect,						// bot skill select menu
			MenuID_BotTeamSelect,						// bot team select menu
			MenuID_BotTerroristModelSelect,				// bot terrorist model select menu
			MenuID_BotCounterModelSelect,				// bot counter-terrorist model select menu
			MenuID_BotKickMenu¹1,						// bot kick menu (page 1)
			MenuID_BotKickMenu¹2,						// bot kick menu (page 2)
			MenuID_BotKickMenu¹3,						// bot kick menu (page 3)
			MenuID_BotKickMenu¹4,						// bot kick menu (page 4)
			MenuID_BotCommand,							// bot command menu

			// Waypoints ---------------------------------------------------------------------------
			MenuID_WaypointMain¹1,						// waypoint main menu (page 1)
			MenuID_WaypointMain¹2,						// waypoint main menu (page 2)
			MenuID_WaypointRadiusSelect,				// waypoint radius select menu
			MenuID_WaypointAdd,							// waypoint add menu
			MenuID_WaypointFlagSet,						// waypoint flag set menu
			MenuID_WaypointAutoPathMaximumDistanceSet,	// waypoint auto-path maximum distance set
			MenuID_WaypointPathConnectionsSet,			// waypoint path connections set

			MenuID_Total								// YaPB menus total
		};

		// structure for menus
		struct Menu_t
		{
			HalfLifeEngine::SDK::Constants::Menu::KeyBit_t validSlots;	// Ored together bits for valid keys.
			DynamicString                                  menuText;	// Pointer to actual string.
		};
/*
	//
	// Group: Private members.
	//
	private:
		extern Menu_t g_menus[MenuID_Total];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline MenuManager (void)
		{
			//
		}

	//
	// Group: Functions.
	//
	public:
		
};

class BlankItem
{
	private:
		DynamicString m_text;
		bool          m_num;

	public:
		inline BlankItem (void) : m_num (false) { /* VOID *//* }
		inline BlankItem (const DynamicString &text, const bool num = false) : m_text (text), m_num (num) { /* VOID *//* }
		inline BlankItem (const BlankItem &source) : m_text (source.m_text), m_num (source.m_num) { /* VOID *//* }

	public:
		inline BlankItem &operator = (const BlankItem &source)
		{
			m_text = source.m_text;
			m_num = source.m_num;

			return *this;
		}

	public:
		// is this text instead of a blank
		inline const bool IsText (void) const { return !IsBlank (); }

		// is this a blank instead of text
		inline const bool IsBlank (void) const { return m_text.IsEmpty (); }

		// does this item take up a number
		inline const bool EatNumber (void) const { return m_num; }

		// the text this item is to display
		inline const DynamicString &GetDisplay (void) const { return m_text; }

		// sets this item to use a blank
		inline void SetBlank (void) { m_text.MakeEmpty (); }

		// sets this item to display text
		inline void SetText (const DynamicString &text) { m_text = text;  }

		// sets whether or not this item takes up a line
		inline void SetEatNumber (const bool val) { m_num = val; }
};

class Menu
{
	friend MenuManager;

	//
	// Group: Private members.
	//
	private:
		struct Item_t
		{
/*			enum Access_t
			{
				Access_All,

				Access_Admin,
				Access_Player,
			};
*//*
			enum State_t
			{
				State_Ignored = -1,

				State_Disabled,
				State_Enabled
			};

			typedef const State_t (*Callback_t) (Human *const human, const MenuID_t id, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t item);

			Localizer::LocalizedStringID_t          textID;
			DynamicString                           consoleCommandName;
			unsigned short                          accessLevel;
			Callback_t                              handlerFunction;
			DynamicArray <BlankItem, unsigned char> blanks;

			inline Item_t (const Localizer::LocalizedStringID_t inputTextID, const DynamicString &inputConsoleCommandName, const unsigned short inputAccessLevel = 0u, const Callback_t inputHandlerFunction = NULL) :
				textID (inputTextID),
				consoleCommandName (inputConsoleCommandName),
				accessLevel (inputAccessLevel),
				handlerFunction (inputHandlerFunction)
			{ /* VOID *//* }
		};

		Localizer::LocalizedStringID_t                  m_titleID;
		DynamicArray <Item_t *, unsigned char>          m_items;
		HalfLifeEngine::SDK::Constants::Menu::ColorID_t m_titleColorID;
		HalfLifeEngine::SDK::Constants::Menu::ColorID_t m_itemColorID;
		DynamicString                                   m_soundName;			// Playing sound on menu open

		unsigned char                                   m_maximumItemsPerPage;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Menu (const Localizer::LocalizedStringID_t titleID, const HalfLifeEngine::SDK::Constants::Menu::ColorID_t titleColorID = HalfLifeEngine::SDK::Constants::Menu::ColorID_Yelow, const HalfLifeEngine::SDK::Constants::Menu::ColorID_t itemColorID = HalfLifeEngine::SDK::Constants::Menu::ColorID_Red, const DynamicString &soundName = "player/geiger1") :
			m_titleID (titleID),
			m_titleColorID (titleColorID),
			m_itemColorID (itemColorID),
			m_soundName (soundName)
		{ /* VOID *//* }
		inline ~Menu (void)
		{
			m_items.DeleteAll ();
		}

	//
	// Group: Private functions.
	//
	private:
		const char *const PrepareText (const MenuID_t id, Human *const human, const HalfLifeEngine::SDK::Constants::Menu::PageIndex_t page, HalfLifeEngine::SDK::Constants::Menu::KeyBit_t &keys);

	//
	// Group: Functions.
	//
	public:
		inline const unsigned char GetItemsNumber (void) const { return m_items.GetElementNumber (); }
		inline const unsigned char GetPagesNumber (void) const
		{
			if (m_maximumItemsPerPage == 0u)
				return 1u;

			return GetItemsNumber () % m_maximumItemsPerPage > 0u ? GetItemsNumber () / m_maximumItemsPerPage + 1u : GetItemsNumber () / m_maximumItemsPerPage;
		}

		inline void AddItem (const Localizer::LocalizedStringID_t textID, const DynamicString &consoleCommandName, const unsigned short accessLevel = 0u, const Item_t::Callback_t handlerFunction = NULL)
		{
			Item_t *const newItem (new Item_t (textID, consoleCommandName, accessLevel, handlerFunction));

			// Reliability check.
			if (newItem == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating menu item!", sizeof (Item_t));

				return;
			}

			m_items += newItem;
		}

		/*inline *//*void Display (const MenuID_t id, Human *const human, const HalfLifeEngine::SDK::Constants::Menu::PageIndex_t page);
/*		{
			HalfLifeEngine::SDK::Constants::Menu::KeyBit_t keys;

			human->DisplayMenu (PrepareText (id, human, page, keys), keys);

			// Stops others from hearing menu sounds....
			human->ExecuteCommand ("speak " + m_soundName + '\n');
		}*//*
};*/
class MenuManager
{
class Menu
{
	public:
		class Item;

		DECLARE_TYPED_ENUMERATION (unsigned char, ItemID_t)
		{
			ItemID_Total = HalfLifeEngine::SDK::Constants::Menu::Key_Total - 1u	// This fixed by Counter-Strike.
		};
		typedef StaticArray <Item *, ItemID_t, ItemID_Total> ItemArray_t;
		DECLARE_TYPED_ENUMERATION (unsigned char, PageID_t)
		{
			PageID_Total = 5u	// Our value.
		};
		typedef StaticArray <ItemArray_t, PageID_t, PageID_Total> PageArray_t;

		class Item
		{
			public:
				virtual inline const HalfLifeEngine::SDK::Constants::Menu::ColorID_t GetColorID         (void)                         const { return HalfLifeEngine::SDK::Constants::Menu::ColorID_Red; }
				virtual inline const Localizer::LocalizedStringID_t                  GetLocalizedTextID (void)                         const { return Localizer::LocalizedStringID_TODO; }//*>* = 0;
				virtual inline const bool                                            HasAccess          (const Human *const/* human*/) const { return true; }
				virtual inline void                                                  AppendToMenu       (Human *const human, DynamicString &menuText, const ItemID_t index) const;///*>*/ = 0;
				virtual inline void                                                  HandleSelection    (Human *const human)
				{
					// If menu still not changed, we should remove this menu. (see HLSDK CHudMenu::SelectMenuItem() function from "cl_dll/menu.cpp")
					CloseMenu (human);	/// @note We should use Human::CloseMenu(), not Human::ResetMenu() because client can use console to select some menu item and function CHudMenu::SelectMenuItem() won't be called, so the menu still be drawed for client!
				}///*>*/ = 0;
				virtual inline void                                                  PlaySelectionSound (Human *const human)           const;
				virtual inline void                                                  CloseMenu          (Human *const human);
				virtual inline void                                                  RedrawMenu         (Human *const human);
		};
		class Item_Empty : public Item
		{
			protected:
				DynamicString m_string;

			public:
				inline Item_Empty (const DynamicString &string = '\n') : m_string (string) { /* VOID */ }

			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void)                                                                      const { return Localizer::LocalizedStringID_None; }
				inline void                                 AppendToMenu       (Human *const/* human*/, DynamicString &menuText, const ItemID_t/* index*/) const { menuText += m_string; }
				inline void                                 PlaySelectionSound (Human *const/* human*/)                                                    const { /* Empty */ }	/// @note This function can't be called as we don't add selection flags for this, but....

			public:
				inline void SetString (const DynamicString &string) { m_string = string; }
		};
		class Item_Next : public Item
		{
			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void)               const { return Localizer::LocalizedStringID_Menu_Item_Next; }
				inline void                                 HandleSelection    (Human *const human);
		};
		class Item_Previous : public Item
		{
			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void)               const { return Localizer::LocalizedStringID_Menu_Item_Previous; }
				inline void                                 HandleSelection    (Human *const human);
		};
		class Item_Exit : public Item
		{
			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void)               const { return Localizer::LocalizedStringID_Menu_Item_Exit; }
		};

	protected:
		PageArray_t m_pages;

	public:
		virtual inline ~Menu (void) { /* VOID */ }

	public:
		virtual inline const Localizer::LocalizedStringID_t                  GetTitleLocalizedTextID (void) const =/*>*/ 0;
//		virtual inline const HalfLifeEngine::SDK::Constants::Menu::ColorID_t GetTitleColorID         (void) const =/*>*/ 0;	/// NEEDED????????????????????????????????????????
//		virtual inline const HalfLifeEngine::SDK::Constants::Menu::ColorID_t GetItemColorID          (void) const =/*>*/ 0;	/// NEEDED????????????????????????????????????????
//		virtual inline const DynamicString                                   GetSoundName            (void) const { return "player/geiger1"; }//=/*>*/ 0;	// Playing sound on menu open

		/**
		* @brief Sends the menu to a client.
		*
		* @param client			Client index to display to.
		* @param time			Time to hold menu for.
		* @param alt_handler	Alternate IMenuHandler.
		* @return				True on success, false otherwise.
		*/
		virtual inline void                                 Draw                    (Human *const human);// =/*>*/ 0;

		virtual inline void                                 Close                   (Human *const human) const;// =/*>*/ 0;

		/**
		* @brief Called when an item is selected.
		*
		* @param client		Client that selected the item.
		* @param item		Item number.
		*/
		virtual inline void                                 HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection);// =/*>*/ 0;

		virtual inline void                                 Think           (Human *const human) { /* Empty */ }// =/*>*/ 0;
};
class Menu_YaPBMain : public Menu
{
	private:
		enum
		{
			ItemID_Control,
			ItemID_Features,

			ItemID_Empty3,

			ItemID_FillServer,
			ItemID_EndRound,

			ItemID_Empty6,
			ItemID_Empty7,
			ItemID_Empty8,
			ItemID_Empty9,

			ItemID_Exit
		};

		class Item_Control : public Item
		{
			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void) const { return Localizer::LocalizedStringID_Menu_Bot_Main_Item_Control; }
				inline void                                 HandleSelection    (Human *const human);
		};

		Item_Control m_itemControl;
		Item_Empty   m_itemEmpty;
		Item_Exit    m_itemExit;

	public:
class Menu_YaPBControl : public Menu
{
	private:
		enum
		{
			ItemID_AddBotQuick,
			ItemID_AddBotSpecified,

			ItemID_Empty3,

			ItemID_KickRandomBot,
			ItemID_KickAllBots,
			ItemID_KickBotMenu,

			ItemID_Empty7,
			ItemID_Empty8,

			ItemID_Previous,
			ItemID_Exit
		};

		class Item_AddBotQuick : public Item
		{
			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void) const { return Localizer::LocalizedStringID_Menu_Bot_Control_Item_AddBotQuick; }
				void                                        HandleSelection    (Human *const human);
		};

		Item_AddBotQuick m_itemAddBotQuick;
		Item_Empty       m_itemEmpty;
		Item_Previous    m_itemPrevious;
		Item_Exit        m_itemExit;

	public:
class Menu_YaPBKick : public Menu
{
	private:
		template <typename clientType> class Item_Kick : public Item
		{
			private:
				clientType *m_client;

			public:
				inline Item_Kick (clientType *const client = NULL) : m_client (client) { /* VOID */ }

			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void) const { return Localizer::LocalizedStringID_None; }
				void                                        AppendToMenu       (Human *const human, DynamicString &menuText, const ItemID_t index) const;
				void                                        HandleSelection    (Human *const human);

			public:
				inline void SetClient (clientType *const client) { m_client = client; }
		};
		typedef Item_Kick <Client> Item_KickClient;
		typedef Item_Kick <YaPB>   Item_KickBot;

		Item_KickBot   m_itemKickBot[HalfLifeEngine::SDK::Constants::MaximumClients];
		Item_Empty     m_itemEmpty;
		Item_Previous  m_itemPrevious;
		Item_Next      m_itemNext;
		Item_Exit      m_itemExit;

	public:
		inline const Localizer::LocalizedStringID_t GetTitleLocalizedTextID (void) const { return Localizer::LocalizedStringID_Menu_Bot_Kick_Title; }

		void                                        Draw                    (Human *const human);
};

		class Item_OpenKickBotMenu : public Item
		{
			public:
				inline const Localizer::LocalizedStringID_t GetLocalizedTextID (void) const { return Localizer::LocalizedStringID_Menu_Bot_Control_Item_KickBotMenu; }
				void                                        HandleSelection    (Human *const human);
		};

		Item_OpenKickBotMenu m_itemOpenKickBotMenu;

	public:
		inline Menu_YaPBControl (void)
		{
			// Add the first page....
			m_pages.PushBackDefaultElement ();	// 0
			{
				// Fill the first page....
				m_pages[0u] += &m_itemAddBotQuick;	// ItemID_AddBotQuick [0][0]

				m_pages[0u] += &m_itemEmpty;	// [0][1]
				m_pages[0u] += &m_itemEmpty;	// [0][2]
				m_pages[0u] += &m_itemEmpty;	// [0][3]
				m_pages[0u] += &m_itemOpenKickBotMenu;	// [0][4]
				m_pages[0u] += &m_itemEmpty;	// [0][5]
				m_pages[0u] += &m_itemEmpty;	// [0][6]
				m_pages[0u] += &m_itemEmpty;	// [0][7]

				m_pages[0u] += &m_itemPrevious;	// ItemID_Previous [0][8]
				m_pages[0u] += &m_itemExit;		// ItemID_Exit [0][9]
			}
		}

	public:
		inline const Localizer::LocalizedStringID_t GetTitleLocalizedTextID (void) const { return Localizer::LocalizedStringID_Menu_Bot_Control_Title; }

//		inline void Draw (Human *const human);
//		void HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection);
};

	public:
		inline Menu_YaPBMain (void)
		{
			// Add the first page....
			m_pages.PushBackDefaultElement ();	// 0
			{
				// Fill the first page....
				m_pages[0u] += &m_itemControl;	// ItemID_Control [0][0]

				m_pages[0u] += &m_itemEmpty;	// [0][1]
				m_pages[0u] += &m_itemEmpty;	// [0][2]
				m_pages[0u] += &m_itemEmpty;	// [0][3]
				m_pages[0u] += &m_itemEmpty;	// [0][4]
				m_pages[0u] += &m_itemEmpty;	// [0][5]
				m_pages[0u] += &m_itemEmpty;	// [0][6]
				m_pages[0u] += &m_itemEmpty;	// [0][7]
				m_pages[0u] += &m_itemEmpty;	// [0][8]

				m_pages[0u] += &m_itemExit;		// ItemID_Exit [0][9]
			}
		}

	public:
		inline const Localizer::LocalizedStringID_t GetTitleLocalizedTextID (void) const { return Localizer::LocalizedStringID_Menu_Bot_Main_Title; }

//		inline void Draw (Human *const human);
//		void HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection);
};

	//
	// Group: Private members.
	//
	private:
		Menu                                           *m_menus[MenuID_Total];

		Menu_YaPBMain                                   m_YaPBMain;
		Menu_YaPBMain::Menu_YaPBControl                 m_YaPBControl;
		Menu_YaPBMain::Menu_YaPBControl::Menu_YaPBKick  m_YaPBKick;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline MenuManager (void)
		{
			for (unsigned char id (MenuID_YaPBMain); id < MenuID_Total; ++id)	/// @warning TEMP!!!
				m_menus[id] = NULL;												/// @warning TEMP!!!

			m_menus[MenuID_YaPBMain]      = &m_YaPBMain;
			m_menus[MenuID_BotControl]    = &m_YaPBControl;
			m_menus[MenuID_BotKickMenu¹1] = &m_YaPBKick;
		}

	//
	// Group: Functions.
	//
	public:
		inline Menu *const &GetMenu (const MenuID_t id)
		{
			// Reliability checks.
			InternalAssert (id > MenuID_None && id < MenuID_Total);
			InternalAssert (m_menus[id] != NULL);

			return m_menus[id];
		}
		inline const Menu *const &GetMenu (const MenuID_t id) const
		{
			// Reliability checks.
			InternalAssert (id > MenuID_None && id < MenuID_Total);
			InternalAssert (m_menus[id] != NULL);

			return m_menus[id];
		}

	//
	// Group: Engine callbacks.
	//
	// Note: This group is declared in Human.hpp.
	//
	public:
		inline void Draw (Human *const human);
		inline void HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection);
		inline void Think (Human *const human);
};

extern MenuManager g_menusManager;

#endif	// ifndef MENUS_INCLUDED