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
// DrawTools.hpp
//
// Class: DrawInterface
//
// Version: $ID:$
//

#include <Core.hpp>

#if defined MULTIPLE_DRAW_METHODS
	DLL_GLOBAL DrawInterface_GL *g_drawInterface (NULL);
/*
void APIENTRY hooked_glPushMatrix (void)
{
//	g_drawInterface->glPushMatrix ();
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "HELLO EDWARD! v=[%.2f, %.2f, %.2f]", v[0], v[1], v[2]);
}
void APIENTRY hooked_glLineWidth (GLfloat width)
{
	g_drawInterface->glLineWidth (width);
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "hooked_glLineWidth(): width=%.2f.", width);
}
void APIENTRY hooked_glVertex3fv (const GLfloat *v)
{
	g_drawInterface->glVertex3fv (v);
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "HELLO EDWARD! v=[%.2f, %.2f, %.2f]", v[0], v[1], v[2]);
}
void APIENTRY hooked_glLoadIdentity (void)
{
//	g_drawInterface->glLoadIdentity ();
//	HalfLifeEngine::Globals::g_halfLifeEngine->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "HELLO EDWARD!");
}
void APIENTRY hooked_glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	if (angle == -90.0f && x == 1.0f && y == 0.0f && z == 0.0f)
		return;

	if (angle == 90.0f && x == 0.0f && y == 0.0f && z == 1.0f)
		return;

	g_drawInterface->glRotatef (angle, x, y, z);
}
void APIENTRY hooked_glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	InternalAssert (left == 0.0);
	InternalAssert (top == 0.0);

	g_drawInterface->glOrtho (0, right, 0, bottom, zNear, zFar);
//	g_drawInterface->glOrtho (0, right, 0, bottom, -1, 1);	// SAME AS ABOVE!!!
}
*//*
inline const bool OpenGLFunctionNameComparator (const char *const functionName1, const char *const functionName2)
{
	return strcmp (functionName1, functionName2) == 0;	// Just compare by full function name....
}
inline const bool D3DFunctionNameComparator (const char *const functionName1, const char *const functionName2)
{
	if (functionName1[0u] == '\0')
		return false;

	// Is "__stdcall" call declaration?
	if (functionName1[0u] != '_')
		return OpenGLFunctionNameComparator (functionName1, functionName2);

	return strncmp (functionName1 + sizeof ('-'), functionName2, GetStringLength <unsigned char> (functionName2)) == 0;	// Just compare by minimal length of function names....
}
*/
const bool DrawInterface_GL::CodeWalk (Trampolines::Bytecode::MoveESIToValue32_t *startAddress, const Trampolines::Bytecode::MoveESIToValue32_t *const endAddress)
{
	#if defined _DEBUG
		for (unsigned short functionIndex (1u/* Skip ponter to virtual table */); functionIndex <= FunctionID_Total; ++functionIndex)
			reinterpret_cast <void **> (this)[functionIndex] = NULL;	// Set all function pointers to NULL.
	#endif	// if defined _DEBUG
/*
	/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	const LibraryHandle hmOpenGL (HalfLifeEngine::Globals::g_halfLifeEngine->IsOpenGLRenderingMode () ? GetModuleHandle ("OpenGL32.DLL") : HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ());
//	const Trampolines::FunctionNameComparator_t comparator (HalfLifeEngine::Globals::g_halfLifeEngine->IsOpenGLRenderingMode () ? OpenGLFunctionNameComparator : D3DFunctionNameComparator);

	// Reliability check.
	if (!hmOpenGL.IsValid ())
		return false;
*/
	AddLogEntry (true, LogLevel_Default, false, "DrawInterface_GL::CodeWalk(): Start assigning OpenGL functions.... Start address=0x%X, End address=0x%X.", reinterpret_cast <unsigned char *const> (startAddress) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()), reinterpret_cast <const unsigned char *const> (endAddress) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()));

	for (/* Empty *//*unsigned short functionIndex (1u/* Skip ponter to virtual table *//*)*/; startAddress < endAddress; ++startAddress/*, ++functionIndex*/)
	{
		// Reliability checks.
		if (IsBadReadPtr (startAddress, sizeof (*startAddress)) || startAddress->opcodes[0u] != 0x89u || startAddress->opcodes[1u] != 0x35u || startAddress->pointer == NULL)
			return false;

		const unsigned short functionIndex (static_cast <unsigned short> (FunctionID_Total - (endAddress - startAddress) + 1u));	/// @warning THIS VARIABLE SHOULD BE REMOVED IF YOU USE PREVIOUS 'functionIndex' DEFINITION!!!
		void *&functionAddress = *startAddress->pointerToPointer;

//		AddLogEntry (true, LogLevel_Default, true, "Function index=%u, pointer=0x%X, pointerToFunctionAddress=0x%X, functionAddress=0x%X.", functionIndex, reinterpret_cast <unsigned char *const> (startAddress) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()), reinterpret_cast <unsigned char *const> (startAddress->pointer) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()), functionAddress);

		// Reliability check.
		if (functionAddress == NULL)	// SHOULD OCCURS ONLY FOR FUNCTIONS "glGetTexLevelParameterfv" AND "glGetTexLevelParameteriv"!!!
		{
			if (reinterpret_cast <glGetTexLevelParameterfv_t *> (this)[functionIndex] == glGetTexLevelParameterfv || reinterpret_cast <glGetTexLevelParameteriv_t *> (this)[functionIndex] == glGetTexLevelParameteriv)
				continue;
/*			// Quick and ugly fix.
			{
				if (HalfLifeEngine::Globals::g_halfLifeEngine->IsOpenGLRenderingMode ())
				{
					if (reinterpret_cast <glGetTexLevelParameterfv_t *> (this)[functionIndex] == glGetTexLevelParameterfv)
					{
						glGetTexLevelParameterfv = hmOpenGL.GetFunctionAddress <glGetTexLevelParameterfv_t> ("glGetTexLevelParameterfv");

						continue;
					}

					if (reinterpret_cast <glGetTexLevelParameteriv_t *> (this)[functionIndex] == glGetTexLevelParameteriv)
					{
						glGetTexLevelParameteriv = hmOpenGL.GetFunctionAddress <glGetTexLevelParameteriv_t> ("glGetTexLevelParameteriv");

						continue;
					}
				}
			}
*/
			// Else: print error message....
			AddLogEntry (true, LogLevel_Error, false,
			"Can't get address of OpenGL[%u] function! (Relative address: 0x%X, pointerToFunctionAddress: 0x%X)\n",
			functionIndex, reinterpret_cast <unsigned char *const> (startAddress) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()), reinterpret_cast <unsigned char *const> (startAddress->pointer) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()));

			return false;
		}

		// Just blindly copy all mapped function pointers from hw.DLL....
		{
			reinterpret_cast <void **> (this)[functionIndex] = functionAddress;

//			goto Label_TryRedirectThisFunction;
			continue;
		}
