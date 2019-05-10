#ifndef ANNOTATIONBUILDER_H
#define ANNOTATIONBUILDER_H

enum DrawFlags
{
	DRAW_WALKABLE_SURFACE =	1 << 0,
	DRAW_WALKABLE_BORDER =	1 << 1,
	DRAW_SELECTED_EDGE =	1 << 2,
	DRAW_ANIM_TRAJECTORY =	1 << 3,
	DRAW_LAND_SAMPLES =		1 << 4,
	DRAW_COLLISION_SLICES =	1 << 5,
	DRAW_ANNOTATIONS =		1 << 6,
};

enum SampleType
{
	EDGE_JUMP_DOWN,
	EDGE_JUMP_OVER,
};

struct AnnotationBuilderConfig
{
	float agentRadius;
	float agentHeight;
	float agentClimb;
	float cellSize;
	float cellHeight;
};

class AnnotationBuilder : public Sample_SoloMesh
{
	AnnotationBuilderConfig m_acfg;
/*
	rcConfig m_cfg;	
	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
*/
	unsigned int drawFlags;
	float buildTime;
	SampleType edgeType;
	
	struct Edge
	{
		float sp[3], sq[3];
	};
	
	Edge* m_edges;
	int m_nedges;
	
	void cleanupAnnotation();	/// @note Original name is "cleanup". (Renamed due to this name already reserved by Sample_SoloMesh class)
	
	static const int MAX_SPINE = 8;
	
	struct TrajectorySample
	{
		float x, ymin, ymax;
	};
	
	struct Trajectory2D
	{
		Trajectory2D() : nspine(0), samples(0), nsamples(0) {}
		~Trajectory2D() { delete [] samples; }
		float spine[2*MAX_SPINE];
		int nspine;
		TrajectorySample* samples;
		int nsamples;
	};
	
	void initTrajectory(Trajectory2D* tra);
	void drawTrajectory(struct duDebugDraw* dd, const float* pa, const float* pb, Trajectory2D* tra, const unsigned int col);
	void drawTrajectorySlice(struct duDebugDraw* dd, const float* pa, const float* pb, Trajectory2D* tra, const unsigned int col);
	bool sampleTrajectory(const float* pa, const float* pb, Trajectory2D* tra);

	struct GroundSample
	{
		float height;
		unsigned char flags;
	};

	struct PotentialSeg
	{
		unsigned char mark;
		int idx;
		float umin, umax;
		float dmin, dmax;
		float sp[3], sq[3];
	};
	
	struct GroundSegment
	{
		inline GroundSegment() : gsamples(0), ngsamples(0) {}
		inline ~GroundSegment() { delete [] gsamples; }
		
		float p[3], q[3];
		GroundSample* gsamples;
		int ngsamples;
		int npass;
	};
	
	struct EdgeSampler
	{
		GroundSegment start;
		GroundSegment end;
		
		float groundRange;
		
		Trajectory2D trajectory;

		float rigp[3], rigq[3];
		float ax[3], ay[3], az[3];
	};
	EdgeSampler* m_sampler;

	
	int findPotentialJumpOverEdges(const float* sp, const float* sq,
								   const float depthRange, const float heightRange,
								   float* outSegs, const int maxOutSegs);
	
	void initJumpDownRig(EdgeSampler* es, const float* sp, const float* sq,
						 const float jumpStartDist, const float jumpEndDist,
						 const float jumpDownDist, const float groundRange);

	void initJumpOverRig(EdgeSampler* es, const float* sp, const float* sq,
						 const float jumpStartDist, const float jumpEndDist,
						 const float jumpHeight, const float groundRange);
	
	bool getFloorPosition(const float* pt, const float hrange, float* height);

	bool getCompactHeightfieldHeigh(const float* pt, const float hrange, float* height);
	bool checkHeightfieldCollision(const float x, const float ymin, const float ymax, const float z);

	void sampleGroundSegment(GroundSegment* seg, const int nsamples, const float groundRange);
	
	void sampleAction(EdgeSampler* es);
	
	void filterJumpOverLinks();
	
	EdgeSampler* sampleEdge(int type, const float* sp, const float* sq);
	void addEdgeLinks(EdgeSampler* es);

