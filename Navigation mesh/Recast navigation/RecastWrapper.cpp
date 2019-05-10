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
// RecastWrapper.cpp
//
// Class: RecastWrapper
//
// Version: $ID:$
//

#include <Core.hpp>

struct SampleItem
{
	enum ID_t
	{
		ID_Solo,
		ID_Tile,
		ID_TempObstacles,
		ID_AnnotationBuilder,
		ID_Debug,

		ID_Total
	};

	Sample *(*create) (void);
	const char* name;
};

Sample* createSolo() { return new Sample_SoloMesh(); }
Sample* createTile() { return new Sample_TileMesh(); }
Sample* createTempObstacle() { return new Sample_TempObstacles(); }
Sample* createAnnotationBuilder() { return new AnnotationBuilder(); }
Sample* createDebug() { return new Sample_Debug(); }

static SampleItem g_samples[] =
{
	{ createSolo, "Solo Mesh" },
	{ createTile, "Tile Mesh" },
	{ createTempObstacle, "Temp Obstacles" },
	{ createAnnotationBuilder, "Annotation Builder" },
//	{ createDebug, "Debug" }
};
static const int g_nsamples = GET_STATIC_ARRAY_SIZE (g_samples);

SampleItem::ID_t g_currentSampleID (SampleItem::ID_AnnotationBuilder/*ID_Solo*/);

class BotInputGeom : public InputGeom
{
	public:
		bool loadMesh(rcContext* ctx, const char* filepath = NULL)
		{
	if (m_mesh)
	{
		delete m_chunkyMesh;
		m_chunkyMesh = 0;
		delete m_mesh;
		m_mesh = 0;
	}
	m_offMeshConCount = 0;
	m_volumeCount = 0;
	
	m_mesh = new RecastWrapper::NavigationMeshLoader ();
	if (!m_mesh)
	{
		ctx->log(RC_LOG_ERROR, "BotInputGeom::loadMesh: Out of memory 'm_mesh'.");
		return false;
	}
	if (!m_mesh->load(filepath))
	{
		ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not load '%s'", filepath);
		return false;
	}

	rcCalcBounds(m_mesh->getVerts(), m_mesh->getVertCount(), m_meshBMin, m_meshBMax);

	m_chunkyMesh = new rcChunkyTriMesh ();
	if (!m_chunkyMesh)
	{
		ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Out of memory 'm_chunkyMesh'.");
		return false;
	}
	if (!rcCreateChunkyTriMesh(m_mesh->getVerts(), m_mesh->getTris(), m_mesh->getTriCount(), 256, m_chunkyMesh))
	{
		ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Failed to build chunky mesh.");
		return false;
	}		

	return true;
		}
};

