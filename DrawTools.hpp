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

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef DRAW_TOOLS_INCLUDED
#define DRAW_TOOLS_INCLUDED

// Abstrace draw interface.
abstract_class DrawInterface
{
	public:
		enum Type_t
		{
			Type_Points,
			Type_Lines,
			Type_Triangles,
			Type_Quads
		};

		// Simple draw object....
		abstract_class Object : public NoCopy
		{
			public:
				virtual inline ~Object (void) { /* VOID */ }

			public:
				virtual inline void Draw (void) = 0;
		};
		class TimedObject : public Link, public Object
		{
			protected:
				float m_endTime;	// Time when does this object go away.

			public:
				inline TimedObject (TimedObject *const next = NULL) : Link (next) { /* VOID */ }
				inline TimedObject (const float duration, TimedObject *const next = NULL) : Link (next), m_endTime (GetTime () + duration) { /* VOID */ }

				virtual inline ~TimedObject (void) { /* VOID */ }

			private:
				static inline const float GetTime (void);	// For some customization.... (Note: This function declared in Core.hpp)

			public:
				inline       void  SetDuration (const float duration)       { m_endTime = GetTime () + duration; }
				inline const float GetEndTime  (void)                 const { return m_endTime; }
				inline const bool  IsDead      (void)                 const { return GetEndTime () <= GetTime (); }
		};

	public:
		virtual inline ~DrawInterface (void) { /* VOID */ }

	public:
		virtual const bool Initialize (void) = 0;

		virtual inline void DepthMask (const bool state) const = 0;

		// Begin drawing primitives.
		// Params:
		//  type - (in) primitive type to draw, one of rcDebugDrawPrimitives.
		//  nverts - (in) number of vertices to be submitted.
		//  size - (in) size of a primitive, applies to point size and line width only.
		virtual inline void Begin          (const Type_t type, const unsigned char size = 1u) const = 0;
		virtual inline void BeginPoints    (const unsigned char size = 1u) const = 0;
		virtual inline void BeginLines     (const unsigned char size = 1u) const = 0;
		virtual inline void BeginTriangles (void)                          const = 0;
		virtual inline void BeginQuads     (void)                          const = 0;

		// Submit a vertex
		// Params:
		//  position - (in) position of the verts.
		//  color - (in) color of the verts.
		virtual inline void SubmitVertex (const Math::Vector3D &position, const ColorWithAlpha <> &color) const = 0;
		virtual inline void SubmitVertex (const Math::Vector3D &position, const ColorWithAlpha <> &color, const Math::Vector2D &uv) const = 0;

		// End drawing primitives.
		virtual inline void End (void) const = 0;

		virtual inline void TextureInternally (const bool state) = 0;
};

#define GL_GLEXT_PROTOTYPES 1

// Include OpenGL Headers
#include <GL/gl.h>		// Header File For The OpenGL Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <GL/glext.h>	// Header File For The Glext Library

// Include OpenGL Libs
#pragma comment (lib, "GLu32.lib")	// Link OpenGL Utility Library
#pragma message ("Linking to library file: GLu32.lib....")

class DrawInterface_GL;

extern DLL_GLOBAL DrawInterface_GL *g_drawInterface;

// OpenGL debug draw implementation.
class DrawInterface_GL : public DrawInterface
{
	public:
		enum FunctionID_t
		{
			FunctionID_GL_Total = 336u,
			FunctionID_WGL_Total = 21u,

			FunctionID_Total = FunctionID_GL_Total + FunctionID_WGL_Total
		};

