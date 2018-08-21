#include <windows.h>					// Include the much need windows.h
#include <gl\gl.h>						// Header File For The OpenGL32 Library
#include <gl\glu.h>						// Header File For The GLu32 Library
#include <gl\glaux.h>					// Header File For The GLaux Library
#include <iostream.h>						// Include our c++ standard header
#include <fstream>						// Include this to use ifstream
#include <string>						// Include this to use the string class
#include <vector>						// Include this to use STL vectors
#include "CVector.h"
#include "Frustum.h"
#include "Camera.h"
#include "Texture.h"
#include "unzip.h"
#include "pak.h"
#include "shader.h"
#include "bsp.h"
#include "Skybox.h"



#define msg(x) MessageBox(NULL,x,"ERROR",MB_OK);

/*********************/
#define TWOPI 6.283185307179586476925287
#define RADTODEG 57.2957795
#define TOLERANCE 0.0000001
/*********************/
#define RADIUS 30.0f




/////////////////////////////////////////

static unsigned char *bspData;

bspLump Lumps[kMaxLumps];

int numVerts;
int numMeshVerts;
int numTextures;
int numShaderRefs;
int numFaces;
int numLightMaps;

int numNodes;
int numLeafs;
int numLeafFaces;
int numPlanes;
int numBrushes;
int numBrushSides;
int numLeafBrushes;
int  numIndicies;

	
bspBrush *pBrushes;
bspBrushSide *pBrushSides;
	
bspNode *pNodes;
bspLeaf *pLeafs;
bspPlane *pPlanes;
bspTexture * pTextures;
bspTexture * pShaderRefs;
int         *m_pIndexArray;

int *pLeafFaces;
int *pLeafBrushes;
	
	

pVisData clusters;
bspVertex *pVerts;
bspFace *pFaces;
bspHeader *Header;

unsigned int *textures;
unsigned int lightMaps[MAX_TEXTURES];

LeafCluster *pLeafClusters;
ClusterList *pClusterList;
CBitset drawn;

Shader *shaders;

Skybox *skybox;
static int *skylist;
int numsky;

/////////////////////////////////////////////////



extern CCamera g_Camera;
extern char buffer2[80];

extern char buffer3[80];


extern CFrustum frustum;


struct LeafCluster
{

	vector<bspLeaf> Leafs;
};

struct ClusterList
{
	
	vector<int> visibleClusters;

};



vector<int> facelist;
#define vec_copy(s,d) {d[0]=s[0];d[1]=s[1];d[2]=s[2];}
#define vec2_copy(s,d) {d[0]=s[0];d[1]=s[1];}




int  BspLoadHeader(const char *filename);
void ReadLump(int lump,int size, int &num ,void **dest);
void BspReadLumps();
void BspLoadTextures(const char *);
void BspLoadLightMaps();
void BspCreateClusterLists();
void BspLoadVisibilityData();
int BspFindLeaf(const CVector3 &pos);

int  IsClusterVisable(int current,int test);
void SwapLeafCoord();
void SwapVertexCoord();
void SwapPlaneCoord();
void SwapNodeCoord();
void BspRenderFace2(int faceIndex,int shader=-1);



void BspLoadMap(const char *filename)
{



/*
char maplist[4096], *fname;

pak_openpak("pak\\pak0.pk3");
int num; 




num = pak_listmaps(4096,maplist);
fname = maplist;
*/

/*
To search for different q3Maps.
num -= 1;

while (num--)
{
cout<<fname<<"\n";
fname+= strlen(fname)+1;
}
*/


//BspLoadHeader(fname);



BspLoadHeader(filename);
BspReadLumps();

BspLoadLightMaps();
BspLoadVisibilityData();
BspCreateClusterLists();

/*
SwapVertexCoord();

SwapLeafCoord();
SwapNodeCoord();
*/

//SwapPlaneCoord();

numShaderRefs = Lumps[kTextures].length/sizeof(bspTexture);
int size = sizeof(bspTexture);
int numt = Lumps[kTextures].length/size;
	
pShaderRefs = (bspTexture*)malloc((numShaderRefs + ADDITIONAL_SHADERS)*sizeof(bspTexture));
memcpy(pShaderRefs,bspData + (Lumps[kTextures].offset), numt* size);



ShaderReadAll();
BspLoadTextures(filename);


//skylist = (int *) malloc(100*sizeof(int));
numsky = 0;


}