	int m_selEdge;

	
	struct JumpLink
	{
		float spine0[MAX_SPINE*3];
		float spine1[MAX_SPINE*3];
		int nspine;
		unsigned char flags;
	};
	JumpLink* m_links;
	int m_nlinks;
	int m_clinks;
	
	JumpLink* addLink();

	
public:
	AnnotationBuilder();
	~AnnotationBuilder();
	
	void clearLinks();
	
	bool handleBuild(/*const AnnotationBuilderConfig& acfg, class NavWorld* world*/);

	void buildAllEdges(int type);
	inline void buildNearestEdge(const float* pos) { buildNearestEdge (edgeType, pos); }
	void buildNearestEdge(int type, const float* pos);
	
	void draw(unsigned int flags);

	void handleRender(void)
	{
		Sample_SoloMesh::handleRender ();

		draw(drawFlags);
	}
	void handleSettings()
	{
		Sample_SoloMesh::handleSettings();

		#if defined MULTIPLE_DRAW_METHODS
/*		imguiSeparator();

			static int paramsScroll = 0;
			if (imguiBeginScrollArea("Annotation Params", width - 250-10, 10, 250, height-20, &paramsScroll))
				mouseOverMenu = true;
			*/
			imguiLabel("Annotation Build Options");
			
			if (imguiCheck("Build Jump-down", edgeType == EDGE_JUMP_DOWN))
				edgeType = EDGE_JUMP_DOWN;
			if (imguiCheck("Build Jump-over", edgeType == EDGE_JUMP_OVER))
				edgeType = EDGE_JUMP_OVER;
			imguiSeparator();
			if (imguiButton("Build All Jump-down"))
			{
				TimeVal t0 = getPerfTime();
				buildAllEdges(EDGE_JUMP_DOWN);
				TimeVal t1 = getPerfTime();
				buildTime = (float)(getPerfDeltaTimeUsec(t0,t1)/1000.0);
			}
			if (imguiButton("Build All Jump-over"))
			{
				TimeVal t0 = getPerfTime();
				buildAllEdges(EDGE_JUMP_OVER);
				TimeVal t1 = getPerfTime();
				buildTime = (float)(getPerfDeltaTimeUsec(t0,t1)/1000.0);
			}
			if (imguiButton("Clear"))
				clearLinks();

			imguiSeparator();

			char msg[64];
			_snprintf(msg,64,"Annotation Build Time: %.1fms", buildTime);
			imguiLabel(msg);

			imguiSeparator();

			imguiLabel("Draw Options");
			if (imguiCheck("Walkable Surface", (drawFlags & DRAW_WALKABLE_SURFACE) > 0u))
				drawFlags ^= DRAW_WALKABLE_SURFACE;
			if (imguiCheck("Walkable Border", (drawFlags & DRAW_WALKABLE_BORDER) > 0u))
				drawFlags ^= DRAW_WALKABLE_BORDER;
			if (imguiCheck("Selected Edge", (drawFlags & DRAW_SELECTED_EDGE) > 0u))
				drawFlags ^= DRAW_SELECTED_EDGE;
			imguiIndent();
			if (imguiCheck("Anim Trajectory", (drawFlags & DRAW_ANIM_TRAJECTORY) > 0u, (drawFlags & DRAW_SELECTED_EDGE) > 0u))
				drawFlags ^= DRAW_ANIM_TRAJECTORY;
			if (imguiCheck("Land Samples", (drawFlags & DRAW_LAND_SAMPLES) > 0u, (drawFlags & DRAW_SELECTED_EDGE) > 0u))
				drawFlags ^= DRAW_LAND_SAMPLES;
			if (imguiCheck("Collision Slices", (drawFlags & DRAW_COLLISION_SLICES) > 0u, (drawFlags & DRAW_SELECTED_EDGE) > 0u))
				drawFlags ^= DRAW_COLLISION_SLICES;
			imguiUnindent();
			if (imguiCheck("All Annotations", (drawFlags & DRAW_ANNOTATIONS) > 0u))
				drawFlags ^= DRAW_ANNOTATIONS;
						
//			imguiEndScrollArea();
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
};

#endif // ANNOTATIONBUILDER_H