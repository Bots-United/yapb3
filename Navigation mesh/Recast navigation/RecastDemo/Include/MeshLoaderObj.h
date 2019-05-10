//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef MESHLOADER_OBJ
#define MESHLOADER_OBJ

class rcMeshLoaderObj
{
public:
	rcMeshLoaderObj(void);
	virtual ~rcMeshLoaderObj(void);

	virtual bool load(const char *const fileName);

	inline const float *const getVerts(void) const { return m_verts; }
	inline const float *const getNormals(void) const { return m_normals; }
	inline const int *const getTris(void) const { return m_tris; }
	inline const int getVertCount(void) const { return m_vertCount; }
	inline const int getTriCount(void) const { return m_triCount; }
	inline const char *const getFileName(void) const { return m_filename; }

protected:
	void addVertex(float x, float y, float z, int& cap);
	void addTriangle(int a, int b, int c, int& cap);

	char m_filename[260];

	float* m_verts;
	int* m_tris;
	float* m_normals;
	int m_vertCount;
	int m_triCount;
};

#endif // MESHLOADER_OBJ