void BspLoadLightMaps()
{
	bspLightMap *plightMaps =(bspLightMap*) malloc ((numLightMaps * sizeof(bspLightMap)));
	memcpy(plightMaps,bspData + Lumps[kLightMaps].offset,numLightMaps * sizeof(bspLightMap));


	for(int i=0;i<numLightMaps;i++)
	{
		CreateTexture(lightMaps[i],(unsigned char *)&plightMaps[i].pImage,128,128,LIGHTMAP_GAMMA);
	}
	delete [] plightMaps;
}

void BspLoadTextures(const char *baseDir)
{
	
	

	textures = (unsigned int *)malloc(numTextures * sizeof(unsigned int));
    glGenTextures(numTextures, textures);
	

	int non_founded = 0;


char buff[80];



	char *path = new char[64];
	for (int i=0;i<numTextures;i++)
	{
		
	
		if(!FindTextureExtension(pTextures[i].name,path))
		{
			sprintf(buffer3,"Failed to load  Texture:  %s\n",pTextures[i].name);
			//messageBox();
			msg(buffer3);
			non_founded++;


			bool hol = true;
		}
		else
		{
		CreateTexture((unsigned int &)textures[i], path,pTextures[i].flags);
		}
	}
	
}

void ReadLump(int lump,int size, int &num ,void **dest)
{
	
	num = Lumps[lump].length/size;
	*dest = malloc(num*size);
	
	
	
	memcpy(*dest,bspData + (Lumps[lump].offset), num* size);
	/*
	pak_seek(Lumps[lump].offset,0);
	pak_read(*dest,num,size);
	*/
}

void SwapPlaneCoord()
{
	for(int i=0;i<numPlanes;i++)
	{
		float temp = pPlanes[i].normal.y;
		pPlanes[i].normal.y = pPlanes[i].normal.z;
		pPlanes[i].normal.z = -temp;
	}
}


void SwapNodeCoord()
{
	// Swap the y and z values, and negate the new z so Y is up.
	
		
	for (int i=0; i < numNodes; i ++)
	{
		float temp = pNodes[i].maxBound.y;
		pNodes[i].maxBound.y = pNodes[i].maxBound.z;
		pNodes[i].maxBound.z = -temp;

		temp = pNodes[i].minBound.y;
		pNodes[i].minBound.y = pNodes[i].minBound.z;
		pNodes[i].minBound.z = -temp;
	}
}

void SwapVertexCoord()
{
	for(int i=0;i<numVerts;i++)
	{
	

		int temp;
		temp = pVerts[i].position.y;
		pVerts[i].position.y = pVerts[i].position.z;
		pVerts[i].position.z = -temp;

		pVerts[i].tex.y *=-1;
		
	}
}

void SwapLeafCoord()
{
	for (int i=0;i<numLeafs;i++)
	{
		float temp;
		temp = pLeafs[i].min.y;
		pLeafs[i].min.y = pLeafs[i].min.z;
		pLeafs[i].min.z = -temp;

		temp = pLeafs[i].max.y;
		pLeafs[i].max.y = pLeafs[i].max.z;
		pLeafs[i].max.z = -temp;

	}
}

int IsClusterVisable(int current,int test)
{


	if(!clusters.pBitSets || current < 0) return 1;

	// Use binary math to get the 8 bit visibility set for the current cluster
	byte visSet = clusters.pBitSets[(current*clusters.bytesPerCluster) + (test / 8)];
	
	// Now that we have our vector (bitset), do some bit shifting to find if
	// the "test" cluster is visible from the "current" cluster, according to the bitset.
	int result = visSet & (1 << ((test) & 7));

	// Return the result ( either 1 (visible) or 0 (not visible) )
	return ( result );


}