void RecastWrapper::Initialize (void)
{
	const DynamicString mapName (g_server->GetMapName ());
	const DynamicString path (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Meshes/" + mapName + ".OBJ");

	geom = new BotInputGeom ();

	if (!geom->loadMesh(&ctx, path))
	{
		delete geom;
		geom = NULL;

		ctx.dumpLog("Geom load log %s:", mapName.GetData ());
	}

	sample = g_samples[g_currentSampleID].create ();

	sample->setContext (&ctx);

	if (sample && geom)
	{
		sample->handleMeshChanged(geom);

		{
		sample->handleSettings();

		ctx.resetLog();

		if (!sample->handleBuild())
			ctx.log(RC_LOG_ERROR, "Can't build navigation mesh for map %s!\n", mapName.GetData ());

		ctx.dumpLog("Build log %s:", mapName.GetData ());
		}
	}
}

#if defined MULTIPLE_DRAW_METHODS
//GLint matrixMode (0);

void RecastWrapper_GL::Think2D (void)
{
	if (!m_imguiInitialized)
		return;

	// Render GUI

	// GL_Set2D() analog from recastnavigation demo....
	{
	g_drawInterface->glPushMatrix ();
//	g_drawInterface->glViewport (0, 0, width, height);
	g_drawInterface->glMatrixMode(GL_PROJECTION);
	g_drawInterface->glLoadIdentity();
	g_drawInterface->glOrtho(0, width, 0, height, -1, 1);
	g_drawInterface->glMatrixMode(GL_MODELVIEW);
	g_drawInterface->glLoadIdentity();

	g_drawInterface->glDisable (GL_DEPTH_TEST);
	g_drawInterface->glDisable (GL_CULL_FACE);
	g_drawInterface->glDisable (GL_BLEND);
	g_drawInterface->glEnable (GL_ALPHA_TEST);

	g_drawInterface->glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	}

	{
	int cursorPointX, cursorPointY;
	(*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetMousePosition) (&cursorPointX, &cursorPointY);
	mx = cursorPointX;
	my = height-1 - cursorPointY;
	}

	bool done (false);
	if (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
	{
		if (!done)
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->GetSurface ()->SetCursor (HalfLifeEngine::SDK::VGUI::dc_arrow);
			HalfLifeEngine::Globals::g_halfLifeEngine->GetSurface ()->UnlockCursor ();
			done = true;
		}
	}
	else
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->GetSurface ()->CalculateMouseVisible ();
		done = false;
	}
/*
	{
	const HalfLifeEngine::SDK::Constants::ButtonType_t buttons (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons);

//	if (SDL_GetMouseState(0,0) & SDL_BUTTON_LMASK)
	if (buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK2)
		mbut |= IMGUI_MBUT_LEFT;
//	if (SDL_GetMouseState(0,0) & SDL_BUTTON_RMASK)
	if (buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK)
		mbut |= IMGUI_MBUT_RIGHT;
	}
*/
	unsigned int time = GetTickCount();
	float dt = (time - lastTime) / 1000.0f;
	lastTime = time;

	t += dt;

		// Hit test mesh.
		if (processHitTest && geom && sample)
		{
			float hitt;
			bool hit = geom->raycastMesh(rays, raye, hitt);
/*
	const HalfLifeEngine::SDK::Structures::con_nprint_t printInfo =
	{
		15u & 31u,
		10.1f,
		Color <float> (0.0f, 1.0f, 0.0f)
	};
	HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().Con_NXPrintf(&printInfo,*/
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk,
/*	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat(
		"proj[16] = {%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f},\n"
		"model[16] = {%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f},\n"
		"view[4] = {%i, %i, %i, %i},\n"
		"rays[3] = {%.2f, %.2f, %.2f},\n"
		"raye[3] = {%.2f, %.2f, %.2f}\n"
		"matrixMode = %s\n",
		proj[0], proj[1], proj[2], proj[3], proj[4], proj[5], proj[6], proj[7], proj[8], proj[9], proj[10], proj[11], proj[12], proj[13], proj[14], proj[15],
		model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10], model[11], model[12], model[13], model[14], model[15],
		view[0], view[1], view[2], view[3],
		rays.x, rays.y, rays.z,
		raye.x, raye.y, raye.z,
		matrixMode == GL_MODELVIEW ? "MODELVIEW" : matrixMode == GL_PROJECTION ? "PROJECTION" : matrixMode == GL_TEXTURE ? "TEXTURE" : "UNKNOWN!"
	);
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "HELLO THERE!, rays: [%.2f, %.2f, %.2f], raye: [%.2f, %.2f, %.2f]\n", rays.x, rays.y, rays.z, raye.x, raye.y, raye.z);
//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (recastToVec3 (rays), recastToVec3 (raye), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 10u);
			g_drawInterface->DrawLineTimed (recastToVec3 (rays), recastToVec3 (raye), ColorWithAlpha <> (0u, 255u, 0u, 255u), 15.0f, 255u, 1u, false);
*/
			if (hit)
			{
				if (/**SDL_GetModState() & KMOD_CTRL*/false)
				{
					// Marker
					mposSet = true;
					mpos[0] = rays[0] + (raye[0] - rays[0])*hitt;
					mpos[1] = rays[1] + (raye[1] - rays[1])*hitt;
					mpos[2] = rays[2] + (raye[2] - rays[2])*hitt;
				}
				else
				{
					float pos[3];
					pos[0] = rays[0] + (raye[0] - rays[0])*hitt;
					pos[1] = rays[1] + (raye[1] - rays[1])*hitt;
					pos[2] = rays[2] + (raye[2] - rays[2])*hitt;
					sample->handleClick(rays, pos, processHitTestShift);
				}
			}
			else
			{
				if (/**SDL_GetModState() & KMOD_CTRL*/false)
				{
					// Marker
					mposSet = false;
				}
			}
		}

	// Update sample simulation.
	const float SIM_RATE = 20;
	const float DELTA_TIME = 1.0f/SIM_RATE;
	timeAcc = rcClamp(timeAcc+dt, -1.0f, 1.0f);
	int simIter = 0;
	while (timeAcc > DELTA_TIME)
	{
		timeAcc -= DELTA_TIME;
		if (simIter < 5)
		{
			if (sample)
				sample->handleUpdate(DELTA_TIME);
		}
		simIter++;
	}

	mouseOverMenu = false;

	imguiBeginFrame (mx, my, mbut, mscroll);

	if (sample)
		sample->handleRenderOverlay (proj, model, view);

	if (showMenu)
	{
		if (imguiBeginScrollArea("Properties", width-250-10, 10, 250, height-20, &propScroll))
			mouseOverMenu = true;

			if (imguiCheck("Show Log", showLog))
				showLog = !showLog;
			if (imguiCheck("Show Tools", showTools))
				showTools = !showTools;

			imguiSeparator();
			imguiLabel("Sample");
			if (imguiButton(sampleName))
			{
				if (showSample)
				{
					showSample = false;
				}
				else
				{
					showSample = true;
					showLevels = false;
					showTestCases = false;
				}
			}

			imguiSeparator();
/*			imguiLabel("Input Mesh");
			if (imguiButton(meshName))
			{
				if (showLevels)
				{
					showLevels = false;
				}
				else
				{
					showSample = false;
					showTestCases = false;
					showLevels = true;
					files.ScanDirectory ("Meshes", ".obj");
				}
			}*/
			if (geom)
			{
				char text[64];
				_snprintf(text, 64, "Verts: %.1fk  Tris: %.1fk",
						 geom->getMesh()->getVertCount()/1000.0f,
						 geom->getMesh()->getTriCount()/1000.0f);
				imguiValue(text);
			}
			imguiSeparator();

			if (geom && sample)
			{
				imguiSeparatorLine();
				
				sample->handleSettings();

				if (imguiButton("Build"))
				{
					ctx.resetLog();
					if (!sample->handleBuild())
					{
						showLog = true;
						logScroll = 0;
					}
					ctx.dumpLog("Build log %s:", meshName.GetData ());
					
					// Clear test.
					delete test;
					test = 0;
				}

				imguiSeparator();
			}

		if (sample)
		{
			imguiSeparatorLine();
			sample->handleDebugMode();
		}

		imguiEndScrollArea();
	}

	// Sample selection dialog.
		if (showSample)
		{
			static int levelScroll = 0;
			if (imguiBeginScrollArea("Choose Sample", width-10-250-10-200, height-10-250, 200, 250, &levelScroll))
				mouseOverMenu = true;

			Sample* newSample = 0;
			int i = 0;
			for (/* Empty */; i < g_nsamples; ++i)
			{
				if (imguiItem(g_samples[i].name))
				{
					newSample = g_samples[i].create();

					break;
				}
			}
			if (newSample)
			{
				sampleName = g_samples[i].name;
				g_currentSampleID = static_cast <SampleItem::ID_t> (i);

				delete sample;
				sample = newSample;
				sample->setContext(&ctx);
				if (geom && sample)
				{
					sample->handleMeshChanged(geom);
				}
				showSample = false;
			}
/*
			if (geom || sample)
			{
				const float* bmin = 0;
				const float* bmax = 0;
				if (sample)
				{
					bmin = sample->getBoundsMin();
					bmax = sample->getBoundsMax();
				}
				else if (geom)
				{
					bmin = geom->getMeshBoundsMin();
					bmax = geom->getMeshBoundsMax();
				}
				// Reset camera and fog to match the mesh bounds.
				if (bmin && bmax)
				{
					camr = sqrtf(rcSqr(bmax[0]-bmin[0]) +
								 rcSqr(bmax[1]-bmin[1]) +
								 rcSqr(bmax[2]-bmin[2])) / 2;
					camx = (bmax[0] + bmin[0]) / 2 + camr;
					camy = (bmax[1] + bmin[1]) / 2 + camr;
					camz = (bmax[2] + bmin[2]) / 2 + camr;
					camr *= 3;
				}
				rx = 45;
				ry = -45;
				g_drawInterface->glFogf(GL_FOG_START, camr*0.1f);
				g_drawInterface->glFogf(GL_FOG_END, camr*1.25f);
			}
*/
			imguiEndScrollArea();
		}
/*
		// Test cases
		if (showTestCases)
		{
			static int testScroll = 0;
			if (imguiBeginScrollArea("Choose Test To Run", width-10-250-10-200, height-10-450, 200, 450, &testScroll))
				mouseOverMenu = true;

			int testToLoad = -1;
			for (int i = 0; i < files.size; ++i)
			{
				if (imguiItem(files.files[i]))
					testToLoad = i;
			}
			
			if (testToLoad != -1)
			{
				char path[256];
				strcpy(path, "Tests/");
				strcat(path, files.files[testToLoad]);
				test = new TestCase ();
				if (test)
				{
					// Load the test.
					if (!test->load(path))
					{
						delete test;
						test = 0;
					}

					// Create sample
					Sample* newSample = 0;
					int i = 0;
					for (/* Empty *//*; i < g_nsamples; ++i)
					{
						if (strcmp(g_samples[i].name, test->getSampleName()) == 0)
						{
							newSample = g_samples[i].create();

							break;
						}
					}
					if (newSample)
					{
						sampleName = g_samples[i].name;
						g_currentSampleID = static_cast <SampleItem::ID_t> (i);

						delete sample;
						sample = newSample;
						sample->setContext(&ctx);
						showSample = false;
					}

					// Load geom.
					meshName = test->getGeomFileName();
					
					delete geom;
					geom = 0;
					
					strcpy(path, "Meshes/");
					strcat(path, meshName);
					
					geom = new InputGeom ();
					if (!geom || !geom->loadMesh(&ctx, path))
					{
						delete geom;
						geom = 0;
						
						showLog = true;
						logScroll = 0;
						ctx.dumpLog("Geom load log %s:", meshName.GetData ());
					}
					if (sample && geom)
					{
						sample->handleMeshChanged(geom);
					}

					ctx.resetLog();
					if (sample && !sample->handleBuild())
					{
						ctx.dumpLog("Build log %s:", meshName.GetData ());
					}
/*
					if (geom || sample)
					{
						const float* bmin = 0;
						const float* bmax = 0;
						if (sample)
						{
							bmin = sample->getBoundsMin();
							bmax = sample->getBoundsMax();
						}
						else if (geom)
						{
							bmin = geom->getMeshBoundsMin();
							bmax = geom->getMeshBoundsMax();
						}
						// Reset camera and fog to match the mesh bounds.
						if (bmin && bmax)
						{
							camr = sqrtf(rcSqr(bmax[0]-bmin[0]) +
										 rcSqr(bmax[1]-bmin[1]) +
										 rcSqr(bmax[2]-bmin[2])) / 2;
							camx = (bmax[0] + bmin[0]) / 2 + camr;
							camy = (bmax[1] + bmin[1]) / 2 + camr;
							camz = (bmax[2] + bmin[2]) / 2 + camr;
							camr *= 3;
						}
						rx = 45;
						ry = -45;
						glFogf(GL_FOG_START, camr*0.1f);
						glFogf(GL_FOG_END, camr*1.25f);
					}
*//*
					// Do the tests.
					if (sample)
						test->doTests(sample->getNavMesh(), sample->getNavMeshQuery());
				}
			}				

			imguiEndScrollArea();
		}
*/
	// Log
		if (showLog && showMenu)
		{
			if (imguiBeginScrollArea("Log", 250+20, 10, width - 300 - 250, 200, &logScroll))
				mouseOverMenu = true;
			for (int i = 0; i < ctx.getLogCount(); ++i)
				imguiLabel(ctx.getLogText(i));
			imguiEndScrollArea();
		}

		// Tools
		if (!showTestCases && showTools && showMenu) // && geom && sample)
		{
			if (imguiBeginScrollArea("Tools", 10, 10, 250, height-20, &toolsScroll))
				mouseOverMenu = true;

			if (sample)
				sample->handleTools();
			
			imguiEndScrollArea();
		}
		
		slideShow.updateAndDraw(dt, (float)width, (float)height);

		GLdouble x, y, z;

		// Marker
		if (mposSet && gluProject((GLdouble)mpos[0], (GLdouble)mpos[1], (GLdouble)mpos[2],
								  model, proj, view, &x, &y, &z))
		{
			// Draw marker circle
			g_drawInterface->glLineWidth(5.0f);
			g_drawInterface->glColor4ub(240,220,0,196);
			g_drawInterface->glBegin(GL_LINE_LOOP);
			float cosine, sine;
			const float r = 25.0f;
			for (int i = 0; i < 20; ++i)
			{
				const float a = (float)i / 20.0f * RC_PI*2;

				Math::GetCosineSine (a, cosine, sine);

				const float fx = (float)x + cosine*r;
				const float fy = (float)y + sine*r;
				g_drawInterface->glVertex2f(fx,fy);
			}
			g_drawInterface->glEnd();
			g_drawInterface->glLineWidth(1.0f);
		}

	imguiEndFrame();
	imguiRenderGLDraw();

	mscroll = 0;
	processHitTest = false;
	processHitTestShift = false;

	// Back to HL engine 2D mode....
	g_drawInterface->GL_Set2D ();

	g_drawInterface->glEnable (GL_TEXTURE_2D);
}

