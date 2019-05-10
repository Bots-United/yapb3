//========= Copyright © 1996-2003, Valve LLC, All rights reserved. ============
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef ISURFACE_H
#define ISURFACE_H

#ifdef _WIN32
#pragma once
#endif
/*
#ifdef CreateFont
#undef CreateFont
#endif

#ifdef PlaySound
#undef PlaySound
#endif
*/
//SRC only defines

struct Vertex_t
{
	Vertex_t() {}
	Vertex_t( const Math::Vector2D &pos, const Math::Vector2D &coord = Math::Vector2D( 0, 0 ) )
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init( const Math::Vector2D &pos, const Math::Vector2D &coord = Math::Vector2D( 0, 0 ) )
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	
	Math::Vector2D	m_Position;
	Math::Vector2D	m_TexCoord;
};


enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2
};


// Refactor these two
struct CharRenderInfo
{
	// In:
	FontDrawType_t	drawType;
	wchar_t			ch;

	// Out
	bool			valid;

	// In/Out (true by default)
	bool			shouldclip;
	// Text pos
	int				x, y;
	// Top left and bottom right
	Vertex_t		verts[ 2 ];
	int				textureId;
	int				abcA;
	int				abcB;
	int				abcC;
	int				fontTall;
	VGUI::HFont			currentFont;
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
abstract_class ISurface : public Classes::Interface::Base//, public Classes::Interface::IAppSystem - NOT EXISTS?!?!??!?!??!
{
public:
	// call to Shutdown surface; surface can no longer be used after this is called
/*off=1 - 1E49014*/	virtual void Shutdown() = 0;

	// frame
/*off=2 - 1E49018 ??*/	virtual void RunFrame() = 0;	// { /* Empty */ }

	// hierarchy root
/*off=3*/	virtual VGUI::VPANEL GetEmbeddedPanel() = 0;
/*off=4*/	virtual void SetEmbeddedPanel( VGUI::VPANEL pPanel ) = 0;

	// drawing context
/*off=5 - 1E49024 ??*/	virtual void PushMakeCurrent(VGUI::VPANEL panel, bool useInsets) = 0;
/*off=6 - 1E49028 ??*/	virtual void PopMakeCurrent(VGUI::VPANEL panel) = 0;

	// rendering functions
/*off=7 - 1E4902C ?? (OR SHOULD BE ON 1E49030 ?)*/	virtual void DrawSetColor(ColorWithAlpha <> col) = 0;
/*off=8 - 1E49030 ?? (OR SHOULD BE ON 1E4902C ?)*/	virtual void DrawSetColor(int r, int g, int b, int a) = 0;
	
/*off=9 - 1E49034 ??*/	virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
/*off=10 - 1E49038 ??*/	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;

/*off=11 - 1E4903C ??*/	virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
/*off=12 - 1E49040 ??*/	virtual void DrawPolyLine(int *px, int *py, int numPoints) = 0;

/*off=13 - 1E49044*/	virtual void DrawSetTextFont(VGUI::HFont font) = 0;
/*off=14 - 1E49048 ?? (OR SHOULD BE ON 1E4904C ?)*/	virtual void DrawSetTextColor(ColorWithAlpha <> col) = 0;
/*off=15 - 1E4904C ?? (OR SHOULD BE ON 1E49048 ?)*/	virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
/*off=16 - 1E49050 ??*/	virtual void DrawSetTextPos(int x, int y) = 0;
/*off=17 - 1E49054 ??*/	virtual void DrawGetTextPos(int& x,int& y) = 0;
/*off=18 - 1E49058 ??*/	virtual void DrawPrintText(const wchar_t *text, int textLen/*, FontDrawType_t drawType = FONT_DRAW_DEFAULT - EXISTS?!?!??!*/) = 0;
/*off=19 - 1E4905C ??*/	virtual void DrawUnicodeChar_UNKNOWN(wchar_t wch) = 0;
/*off=20 - 1E49060 ??*/	virtual void DrawUnicodeChar(wchar_t wch/*, FontDrawType_t drawType = FONT_DRAW_DEFAULT - EXISTS?!?!??!*/) = 0;

/*off=21 - 1E49064*/	virtual void DrawFlushText() = 0;		// flushes any buffered text (for rendering optimizations)
/*off=22 - 1E49068*/	virtual IHTML *CreateHTMLWindow(IHTMLEvents *events,VGUI::VPANEL context)=0;
/*off=23 - 1E4906C*/	virtual void PaintHTMLWindow(IHTML *htmlwin) =0;
/*off=24 - 1E49070*/	virtual void DeleteHTMLWindow(IHTML *htmlwin)=0;

//*off=23 - 1E4906C*/	virtual int	 DrawGetTextureId( char const *filename ) = 0;
//*off=24 - 1E49070*/	virtual bool DrawGetTextureFile(int id, char *filename, int maxlen ) = 0;
/*off=25 - 1E49074*/	virtual void DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload) = 0;
/*off=26 - 1E49078*/	virtual void DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload)=0;
/*off=27 - 1E4907C*/	virtual void DrawSetTexture(int id) = 0;
/*off=28 - 1E49080*/	virtual void DrawGetTextureSize(int id, int &wide, int &tall) = 0;
/*off=29 - 1E49084*/	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
/*off=30 - 1E49088*/	virtual bool IsTextureIDValid(int id) = 0;