void BspCreateClusterLists()
{
	pLeafClusters = new LeafCluster[clusters.numClusters];
	
	int j,k,a;

	a = 0;
	for(j=0;j<clusters.numClusters;j++)
	{
		//a=0;
		for (k=0;k<numLeafs;k++)
		{
			
		  if(j==0)
		  {
			if(pLeafs[k].cluster == -1)
			{
				pLeafClusters[0].Leafs.push_back(pLeafs[k]);
				continue;
				
			}
		  }

			if(pLeafs[k].cluster == j)
			{
				pLeafClusters[j].Leafs.push_back(pLeafs[k]);
					a++;
				
			
			}
		}
	}
	
	int sum = 0;
	for (j=0;j<clusters.numClusters;j++)
	{
		sum+=pLeafClusters[j].Leafs.size();
	}
	
	
	pClusterList = new ClusterList[clusters.numClusters];

int m = 0;
	for(j=0;j<clusters.numClusters;j++)
	{
		m=0;
		pClusterList[j].visibleClusters.clear();
		pClusterList[j].visibleClusters.push_back(j);
		for(k=0;k<clusters.numClusters;k++)
		{
			
			if(k ==j)
			continue;
			if(IsClusterVisable(j,k))
			{
				pClusterList[j].visibleClusters.push_back(k);
				m++;
			}
		}

	}
	
	drawn.Resize(numFaces);
}

void BspLoadVisibilityData()
{
//	fseek(fp,Lumps[kVisData].offset,SEEK_SET);

	if(Lumps[kVisData].length)
	{
		memcpy(&(clusters.numClusters),bspData+(Lumps[kVisData].offset),sizeof(int));
//		fread(&(clusters.numClusters),1,sizeof(int),fp);

memcpy(&(clusters.bytesPerCluster),bspData+(Lumps[kVisData].offset+4),sizeof(int));
//		fread(&(clusters.bytesPerCluster),1,sizeof(int),fp);

		int size = clusters.numClusters * clusters.bytesPerCluster;
		clusters.pBitSets = (unsigned char*)malloc((size * sizeof(unsigned char)));//new unsigned char[size];
	
		if(!clusters.pBitSets)
			return;
int offset = ((Lumps[kVisData].offset)+8);
memcpy(clusters.pBitSets,bspData+offset,((sizeof(unsigned char))*(size)));

/*
pak_seek(Lumps[kVisData].offset+8,0);
pak_read(clusters.pBitSets,num,size);
//		fread(clusters.pBitSets,1,sizeof(unsigned char)*size,fp);
*/
	}
	else
		clusters.pBitSets = NULL;
}

void BspReadLumps()
{


ReadLump(kVerts,sizeof(bspVertex),numVerts,(void**)&pVerts);


ReadLump(kFaces,sizeof(bspFace),numFaces,(void**)&pFaces);
//ReadLump(kTextures,sizeof(bspTexture),numTextures,(void**)&pTextures);

/*DO ME!*/
numLightMaps = Lumps[kLightMaps].length/sizeof(bspLightMap);



ReadLump(kNodes,sizeof(bspNode),numNodes,(void**)&pNodes);
ReadLump(kLeafs,sizeof(bspLeaf),numLeafs,(void**)&pLeafs);
ReadLump(kLeafFaces,sizeof(int),numLeafFaces,(void**)&pLeafFaces);
ReadLump(kPlanes,sizeof(bspPlane),numPlanes,(void**)&pPlanes);
ReadLump(kBrushes,sizeof(bspBrush),numBrushes,(void**)&pBrushes);
ReadLump(kBrushSides,sizeof(bspBrushSide),numBrushSides,(void**)&pBrushSides);
ReadLump(kLeafBrushes,sizeof(int),numLeafBrushes,(void**)&pLeafBrushes);


ReadLump(kMeshVerts,sizeof(int),numIndicies,(void**)&m_pIndexArray);



SwapVertexCoord();
SwapLeafCoord();
SwapPlaneCoord();


}