void RecastWrapper_GL::Think (void)
{
	/// @todo TRANSLATE HL ENGINE COORDINATE SYSTEM TO RECASTNAVIGATION, REMOVE recastToVec3() CALLS FROM DebugDrawGL CLASS!!!

	g_drawInterface->glEnable (GL_DEPTH_TEST);
///	g_drawInterface->glEnable (GL_FOG);
/*
	g_drawInterface->glPushMatrix ();
	g_drawInterface->glLoadIdentity ();
	g_drawInterface->glRotatef (90, 1, 0, 0);
	g_drawInterface->glRotatef (-90, 0, 0, 1);
*/
	// Get hit ray position and direction.
	{
/*	const GLdouble originalProj[16u] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.33, 0.0, 0.0, 0.0, 0.0, -1.0, -1.0, 0.0, 0.0, -2.0, 0.0};
	const GLdouble originalModel[16u] = {0.71, -0.5, 0.5, 0.0, 0.0, 0.71, 0.71, 0.0, -0.71, -0.5, 0.5, 0.0, -407.29, 359.42, -704.29, 1.0};
	MemoryCopy (originalProj, proj, GET_STATIC_ARRAY_SIZE (proj));
	MemoryCopy (originalModel, model, GET_STATIC_ARRAY_SIZE (model));
/*/	g_drawInterface->glGetDoublev(GL_PROJECTION_MATRIX, proj);
	g_drawInterface->glGetDoublev(GL_MODELVIEW_MATRIX, model);
	g_drawInterface->glGetIntegerv(GL_VIEWPORT, view);
	GLdouble x, y, z;
	gluUnProject(mx, my, 0.0f, model, proj, view, &x, &y, &z);
	rays.x = (float)x; rays.y = (float)z; rays.z = (float)y;
//	rays.x = (float)x; rays.y = (float)y; rays.z = (float)z;
	gluUnProject(mx, my, 1.0f, model, proj, view, &x, &y, &z);
	raye.x = (float)x; raye.y = (float)z; raye.z = (float)y;
//	raye.x = (float)x; raye.y = (float)y; raye.z = (float)z;
/*
static bool ALREADY_CALLED (false);
if (!ALREADY_CALLED && (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
{
	GLint matrixMode (0);
	g_drawInterface->glGetIntegerv (GL_MATRIX_MODE, &matrixMode);

		FILE *fp (fopen ("C://Users//Эдуард//Desktop//YAPB LOG.LOG", "at"));
		fprintf(fp,
		"proj[16] = {%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f},\n"
		"model[16] = {%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f},\n"
		"view[4] = {%i, %i, %i, %i},\n"
		"rays[3] = {%.2f, %.2f, %.2f},\n"
		"raye[3] = {%.2f, %.2f, %.2f},\n"
		"matrixMode = %s\n",
		proj[0], proj[1], proj[2], proj[3], proj[4], proj[5], proj[6], proj[7], proj[8], proj[9], proj[10], proj[11], proj[12], proj[13], proj[14], proj[15],
		model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10], model[11], model[12], model[13], model[14], model[15],
		view[0], view[1], view[2], view[3],
		rays[0], rays[1], rays[2],
		raye[0], raye[1], raye[2],
		matrixMode == GL_MODELVIEW ? "MODELVIEW" : matrixMode == GL_PROJECTION ? "PROJECTION" : matrixMode == GL_TEXTURE ? "TEXTURE" : "UNKNOWN!"
	);
		fclose (fp);
		ALREADY_CALLED = true;
}
else ALREADY_CALLED = false;
*/
//	g_drawInterface->glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
	}
/*	const GLdouble originalProj[16u] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.33, 0.0, 0.0, 0.0, 0.0, -1.0, -1.0, 0.0, 0.0, -2.0, 0.0};
	const GLdouble originalModel[16u] = {0.71, -0.5, 0.5, 0.0, 0.0, 0.71, 0.71, 0.0, -0.71, -0.5, 0.5, 0.0, -407.29, 359.42, -704.29, 1.0};
	MemoryCopy (originalProj, proj, GET_STATIC_ARRAY_SIZE (proj));
	MemoryCopy (originalModel, model, GET_STATIC_ARRAY_SIZE (model));
/*
	g_drawInterface->glPopMatrix ();

	g_drawInterface->glEnable (GL_DEPTH_TEST);
	g_drawInterface->glEnable (GL_CULL_FACE);
	g_drawInterface->glEnable (GL_BLEND);
*/
	if (sample)
		sample->handleRender ();

///	g_drawInterface->glDisable (GL_FOG);
}

