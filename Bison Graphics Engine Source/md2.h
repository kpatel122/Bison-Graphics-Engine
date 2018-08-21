#define byte unsigned char
// These are the needed defines for the max values when loading .MD2 files
#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_VERTICES * 4 + 128)





struct MD2_HEADER
{
int ident;
int version;
int skinwidth;
int skinheight;
int framesize;
int numSkins;
int numXYZ;
int numST;
int numTris;
int numGLcmds;
int numFrames;
int offsetSkins;
int offsetST;
int offsetTris;
int offsetFrames;
int offsetGLcmds;
int offsetEnd;
};

struct MD2_FRAME_POINT
{
	byte vertex[3];
	byte normalIndex;

};

struct MD2_VERTEX
{
	float vertex[3];
	float normal[3];
};

struct MD2_FACE
{
short vertexIndex[3];
short texIndex[3];

};

struct MD2_TEXINDEX
{
	short u;
    short v;
};


struct MD2_TEXCOORD
{
	float u;
    float v;
};

struct MD2_FRAME_INDEX
{
	float scale[3];
    float translate[3];
	char name[16];
	struct MD2_FRAME_POINT framePoint[1];
};

struct MD2_FRAME
{
   char strName[16];
   MD2_VERTEX *pVertices;
};



struct PLANE
{
	struct CVector3 normal;
	float d;
};


typedef char MD2_SKIN[64];


void SetRenderType(CModel3d *pModel,int type);
float ReturnCurrentTime(CModel3d *pModel, int nextFrame);
void AnimateModel(CModel3d *pModel,float speed);

class CModelMD2
{

public:
	CModelMD2();
	~CModelMD2();
	int Load(CModel3d *obj, char* model, char* skin = NULL);
	struct tImage *LoadSkin(char *skin, unsigned int &texID);
	int Convert(CModel3d *obj);
	int ComputeNormals(CObject3d *obj);
	int ParseAnimations(CModel3d *pModel);
	int SideOfPlane(const PLANE &plane, const CVector3 &pos);
	float DistanceFromPlane(const PLANE &plane, const CVector3 &pos);
	void SetConn(CObject3d *obj);
	void ProjectSiloute(CVector3 &lpos,CModel3d *pModel,float time,bool outline);
	unsigned int textureID;
	int Render();
	int numberSkins;
	void CleanUp(CModel3d *obj);

	unsigned int *pSkinID;

    FILE *p_file;
	
	MD2_HEADER m_Header;
	MD2_SKIN *p_Skins;
	MD2_TEXCOORD * p_TexCoord;
	MD2_FACE *p_Triangles;
	MD2_FRAME *p_Frames;

	
};