/*off=31 - 1E4908C*/	virtual int CreateNewTextureID( bool procedural = false ) = 0;

/*off=32 - 1E49090*/	virtual void GetScreenSize(int &wide, int &tall) = 0;
/*off=33 - 1E49094*/	virtual void SetAsTopMost(VGUI::VPANEL panel, bool state) = 0;
/*off=34*/	virtual void BringToFront(VGUI::VPANEL panel) = 0;
/*off=35*/	virtual void SetForegroundWindow (VGUI::VPANEL panel) = 0;
/*off=36*/	virtual void SetPanelVisible(VGUI::VPANEL panel, bool state) = 0;
/*off=37 - 1E490A4*/	virtual void SetMinimized(VGUI::VPANEL panel, bool state) = 0;
/*off=38*/	virtual bool IsMinimized(VGUI::VPANEL panel) = 0;
/*off=39*/	virtual void FlashWindow(VGUI::VPANEL panel, bool state) = 0;
/*off=40*/	virtual void SetTitle(VGUI::VPANEL panel, const wchar_t *title) = 0;
/*off=41*/	virtual void SetAsToolBar(VGUI::VPANEL panel, bool state) = 0;		// removes the window's task bar entry (for context menu's, etc.)

	// windows stuff
/*off=42*/	virtual void CreatePopup(VGUI::VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true , bool kbInput = true) = 0;
/*off=43*/	virtual void SwapBuffers(VGUI::VPANEL panel) = 0;
/*off=44*/	virtual void Invalidate(VGUI::VPANEL panel) = 0;
/*off=45 - 1E490C4*/	virtual void SetCursor(VGUI::HCursor cursor) = 0;
/*off=46 - 1E490C8*/	virtual bool IsCursorVisible() = 0;
/*off=47 - 1E490CC*/	virtual void ApplyChanges() = 0;	// { /* Empty */ }
/*off=48 - 1E490D0*/	virtual bool IsWithin(int x, int y) = 0;	// { return true; }
/*off=49 - 1E490D4*/	virtual bool HasFocus() = 0;	// { return true; }
	
	// returns true if the surface supports minimize & maximize capabilities
	enum SurfaceFeature_e
	{
		ANTIALIASED_FONTS	= 1,
		DROPSHADOW_FONTS	= 2/*,
		ESCAPE_KEY			= 3,
		OPENING_NEW_HTML_WINDOWS = 4,
		FRAME_MINIMIZE_MAXIMIZE	 = 5,
		OUTLINE_FONTS	= 6 - NOT EXISTS???????*/
	};
/*off=50 - 1E490D8*/	virtual bool SupportsFeature(SurfaceFeature_e feature) = 0;

	// NEW functions based on GameUI needs
/*off=51 - 1E490DC ??*/	virtual void RestrictPaintToSinglePanel(VGUI::VPANEL panel) = 0;
/*off=52 - 1E490E0*/	virtual void SetModalPanel(VGUI::VPANEL ) = 0;	// { /* Empty */ }
/*off=53 - 1E490E4 ??*/	virtual VGUI::VPANEL GetModalPanel() = 0;
/*off=54 - 1E490E8*/	virtual void UnlockCursor() = 0;
/*off=55 - 1E490EC*/	virtual void LockCursor() = 0;
/*off=56 - 1E490F0*/	virtual void SetTranslateExtendedKeys(bool state) = 0;
/*off=57 - 1E490F4*/	virtual VGUI::VPANEL GetTopmostPopup() = 0;

	// engine-only focus handling (replacing WM_FOCUS windows handling)
/*off=58 - 1E490F8*/	virtual void SetTopLevelFocus(VGUI::VPANEL panel) = 0;

	// fonts
	// creates an empty handle to a vgui font. Windows fonts can be add to this via AddGlyphSetToFont().
/*off=59 - 1E490FC*/	virtual VGUI::HFont CreateFont() = 0;

	// adds to the font
	enum EFontFlags
	{
		FONTFLAG_NONE,
		FONTFLAG_ITALIC			= 0x001,
		FONTFLAG_UNDERLINE		= 0x002,
		FONTFLAG_STRIKEOUT		= 0x004,
		FONTFLAG_SYMBOL			= 0x008,
		FONTFLAG_ANTIALIAS		= 0x010,
		FONTFLAG_GAUSSIANBLUR	= 0x020,
		FONTFLAG_ROTARY			= 0x040,
		FONTFLAG_DROPSHADOW		= 0x080,
// EXISTS????		FONTFLAG_ADDITIVE		= 0x100,
		FONTFLAG_OUTLINE		= 0x200
	};
