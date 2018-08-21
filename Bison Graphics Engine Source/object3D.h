//#include "CVector.h"

#define RELEASE(x) { if (x){delete[] x; x = NULL;} }

struct OBJ_VERTEX
{
	float x,y,z;
};


struct OBJ_FACE
{
	bool drawn[3];
	int vertexIndex[3];
	int texIndex[3];
	short faceNeighbour[3];
};

struct AnimInfo
{
	int startFrame;
	int endFrame;
	char frameName[256];
};

class CObject3d
{
public:
	int textureID;
	
	int *pSkins;
	int numberSkins;

	int numVerts;
	int numTexCoords;
	int numFaces;
	//int numFrames;
	CVector3 *pVerts;
	CVector2 *pTexCoords;
	CVector3 *pNormals;

	struct OBJ_FACE *pFace;
	struct	PLANE *pPlanes;

    //CObject3d() {numberSkins = numTexCoords = numFaces = numVerts =  0;}
	//~CObject3d(){RELEASE(pVerts);RELEASE(pTexCoords);RELEASE(pNormals);RELEASE(pSkins)}


};


class CModel3d
{
public:
	int numFrames;
	int numAnimations;
	int currentFrame;
	int currentAnimation;
	int render_type;
	int currFrame;
	int nextFrame;
	float interpol; 
	
	float elapsedTime;  
	float lastTime;	
	
	

	vector<CObject3d> pObject;
	vector<AnimInfo> pAnimInfo;


};

