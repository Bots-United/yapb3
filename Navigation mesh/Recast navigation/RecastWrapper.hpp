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
// RecastWrapper.hpp
//
// Class: RecastWrapper
//
// Version: $ID:$
//

#ifndef RECAST_WRAPPER_INCLUDED
#define RECAST_WRAPPER_INCLUDED

#if defined MULTIPLE_DRAW_METHODS
class RecastWrapper_GL;
#endif	// if defined MULTIPLE_DRAW_METHODS

class RecastWrapper
{
	public:
		#include <Navigation mesh/Recast navigation/BotMeshLoaderObj.hpp>

		bool m_isActive;

		BuildContext ctx;
		InputGeom *geom;
		Sample *sample;
		TestCase *test;

	public:
		inline RecastWrapper (void) :
			m_isActive (false),

			geom (NULL),
			sample (NULL),
			test (NULL)
		{ /* VOID */ }

		virtual inline ~RecastWrapper (void)
		{
			delete test;
			delete sample;
			delete geom;
		}

	public:
		virtual void Think2D (void) {}
		virtual void Think (void) {}
		virtual const bool MouseEvent (const int state, bool down) { return false; }
		virtual const bool KeyEvent (const int down, const int keynum, const char *const pszCurrentBinding) { return false; }

	public:
		inline const bool IsActive (void) const { return m_isActive; }
		inline void SetActive (const bool isActive) { m_isActive = isActive; }
		inline void Deactivate (void) { SetActive (false); }
		inline void Activate (void) { SetActive (true); }

		virtual void Initialize (void);

		#if defined MULTIPLE_DRAW_METHODS
			inline RecastWrapper_GL *const GetRecastWrapper_GLPointer (void);	// Note: This function declared below....
		#endif	// if defined MULTIPLE_DRAW_METHODS
};

#if defined MULTIPLE_DRAW_METHODS
class RecastWrapper_GL : public RecastWrapper
{
	public:
		SlideShow slideShow;

		float t;
		float timeAcc;
		unsigned int lastTime;

		unsigned char mbut;

		bool m_imguiInitialized;
		bool m_crosshairEnabled;

		// Handle input events.
		int mscroll;
		bool processHitTest;
		bool processHitTestShift;

	int mx, my;
	Math::Vector3D rays, raye;
	bool mouseOverMenu;
	bool showMenu;
	bool showLog;
	bool showTools;
	bool showLevels;
	bool showSample;
	bool showTestCases;
	int width, height;

	int propScroll;
	int logScroll;
	int toolsScroll;
	
	DynamicString sampleName;
	
	FileList files;
	DynamicString meshName;
	
	Math::Vector3D mpos;
	bool mposSet;

	GLdouble proj[16];
	GLdouble model[16];
	GLint view[4];

	public:
		inline RecastWrapper_GL (void) :
			RecastWrapper (),

			t (0.0f),
			timeAcc (0.0f),
			lastTime (GetTickCount ()),
			mbut (0u),
			m_imguiInitialized (false),
			m_crosshairEnabled (false),

	mscroll (0),
	processHitTest (false),
	processHitTestShift (false),

	mx (0), my (0),
	mouseOverMenu (false),
	showMenu (true),
	showLog (false),
	showTools (true),
	showLevels (false),
	showSample (false),
	showTestCases (false),

	propScroll (0),
	logScroll (0),
	toolsScroll (0),

	sampleName ("Choose Sample..."),
	meshName ("Choose Mesh..."),

	mposSet (false)
	{
			memset (proj, 0, sizeof (proj));
			memset (model, 0, sizeof (model));
			memset (view, 0, sizeof (view));

			if (!imguiRenderGLInit (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/DroidSans.ttf"))
				ctx.log (RC_LOG_ERROR, "Could not init GUI renderer.\n");
			else
			{
				m_imguiInitialized = true;

				slideShow.init ("slides/");

	{
		HalfLifeEngine::SDK::Structures::ScreenInformation_t screenInformation = {sizeof (HalfLifeEngine::SDK::Structures::ScreenInformation_t)/*, ...*/};

		(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnGetScreenInfo) (&screenInformation);
		width = screenInformation.width;
		height = screenInformation.height;
	}
			}
		}

		virtual inline ~RecastWrapper_GL (void)
		{
			imguiRenderGLDestroy ();
		}

	public:
		void Think2D (void);
		void Think (void);
		const bool MouseEvent (const int state, bool down);
		const bool KeyEvent (const int down, const int keynum, const char *const pszCurrentBinding);
};

inline RecastWrapper_GL *const RecastWrapper::GetRecastWrapper_GLPointer (void) { return static_cast <RecastWrapper_GL *const> (this); }
#endif	// if defined MULTIPLE_DRAW_METHODS

#endif	// RECAST_WRAPPER_INCLUDED