void AllocLumpMem()
{
	/*
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
pVerts = new bspVertex[numVerts];
*/
}

/*
int BspLoadHeader(const char *filename)
{
	bspHeader *Header;
	int length;

    if(!(pak_open(filename)))
		return 0;

	
	
	length = pak_getlen();
    bspData = (unsigned char*)malloc(length*sizeof(unsigned char));
    
	
	   
    Header = (bspHeader *)bspData;
	
	pak_read(bspData, length, 1);
	memcpy(Lumps,bspData+sizeof(bspHeader),sizeof(bspLump)*kMaxLumps);
    
	
 

	
/*
	pak_read(&Header,1,sizeof(bspHeader));
	pak_read(Lumps,kMaxLumps,sizeof(bspLump));
*/
	
//	return 1;

//}


int BspLoadHeader(const char *filename)
{
	
	

	int length;
	
	
	FILE *fp = fopen(filename,"rb");
	length = GetLen(fp);

	
    bspData = (unsigned char*)malloc(length*sizeof(unsigned char));
    
	
	   
    Header = (bspHeader *)bspData;
	
	//pak_read(bspData, length, 1);

	fread(bspData,length,1,fp);
	memcpy(Lumps,bspData+sizeof(bspHeader),sizeof(bspLump)*kMaxLumps);
    
	
	fclose(fp); 
	
	
	return 1;
}

void bspCleanUp()
{
		
	RELEASEC(Header);
	RELEASEC(pVerts);
	//RELEASEC(bspData);

	
	RELEASEC(pFaces);
	RELEASEC(pNodes);
	RELEASEC(pLeafs);
	RELEASEC(pLeafFaces);
	RELEASEC(pPlanes);
	RELEASEC(pBrushes);
	RELEASEC(pBrushSides);
	RELEASEC(pLeafBrushes);
	RELEASEC(pLeafBrushes);
	RELEASEC(m_pIndexArray);
	RELEASEC(clusters.pBitSets);
	
	RELEASEC(pTextures);
	RELEASEC(shaders);
	RELEASEC(pShaderRefs);
	RELEASEC(textures);

	
	RELEASE(pLeafClusters);
	RELEASE(pClusterList); 



}



void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	// Turn on texture mapping if it's not already
	//glEnable(GL_TEXTURE_2D);

	// Bind the BACK texture of the sky map to the BACK side of the cube
	//glBindTexture(GL_TEXTURE_2D, 58);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	
		CVector3 eyepos = g_Camera.Position();
	
	x = eyepos.x;
	y= eyepos.y;
	z = eyepos.z;
	
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

			float skyheight = 512.0f;
	/* Center skybox on camera to give the illusion of a larger space */
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glTranslatef(eyepos.x, eyepos.y, eyepos.z);
//  glScalef(skyheight, skyheight, skyheight);

	
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the BACK Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
		
	glEnd();

	// Bind the FRONT texture of the sky map to the FRONT side of the box
//	glBindTexture(GL_TEXTURE_2D, g_Texture[FRONT_ID]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);	
	
		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
//	glBindTexture(GL_TEXTURE_2D, g_Texture[BOTTOM_ID]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
	
		// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// Bind the TOP texture of the sky map to the TOP side of the box
//	glBindTexture(GL_TEXTURE_2D, g_Texture[TOP_ID]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the TOP Side
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		
	glEnd();

	// Bind the LEFT texture of the sky map to the LEFT side of the box
//	glBindTexture(GL_TEXTURE_2D, g_Texture[LEFT_ID]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();

	// Bind the RIGHT texture of the sky map to the RIGHT side of the box
//	glBindTexture(GL_TEXTURE_2D, g_Texture[RIGHT_ID]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		

		// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();

	/* Restore world space */
//   glMatrixMode(GL_MODELVIEW);
//   glPopMatrix();

}