const bool RecastWrapper_GL::MouseEvent (const int state, bool down)
{
	static int mouse_oldbuttonstate (0);

	// perform button actions
	for (int i=0 ; i < MOUSE_BUTTON_COUNT/*mouse_buttons*/; i++)
	{
/*		// Mouse buttons 1 & 2 are swallowed when the mouse is visible
		if ( (i < 2) && ( m_fCameraInterceptingMouse || vgui::surface()->IsCursorVisible() ) )
			continue;
*/
		// Only fire changed buttons
		if ( (state & (1<<i)) && !(mouse_oldbuttonstate & (1<<i)) )
		{
			/*if (*/KeyEvent (down/*true*/, K_MOUSE1 + i, NULL);//)
//				return true;
		}
		if ( !(state & (1<<i)) && (mouse_oldbuttonstate & (1<<i)) )
		{
			// Force 0 instead of down, because MouseMove calls this with down set to true.
			/*if (*/KeyEvent (false, K_MOUSE1 + i, NULL);//)
//				return true;
		}
	}

	mouse_oldbuttonstate = state;

	return false;
}

const bool RecastWrapper_GL::KeyEvent (const int down, const int keynum, const char *const/* pszCurrentBinding*/)
{
//	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("KeyEvent(): down=%i, keynum=%i\n", down, keynum);

	if (!IsActive ())
	{
		if (down && keynum == 'z')
		{
			m_isActive = true;

			return true;
		}

		return false;
	}

	if (down)
	{
		if (keynum == 'z')
		{
			m_isActive = !m_isActive;

			return true;
		}
		else if (keynum == 't')
		{
						showLevels = false;
						showSample = false;
						showTestCases = true;
						files.ScanDirectory ("Tests", ".txt");

						return true;
		}
					else if (keynum == K_TAB)
					{
						showMenu = !showMenu;

						return true;
					}
					else if (keynum == K_SPACE)
					{
						if (sample)
							sample->handleToggle();
						return true;
					}
					else if (keynum == '1')
					{
						if (sample)
							sample->handleStep();
						return true;
					}
					else if (keynum == '9')
					{
						if (geom)
							geom->save("geomset.txt");
						return true;
					}
					else if (keynum == '0')
					{
						delete geom;
						geom = new InputGeom ();
						if (!geom || !geom->load(&ctx, "geomset.txt"))
						{
							delete geom;
							geom = 0;
							
							showLog = true;
							logScroll = 0;
							ctx.dumpLog("Geom load log %s:", meshName.GetData ());
						}
						if (sample && geom)
						{
							sample->handleMeshChanged(geom);
						}
/*
						if (geom || sample)
						{
							const float* bmin = 0;
							const float* bmax = 0;
							if (sample)
							{
								bmin = sample->getBoundsMin();
								bmax = sample->getBoundsMax();
							}
							else if (geom)
							{
								bmin = geom->getMeshBoundsMin();
								bmax = geom->getMeshBoundsMax();
							}
							// Reset camera and fog to match the mesh bounds.
							if (bmin && bmax)
							{
								camr = sqrtf(rcSqr(bmax[0]-bmin[0]) +
											 rcSqr(bmax[1]-bmin[1]) +
											 rcSqr(bmax[2]-bmin[2])) / 2;
								camx = (bmax[0] + bmin[0]) / 2 + camr;
								camy = (bmax[1] + bmin[1]) / 2 + camr;
								camz = (bmax[2] + bmin[2]) / 2 + camr;
								camr *= 3;
							}
							rx = 45;
							ry = -45;
							glFogf(GL_FOG_START, camr*0.2f);
							glFogf(GL_FOG_END, camr*1.25f);
						}
*/
						return true;
					}
					else if (keynum == HalfLifeEngine::SDK::Constants::FROM_SDL::SDLK_RIGHT)
					{
						slideShow.nextSlide();
						return true;
					}
					else if (keynum == HalfLifeEngine::SDK::Constants::FROM_SDL::SDLK_LEFT)
					{
						slideShow.prevSlide();
						return true;
					}
					else if (keynum == K_MOUSE1)	// Mouse left button pushed
					{
						mbut |= IMGUI_MBUT_LEFT;
//						return true;
					}
					else if (keynum == K_MOUSE2)	// Mouse right button pushed
					{
						mbut |= IMGUI_MBUT_RIGHT;
//! FROM PARIS 2011
						if (g_currentSampleID == SampleItem::ID_AnnotationBuilder)
						{
								float t;
								if (geom->raycastMesh(rays, raye, t))
								{
									float p[3];
									dtVlerp(p, rays, raye, t);
									static_cast <AnnotationBuilder *> (sample)->buildNearestEdge(p);
								}
						}

//						return true;
					}
	}
	else
	{
					// Handle mouse clicks here.
					if (keynum == K_MOUSE1)	// Mouse left button pushed
					{
						mbut &= ~IMGUI_MBUT_LEFT;
						if (!mouseOverMenu)
						{
							processHitTest = true;
							processHitTestShift = /**(SDL_GetModState() & KMOD_SHIFT)*/false ? true : false;
						return true;
						}
					}
					else if (keynum == K_MOUSE2)	// Mouse right button pushed
					{
						mbut &= ~IMGUI_MBUT_RIGHT;
						if (!mouseOverMenu)
						{
							processHitTest = true;
							processHitTestShift = true;
						return true;
						}
					}
	}

					// Handle mouse clicks here.
					if (keynum == K_MWHEELDOWN)
					{
						if (mouseOverMenu)
						{
							++mscroll;
						return true;
						}
					}
					else if (keynum == K_MWHEELUP)
					{
						if (mouseOverMenu)
						{
							--mscroll;
						return true;
						}
					}

	return false;
}
#endif	// if defined MULTIPLE_DRAW_METHODS