/*/
		#define GET_GL_FUNCTION_ADDRESS(name)										\
			/*if (functionAddress == hmOpenGL.GetFunctionAddress <void *> (#name))*//*	\
			if (functionAddress == hmOpenGL.GetFunctionAddressByRelativeName (#name, comparator))	\
			{																		\
				name = static_cast <name##_t> (functionAddress);					\
																					\
				/*goto Label_TryRedirectThisFunction;*//*									\
				continue;															\
			}

		GET_GL_FUNCTION_ADDRESS (glAccum);
		GET_GL_FUNCTION_ADDRESS (glAlphaFunc);
		GET_GL_FUNCTION_ADDRESS (glAreTexturesResident);
		GET_GL_FUNCTION_ADDRESS (glArrayElement);
		GET_GL_FUNCTION_ADDRESS (glBegin);
		GET_GL_FUNCTION_ADDRESS (glBindTexture);	/// @warning This function can take the pointer as well on "glBindTextureEXT" if expansion "GL_EXT_texture_object" is present.
		GET_GL_FUNCTION_ADDRESS (glBitmap);
		GET_GL_FUNCTION_ADDRESS (glBlendFunc);
		GET_GL_FUNCTION_ADDRESS (glCallList);
		GET_GL_FUNCTION_ADDRESS (glCallLists);
		GET_GL_FUNCTION_ADDRESS (glClear);
		GET_GL_FUNCTION_ADDRESS (glClearAccum);
		GET_GL_FUNCTION_ADDRESS (glClearColor);
		GET_GL_FUNCTION_ADDRESS (glClearDepth);
		GET_GL_FUNCTION_ADDRESS (glClearIndex);
		GET_GL_FUNCTION_ADDRESS (glClearStencil);
		GET_GL_FUNCTION_ADDRESS (glClipPlane);
		GET_GL_FUNCTION_ADDRESS (glColor3b);
		GET_GL_FUNCTION_ADDRESS (glColor3bv);
		GET_GL_FUNCTION_ADDRESS (glColor3d);
		GET_GL_FUNCTION_ADDRESS (glColor3dv);
		GET_GL_FUNCTION_ADDRESS (glColor3f);
		GET_GL_FUNCTION_ADDRESS (glColor3fv);
		GET_GL_FUNCTION_ADDRESS (glColor3i);
		GET_GL_FUNCTION_ADDRESS (glColor3iv);
		GET_GL_FUNCTION_ADDRESS (glColor3s);
		GET_GL_FUNCTION_ADDRESS (glColor3sv);
		GET_GL_FUNCTION_ADDRESS (glColor3ub);
		GET_GL_FUNCTION_ADDRESS (glColor3ubv);
		GET_GL_FUNCTION_ADDRESS (glColor3ui);
		GET_GL_FUNCTION_ADDRESS (glColor3uiv);
		GET_GL_FUNCTION_ADDRESS (glColor3us);
		GET_GL_FUNCTION_ADDRESS (glColor3usv);
		GET_GL_FUNCTION_ADDRESS (glColor4b);
		GET_GL_FUNCTION_ADDRESS (glColor4bv);
		GET_GL_FUNCTION_ADDRESS (glColor4d);
		GET_GL_FUNCTION_ADDRESS (glColor4dv);
		GET_GL_FUNCTION_ADDRESS (glColor4f);
		GET_GL_FUNCTION_ADDRESS (glColor4fv);
		GET_GL_FUNCTION_ADDRESS (glColor4i);
		GET_GL_FUNCTION_ADDRESS (glColor4iv);
		GET_GL_FUNCTION_ADDRESS (glColor4s);
		GET_GL_FUNCTION_ADDRESS (glColor4sv);
		GET_GL_FUNCTION_ADDRESS (glColor4ub);
		GET_GL_FUNCTION_ADDRESS (glColor4ubv);
		GET_GL_FUNCTION_ADDRESS (glColor4ui);
		GET_GL_FUNCTION_ADDRESS (glColor4uiv);
		GET_GL_FUNCTION_ADDRESS (glColor4us);
		GET_GL_FUNCTION_ADDRESS (glColor4usv);
		GET_GL_FUNCTION_ADDRESS (glColorMask);
		GET_GL_FUNCTION_ADDRESS (glColorMaterial);
		GET_GL_FUNCTION_ADDRESS (glColorPointer);
		GET_GL_FUNCTION_ADDRESS (glCopyPixels);
		GET_GL_FUNCTION_ADDRESS (glCopyTexImage1D);
		GET_GL_FUNCTION_ADDRESS (glCopyTexImage2D);
		GET_GL_FUNCTION_ADDRESS (glCopyTexSubImage1D);
		GET_GL_FUNCTION_ADDRESS (glCopyTexSubImage2D);
		GET_GL_FUNCTION_ADDRESS (glCullFace);
		GET_GL_FUNCTION_ADDRESS (glDeleteLists);
		GET_GL_FUNCTION_ADDRESS (glDeleteTextures);
		GET_GL_FUNCTION_ADDRESS (glDepthFunc);
		GET_GL_FUNCTION_ADDRESS (glDepthMask);
		GET_GL_FUNCTION_ADDRESS (glDepthRange);
		GET_GL_FUNCTION_ADDRESS (glDisable);
		GET_GL_FUNCTION_ADDRESS (glDisableClientState);
		GET_GL_FUNCTION_ADDRESS (glDrawArrays);
		GET_GL_FUNCTION_ADDRESS (glDrawBuffer);
		GET_GL_FUNCTION_ADDRESS (glDrawElements);
		GET_GL_FUNCTION_ADDRESS (glDrawPixels);
		GET_GL_FUNCTION_ADDRESS (glEdgeFlag);
		GET_GL_FUNCTION_ADDRESS (glEdgeFlagPointer);
		GET_GL_FUNCTION_ADDRESS (glEdgeFlagv);
		GET_GL_FUNCTION_ADDRESS (glEnable);
		GET_GL_FUNCTION_ADDRESS (glEnableClientState);
		GET_GL_FUNCTION_ADDRESS (glEnd);
		GET_GL_FUNCTION_ADDRESS (glEndList);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord1d);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord1dv);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord1f);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord1fv);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord2d);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord2dv);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord2f);
		GET_GL_FUNCTION_ADDRESS (glEvalCoord2fv);
		GET_GL_FUNCTION_ADDRESS (glEvalMesh1);
		GET_GL_FUNCTION_ADDRESS (glEvalMesh2);
		GET_GL_FUNCTION_ADDRESS (glEvalPoint1);
		GET_GL_FUNCTION_ADDRESS (glEvalPoint2);
		GET_GL_FUNCTION_ADDRESS (glFeedbackBuffer);
		GET_GL_FUNCTION_ADDRESS (glFinish);
		GET_GL_FUNCTION_ADDRESS (glFlush);
		GET_GL_FUNCTION_ADDRESS (glFogf);
		GET_GL_FUNCTION_ADDRESS (glFogfv);
		GET_GL_FUNCTION_ADDRESS (glFogi);
		GET_GL_FUNCTION_ADDRESS (glFogiv);
		GET_GL_FUNCTION_ADDRESS (glFrontFace);
		GET_GL_FUNCTION_ADDRESS (glFrustum);
		GET_GL_FUNCTION_ADDRESS (glGenLists);
		GET_GL_FUNCTION_ADDRESS (glGenTextures);
		GET_GL_FUNCTION_ADDRESS (glGetBooleanv);
		GET_GL_FUNCTION_ADDRESS (glGetClipPlane);
		GET_GL_FUNCTION_ADDRESS (glGetDoublev);
		GET_GL_FUNCTION_ADDRESS (glGetError);
		GET_GL_FUNCTION_ADDRESS (glGetFloatv);
		GET_GL_FUNCTION_ADDRESS (glGetIntegerv);
		GET_GL_FUNCTION_ADDRESS (glGetLightfv);
		GET_GL_FUNCTION_ADDRESS (glGetLightiv);
		GET_GL_FUNCTION_ADDRESS (glGetMapdv);
		GET_GL_FUNCTION_ADDRESS (glGetMapfv);
		GET_GL_FUNCTION_ADDRESS (glGetMapiv);
		GET_GL_FUNCTION_ADDRESS (glGetMaterialfv);
		GET_GL_FUNCTION_ADDRESS (glGetMaterialiv);
		GET_GL_FUNCTION_ADDRESS (glGetPixelMapfv);
		GET_GL_FUNCTION_ADDRESS (glGetPixelMapuiv);
		GET_GL_FUNCTION_ADDRESS (glGetPixelMapusv);
		GET_GL_FUNCTION_ADDRESS (glGetPointerv);
		GET_GL_FUNCTION_ADDRESS (glGetPolygonStipple);
		GET_GL_FUNCTION_ADDRESS (glGetString);
		GET_GL_FUNCTION_ADDRESS (glGetTexEnvfv);
		GET_GL_FUNCTION_ADDRESS (glGetTexEnviv);
		GET_GL_FUNCTION_ADDRESS (glGetTexGendv);
		GET_GL_FUNCTION_ADDRESS (glGetTexGenfv);
		GET_GL_FUNCTION_ADDRESS (glGetTexGeniv);
		GET_GL_FUNCTION_ADDRESS (glGetTexImage);
//		GET_GL_FUNCTION_ADDRESS (glGetTexLevelParameterfv);	/// @warning This function is always NULL in Half-Life engine because: For OpenGL: the engine tries to get pointer to this function by WRONG name "glGetLevelParameterfv"!!! / For Direct3D: always just NULL.... (Don't ask why....)
//		GET_GL_FUNCTION_ADDRESS (glGetTexLevelParameteriv);	/// @warning This function is always NULL in Half-Life engine because: For OpenGL: the engine tries to get pointer to this function by WRONG name "glGetLevelParameteriv"!!! / For Direct3D: always just NULL.... (Don't ask why....)
		GET_GL_FUNCTION_ADDRESS (glGetTexParameterfv);
		GET_GL_FUNCTION_ADDRESS (glGetTexParameteriv);
		GET_GL_FUNCTION_ADDRESS (glHint);
		GET_GL_FUNCTION_ADDRESS (glIndexMask);
		GET_GL_FUNCTION_ADDRESS (glIndexPointer);
		GET_GL_FUNCTION_ADDRESS (glIndexd);
		GET_GL_FUNCTION_ADDRESS (glIndexdv);
		GET_GL_FUNCTION_ADDRESS (glIndexf);
		GET_GL_FUNCTION_ADDRESS (glIndexfv);
		GET_GL_FUNCTION_ADDRESS (glIndexi);
		GET_GL_FUNCTION_ADDRESS (glIndexiv);
		GET_GL_FUNCTION_ADDRESS (glIndexs);
		GET_GL_FUNCTION_ADDRESS (glIndexsv);
		GET_GL_FUNCTION_ADDRESS (glIndexub);
		GET_GL_FUNCTION_ADDRESS (glIndexubv);
		GET_GL_FUNCTION_ADDRESS (glInitNames);
		GET_GL_FUNCTION_ADDRESS (glInterleavedArrays);
		GET_GL_FUNCTION_ADDRESS (glIsEnabled);
		GET_GL_FUNCTION_ADDRESS (glIsList);
		GET_GL_FUNCTION_ADDRESS (glIsTexture);
		GET_GL_FUNCTION_ADDRESS (glLightModelf);
		GET_GL_FUNCTION_ADDRESS (glLightModelfv);
		GET_GL_FUNCTION_ADDRESS (glLightModeli);
		GET_GL_FUNCTION_ADDRESS (glLightModeliv);
		GET_GL_FUNCTION_ADDRESS (glLightf);
		GET_GL_FUNCTION_ADDRESS (glLightfv);
		GET_GL_FUNCTION_ADDRESS (glLighti);
		GET_GL_FUNCTION_ADDRESS (glLightiv);
		GET_GL_FUNCTION_ADDRESS (glLineStipple);
		GET_GL_FUNCTION_ADDRESS (glLineWidth);
		GET_GL_FUNCTION_ADDRESS (glListBase);
		GET_GL_FUNCTION_ADDRESS (glLoadIdentity);
		GET_GL_FUNCTION_ADDRESS (glLoadMatrixd);
		GET_GL_FUNCTION_ADDRESS (glLoadMatrixf);
		GET_GL_FUNCTION_ADDRESS (glLoadName);
		GET_GL_FUNCTION_ADDRESS (glLogicOp);
		GET_GL_FUNCTION_ADDRESS (glMap1d);
		GET_GL_FUNCTION_ADDRESS (glMap1f);
		GET_GL_FUNCTION_ADDRESS (glMap2d);
		GET_GL_FUNCTION_ADDRESS (glMap2f);
		GET_GL_FUNCTION_ADDRESS (glMapGrid1d);
		GET_GL_FUNCTION_ADDRESS (glMapGrid1f);
		GET_GL_FUNCTION_ADDRESS (glMapGrid2d);
		GET_GL_FUNCTION_ADDRESS (glMapGrid2f);
		GET_GL_FUNCTION_ADDRESS (glMaterialf);
		GET_GL_FUNCTION_ADDRESS (glMaterialfv);
		GET_GL_FUNCTION_ADDRESS (glMateriali);
		GET_GL_FUNCTION_ADDRESS (glMaterialiv);
		GET_GL_FUNCTION_ADDRESS (glMatrixMode);
		GET_GL_FUNCTION_ADDRESS (glMultMatrixd);
		GET_GL_FUNCTION_ADDRESS (glMultMatrixf);
		GET_GL_FUNCTION_ADDRESS (glNewList);
		GET_GL_FUNCTION_ADDRESS (glNormal3b);
		GET_GL_FUNCTION_ADDRESS (glNormal3bv);
		GET_GL_FUNCTION_ADDRESS (glNormal3d);
		GET_GL_FUNCTION_ADDRESS (glNormal3dv);
		GET_GL_FUNCTION_ADDRESS (glNormal3f);
		GET_GL_FUNCTION_ADDRESS (glNormal3fv);
		GET_GL_FUNCTION_ADDRESS (glNormal3i);
		GET_GL_FUNCTION_ADDRESS (glNormal3iv);
		GET_GL_FUNCTION_ADDRESS (glNormal3s);
		GET_GL_FUNCTION_ADDRESS (glNormal3sv);
		GET_GL_FUNCTION_ADDRESS (glNormalPointer);
		GET_GL_FUNCTION_ADDRESS (glOrtho);
		GET_GL_FUNCTION_ADDRESS (glPassThrough);
		GET_GL_FUNCTION_ADDRESS (glPixelMapfv);
		GET_GL_FUNCTION_ADDRESS (glPixelMapuiv);
		GET_GL_FUNCTION_ADDRESS (glPixelMapusv);
		GET_GL_FUNCTION_ADDRESS (glPixelStoref);
		GET_GL_FUNCTION_ADDRESS (glPixelStorei);
		GET_GL_FUNCTION_ADDRESS (glPixelTransferf);
		GET_GL_FUNCTION_ADDRESS (glPixelTransferi);
		GET_GL_FUNCTION_ADDRESS (glPixelZoom);
		GET_GL_FUNCTION_ADDRESS (glPointSize);
		GET_GL_FUNCTION_ADDRESS (glPolygonMode);
		GET_GL_FUNCTION_ADDRESS (glPolygonOffset);
		GET_GL_FUNCTION_ADDRESS (glPolygonStipple);
		GET_GL_FUNCTION_ADDRESS (glPopAttrib);
		GET_GL_FUNCTION_ADDRESS (glPopClientAttrib);
		GET_GL_FUNCTION_ADDRESS (glPopMatrix);
		GET_GL_FUNCTION_ADDRESS (glPopName);
		GET_GL_FUNCTION_ADDRESS (glPrioritizeTextures);
		GET_GL_FUNCTION_ADDRESS (glPushAttrib);
		GET_GL_FUNCTION_ADDRESS (glPushClientAttrib);
		GET_GL_FUNCTION_ADDRESS (glPushMatrix);
		GET_GL_FUNCTION_ADDRESS (glPushName);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2d);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2dv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2f);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2fv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2i);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2iv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2s);
		GET_GL_FUNCTION_ADDRESS (glRasterPos2sv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3d);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3dv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3f);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3fv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3i);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3iv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3s);
		GET_GL_FUNCTION_ADDRESS (glRasterPos3sv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4d);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4dv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4f);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4fv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4i);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4iv);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4s);
		GET_GL_FUNCTION_ADDRESS (glRasterPos4sv);
		GET_GL_FUNCTION_ADDRESS (glReadBuffer);
		GET_GL_FUNCTION_ADDRESS (glReadPixels);
		GET_GL_FUNCTION_ADDRESS (glRectd);
		GET_GL_FUNCTION_ADDRESS (glRectdv);
		GET_GL_FUNCTION_ADDRESS (glRectf);
		GET_GL_FUNCTION_ADDRESS (glRectfv);
		GET_GL_FUNCTION_ADDRESS (glRecti);
		GET_GL_FUNCTION_ADDRESS (glRectiv);
		GET_GL_FUNCTION_ADDRESS (glRects);
		GET_GL_FUNCTION_ADDRESS (glRectsv);
		GET_GL_FUNCTION_ADDRESS (glRenderMode);
		GET_GL_FUNCTION_ADDRESS (glRotated);
		GET_GL_FUNCTION_ADDRESS (glRotatef);
		GET_GL_FUNCTION_ADDRESS (glScaled);
		GET_GL_FUNCTION_ADDRESS (glScalef);
		GET_GL_FUNCTION_ADDRESS (glScissor);
		GET_GL_FUNCTION_ADDRESS (glSelectBuffer);
		GET_GL_FUNCTION_ADDRESS (glShadeModel);
		GET_GL_FUNCTION_ADDRESS (glStencilFunc);
		GET_GL_FUNCTION_ADDRESS (glStencilMask);
		GET_GL_FUNCTION_ADDRESS (glStencilOp);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1d);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1dv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1f);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1fv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1i);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1iv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1s);
		GET_GL_FUNCTION_ADDRESS (glTexCoord1sv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2d);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2dv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2f);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2fv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2i);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2iv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2s);
		GET_GL_FUNCTION_ADDRESS (glTexCoord2sv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3d);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3dv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3f);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3fv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3i);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3iv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3s);
		GET_GL_FUNCTION_ADDRESS (glTexCoord3sv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4d);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4dv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4f);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4fv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4i);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4iv);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4s);
		GET_GL_FUNCTION_ADDRESS (glTexCoord4sv);
		GET_GL_FUNCTION_ADDRESS (glTexCoordPointer);
		GET_GL_FUNCTION_ADDRESS (glTexEnvf);
		GET_GL_FUNCTION_ADDRESS (glTexEnvfv);
		GET_GL_FUNCTION_ADDRESS (glTexEnvi);
		GET_GL_FUNCTION_ADDRESS (glTexEnviv);
		GET_GL_FUNCTION_ADDRESS (glTexGend);
		GET_GL_FUNCTION_ADDRESS (glTexGendv);
		GET_GL_FUNCTION_ADDRESS (glTexGenf);
		GET_GL_FUNCTION_ADDRESS (glTexGenfv);
		GET_GL_FUNCTION_ADDRESS (glTexGeni);
		GET_GL_FUNCTION_ADDRESS (glTexGeniv);
		GET_GL_FUNCTION_ADDRESS (glTexImage1D);
		GET_GL_FUNCTION_ADDRESS (glTexImage2D);
		GET_GL_FUNCTION_ADDRESS (glTexParameterf);
		GET_GL_FUNCTION_ADDRESS (glTexParameterfv);
		GET_GL_FUNCTION_ADDRESS (glTexParameteri);
		GET_GL_FUNCTION_ADDRESS (glTexParameteriv);
		GET_GL_FUNCTION_ADDRESS (glTexSubImage1D);
		GET_GL_FUNCTION_ADDRESS (glTexSubImage2D);
		GET_GL_FUNCTION_ADDRESS (glTranslated);
		GET_GL_FUNCTION_ADDRESS (glTranslatef);
		GET_GL_FUNCTION_ADDRESS (glVertex2d);
		GET_GL_FUNCTION_ADDRESS (glVertex2dv);
		GET_GL_FUNCTION_ADDRESS (glVertex2f);
		GET_GL_FUNCTION_ADDRESS (glVertex2fv);
		GET_GL_FUNCTION_ADDRESS (glVertex2i);
		GET_GL_FUNCTION_ADDRESS (glVertex2iv);
		GET_GL_FUNCTION_ADDRESS (glVertex2s);
		GET_GL_FUNCTION_ADDRESS (glVertex2sv);
		GET_GL_FUNCTION_ADDRESS (glVertex3d);
		GET_GL_FUNCTION_ADDRESS (glVertex3dv);
		GET_GL_FUNCTION_ADDRESS (glVertex3f);
		GET_GL_FUNCTION_ADDRESS (glVertex3fv);
		GET_GL_FUNCTION_ADDRESS (glVertex3i);
		GET_GL_FUNCTION_ADDRESS (glVertex3iv);
		GET_GL_FUNCTION_ADDRESS (glVertex3s);
		GET_GL_FUNCTION_ADDRESS (glVertex3sv);
		GET_GL_FUNCTION_ADDRESS (glVertex4d);
		GET_GL_FUNCTION_ADDRESS (glVertex4dv);
		GET_GL_FUNCTION_ADDRESS (glVertex4f);
		GET_GL_FUNCTION_ADDRESS (glVertex4fv);
		GET_GL_FUNCTION_ADDRESS (glVertex4i);
		GET_GL_FUNCTION_ADDRESS (glVertex4iv);
		GET_GL_FUNCTION_ADDRESS (glVertex4s);
		GET_GL_FUNCTION_ADDRESS (glVertex4sv);
		GET_GL_FUNCTION_ADDRESS (glVertexPointer);
		GET_GL_FUNCTION_ADDRESS (glViewport);

		GET_GL_FUNCTION_ADDRESS (wglCopyContext);
		GET_GL_FUNCTION_ADDRESS (wglCreateContext);
		GET_GL_FUNCTION_ADDRESS (wglCreateLayerContext);
		GET_GL_FUNCTION_ADDRESS (wglDeleteContext);
		GET_GL_FUNCTION_ADDRESS (wglDescribeLayerPlane);
		GET_GL_FUNCTION_ADDRESS (wglGetCurrentContext);
		GET_GL_FUNCTION_ADDRESS (wglGetCurrentDC);
		GET_GL_FUNCTION_ADDRESS (wglGetLayerPaletteEntries);
		GET_GL_FUNCTION_ADDRESS (wglGetProcAddress);
		GET_GL_FUNCTION_ADDRESS (wglMakeCurrent);
		GET_GL_FUNCTION_ADDRESS (wglRealizeLayerPalette);
		GET_GL_FUNCTION_ADDRESS (wglSetLayerPaletteEntries);
		GET_GL_FUNCTION_ADDRESS (wglShareLists);
		GET_GL_FUNCTION_ADDRESS (wglSwapLayerBuffers);
		GET_GL_FUNCTION_ADDRESS (wglUseFontBitmapsA);
		GET_GL_FUNCTION_ADDRESS (wglUseFontOutlinesA);

		GET_GL_FUNCTION_ADDRESS (wglChoosePixelFormat);
		GET_GL_FUNCTION_ADDRESS (wglSetPixelFormat);

		if (HalfLifeEngine::Globals::g_halfLifeEngine->IsOpenGLRenderingMode ())
		{
			goto Label_AssignAsIs;
//			GET_GL_FUNCTION_ADDRESS (wglDescribePixelFormat);
//			GET_GL_FUNCTION_ADDRESS (wglGetPixelFormat);
//			GET_GL_FUNCTION_ADDRESS (wglSwapBuffers);
		}

		#undef GET_GL_FUNCTION_ADDRESS

		// Else: print error message....
		AddLogEntry (true, LogLevel_Error, false,
		"Can't detect address of OpenGL[%u] function! (Relative address: 0x%X, pointerToFunctionAddress: 0x%X, functionAddress: 0x%X)\n"
			"\tWell just try to assign a pointer to the mapped function....",
		functionIndex, reinterpret_cast <unsigned char *const> (startAddress) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()), reinterpret_cast <unsigned char *const> (startAddress->pointer) - reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()), functionAddress);

		Label_AssignAsIs:
		// Reliability check.
		InternalAssert (reinterpret_cast <void **> (this)[functionIndex] == NULL);

		reinterpret_cast <void **> (this)[functionIndex] = functionAddress;	// Assign as is....

		continue;
/*
		Label_TryRedirectThisFunction:
		{
			#define REDIRECT_GL_FUNCTION_ADDRESS(name)									\
				if (functionAddress == hmOpenGL.GetFunctionAddress <void *> (#name))	\
				{																		\
					functionAddress = &hooked_##name;									\
																						\
					continue;															\
				}

//			REDIRECT_GL_FUNCTION_ADDRESS (glBegin);
//			REDIRECT_GL_FUNCTION_ADDRESS (glBlendFunc);
//			REDIRECT_GL_FUNCTION_ADDRESS (glClear);
//			REDIRECT_GL_FUNCTION_ADDRESS (glDisable);
//			REDIRECT_GL_FUNCTION_ADDRESS (glEnable);
//			REDIRECT_GL_FUNCTION_ADDRESS (glPushMatrix);
//			REDIRECT_GL_FUNCTION_ADDRESS (glPopMatrix);
//			REDIRECT_GL_FUNCTION_ADDRESS (glVertex3f);
//			REDIRECT_GL_FUNCTION_ADDRESS (glVertex3fv);
//			REDIRECT_GL_FUNCTION_ADDRESS (wglSwapBuffers);
//			REDIRECT_GL_FUNCTION_ADDRESS (glPointSize);
//			REDIRECT_GL_FUNCTION_ADDRESS (glLineWidth);
//			REDIRECT_GL_FUNCTION_ADDRESS (glColor3b);
//			REDIRECT_GL_FUNCTION_ADDRESS (glColor4ubv);
//			REDIRECT_GL_FUNCTION_ADDRESS (glEnd);
//			REDIRECT_GL_FUNCTION_ADDRESS (glLoadIdentity);
//			REDIRECT_GL_FUNCTION_ADDRESS (glRotatef);
			REDIRECT_GL_FUNCTION_ADDRESS (glOrtho);

			#undef REDIRECT_GL_FUNCTION_ADDRESS
		}*/
	}

	AddLogEntry (true, LogLevel_Default, false, "DrawInterface_GL::CodeWalk(): Done! All %u OpenGL functions successfully assigned.", FunctionID_Total);

	return true;
}
const bool DrawInterface_GL::Initialize (void)
{
	// Reliability check.
	CompileTimeAssert (sizeof (*this) == Trampolines::Bytecode::Pointer32Size/* Virtual table ponter */ + FunctionID_Total * Trampolines::Bytecode::Pointer32Size + sizeof (GL_BindFunctionPointer) + sizeof (GL_Set2DFunctionPointer) + sizeof (GL_Set3DFunctionPointer) + sizeof (m_instantDrawObjects) + sizeof (m_timedDrawObjects) + sizeof (m_tex));

	const unsigned char signature[] =
	{
		Trampolines::Bytecode::MoveValue32ToEAX, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,	// mov  eax, 'glw_state.hinstOpenGL'
		Trampolines::Bytecode::PushESI,																																					// push esi
		0x33, 0xF6,																																										// xor  esi, esi        ; Logical Exclusive OR
		0x3B, 0xC6,																																										// cmp  eax, esi        ; Compare Two Operands
		0x74, 0x07,																																										// jz   short loc_1D862A3 ; Jump if Zero (ZF=1)
		Trampolines::Bytecode::PushEAX,																																					// push eax
		0xFF, 0x15, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard									// call ds:FreeLibrary  ; Indirect Call Near Procedure
	};	// From hw.DLL (Address: 0x1D86290)
	unsigned char *const QGL_ShutdownFunction (static_cast <unsigned char *const> (MemoryUtilities::/*DecodeAnd*/FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), signature, sizeof (signature))));	/// @note This function is unused by Half-Life engine!

	// Reliability check.
	if (QGL_ShutdownFunction == NULL)
		return false;

	Trampolines::Bytecode::MoveESIToValue32_t *const importTableStartAddress (reinterpret_cast <Trampolines::Bytecode::MoveESIToValue32_t *> (QGL_ShutdownFunction + sizeof (signature) + Trampolines::Bytecode::MoveESIToValue32Size));	// Skip 'signature' and "mov 'glw_state.hinstOpenGL', esi" ("glw_state.hinstOpenGL = NULL;") (Address: 0x1D862A9)
	const Trampolines::Bytecode::MoveESIToValue32_t *const importTableEndAddress (importTableStartAddress + FunctionID_Total);	// (Address: 0x1D86B07)

	if (!CodeWalk (importTableStartAddress, importTableEndAddress))
		return false;