/*
void BspRenderSky(int *skylist, int face)
{
	int s, i, shader;
    float skyheight;
    unsigned int *elem;

    shader = pFaces[skylist[0]].textureID;
    skyheight = shaders[shader].skyheight;
    arrays.numverts = arrays.numelems = 0;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    /* Center skybox on camera to give the illusion of a larger space */
    
/*

glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

	CVector3 eyepos = g_Camera.Position();
    //glTranslatef(eyepos.x,eyepos.y, eyepos.z);
     glTranslatef(388.0,-136.0, 128.0);
	
	glScalef(skyheight, skyheight, skyheight);

    /* FIXME: Need to cull skybox based on face list */
    




/*
for (s=0; s < 5; s++)
    {
		elem = skybox->elems;
		for (i=0; i < skybox->numelems; i++)
		{
			arrays.elems[arrays.numelems++] = arrays.numverts + *elem++;
		}
		for (i=0; i < skybox->numpoints; i++)
		{
			
			vec_copy(skybox->points[s][i], arrays.verts[arrays.numverts]);
			arrays.verts[arrays.numverts][3] = 1.0f;
			vec2_copy(skybox->tex_st[s][i], arrays.tex_st[arrays.numverts]);
			arrays.numverts++;
		}
    }

//render_flush(shader, 0);
	BspRenderFace2(face,shader);

  
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
*/

void BspRenderMap(const CVector3& pos)
{
	static bool once = false;

	numsky = 0;
	facelist.empty();
	CVector3 oldpos;

oldpos = g_Camera.Position();
	
	
	g_Camera.Update();
g_Camera.SetPosition(g_Camera.m_vNextPosition);
g_Camera.SetView(g_Camera.m_vNextView);
//g_Camera.CalculateFrameRate();


	static int collision =0;
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].position));
	
	
	//glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].tex));
	
	//glClientActiveTextureARB(GL_TEXTURE1_ARB);
	//glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].lightMap));
	




	
	/*
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	*/


	drawn.ClearAll();

	int i = numLeafs;
	
	
	
	int currentLeaf = BspFindLeaf(pos);

	int currentcluster = pLeafs[currentLeaf].cluster;

	
	/*
	Level rendering
	. 
	1. get the current cluster
	2. get its PVS
	3. for each cluster in the PVS
	4.  loop through the leafs in the cluster
	5. check intersection with the fustrum
	6. render
	
	*/
int plane;
float dist;

if(currentcluster == -1)
currentcluster=0;

//sprintf(buffer3,"Current Cluster % d",currentcluster);		
	


	glDepthMask(GL_TRUE);
//glDepthMask(GL_TRUE);

    /* FIXME: color buffer clear can be optionally removed */
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
 
    glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();


for (int j=0;j<pClusterList[currentcluster].visibleClusters.size();j++)
	{
		int index = pClusterList[currentcluster].visibleClusters[j];
	
		
	//sprintf(buffer2,"pos x: %f pos y: %f pos z: %f",g_Camera.Position().x,g_Camera.Position().y,g_Camera.Position().z);		

	for(int k=0;k<pLeafClusters[index].Leafs.size();k++)
			{


				bspLeaf *leaf = &pLeafClusters[index].Leafs[k];
		
			
				
			
				if(!frustum.BoxInFrustum(leaf->min.x,leaf->min.y,leaf->min.z,
										leaf->max.x,leaf->max.y,leaf->max.z))
				
				continue;
				

				

			    int	faces = leaf->numLeafFace;

				while(faces--)
				{
					int faceindex = pLeafFaces[leaf->leafFaceIndex + faces];
				
					//if((pFaces[faceindex].type!=FACE_POLYGON) || (pFaces[faceindex].type!=3)) 
					//continue;
			
					if(!drawn.On(faceindex))
					{
					drawn.Set(faceindex);
					BspRenderFace2(faceindex);
					}
				}
			
			}	
		
	}



//now that we have accounted for collisions, update the cameras view
g_Camera.SetPosition(g_Camera.m_vNextPosition);
g_Camera.SetView(g_Camera.m_vNextView);
}