/*off=60 - 1E49100*/	virtual bool AddGlyphSetToFont(VGUI::HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange) = 0;
	// adds a custom font file (only supports true type font files (.ttf) for now)
/*off=61 - 1E49104*/	virtual bool AddCustomFontFile(const char *fontFileName) = 0;

	// returns the details about the font
/*off=62 - 1E49108*/	virtual int GetFontTall(VGUI::HFont font) = 0;
// - NOT EXISTS?!?!??!	virtual bool IsFontAdditive(VGUI::HFont font) = 0;
/*off=63 - 1E4910C*/	virtual void GetCharABCwide(VGUI::HFont font, int ch, int &a, int &b, int &c) = 0;
/*off=64 - 1E49110*/	virtual int GetCharacterWidth(VGUI::HFont font, int ch) = 0;
/*off=65 - 1E49114*/	virtual void GetTextSize(VGUI::HFont font, const wchar_t *text, int &wide, int &tall) = 0;

	// notify icons?!?
/*off=66 - 1E49118*/	virtual VGUI::VPANEL GetNotifyPanel() = 0;	// { return 0; }
/*off=67 - 1E4911C*/	virtual void SetNotifyIcon(VGUI::VPANEL context, VGUI::HTexture icon, VGUI::VPANEL panelToReceiveMessages, const char *text) = 0;	// { /* Empty */ }

	// plays a sound
/*off=68 - 1E49120*/	virtual void PlaySound(const char *fileName) = 0;

	//!! these functions should not be accessed directly, but only through other vgui items
	//!! need to move these to seperate interface
/*off=69*/	virtual int GetPopupCount() = 0;
/*off=70*/	virtual VGUI::VPANEL GetPopup(int index) = 0;
/*off=71*/	virtual bool ShouldPaintChildPanel(VGUI::VPANEL childPanel) = 0;
	virtual bool RecreateContext(VGUI::VPANEL panel) = 0;
	virtual void AddPanel(VGUI::VPANEL panel) = 0;
	virtual void ReleasePanel(VGUI::VPANEL panel) = 0;
	virtual void MovePopupToFront(VGUI::VPANEL panel) = 0;
	virtual void MovePopupToBack(VGUI::VPANEL panel) = 0;

	virtual void SolveTraverse(VGUI::VPANEL panel, bool forceApplySchemeSettings = false) = 0;
/*off=78*/	virtual void PaintTraverse(VGUI::VPANEL panel) = 0;

	virtual void EnableMouseCapture(VGUI::VPANEL panel, bool state) = 0;

	// returns the size of the workspace
	virtual void GetWorkspaceBounds(int &x, int &y, int &wide, int &tall) = 0;

	// gets the absolute coordinates of the screen (in windows space)
	virtual void GetAbsoluteWindowBounds(int &x, int &y, int &wide, int &tall) = 0;

	// gets the base resolution used in proportional mode
	virtual void GetProportionalBase( int &width, int &height ) = 0;

/*off=83*/	virtual void CalculateMouseVisible() = 0;
/*off=84*/	virtual bool NeedKBInput() = 0;

/*off=85*/	virtual bool HasCursorPosFunctions() = 0;
/*off=86*/	virtual void SurfaceGetCursorPos(int &x, int &y) = 0;	// { /* Empty */ }
/*off=87*/	virtual void SurfaceSetCursorPos(int x, int y) = 0;	// { /* Empty */ }
#if 0
/// @todo DONE THIS SHIT!!!!

	// SRC only functions!!!
	virtual void DrawTexturedLine( const Vertex_t &a, const Vertex_t &b ) = 0;
	virtual void DrawOutlinedCircle(int x, int y, int radius, int segments) = 0;
	virtual void DrawTexturedPolyLine( const Vertex_t *p,int n ) = 0; // (Note: this connects the first and last points).
	virtual void DrawTexturedSubRect( int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1 ) = 0;
	virtual void DrawTexturedPolygon(int n, Vertex_t *pVertices) = 0;
	virtual const wchar_t *GetTitle(VGUI::VPANEL panel) = 0;
	virtual bool IsCursorLocked( void ) const = 0;
	virtual void SetWorkspaceInsets( int left, int top, int right, int bottom ) = 0;

	// Lower level char drawing code, call DrawGet then pass in info to DrawRender
	virtual bool DrawGetUnicodeCharRenderInfo( wchar_t ch, CharRenderInfo& info ) = 0;
	virtual void DrawRenderCharFromInfo( const CharRenderInfo& info ) = 0;
#endif	// if 0
};	// total 101 or 102(+ unk_1E54430)

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface026"

#endif // ISURFACE_H