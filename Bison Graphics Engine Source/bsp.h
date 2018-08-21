
/*
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);

extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC  glClientActiveTextureARB;
*/

#define RELEASE(x) { if (x){delete[] x; x = NULL;} }
#define RELEASEC(x) { if (x){free(x); x = NULL;} }


#define LIGHTMAP_GAMMA 10


#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1



#define MAX_TEXTURES 512
#define FACE_POLYGON 1
#define ADDITIONAL_SHADERS 100

enum
{
    FACETYPE_NORMAL   = 1,
    FACETYPE_MESH     = 2,
    FACETYPE_TRISURF  = 3,
    FACETYPE_FLARE    = 4
};

struct iVector3
{
	int x,y,z;
};

struct bspHeader
{
	char id[4];
	int version;
};

struct bspLump
{
	int offset;
	int length;
};

struct bspVertex
{
	CVector3 position;
	CVector2 tex;
	CVector2 lightMap;
	CVector3 normal;
	char colour[4];

};

struct bspFace
{
	int textureID;
	int effect;
	int type;
	int startVertex;
	int numVerts;
	int meshIndex;
	int numMeshVerts;
	int lightMapID;
	int lightMapCorner[2];
	int lightMapSize[2];
	CVector3 lighMapPos;
	CVector3 lightMapVec[2];
	CVector3 normal;
	int size[2];

};


struct bspTexture
{
	char name[64];
	int flags;
	int contents;
};

struct bspLightMap
{
	unsigned char pImage[128][128][3];
};


struct bspNode
{
	int plane;
    int front;
	int back;
	iVector3 minBound;
	iVector3 maxBound;

};


struct bspLeaf
{
	int cluster;
	int area;
	iVector3 min;
	iVector3 max;
	int leafFaceIndex;
	int numLeafFace;
	int leafBrushIndex;
	int numLeafBrush;
};

struct bspPlane
{
	CVector3 normal;
	float d;
};

struct pVisData
{
	int numClusters;
	int bytesPerCluster;
	unsigned char *pBitSets;
};


struct bspBrush
{
	int brushSide;
	int numBrushSides;
	int textureID;
};

struct bspBrushSide
{
	int planeIndex;
	int textureID;
};

struct Entity
{
	char *name;
	char *value;
};

enum eLumps
{
	kEntities = 0,
	kTextures,
	kPlanes,
	kNodes,
	kLeafs,
	kLeafFaces,
	kLeafBrushes,
	kModels,
	kBrushes,
	kBrushSides,
	kVerts,
	kMeshVerts,
	kShaders,
	kFaces,
	kLightMaps,
	kLightVolumes,
	kVisData,
	kMaxLumps

};

class CBitset 
{

public:

	// Initialize all the data members
    CBitset() : m_bits(0), m_size(0) {}

	// This is our deconstructor
	~CBitset() 
	{
		// If we have valid memory, get rid of it
		if(m_bits) 
		{
			delete m_bits;
			m_bits = NULL;
		}
	}

	// This resizes our bitset to a size so each face has a bit associated with it
	void Resize(int count) 
	{ 
		// Get the size of integers we need
		m_size = count/32 + 1;

		// Make sure we haven't already allocated memory for the bits
        if(m_bits) 
		{
			delete m_bits;
			m_bits = 0;
		}

		// Allocate the bits and initialize them
		m_bits = new unsigned int[m_size];
		ClearAll();
	}

	// This does the binary math to set the desired bit
	void Set(int i) 
	{
		m_bits[i >> 5] |= (1 << (i & 31));
	}

	// This returns if the desired bit slot is a 1 or a 0
	int On(int i) 
	{
		return m_bits[i >> 5] & (1 << (i & 31 ));
	}

	// This clears a bit to 0
	void Clear(int i) 
	{
		m_bits[i >> 5] &= ~(1 << (i & 31));
	}

	// This initializes the bits to 0
	void ClearAll() 
	{
		memset(m_bits, 0, sizeof(unsigned int) * m_size);
	}

private:

	// Our private bit data that holds the bits and size
	unsigned int *m_bits;
	int m_size;
};
typedef unsigned char Colour4[4];
typedef float TexCoord[2];
typedef float vec4[4];  
/* Triangle arrays */
typedef struct
{
    int numverts;
    vec4 *verts;
    Colour4 *colour;
    TexCoord *tex_st;
    TexCoord *lm_st;
    int numelems;
    int *elems;
} TriArrays;

static TriArrays arrays;




extern bspLump Lumps[kMaxLumps];

extern	int numVerts;
extern  int numMeshVerts;
extern  int numTextures;
extern  int numShaderRefs;
extern	int numFaces;
extern	int numLightMaps;

extern	int numNodes;
extern	int numLeafs;
extern	int numLeafFaces;
extern	int numPlanes;
extern	int numBrushes;
extern	int numBrushSides;
extern	int numLeafBrushes;
extern int  numIndicies;

	
extern	bspBrush *pBrushes;
extern	bspBrushSide *pBrushSides;
	
extern	bspNode *pNodes;
extern	bspLeaf *pLeafs;
extern	bspPlane *pPlanes;
extern	bspTexture * pTextures;
extern	bspTexture * pShaderRefs;
extern  int        *m_pIndexArray;


extern	unsigned int *textures;
extern 	unsigned int lightMaps[MAX_TEXTURES];


extern	int *pLeafFaces;
extern	int *pLeafBrushes;
	



extern	bspVertex *pVerts;

  
extern	bspFace *pFaces;
extern	pVisData clusters;


extern	struct LeafCluster *pLeafClusters;
extern	struct ClusterList *pClusterList;
extern CBitset drawn;

void BspLoadMap(const char *filename);
void BspRenderMap(const CVector3& pos);
void BspRenderFace(int faceIndex);
void BspRenderFlush();
void bspCleanUp();