void BspRenderFlush()
{
	for(int i = 0; i <facelist.size(); i++)
	{
		BspRenderFace(facelist[i]);
	}
}

void BspRenderFace(int faceIndex)
{

	
		bspFace *Face = &pFaces[faceIndex];


		// Turn on texture mapping and bind the face's texture map
	
		
		//glDepthMask(GL_TRUE);
     	//glDepthFunc(515);

		glDisable(GL_BLEND);
		

		glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].lightMap));
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, lightMaps[Face->lightMapID]);
	

		glDrawArrays(GL_TRIANGLE_FAN,Face->startVertex,Face->numVerts);
	
	
		

		glEnable(GL_BLEND);
		//glDepthMask(GL_FALSE);
		//glDepthFunc(GL_EQUAL);
		glBlendFunc(GL_ONE,GL_SRC_COLOR);

		
		glEnable(GL_TEXTURE_2D);
		glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].tex));
		glBindTexture(GL_TEXTURE_2D,  textures[Face->textureID]);
		
		glDrawArrays(GL_TRIANGLE_FAN,Face->startVertex,Face->numVerts);
	


}


static void
render_clearstate(ShaderPass *pass)
{
	
    if (pass->flags & SHADER_TCMOD)
    {
	/* Revert to identity texture transform */
	
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
    }
	/*
    if (pass->rgbgen == SHADER_GEN_VERTEX)
	glDisableClientState(GL_COLOR_ARRAY);
*/
}

int RenderSetState(ShaderPass *pass,int faceID)
{
	
	
	


	
	bspFace *Face = &pFaces[faceID];




	if (pass->flags & SHADER_LIGHTMAP)
	{
	/* Select lightmap texture */
		
	
		glEnable(GL_TEXTURE_2D);
	
	
	//glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].lightMap));
	glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[Face->startVertex].lightMap));
	
	glBindTexture(GL_TEXTURE_2D, lightMaps[Face->lightMapID]);
	
	}
	else if (pass->flags & SHADER_ANIMMAP)
	{
		/*TODO*/
		int texID;

		glEnable(GL_TEXTURE_2D);
		
//m_pVerts[pFace->startVertIndex].vTextureCoord)

		//glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].tex));
		glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[Face->startVertex].tex));
		
		
		//glBindTexture(GL_TEXTURE_2D,  textures[Face->textureID]);
		if (pass->texture < 0) return 0;
		texID = textures[pass->texture];
		if (texID < 0) return 0;
		
		glBindTexture(GL_TEXTURE_2D,  texID);
		
	}
	else
	{
		
		
		int texID;

		glEnable(GL_TEXTURE_2D);

		
		//glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].tex));
		glTexCoordPointer(2,GL_FLOAT,sizeof(bspVertex), &(pVerts[Face->startVertex].tex));
		
		
		//glBindTexture(GL_TEXTURE_2D,  textures[Face->textureID]);
		if (pass->texture < 0) return 0;
		texID = textures[pass->texture];
		if (texID < 0) return 0;
		
		glBindTexture(GL_TEXTURE_2D,  texID);
		
		
	}



	if (pass->flags & SHADER_BLEND)
    {
		
	glEnable(GL_BLEND);
	glBlendFunc(pass->blend[0],pass->blend[1]);
    }
    else
	glDisable(GL_BLEND);
	

	/*
	if (pass->flags & SHADER_ALPHAFUNC)
    {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(pass->alpha, pass->);
    }
    else
	{
		glDisable(GL_ALPHA_TEST);
	}
*/


	

	glDepthFunc(pass->depth);
	
	if (pass->flags & SHADER_DEPTHWRITE)
		glDepthMask(GL_TRUE);
    else
		glDepthMask(GL_FALSE);


	return 1;

}