	public:
		#define DECLARE_FUNCTION(name, APIType, returnElementType, arguments)	\
			typedef returnElementType (APIType *name##_t) arguments;			\
			name##_t name;

		#define DECLARE_GL_FUNCTION(name, returnElementType, arguments)	\
			DECLARE_FUNCTION (name, APIENTRY, returnElementType, arguments);

		#define DECLARE_WGL_FUNCTION(name, returnElementType, arguments)	\
			DECLARE_FUNCTION (name, WINAPI, returnElementType, arguments);

		// GL Function Type Definitions....
		DECLARE_GL_FUNCTION (glAccum,                               void, (GLenum op, GLfloat value));
		DECLARE_GL_FUNCTION (glAlphaFunc,                           void, (GLenum func, GLclampf ref));
		DECLARE_GL_FUNCTION (glAreTexturesResident,            GLboolean, (GLsizei n, const GLuint *textures, GLboolean *residences));
		DECLARE_GL_FUNCTION (glArrayElement,                        void, (GLint i));
		DECLARE_GL_FUNCTION (glBegin,                               void, (GLenum mode));
		DECLARE_GL_FUNCTION (glBindTexture,                         void, (GLenum target, GLuint texture));
		DECLARE_GL_FUNCTION (glBitmap,                              void, (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap));
		DECLARE_GL_FUNCTION (glBlendFunc,                           void, (GLenum sfactor, GLenum dfactor));
		DECLARE_GL_FUNCTION (glCallList,                            void, (GLuint list));
		DECLARE_GL_FUNCTION (glCallLists,                           void, (GLsizei n, GLenum type, const GLvoid *lists));
		DECLARE_GL_FUNCTION (glClear,                               void, (GLbitfield mask));
		DECLARE_GL_FUNCTION (glClearAccum,                          void, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
		DECLARE_GL_FUNCTION (glClearColor,                          void, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
		DECLARE_GL_FUNCTION (glClearDepth,                          void, (GLclampd depth));
		DECLARE_GL_FUNCTION (glClearIndex,                          void, (GLfloat c));
		DECLARE_GL_FUNCTION (glClearStencil,                        void, (GLint s));
		DECLARE_GL_FUNCTION (glClipPlane,                           void, (GLenum plane, const GLdouble *equation));
		DECLARE_GL_FUNCTION (glColor3b,                             void, (GLbyte red, GLbyte green, GLbyte blue));
		DECLARE_GL_FUNCTION (glColor3bv,                            void, (const GLbyte *v));
		DECLARE_GL_FUNCTION (glColor3d,                             void, (GLdouble red, GLdouble green, GLdouble blue));
		DECLARE_GL_FUNCTION (glColor3dv,                            void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glColor3f,                             void, (GLfloat red, GLfloat green, GLfloat blue));
		DECLARE_GL_FUNCTION (glColor3fv,                            void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glColor3i,                             void, (GLint red, GLint green, GLint blue));
		DECLARE_GL_FUNCTION (glColor3iv,                            void, (const GLint *v));
		DECLARE_GL_FUNCTION (glColor3s,                             void, (GLshort red, GLshort green, GLshort blue));
		DECLARE_GL_FUNCTION (glColor3sv,                            void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glColor3ub,                            void, (GLubyte red, GLubyte green, GLubyte blue));
		DECLARE_GL_FUNCTION (glColor3ubv,                           void, (const GLubyte *v));
		DECLARE_GL_FUNCTION (glColor3ui,                            void, (GLuint red, GLuint green, GLuint blue));
		DECLARE_GL_FUNCTION (glColor3uiv,                           void, (const GLuint *v));
		DECLARE_GL_FUNCTION (glColor3us,                            void, (GLushort red, GLushort green, GLushort blue));
		DECLARE_GL_FUNCTION (glColor3usv,                           void, (const GLushort *v));
		DECLARE_GL_FUNCTION (glColor4b,                             void, (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha));
		DECLARE_GL_FUNCTION (glColor4bv,                            void, (const GLbyte *v));
		DECLARE_GL_FUNCTION (glColor4d,                             void, (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha));
		DECLARE_GL_FUNCTION (glColor4dv,                            void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glColor4f,                             void, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
		DECLARE_GL_FUNCTION (glColor4fv,                            void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glColor4i,                             void, (GLint red, GLint green, GLint blue, GLint alpha));
		DECLARE_GL_FUNCTION (glColor4iv,                            void, (const GLint *v));
		DECLARE_GL_FUNCTION (glColor4s,                             void, (GLshort red, GLshort green, GLshort blue, GLshort alpha));
		DECLARE_GL_FUNCTION (glColor4sv,                            void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glColor4ub,                            void, (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha));
		DECLARE_GL_FUNCTION (glColor4ubv,                           void, (const GLubyte *v));
		DECLARE_GL_FUNCTION (glColor4ui,                            void, (GLuint red, GLuint green, GLuint blue, GLuint alpha));
		DECLARE_GL_FUNCTION (glColor4uiv,                           void, (const GLuint *v));
		DECLARE_GL_FUNCTION (glColor4us,                            void, (GLushort red, GLushort green, GLushort blue, GLushort alpha));
		DECLARE_GL_FUNCTION (glColor4usv,                           void, (const GLushort *v));
		DECLARE_GL_FUNCTION (glColorMask,                           void, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha));
		DECLARE_GL_FUNCTION (glColorMaterial,                       void, (GLenum face, GLenum mode));
		DECLARE_GL_FUNCTION (glColorPointer,                        void, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glCopyPixels,                          void, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type));
		DECLARE_GL_FUNCTION (glCopyTexImage1D,                      void, (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border));
		DECLARE_GL_FUNCTION (glCopyTexImage2D,                      void, (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border));
		DECLARE_GL_FUNCTION (glCopyTexSubImage1D,                   void, (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width));
		DECLARE_GL_FUNCTION (glCopyTexSubImage2D,                   void, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height));
		DECLARE_GL_FUNCTION (glCullFace,                            void, (GLenum mode));
		DECLARE_GL_FUNCTION (glDeleteLists,                         void, (GLuint list, GLsizei range));
		DECLARE_GL_FUNCTION (glDeleteTextures,                      void, (GLsizei n, const GLuint *textures));
		DECLARE_GL_FUNCTION (glDepthFunc,                           void, (GLenum func));
		DECLARE_GL_FUNCTION (glDepthMask,                           void, (GLboolean flag));
		DECLARE_GL_FUNCTION (glDepthRange,                          void, (GLclampd zNear, GLclampd zFar));
		DECLARE_GL_FUNCTION (glDisable,                             void, (GLenum cap));
		DECLARE_GL_FUNCTION (glDisableClientState,                  void, (GLenum array));
		DECLARE_GL_FUNCTION (glDrawArrays,                          void, (GLenum mode, GLint first, GLsizei count));
		DECLARE_GL_FUNCTION (glDrawBuffer,                          void, (GLenum mode));
		DECLARE_GL_FUNCTION (glDrawElements,                        void, (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices));
		DECLARE_GL_FUNCTION (glDrawPixels,                          void, (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels));
		DECLARE_GL_FUNCTION (glEdgeFlag,                            void, (GLboolean flag));
		DECLARE_GL_FUNCTION (glEdgeFlagPointer,                     void, (GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glEdgeFlagv,                           void, (const GLboolean *flag));
		DECLARE_GL_FUNCTION (glEnable,                              void, (GLenum cap));
		DECLARE_GL_FUNCTION (glEnableClientState,                   void, (GLenum array));
		DECLARE_GL_FUNCTION (glEnd,                                 void, (void));
		DECLARE_GL_FUNCTION (glEndList,                             void, (void));
		DECLARE_GL_FUNCTION (glEvalCoord1d,                         void, (GLdouble u));
		DECLARE_GL_FUNCTION (glEvalCoord1dv,                        void, (const GLdouble *u));
		DECLARE_GL_FUNCTION (glEvalCoord1f,                         void, (GLfloat u));
		DECLARE_GL_FUNCTION (glEvalCoord1fv,                        void, (const GLfloat *u));
		DECLARE_GL_FUNCTION (glEvalCoord2d,                         void, (GLdouble u, GLdouble v));
		DECLARE_GL_FUNCTION (glEvalCoord2dv,                        void, (const GLdouble *u));
		DECLARE_GL_FUNCTION (glEvalCoord2f,                         void, (GLfloat u, GLfloat v));
		DECLARE_GL_FUNCTION (glEvalCoord2fv,                        void, (const GLfloat *u));
		DECLARE_GL_FUNCTION (glEvalMesh1,                           void, (GLenum mode, GLint i1, GLint i2));
		DECLARE_GL_FUNCTION (glEvalMesh2,                           void, (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2));
		DECLARE_GL_FUNCTION (glEvalPoint1,                          void, (GLint i));
		DECLARE_GL_FUNCTION (glEvalPoint2,                          void, (GLint i, GLint j));
		DECLARE_GL_FUNCTION (glFeedbackBuffer,                      void, (GLsizei size, GLenum type, GLfloat *buffer));
		DECLARE_GL_FUNCTION (glFinish,                              void, (void));
		DECLARE_GL_FUNCTION (glFlush,                               void, (void));
		DECLARE_GL_FUNCTION (glFogf,                                void, (GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glFogfv,                               void, (GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glFogi,                                void, (GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glFogiv,                               void, (GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glFrontFace,                           void, (GLenum mode));
		DECLARE_GL_FUNCTION (glFrustum,                             void, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar));
		DECLARE_GL_FUNCTION (glGenLists,                          GLuint, (GLsizei range));
		DECLARE_GL_FUNCTION (glGenTextures,                         void, (GLsizei n, GLuint *textures));
		DECLARE_GL_FUNCTION (glGetBooleanv,                         void, (GLenum pname, GLboolean *params));
		DECLARE_GL_FUNCTION (glGetClipPlane,                        void, (GLenum plane, GLdouble *equation));
		DECLARE_GL_FUNCTION (glGetDoublev,                          void, (GLenum pname, GLdouble *params));
		DECLARE_GL_FUNCTION (glGetError,                          GLenum, (void));
		DECLARE_GL_FUNCTION (glGetFloatv,                           void, (GLenum pname, GLfloat *params));
		DECLARE_GL_FUNCTION (glGetIntegerv,                         void, (GLenum pname, GLint *params));
		DECLARE_GL_FUNCTION (glGetLightfv,                          void, (GLenum light, GLenum pname, GLfloat *params));
		DECLARE_GL_FUNCTION (glGetLightiv,                          void, (GLenum light, GLenum pname, GLint *params));
		DECLARE_GL_FUNCTION (glGetMapdv,                            void, (GLenum target, GLenum query, GLdouble *v));
		DECLARE_GL_FUNCTION (glGetMapfv,                            void, (GLenum target, GLenum query, GLfloat *v));
		DECLARE_GL_FUNCTION (glGetMapiv,                            void, (GLenum target, GLenum query, GLint *v));
		DECLARE_GL_FUNCTION (glGetMaterialfv,                       void, (GLenum face, GLenum pname, GLfloat *params));
		DECLARE_GL_FUNCTION (glGetMaterialiv,                       void, (GLenum face, GLenum pname, GLint *params));
		DECLARE_GL_FUNCTION (glGetPixelMapfv,                       void, (GLenum map, GLfloat *values));
		DECLARE_GL_FUNCTION (glGetPixelMapuiv,                      void, (GLenum map, GLuint *values));
		DECLARE_GL_FUNCTION (glGetPixelMapusv,                      void, (GLenum map, GLushort *values));
		DECLARE_GL_FUNCTION (glGetPointerv,                         void, (GLenum pname, GLvoid* *params));
		DECLARE_GL_FUNCTION (glGetPolygonStipple,                   void, (GLubyte *mask));
		DECLARE_GL_FUNCTION (glGetString,                const GLubyte *, (GLenum name));
		DECLARE_GL_FUNCTION (glGetTexEnvfv,                         void, (GLenum target, GLenum pname, GLfloat *params));
		DECLARE_GL_FUNCTION (glGetTexEnviv,                         void, (GLenum target, GLenum pname, GLint *params));
		DECLARE_GL_FUNCTION (glGetTexGendv,                         void, (GLenum coord, GLenum pname, GLdouble *params));
		DECLARE_GL_FUNCTION (glGetTexGenfv,                         void, (GLenum coord, GLenum pname, GLfloat *params));
		DECLARE_GL_FUNCTION (glGetTexGeniv,                         void, (GLenum coord, GLenum pname, GLint *params));
		DECLARE_GL_FUNCTION (glGetTexImage,                         void, (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels));
		DECLARE_GL_FUNCTION (glGetTexLevelParameterfv,              void, (GLenum target, GLint level, GLenum pname, GLfloat *params));	/// @note Dummy NULL pointer.
		DECLARE_GL_FUNCTION (glGetTexLevelParameteriv,              void, (GLenum target, GLint level, GLenum pname, GLint *params));	/// @note Dummy NULL pointer.
		DECLARE_GL_FUNCTION (glGetTexParameterfv,                   void, (GLenum target, GLenum pname, GLfloat *params));
		DECLARE_GL_FUNCTION (glGetTexParameteriv,                   void, (GLenum target, GLenum pname, GLint *params));
		DECLARE_GL_FUNCTION (glHint,                                void, (GLenum target, GLenum mode));
		DECLARE_GL_FUNCTION (glIndexMask,                           void, (GLuint mask));
		DECLARE_GL_FUNCTION (glIndexPointer,                        void, (GLenum type, GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glIndexd,                              void, (GLdouble c));
		DECLARE_GL_FUNCTION (glIndexdv,                             void, (const GLdouble *c));
		DECLARE_GL_FUNCTION (glIndexf,                              void, (GLfloat c));
		DECLARE_GL_FUNCTION (glIndexfv,                             void, (const GLfloat *c));
		DECLARE_GL_FUNCTION (glIndexi,                              void, (GLint c));
		DECLARE_GL_FUNCTION (glIndexiv,                             void, (const GLint *c));
		DECLARE_GL_FUNCTION (glIndexs,                              void, (GLshort c));
		DECLARE_GL_FUNCTION (glIndexsv,                             void, (const GLshort *c));
		DECLARE_GL_FUNCTION (glIndexub,                             void, (GLubyte c));
		DECLARE_GL_FUNCTION (glIndexubv,                            void, (const GLubyte *c));
		DECLARE_GL_FUNCTION (glInitNames,                           void, (void));
		DECLARE_GL_FUNCTION (glInterleavedArrays,                   void, (GLenum format, GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glIsEnabled,                      GLboolean, (GLenum cap));
		DECLARE_GL_FUNCTION (glIsList,                         GLboolean, (GLuint list));
		DECLARE_GL_FUNCTION (glIsTexture,                      GLboolean, (GLuint texture));
		DECLARE_GL_FUNCTION (glLightModelf,                         void, (GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glLightModelfv,                        void, (GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glLightModeli,                         void, (GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glLightModeliv,                        void, (GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glLightf,                              void, (GLenum light, GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glLightfv,                             void, (GLenum light, GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glLighti,                              void, (GLenum light, GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glLightiv,                             void, (GLenum light, GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glLineStipple,                         void, (GLint factor, GLushort pattern));
		DECLARE_GL_FUNCTION (glLineWidth,                           void, (GLfloat width));
		DECLARE_GL_FUNCTION (glListBase,                            void, (GLuint base));
		DECLARE_GL_FUNCTION (glLoadIdentity,                        void, (void));
		DECLARE_GL_FUNCTION (glLoadMatrixd,                         void, (const GLdouble *m));
		DECLARE_GL_FUNCTION (glLoadMatrixf,                         void, (const GLfloat *m));
		DECLARE_GL_FUNCTION (glLoadName,                            void, (GLuint name));
		DECLARE_GL_FUNCTION (glLogicOp,                             void, (GLenum opcode));
		DECLARE_GL_FUNCTION (glMap1d,                               void, (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points));
		DECLARE_GL_FUNCTION (glMap1f,                               void, (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points));
		DECLARE_GL_FUNCTION (glMap2d,                               void, (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points));
		DECLARE_GL_FUNCTION (glMap2f,                               void, (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points));
		DECLARE_GL_FUNCTION (glMapGrid1d,                           void, (GLint un, GLdouble u1, GLdouble u2));
		DECLARE_GL_FUNCTION (glMapGrid1f,                           void, (GLint un, GLfloat u1, GLfloat u2));
		DECLARE_GL_FUNCTION (glMapGrid2d,                           void, (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2));
		DECLARE_GL_FUNCTION (glMapGrid2f,                           void, (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2));
		DECLARE_GL_FUNCTION (glMaterialf,                           void, (GLenum face, GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glMaterialfv,                          void, (GLenum face, GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glMateriali,                           void, (GLenum face, GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glMaterialiv,                          void, (GLenum face, GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glMatrixMode,                          void, (GLenum mode));
		DECLARE_GL_FUNCTION (glMultMatrixd,                         void, (const GLdouble *m));
		DECLARE_GL_FUNCTION (glMultMatrixf,                         void, (const GLfloat *m));
		DECLARE_GL_FUNCTION (glNewList,                             void, (GLuint list, GLenum mode));
		DECLARE_GL_FUNCTION (glNormal3b,                            void, (GLbyte nx, GLbyte ny, GLbyte nz));
		DECLARE_GL_FUNCTION (glNormal3bv,                           void, (const GLbyte *v));
		DECLARE_GL_FUNCTION (glNormal3d,                            void, (GLdouble nx, GLdouble ny, GLdouble nz));
		DECLARE_GL_FUNCTION (glNormal3dv,                           void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glNormal3f,                            void, (GLfloat nx, GLfloat ny, GLfloat nz));
		DECLARE_GL_FUNCTION (glNormal3fv,                           void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glNormal3i,                            void, (GLint nx, GLint ny, GLint nz));
		DECLARE_GL_FUNCTION (glNormal3iv,                           void, (const GLint *v));
		DECLARE_GL_FUNCTION (glNormal3s,                            void, (GLshort nx, GLshort ny, GLshort nz));
		DECLARE_GL_FUNCTION (glNormal3sv,                           void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glNormalPointer,                       void, (GLenum type, GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glOrtho,                               void, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar));
		DECLARE_GL_FUNCTION (glPassThrough,                         void, (GLfloat token));
		DECLARE_GL_FUNCTION (glPixelMapfv,                          void, (GLenum map, GLsizei mapsize, const GLfloat *values));
		DECLARE_GL_FUNCTION (glPixelMapuiv,                         void, (GLenum map, GLsizei mapsize, const GLuint *values));
		DECLARE_GL_FUNCTION (glPixelMapusv,                         void, (GLenum map, GLsizei mapsize, const GLushort *values));
		DECLARE_GL_FUNCTION (glPixelStoref,                         void, (GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glPixelStorei,                         void, (GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glPixelTransferf,                      void, (GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glPixelTransferi,                      void, (GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glPixelZoom,                           void, (GLfloat xfactor, GLfloat yfactor));
		DECLARE_GL_FUNCTION (glPointSize,                           void, (GLfloat size));
		DECLARE_GL_FUNCTION (glPolygonMode,                         void, (GLenum face, GLenum mode));
		DECLARE_GL_FUNCTION (glPolygonOffset,                       void, (GLfloat factor, GLfloat units));
		DECLARE_GL_FUNCTION (glPolygonStipple,                      void, (const GLubyte *mask));
		DECLARE_GL_FUNCTION (glPopAttrib,                           void, (void));
		DECLARE_GL_FUNCTION (glPopClientAttrib,                     void, (void));
		DECLARE_GL_FUNCTION (glPopMatrix,                           void, (void));
		DECLARE_GL_FUNCTION (glPopName,                             void, (void));
		DECLARE_GL_FUNCTION (glPrioritizeTextures,                  void, (GLsizei n, const GLuint *textures, const GLclampf *priorities));
		DECLARE_GL_FUNCTION (glPushAttrib,                          void, (GLbitfield mask));
		DECLARE_GL_FUNCTION (glPushClientAttrib,                    void, (GLbitfield mask));
		DECLARE_GL_FUNCTION (glPushMatrix,                          void, (void));
		DECLARE_GL_FUNCTION (glPushName,                            void, (GLuint name));
		DECLARE_GL_FUNCTION (glRasterPos2d,                         void, (GLdouble x, GLdouble y));
		DECLARE_GL_FUNCTION (glRasterPos2dv,                        void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glRasterPos2f,                         void, (GLfloat x, GLfloat y));
		DECLARE_GL_FUNCTION (glRasterPos2fv,                        void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glRasterPos2i,                         void, (GLint x, GLint y));
		DECLARE_GL_FUNCTION (glRasterPos2iv,                        void, (const GLint *v));
		DECLARE_GL_FUNCTION (glRasterPos2s,                         void, (GLshort x, GLshort y));
		DECLARE_GL_FUNCTION (glRasterPos2sv,                        void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glRasterPos3d,                         void, (GLdouble x, GLdouble y, GLdouble z));
		DECLARE_GL_FUNCTION (glRasterPos3dv,                        void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glRasterPos3f,                         void, (GLfloat x, GLfloat y, GLfloat z));
		DECLARE_GL_FUNCTION (glRasterPos3fv,                        void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glRasterPos3i,                         void, (GLint x, GLint y, GLint z));
		DECLARE_GL_FUNCTION (glRasterPos3iv,                        void, (const GLint *v));
		DECLARE_GL_FUNCTION (glRasterPos3s,                         void, (GLshort x, GLshort y, GLshort z));
		DECLARE_GL_FUNCTION (glRasterPos3sv,                        void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glRasterPos4d,                         void, (GLdouble x, GLdouble y, GLdouble z, GLdouble w));
		DECLARE_GL_FUNCTION (glRasterPos4dv,                        void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glRasterPos4f,                         void, (GLfloat x, GLfloat y, GLfloat z, GLfloat w));
		DECLARE_GL_FUNCTION (glRasterPos4fv,                        void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glRasterPos4i,                         void, (GLint x, GLint y, GLint z, GLint w));
		DECLARE_GL_FUNCTION (glRasterPos4iv,                        void, (const GLint *v));
		DECLARE_GL_FUNCTION (glRasterPos4s,                         void, (GLshort x, GLshort y, GLshort z, GLshort w));
		DECLARE_GL_FUNCTION (glRasterPos4sv,                        void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glReadBuffer,                          void, (GLenum mode));
		DECLARE_GL_FUNCTION (glReadPixels,                          void, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels));
		DECLARE_GL_FUNCTION (glRectd,                               void, (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2));
		DECLARE_GL_FUNCTION (glRectdv,                              void, (const GLdouble *v1, const GLdouble *v2));
		DECLARE_GL_FUNCTION (glRectf,                               void, (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2));
		DECLARE_GL_FUNCTION (glRectfv,                              void, (const GLfloat *v1, const GLfloat *v2));
		DECLARE_GL_FUNCTION (glRecti,                               void, (GLint x1, GLint y1, GLint x2, GLint y2));
		DECLARE_GL_FUNCTION (glRectiv,                              void, (const GLint *v1, const GLint *v2));
		DECLARE_GL_FUNCTION (glRects,                               void, (GLshort x1, GLshort y1, GLshort x2, GLshort y2));
		DECLARE_GL_FUNCTION (glRectsv,                              void, (const GLshort *v1, const GLshort *v2));
		DECLARE_GL_FUNCTION (glRenderMode,                         GLint, (GLenum mode));
		DECLARE_GL_FUNCTION (glRotated,                             void, (GLdouble angle, GLdouble x, GLdouble y, GLdouble z));
		DECLARE_GL_FUNCTION (glRotatef,                             void, (GLfloat angle, GLfloat x, GLfloat y, GLfloat z));
		DECLARE_GL_FUNCTION (glScaled,                              void, (GLdouble x, GLdouble y, GLdouble z));
		DECLARE_GL_FUNCTION (glScalef,                              void, (GLfloat x, GLfloat y, GLfloat z));
		DECLARE_GL_FUNCTION (glScissor,                             void, (GLint x, GLint y, GLsizei width, GLsizei height));
		DECLARE_GL_FUNCTION (glSelectBuffer,                        void, (GLsizei size, GLuint *buffer));
		DECLARE_GL_FUNCTION (glShadeModel,                          void, (GLenum mode));
		DECLARE_GL_FUNCTION (glStencilFunc,                         void, (GLenum func, GLint ref, GLuint mask));
		DECLARE_GL_FUNCTION (glStencilMask,                         void, (GLuint mask));
		DECLARE_GL_FUNCTION (glStencilOp,                           void, (GLenum fail, GLenum zfail, GLenum zpass));
		DECLARE_GL_FUNCTION (glTexCoord1d,                          void, (GLdouble s));
		DECLARE_GL_FUNCTION (glTexCoord1dv,                         void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glTexCoord1f,                          void, (GLfloat s));
		DECLARE_GL_FUNCTION (glTexCoord1fv,                         void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glTexCoord1i,                          void, (GLint s));
		DECLARE_GL_FUNCTION (glTexCoord1iv,                         void, (const GLint *v));
		DECLARE_GL_FUNCTION (glTexCoord1s,                          void, (GLshort s));
		DECLARE_GL_FUNCTION (glTexCoord1sv,                         void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glTexCoord2d,                          void, (GLdouble s, GLdouble t));
		DECLARE_GL_FUNCTION (glTexCoord2dv,                         void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glTexCoord2f,                          void, (GLfloat s, GLfloat t));
		DECLARE_GL_FUNCTION (glTexCoord2fv,                         void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glTexCoord2i,                          void, (GLint s, GLint t));
		DECLARE_GL_FUNCTION (glTexCoord2iv,                         void, (const GLint *v));
		DECLARE_GL_FUNCTION (glTexCoord2s,                          void, (GLshort s, GLshort t));
		DECLARE_GL_FUNCTION (glTexCoord2sv,                         void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glTexCoord3d,                          void, (GLdouble s, GLdouble t, GLdouble r));
		DECLARE_GL_FUNCTION (glTexCoord3dv,                         void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glTexCoord3f,                          void, (GLfloat s, GLfloat t, GLfloat r));
		DECLARE_GL_FUNCTION (glTexCoord3fv,                         void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glTexCoord3i,                          void, (GLint s, GLint t, GLint r));
		DECLARE_GL_FUNCTION (glTexCoord3iv,                         void, (const GLint *v));
		DECLARE_GL_FUNCTION (glTexCoord3s,                          void, (GLshort s, GLshort t, GLshort r));
		DECLARE_GL_FUNCTION (glTexCoord3sv,                         void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glTexCoord4d,                          void, (GLdouble s, GLdouble t, GLdouble r, GLdouble q));
		DECLARE_GL_FUNCTION (glTexCoord4dv,                         void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glTexCoord4f,                          void, (GLfloat s, GLfloat t, GLfloat r, GLfloat q));
		DECLARE_GL_FUNCTION (glTexCoord4fv,                         void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glTexCoord4i,                          void, (GLint s, GLint t, GLint r, GLint q));
		DECLARE_GL_FUNCTION (glTexCoord4iv,                         void, (const GLint *v));
		DECLARE_GL_FUNCTION (glTexCoord4s,                          void, (GLshort s, GLshort t, GLshort r, GLshort q));
		DECLARE_GL_FUNCTION (glTexCoord4sv,                         void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glTexCoordPointer,                     void, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glTexEnvf,                             void, (GLenum target, GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glTexEnvfv,                            void, (GLenum target, GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glTexEnvi,                             void, (GLenum target, GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glTexEnviv,                            void, (GLenum target, GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glTexGend,                             void, (GLenum coord, GLenum pname, GLdouble param));
		DECLARE_GL_FUNCTION (glTexGendv,                            void, (GLenum coord, GLenum pname, const GLdouble *params));
		DECLARE_GL_FUNCTION (glTexGenf,                             void, (GLenum coord, GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glTexGenfv,                            void, (GLenum coord, GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glTexGeni,                             void, (GLenum coord, GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glTexGeniv,                            void, (GLenum coord, GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glTexImage1D,                          void, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels));
		DECLARE_GL_FUNCTION (glTexImage2D,                          void, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels));
		DECLARE_GL_FUNCTION (glTexParameterf,                       void, (GLenum target, GLenum pname, GLfloat param));
		DECLARE_GL_FUNCTION (glTexParameterfv,                      void, (GLenum target, GLenum pname, const GLfloat *params));
		DECLARE_GL_FUNCTION (glTexParameteri,                       void, (GLenum target, GLenum pname, GLint param));
		DECLARE_GL_FUNCTION (glTexParameteriv,                      void, (GLenum target, GLenum pname, const GLint *params));
		DECLARE_GL_FUNCTION (glTexSubImage1D,                       void, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels));
		DECLARE_GL_FUNCTION (glTexSubImage2D,                       void, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels));
		DECLARE_GL_FUNCTION (glTranslated,                          void, (GLdouble x, GLdouble y, GLdouble z));
		DECLARE_GL_FUNCTION (glTranslatef,                          void, (GLfloat x, GLfloat y, GLfloat z));
		DECLARE_GL_FUNCTION (glVertex2d,                            void, (GLdouble x, GLdouble y));
		DECLARE_GL_FUNCTION (glVertex2dv,                           void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glVertex2f,                            void, (GLfloat x, GLfloat y));
		DECLARE_GL_FUNCTION (glVertex2fv,                           void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glVertex2i,                            void, (GLint x, GLint y));
		DECLARE_GL_FUNCTION (glVertex2iv,                           void, (const GLint *v));
		DECLARE_GL_FUNCTION (glVertex2s,                            void, (GLshort x, GLshort y));
		DECLARE_GL_FUNCTION (glVertex2sv,                           void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glVertex3d,                            void, (GLdouble x, GLdouble y, GLdouble z));
		DECLARE_GL_FUNCTION (glVertex3dv,                           void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glVertex3f,                            void, (GLfloat x, GLfloat y, GLfloat z));
		DECLARE_GL_FUNCTION (glVertex3fv,                           void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glVertex3i,                            void, (GLint x, GLint y, GLint z));
		DECLARE_GL_FUNCTION (glVertex3iv,                           void, (const GLint *v));
		DECLARE_GL_FUNCTION (glVertex3s,                            void, (GLshort x, GLshort y, GLshort z));
		DECLARE_GL_FUNCTION (glVertex3sv,                           void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glVertex4d,                            void, (GLdouble x, GLdouble y, GLdouble z, GLdouble w));
		DECLARE_GL_FUNCTION (glVertex4dv,                           void, (const GLdouble *v));
		DECLARE_GL_FUNCTION (glVertex4f,                            void, (GLfloat x, GLfloat y, GLfloat z, GLfloat w));
		DECLARE_GL_FUNCTION (glVertex4fv,                           void, (const GLfloat *v));
		DECLARE_GL_FUNCTION (glVertex4i,                            void, (GLint x, GLint y, GLint z, GLint w));
		DECLARE_GL_FUNCTION (glVertex4iv,                           void, (const GLint *v));
		DECLARE_GL_FUNCTION (glVertex4s,                            void, (GLshort x, GLshort y, GLshort z, GLshort w));
		DECLARE_GL_FUNCTION (glVertex4sv,                           void, (const GLshort *v));
		DECLARE_GL_FUNCTION (glVertexPointer,                       void, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
		DECLARE_GL_FUNCTION (glViewport,                            void, (GLint x, GLint y, GLsizei width, GLsizei height));

		DECLARE_WGL_FUNCTION (wglCopyContext,                       BOOL, (HGLRC, HGLRC, UINT));
		DECLARE_WGL_FUNCTION (wglCreateContext,                    HGLRC, (HDC));
		DECLARE_WGL_FUNCTION (wglCreateLayerContext,                void, (HDC, int));
		DECLARE_WGL_FUNCTION (wglDeleteContext,                     BOOL, (HGLRC));
		DECLARE_WGL_FUNCTION (wglDescribeLayerPlane,                BOOL, (HDC, int, int, UINT, LPLAYERPLANEDESCRIPTOR));
		DECLARE_WGL_FUNCTION (wglGetCurrentContext,                HGLRC, (VOID));
		DECLARE_WGL_FUNCTION (wglGetCurrentDC,                       HDC, (VOID));
		DECLARE_WGL_FUNCTION (wglGetLayerPaletteEntries,             int, (HDC, int, int, int, COLORREF *));
		DECLARE_WGL_FUNCTION (wglGetProcAddress,                    PROC, (LPCSTR));
		DECLARE_WGL_FUNCTION (wglMakeCurrent,                       BOOL, (HDC, HGLRC));
		DECLARE_WGL_FUNCTION (wglRealizeLayerPalette,               BOOL, (HDC, int, BOOL));
		DECLARE_WGL_FUNCTION (wglSetLayerPaletteEntries,             int, (HDC, int, int, int, CONST COLORREF *));
		DECLARE_WGL_FUNCTION (wglShareLists,                        BOOL, (HGLRC, HGLRC));
		DECLARE_WGL_FUNCTION (wglSwapLayerBuffers,                  BOOL, (HDC, UINT));
		DECLARE_WGL_FUNCTION (wglUseFontBitmapsA,                   BOOL, (HDC, DWORD, DWORD, DWORD));	// dword_275DCC4
		DECLARE_WGL_FUNCTION (wglUseFontOutlinesA,                  BOOL, (HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT));

		DECLARE_WGL_FUNCTION (wglChoosePixelFormat,                  int, (HDC, CONST PIXELFORMATDESCRIPTOR *));
		DECLARE_WGL_FUNCTION (wglDescribePixelFormat,                int, (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR));
		DECLARE_WGL_FUNCTION (wglGetPixelFormat,                     int, (HDC));
		DECLARE_WGL_FUNCTION (wglSetPixelFormat,                    BOOL, (HDC, int, CONST PIXELFORMATDESCRIPTOR *));
		DECLARE_WGL_FUNCTION (wglSwapBuffers,                       BOOL, (HDC));

		/// @note Below functions are not included, they unused by Half-Life engine.
		// qwglSwapIntervalEXT
		// qwglGetDeviceGammaRampEXT
		// qwglSetDeviceGammaRampEXT

		#undef DECLARE_FUNCTION
		#undef DECLARE_GL_FUNCTION
		#undef DECLARE_WGL_FUNCTION

		///@{
		// Engine functions....
		typedef void (*GL_BindFunctionPointer_t) (const int texnum);
		GL_BindFunctionPointer_t GL_BindFunctionPointer;
		inline void GL_Bind (const int texnum) const { (*GL_BindFunctionPointer) (texnum); }
		static void APIENTRY glBindTextureWrapper (GLenum target, GLuint texture)
		{
			/// @note THIS IS JUST A QUICK CRASH FIX!!!

			// Reliability check.
			InternalAssert (target == GL_TEXTURE_2D);

			// Call engine GL_Bind() function instead of OpenGL glBindTexture() function....
			g_drawInterface->GL_Bind (texture);
		}

		typedef void (*GL_Set2DFunctionPointer_t) (void);
		GL_Set2DFunctionPointer_t GL_Set2DFunctionPointer;
		inline void GL_Set2D (void) const { (*GL_Set2DFunctionPointer) (); }
		typedef void (*GL_Set3DFunctionPointer_t) (void);
		GL_Set3DFunctionPointer_t GL_Set3DFunctionPointer;
		inline void GL_Set3D (void) const { (*GL_Set3DFunctionPointer) (); }
		///@}

class GLCheckerTexture
{
private:
	GLuint m_texId;

public:
	inline GLCheckerTexture (void) : m_texId (0u) { /* VOID */ }
	
	inline ~GLCheckerTexture (void)
	{
		if (m_texId > 0u)
			g_drawInterface->glDeleteTextures (1, &m_texId);	/// IS 'g_drawInterface' POINTER STILLS VALID?!??!?!?!?!?!?!??!?!?!?!?!?!?!?!?!
	}

public:
	inline void Bind (void)
	{
		if (m_texId > 0u)
		{
			g_drawInterface->glBindTexture (GL_TEXTURE_2D, m_texId);

			return;
		}

		// Create checker pattern.
		const unsigned int col0 (ColorWithAlpha <> (215u, 215u, 215u, 255u));
		const unsigned int col1 (ColorWithAlpha <> (255u, 255u, 255u, 255u));
		static const unsigned char TSIZE (64u);
		unsigned int data[TSIZE * TSIZE];

		g_drawInterface->glGenTextures (1, &m_texId);
		g_drawInterface->glBindTexture (GL_TEXTURE_2D, m_texId);

		unsigned char level (0u);
		unsigned char size (TSIZE);
		unsigned char x, y;

		do
		{
			for (y = 0u; y < size; ++y)
				for (x = 0u; x < size; ++x)
					data[x + y * size] = x == 0u || y == 0u ? col0 : col1;

			g_drawInterface->glTexImage2D (GL_TEXTURE_2D, level, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			++level;
		} while ((size /= 2u) > 0u);

		g_drawInterface->glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		g_drawInterface->glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
};

		template <class baseObjectClass> class Object_WindingTemplate : public baseObjectClass
		{
			protected:
				const Math::Winding     &m_winding;
				const ColorWithAlpha <>  m_color;
				const unsigned char      m_width;
				const bool               m_depthTest;

			public:
				inline Object_WindingTemplate (const Math::Winding &winding, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) :
					m_winding (winding),
					m_color (color.GetWithBrightness (brightness)),
					m_width (width),
					m_depthTest (depthTest)
				{ /* VOID */ }

				virtual inline ~Object_WindingTemplate (void) { /* VOID */ }

			public:
				virtual inline void Draw (void)
				{
					g_drawInterface->glLineWidth (m_width);

					if (m_depthTest)
						g_drawInterface->glEnable (GL_DEPTH_TEST);
					else
						g_drawInterface->glDisable (GL_DEPTH_TEST);

					g_drawInterface->glBegin (GL_LINE_STRIP);	// Drawing Using Lines Strip
						g_drawInterface->glColor4ubv (m_color);

						for (unsigned int index (0u); index < m_winding.m_pointsNumber; ++index)
							g_drawInterface->glVertex3fv (m_winding.m_points[index]);
					g_drawInterface->glEnd ();
/*
					if (m_depthTest)
						g_drawInterface->glDisable (GL_DEPTH_TEST);
					else
						g_drawInterface->glEnable (GL_DEPTH_TEST);*/
				}
		};
		typedef Object_WindingTemplate <Object> Object_Winding;

		template <class baseObjectClass> class Object_LineTemplate : public baseObjectClass
		{
			protected:
				Math::Vector3D    m_source;
				ColorWithAlpha <> m_colorSource;
				Math::Vector3D    m_destination;
				ColorWithAlpha <> m_colorDestination;
				unsigned char     m_width;
				bool              m_depthTest;

			public:
				inline Object_LineTemplate (const Math::Vector3D &source, const ColorWithAlpha <> &colorSource, const Math::Vector3D &destination, const ColorWithAlpha <> &colorDestination, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) :
					m_source (source),
					m_colorSource (colorSource.GetWithBrightness (brightness)),
					m_destination (destination),
					m_colorDestination (colorDestination.GetWithBrightness (brightness)),
					m_width (width),
					m_depthTest (depthTest)
				{ /* VOID */ }

				virtual inline ~Object_LineTemplate (void) { /* VOID */ }

			public:
				virtual inline void Draw (void)
				{
					g_drawInterface->glLineWidth (m_width);

					if (m_depthTest)
						g_drawInterface->glEnable (GL_DEPTH_TEST);
					else
						g_drawInterface->glDisable (GL_DEPTH_TEST);

					g_drawInterface->glBegin (GL_LINES);	// Drawing Using Lines
/*
						g_drawInterface->glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, m_colorSource);
						g_drawInterface->glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, m_colorSource);
*/
						g_drawInterface->glColor4ubv (m_colorSource);
						g_drawInterface->glVertex3fv (m_source);

						g_drawInterface->glColor4ubv (m_colorDestination);
						g_drawInterface->glVertex3fv (m_destination);
					g_drawInterface->glEnd ();
/*
					if (m_depthTest)
						g_drawInterface->glDisable (GL_DEPTH_TEST);
					else
						g_drawInterface->glEnable (GL_DEPTH_TEST);*/
				}

				inline void SetColorSource      (const ColorWithAlpha <> &pf) { m_colorSource = pf; }
				inline void SetColorDestination (const ColorWithAlpha <> &pf) { m_colorDestination = pf; }
		};
		typedef Object_LineTemplate <Object> Object_Line;
		class TimedObject_Line : public Object_LineTemplate <TimedObject>
		{
			public:
				inline TimedObject_Line (const Math::Vector3D &source, const ColorWithAlpha <> &colorSource, const Math::Vector3D &destination, const ColorWithAlpha <> &colorDestination, const float duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) :
					Object_LineTemplate (source, colorSource, destination, colorDestination, brightness, width, depthTest)
				{
					SetDuration (duration);
				}

				virtual inline ~TimedObject_Line (void) { /* VOID */ }
		};

		template <class baseObjectClass> class Object_TriangleTemplate : public baseObjectClass
		{
			protected:
				Math::Vector3D    m_point1;
				Math::Vector3D    m_point2;
				Math::Vector3D    m_point3;
				ColorWithAlpha <> m_color;
				unsigned char     m_width;
				bool              m_depthTest;

			public:
				inline Object_TriangleTemplate (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) :
					m_point1 (point1),
					m_point2 (point2),
					m_point3 (point3),
					m_color (color.GetWithBrightness (brightness)),
					m_width (width),
					m_depthTest (depthTest)
				{ /* VOID */ }

				virtual inline ~Object_TriangleTemplate (void) { /* VOID */ }

			public:
				virtual inline void Draw (void)
				{
					g_drawInterface->glLineWidth (m_width);

					if (m_depthTest)
						g_drawInterface->glEnable (GL_DEPTH_TEST);
					else
						g_drawInterface->glDisable (GL_DEPTH_TEST);

					g_drawInterface->glBegin (GL_TRIANGLES);	// Drawing Using Triangles
/*
						g_drawInterface->glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, m_color);
						g_drawInterface->glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, m_color);
*/
						g_drawInterface->glColor4ubv (m_color);
						g_drawInterface->glVertex3fv (m_point1);
						g_drawInterface->glVertex3fv (m_point2);
						g_drawInterface->glVertex3fv (m_point3);
					g_drawInterface->glEnd ();
/*
					if (m_depthTest)
						g_drawInterface->glDisable (GL_DEPTH_TEST);
					else
						g_drawInterface->glEnable (GL_DEPTH_TEST);*/
				}
		};
		typedef Object_TriangleTemplate <Object> Object_Triangle;

		template <class baseObjectClass> class Object_QuadrangleTemplate : public baseObjectClass
		{
			protected:
				Math::Vector3D    m_point1;
				Math::Vector3D    m_point2;
				Math::Vector3D    m_point3;
				Math::Vector3D    m_point4;
				ColorWithAlpha <> m_color;
				unsigned char     m_width;
				bool              m_depthTest;

			public:
				inline Object_QuadrangleTemplate (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const Math::Vector3D &point4, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true) :
					m_point1 (point1),
					m_point2 (point2),
					m_point3 (point3),
					m_point4 (point4),
					m_color (color.GetWithBrightness (brightness)),
					m_width (width),
					m_depthTest (depthTest)
				{ /* VOID */ }

				virtual inline ~Object_QuadrangleTemplate (void) { /* VOID */ }

			public:
				virtual inline void Draw (void)
				{
					g_drawInterface->glLineWidth (m_width);

					if (m_depthTest)
						g_drawInterface->glEnable (GL_DEPTH_TEST);
					else
						g_drawInterface->glDisable (GL_DEPTH_TEST);

					g_drawInterface->glBegin (GL_QUADS);	// Drawing Using Quadrangles
/*
						g_drawInterface->glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, m_color);
						g_drawInterface->glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, m_color);
*/
						g_drawInterface->glColor4ubv (m_color);
						g_drawInterface->glVertex3fv (m_point1);
						g_drawInterface->glVertex3fv (m_point2);
						g_drawInterface->glVertex3fv (m_point3);
						g_drawInterface->glVertex3fv (m_point4);
					g_drawInterface->glEnd ();
/*
					if (m_depthTest)
						g_drawInterface->glDisable (GL_DEPTH_TEST);
					else
						g_drawInterface->glEnable (GL_DEPTH_TEST);*/
				}
		};
		typedef Object_QuadrangleTemplate <Object> Object_Quadrangle;

		template <class baseObjectClass> class Object_SphereTemplate : public baseObjectClass
		{
			protected:
				const Math::Vector3D    m_center;
				const float             m_radius;
				const unsigned short    m_segmentsNumber;
				const ColorWithAlpha <> m_color;
				const unsigned char     m_width;
				const bool              m_depthTest;

			public:
				inline Object_SphereTemplate (const Math::Vector3D &center, const float radius, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true, const unsigned short segmentsNumber = 16u) :
					m_center (center),
					m_radius (radius),
					m_segmentsNumber (segmentsNumber),
					m_color (color.GetWithBrightness (brightness)),
					m_width (width),
					m_depthTest (depthTest)
				{ /* VOID */ }

				virtual inline ~Object_SphereTemplate (void) { /* VOID */ }

			public:
				virtual inline void Draw (void)
				{
					g_drawInterface->glLineWidth (m_width);

					if (m_depthTest)
						g_drawInterface->glEnable (GL_DEPTH_TEST);
					else
						g_drawInterface->glDisable (GL_DEPTH_TEST);

					g_drawInterface->glBegin (GL_LINES);	// Drawing Using Lines
						g_drawInterface->glColor4ubv (m_color);

	Math::Vector3D *const prevPoints (new Math::Vector3D[m_segmentsNumber]);

			prevPoints[0] = m_center + Math::Vector3D (0.0f, 0.0f, m_radius);
			for (unsigned short z (1u); z < m_segmentsNumber; ++z)
				prevPoints[z] = prevPoints[0];

	// For each vertical slice.. (the top and bottom ones are just a single point).
	for (unsigned short iSlice (1u); iSlice < m_segmentsNumber; ++iSlice)
	{
		const float flHalfSliceAngle (Math::Pi * (float)iSlice / (m_segmentsNumber - 1u));

			for (unsigned short iSubPt (0u); iSubPt < m_segmentsNumber; ++iSubPt)
			{
				const float flHalfAngle (Math::Pi * (float)iSubPt / (m_segmentsNumber - 1u));
				const float flAngle (flHalfAngle * 2.0f);
				Math::Vector3D pt;

				// Calculate the new one
				if (iSlice == m_segmentsNumber - 1u)
					pt = m_center - Math::Vector3D (0.0f, 0.0f, m_radius );
				else
				{
					pt.x = Math::cosf (flAngle) * Math::sinf (flHalfSliceAngle);
					pt.y = Math::sinf (flAngle) * Math::sinf (flHalfSliceAngle);
					pt.z = Math::cosf (flHalfSliceAngle);
					
					pt *= m_radius;
					pt += m_center;
				}

				// Draw the line
				g_drawInterface->glVertex3fv (pt);
				g_drawInterface->glVertex3fv (prevPoints[iSubPt]);

				// Store off our last position
				prevPoints[iSubPt] = pt;
			}
			
			if (iSlice != m_segmentsNumber - 1u)
				for (unsigned short i (0u); i < m_segmentsNumber; ++i)
				{
					// Draw the line
					g_drawInterface->glVertex3fv (prevPoints[i]);
					g_drawInterface->glVertex3fv (prevPoints[(i + 1u) % m_segmentsNumber]);
				}
	}

	delete [] prevPoints;
					g_drawInterface->glEnd ();
/*
					if (m_depthTest)
						g_drawInterface->glDisable (GL_DEPTH_TEST);
					else
						g_drawInterface->glEnable (GL_DEPTH_TEST);*/
				}
		};
		typedef Object_SphereTemplate <Object> Object_Sphere;

		enum { MaximumDrawObjects = 1000u };
		typedef StaticArray <Object *, unsigned int, MaximumDrawObjects> DrawInstantObjectsArray_t;
		DrawInstantObjectsArray_t m_instantDrawObjects;

		typedef Stack <TimedObject *, unsigned int, Link> DrawTimedObjectsArray_t;
		DrawTimedObjectsArray_t m_timedDrawObjects;

		GLCheckerTexture m_tex;

	public:
		virtual inline ~DrawInterface_GL (void)
		{
			DeleteAllDrawObjects ();
		}

	private:
		const bool CodeWalk (Trampolines::Bytecode::MoveESIToValue32_t *startAddress, const Trampolines::Bytecode::MoveESIToValue32_t *const endAddress);

	public:
		const bool Initialize (void);

		inline void DeleteAllDrawObjects (void)
		{
			m_instantDrawObjects.DeleteAndPurgeAll ();
			m_timedDrawObjects.DeleteAndPurgeAll ();
		}

		inline void DepthMask (const bool state) const { glDepthMask (state); }
		inline void Begin (const Type_t type, const unsigned char size = 1u) const
		{
			switch (type)
			{
				case Type_Points:
					glPointSize (size);
					glBegin (GL_POINTS);

					break;

				case Type_Lines:
					glLineWidth (size);
					glBegin (GL_LINES);

					break;

				case Type_Triangles:
					glBegin (GL_TRIANGLES);

					break;

				case Type_Quads:
					glBegin (GL_QUADS);

					break;
			}
		}
		inline void BeginPoints (const unsigned char size = 1u) const
		{
			glPointSize (size);
			glBegin (GL_POINTS);
		}
		inline void BeginLines (const unsigned char size = 1u) const
		{
			glLineWidth (size);
			glBegin (GL_LINES);
		}
		inline void BeginTriangles (void) const { glBegin (GL_TRIANGLES); }
		inline void BeginQuads (void) const { glBegin (GL_QUADS); }
		inline void SubmitVertex (const Math::Vector3D &position, const Color <> &/*color*/) const
		{
///			glColor3b (color.red, color.green, color.blue);
			glVertex3fv (position);
		}
		inline void SubmitVertex (const Math::Vector3D &position, const ColorWithAlpha <> &color) const
		{
			glColor4ubv (reinterpret_cast <const GLubyte *> (&color.GetRawColor ()));
			glVertex3fv (position);
		}
		inline void SubmitVertex (const Math::Vector3D &position, const ColorWithAlpha <> &color, const Math::Vector2D &uv) const
		{
			glColor4ubv (reinterpret_cast <const GLubyte *> (&color.GetRawColor ()));
			glTexCoord2fv (uv);
			glVertex3fv (position);
		}
		inline void End (void) const
		{
			glEnd ();

//			glLineWidth (1.0f);
//			glPointSize (1.0f);
		}

		inline void TextureInternally (const bool state)
		{
			/// @warning THIS FUNCTION CRASHES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			if (state)
			{
				glEnable (GL_TEXTURE_2D);

				m_tex.Bind ();
			}
			else
				glDisable (GL_TEXTURE_2D);
		}

//		inline const bool IsLineVisibleForHost (const Math::Vector3D &source, const Math::Vector3D &destination) const;	// Note: This function declared in Core.hpp.

		#define ADD_NEW_INSTANT_OBJECT(newObject)		\
		{												\
			Object *const newObjectPointer (newObject);	\
														\
			/* Reliability check. */					\
			InternalAssert (newObjectPointer != NULL);	\
														\
			m_instantDrawObjects += newObjectPointer;	\
		}
		#define ADD_NEW_TIMED_OBJECT(newObject)					\
		{														\
			TimedObject *const newObjectPointer (newObject);	\
																\
			/* Reliability check. */							\
			InternalAssert (newObjectPointer != NULL);			\
																\
			m_timedDrawObjects += newObjectPointer;				\
		}

		inline void DrawWinding (const Math::Winding &winding, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)// const
		{
			ADD_NEW_INSTANT_OBJECT (new Object_Winding (winding, color, brightness, width, depthTest));
		}
		inline void DrawSphere (const Math::Vector3D &center, const float radius, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true, const unsigned short segmentsNumber = 16u)// const
		{
			ADD_NEW_INSTANT_OBJECT (new Object_Sphere (center, radius, color, brightness, width, depthTest, segmentsNumber));
		}
		inline void DrawLineTimed (const Math::Vector3D &source, const Math::Vector3D &destination, const ColorWithAlpha <> &color, const float duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)// const
		{
			DrawLineTimed (source, color, destination, color, duration, brightness, width, depthTest);
		}
		inline void DrawLineTimed (const Math::Vector3D &source, const ColorWithAlpha <> &colorSource, const Math::Vector3D &destination, const ColorWithAlpha <> &colorDestination, const float duration, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)// const
		{
			ADD_NEW_TIMED_OBJECT (new TimedObject_Line (source, colorSource, destination, colorDestination, duration, brightness, width, depthTest));
		}
		inline void DrawLine (const Math::Vector3D &source, const Math::Vector3D &destination, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)// const
		{
			DrawLine (source, color, destination, color, brightness, width, depthTest);
		}
		inline void DrawLine (const Math::Vector3D &source, const ColorWithAlpha <> &colorSource, const Math::Vector3D &destination, const ColorWithAlpha <> &colorDestination, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)// const
		{
/*			if (!IsLineVisibleForHost (source, destination))
				return;
*/
			ADD_NEW_INSTANT_OBJECT (new Object_Line (source, colorSource, destination, colorDestination, brightness, width, depthTest));
#if 0
//*		glEnable(GL_DEPTH_TEST);

			glPushMatrix ();
			glLoadIdentity ();
			glDisable (GL_TEXTURE_2D);
			glColor3b (colorSource.red, colorSource.green, colorSource.blue);

//		glEnable(GL_FOG);
/*/
//			BeginPoints (/*width*///);
			BeginLines (/*width*/);

			SubmitVertex (source, colorSource);
			SubmitVertex (destination, colorDestination);

			End ();
//*
//		glDisable(GL_FOG);

			glEnable (GL_TEXTURE_2D);
			glPopMatrix ();

//		glDisable(GL_DEPTH_TEST);*/
#endif	// if 0
		}
		inline void InstantDrawLine (const Math::Vector3D &/*source*/, const Math::Vector3D &/*destination*/, const ColorWithAlpha <> &/*color*/, const unsigned char/* width*/ = 1u) const
		{
#if 0
/*		glEnable(GL_DEPTH_TEST);

			glPushMatrix ();
			glLoadIdentity ();
			glDisable (GL_TEXTURE_2D);
			glColor4ubv (reinterpret_cast <const GLubyte *> (&color.GetRawColor ()));

		glEnable(GL_FOG);
*/
//			BeginPoints (/*width*/);
			BeginLines (/*width*/);

			SubmitVertex (source, color);
			SubmitVertex (destination, color);

			End ();
/*
		glDisable(GL_FOG);

			glEnable (GL_TEXTURE_2D);
			glPopMatrix ();

		glDisable(GL_DEPTH_TEST);*/
#endif	// if 0
		}

		inline void DrawTriangle (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// Purpose: Draw triangle.

			ADD_NEW_INSTANT_OBJECT (new Object_Triangle (point1, point2, point3, color, brightness, width, depthTest));
		}

		inline void DrawQuadrangle (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const Math::Vector3D &point4, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// Purpose: Draw triangle.

			ADD_NEW_INSTANT_OBJECT (new Object_Quadrangle (point1, point2, point3, point4, color, brightness, width, depthTest));
		}

		inline void DrawHorizontalArrow (const Math::Vector3D &source, const Math::Vector3D &destination, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// Purpose: Draw a horizontal arrow pointing in the specified direction.

			const Math::Vector3D lineDir ((destination - source).Normalize ());
			const Math::Vector3D upVec (Math::Vector3D::UnitZ);
			const Math::Vector3D sideDir (lineDir ^ upVec);
			const float radius (width * 0.5f);
			const Math::Vector3D p1 (source - sideDir * radius);
			const Math::Vector3D p2 (destination - lineDir * width - sideDir * radius);
			const Math::Vector3D p3 (destination - lineDir * width - sideDir * width);
			const Math::Vector3D p4 (destination);
			const Math::Vector3D p5 (destination - lineDir * width + sideDir * width);
			const Math::Vector3D p6 (destination - lineDir * width + sideDir * radius);
			const Math::Vector3D p7 (source + sideDir * radius);

			// Outline the arrow
			DrawLine (p1, p2, color, brightness, 1u, depthTest);
			DrawLine (p2, p3, color, brightness, 1u, depthTest);
			DrawLine (p3, p4, color, brightness, 1u, depthTest);
			DrawLine (p4, p5, color, brightness, 1u, depthTest);
			DrawLine (p5, p6, color, brightness, 1u, depthTest);
			DrawLine (p6, p7, color, brightness, 1u, depthTest);

			// Fill us in with triangles
			DrawTriangle (p5, p4, p3, color, brightness, 1u, depthTest);	// Tip
			DrawTriangle (p1, p7, p6, color, brightness, 1u, depthTest);	// Shaft
			DrawTriangle (p6, p2, p1, color, brightness, 1u, depthTest);

			// And backfaces
			DrawTriangle (p3, p4, p5, color, brightness, 1u, depthTest);	// Tip
			DrawTriangle (p6, p7, p1, color, brightness, 1u, depthTest);	// Shaft
			DrawTriangle (p1, p2, p6, color, brightness, 1u, depthTest);
		}

		inline void DrawCross2D (const Math::Vector3D &position, const float length, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), color, brightness, width, depthTest);
			DrawLine (position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), color, brightness, width, depthTest);
		}
		inline void DrawCross3D (const Math::Vector3D &position, const float length, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), color, brightness, width, depthTest);
			DrawLine (position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), color, brightness, width, depthTest);
			DrawLine (position - Math::Vector3D (0.0f, 0.0f, length), position + Math::Vector3D (0.0f, 0.0f, length), color, brightness, width, depthTest);
		}
		inline void DrawCross3DWithoutNegativeZ (const Math::Vector3D &position, const float length, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), color, brightness, width, depthTest);
			DrawLine (position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), color, brightness, width, depthTest);
			DrawLine (position, position + Math::Vector3D (0.0f, 0.0f, length), color, brightness, width, depthTest);
		}
		inline void DrawBox (const Math::Vector3D &absoluteMins, const Math::Vector3D &absoluteMaxs, const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			// bottom square
			DrawLine (absoluteMins, Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), absoluteMins, color, brightness, width, depthTest);

			// verticals
			DrawLine (absoluteMins, Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), absoluteMaxs, color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), color, brightness, width, depthTest);

			// top square
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), absoluteMaxs, color, brightness, width, depthTest);
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), color, brightness, width, depthTest);
			DrawLine (absoluteMaxs, Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), color, brightness, width, depthTest);
		}
		inline void DrawBox (const Math::Vector3D points[8u], const ColorWithAlpha <> &color, const unsigned char brightness = 255u, const unsigned char width = 1u, const bool depthTest = true)
		{
			DrawLine (points[0u], points[1u], color, brightness, width, depthTest);
			DrawLine (points[2u], points[3u], color, brightness, width, depthTest);
			DrawLine (points[4u], points[5u], color, brightness, width, depthTest);
			DrawLine (points[6u], points[7u], color, brightness, width, depthTest);

			DrawLine (points[0u], points[2u], color, brightness, width, depthTest);
			DrawLine (points[2u], points[4u], color, brightness, width, depthTest);
			DrawLine (points[4u], points[6u], color, brightness, width, depthTest);
			DrawLine (points[6u], points[0u], color, brightness, width, depthTest);

			DrawLine (points[1u], points[3u], color, brightness, width, depthTest);
			DrawLine (points[3u], points[5u], color, brightness, width, depthTest);
			DrawLine (points[5u], points[7u], color, brightness, width, depthTest);
			DrawLine (points[7u], points[1u], color, brightness, width, depthTest);
		}

		#undef ADD_NEW_INSTANT_OBJECT
		#undef ADD_NEW_TIMED_OBJECT

		inline void DrawInstantObjects (void)
		{
			if (m_instantDrawObjects.IsEmpty ())
				return;

			for (DrawInstantObjectsArray_t::IndexType_t index (0u); index < m_instantDrawObjects.GetElementNumber (); ++index)
			{
				m_instantDrawObjects[index]->Draw ();

				delete m_instantDrawObjects[index];
			}

			m_instantDrawObjects.RemoveAll ();
		}
		inline void DrawTimedObjects (void)
		{
			if (m_timedDrawObjects.IsEmpty ())
				return;

			Link **node = &m_timedDrawObjects.GetFirstReference ();

			do
			{
				// Is it time to kill this overlay?
				if (static_cast <TimedObject *> (*node)->IsDead ())
				{
					m_timedDrawObjects.Pop (*node);

					continue;
				}

				static_cast <TimedObject *> (*node)->Draw ();

				node = &(*node)->GetNextReference ();
			} while (*node != NULL);
		}
		inline void DrawAllObjects (void)
		{
			DrawInstantObjects ();
			DrawTimedObjects ();
		}
};

#endif	// ifndef DRAW_TOOLS_INCLUDED