/*
	#define WRAP_OGL_FUNC(type, func)	\
		func = reinterpret_cast <type> (GetProcAddress (hOgl, #func));	\
		\
		if (func == NULL)	\
			MessageBox (0, #func "'s address was not found - terminating", "YaPB Error", 0);	\

//	glDisable		= (gli)				GetProcAddress(hOgl, "glDisable"); // <-- Original Format
//	unsigned int b = (unsigned int)GetProcAddress (GetModuleHandle ("opengl32.dll"), "wglSwapBuffers");

	// Main OGL functions -> Hooked
	// Will give error 'Missing ; before GetProcAddress' if store is not a correct typedef and is a glFunction already
	// WRAP_OGL_FUNC(gl		,gl);
	WRAP_OGL_FUNC (glBegin_t,     glBegin);
	WRAP_OGL_FUNC (glEnd_t,       glEnd);
	WRAP_OGL_FUNC (glDepthMask_t, glDepthMask);
	WRAP_OGL_FUNC (glPointSize_t, glPointSize);
	WRAP_OGL_FUNC (glLineWidth_t, glLineWidth);
	WRAP_OGL_FUNC (glColor3b_t,   glColor3b);
	WRAP_OGL_FUNC (glColor4ubv_t, glColor4ubv);
	WRAP_OGL_FUNC (glVertex3fv_t, glVertex3fv);

	WRAP_OGL_FUNC (glEnable_t, glEnable);
	WRAP_OGL_FUNC (glDisable_t, glDisable);
	WRAP_OGL_FUNC (glPushMatrix_t, glPushMatrix);
	WRAP_OGL_FUNC (glPopMatrix_t, glPopMatrix);
	WRAP_OGL_FUNC (glLoadIdentity_t, glLoadIdentity);*/

	// Other shit....
	{
		const unsigned int base (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()));

		GL_BindFunctionPointer = static_cast <GL_BindFunctionPointer_t> (Trampolines::GetRealAddressOfRelativeAddress32 (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->SpriteTexture) + GET_STATIC_STRING_LENGTH ("\xE8\x6B\xA9\xF6\xFF\x8B\x4C\x24\x04\x8B\x44\x24\x08\x50\x8B\x91\x84\x01\x00\x00\x52\xE8\x46\x49\xF9\xFF\x83\xC4\x08\x85\xC0\x75\x01\xC3\x8B\x40\x18\x50\xE8"))));
//		GL_BindFunctionPointer = reinterpret_cast <GL_BindFunctionPointer_t> (base + 0x477E0u);
		GL_Set2DFunctionPointer = reinterpret_cast <GL_Set2DFunctionPointer_t> (base + 0x5BCF0u);
		GL_Set3DFunctionPointer = reinterpret_cast <GL_Set3DFunctionPointer_t> (base + 0x5BEF0u);

		// Reliability checks.
		InternalAssert (GL_BindFunctionPointer != NULL);
		InternalAssert (GL_Set2DFunctionPointer != NULL);
		InternalAssert (GL_Set3DFunctionPointer != NULL);

		glBindTexture = glBindTextureWrapper;	// Don't save original OpenGL function pointer....
	}

	return true;
}
#endif	// if defined MULTIPLE_DRAW_METHODS