/*
static void
render_stripmine(int numelems, int *elems)
{
    int toggle;
    unsigned int a, b, elem;

    /* Vertexes are in tristrip order where possible.  If we can't lock
     * the vertex arrays (glLockArraysEXT), then it's better to send
     * tristrips instead of triangles (less transformations).
     * This function looks for and sends tristrips.
     */

    /* Tristrip order elems look like this:
     *  0 1 2 2 1 3 2 3 4 4 3 5 4 5 7 7 5 6  <-- elems
     *    b a a b b a b a a b b a b a a b b  <-- ab pattern
     *    \ 1 / \ 2 / \ 3 / \ 4 / \ 5 /      <-- baa/bba groups
     */
/*    
    elem = 0;
    while (elem < numelems)
    {
	toggle = 1;
	glBegin(GL_TRIANGLE_STRIP);
	
	glArrayElement(elems[elem++]);
	b = elems[elem++];
	glArrayElement(b);
	a = elems[elem++];
	glArrayElement(a);
	
	while (elem < numelems)
	{
	    if (a != elems[elem] || b != elems[elem+1])
		break;
	    
	    if (toggle)
	    {
		b = elems[elem+2];
		glArrayElement(b);
	    }
	    else
	    {
		a = elems[elem+2];
		glArrayElement(a);
	    }
	    elem += 3;
	    toggle = !toggle;
	}
	glEnd();
    }
}
*/
void BspRenderFace2(int faceIndex,int shader)
{
	
	
	bspFace *Face = &pFaces[faceIndex];
	Shader *currShader;

//	if(shader == -1)
//	{
	    //glVertexPointer(3,GL_FLOAT,sizeof(bspVertex), &(pVerts[0].position));
		
		glVertexPointer(3,GL_FLOAT,sizeof(bspVertex), &(pVerts[Face->startVertex].position));
		
		currShader = &shaders[Face->textureID];

		bool br;

//	}
//	else
//	{
//		glVertexPointer(4, GL_FLOAT, 0, arrays.verts);
//		currShader = &shaders[shader];
//		
//	}
	 

	/* Face culling */
    
		
	if (currShader->flags & SHADER_NOCULL)
		glDisable(GL_CULL_FACE);
    else
		glEnable(GL_CULL_FACE);

	


	int numP = currShader->numPasses;

	if((numP < 0) || numP > 4)
		return;

	if(!currShader)
		return;

	
		
		// Turn on texture mapping and bind the face's texture map
	for (int j=0; j< numP; j++)
	{
		
		
		
		if(!RenderSetState(&currShader->pass[j],faceIndex))
			continue;
		
		
		
		if(currShader->flags == SHADER_SKY)
		{
			glFrontFace(GL_CW);
			glEnable(GL_TEXTURE_2D);
			CreateSkyBox(0, 0, 0, 3000, 3000, 3000);
			glFrontFace(GL_CCW);
			continue;
		}
		

		//	glDrawArrays(GL_TRIANGLE_FAN,Face->startVertex,Face->numVerts);
		
		
			
		
		glDrawElements(GL_TRIANGLES, Face->numMeshVerts,
		GL_UNSIGNED_INT,&m_pIndexArray[Face->meshIndex]);
		

	}
 
	


}


int BspFindLeaf(const CVector3 &pos)
{
	
	

	int i = 0;
	float distance = 0.0f;

	


	while(i>=0)
	{
		const bspNode &node = pNodes[i];
		const bspPlane &plane = pPlanes[node.plane];

		distance = plane.normal.x * pos.x +
                   plane.normal.y * pos.y +
				   plane.normal.z * pos.z - plane.d;

		if (distance >=0 )
		{
			i = node.front;
		}
		else
		{
			i = node.back;
		}

	}
	
	return ~i;
	
}

//cout<<"verts: "<<numVerts<<endl;	
//cout<<"faces: "<<numFaces<<endl;	
//cout<<"Textures: "<<numTextures<<endl;




//pTextures = new bspTexture[numTextures];
//pak_seek(Lumps[kTextures].offset,0);
//pak_read(pTextures,numTextures,sizeof(bspTexture));


//pTextures = new bspTexture[numTextures];
//pak_seek(Lumps[kTextures].offset,0);
//pak_read(pTextures,numTextures,sizeof(bspTexture));
