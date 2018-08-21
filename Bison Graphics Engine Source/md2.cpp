
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <gl\glaux.h>
#include <crtdbg.h>
#include <assert.h>
#include "image.h"
using namespace std;


extern float time;


#include "Texture.h"
#include "CVector.h"
#include "object3d.h"
#include "MD2.H"

#define msg(x) MessageBox(NULL,x,"ERROR",MB_OK);


extern CVector3 *Verts;
// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))


void DrawQuad2(CVector3 v1, CVector3 v2,CVector3 v3, CVector3 v4,bool outline)
{



if(!outline)
{
	 glBegin(GL_QUADS);
	
	
	  
			glColor3f(1,0,0);
			glVertex3f(v1.x,v1.y,v1.z);
			
			glColor3f(1,1,1);
			glVertex3f(v3.x,v3.y,v3.z);

			glColor3f(0,0,1);
			glVertex3f(v4.x,v4.y,v4.z);

			glColor3f(0,1,0);
			glVertex3f(v2.x,v2.y,v2.z);
			
			


			
		
	glEnd();
}

if(outline)
{
	glDisable(GL_CULL_FACE);	
	glColor3f(1,1,1);
	glLineWidth(1);
	
	glBegin(GL_LINES);
	
	glVertex3f(v1.x,v1.y,v1.z);
	glVertex3f(v3.x,v3.y,v3.z);

	glEnd();
    glLineWidth(1);


glEnable(GL_CULL_FACE);
}

}


// This calculates a vector between 2 points and returns the result
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;							// The variable to hold the resultant vector

	vVector.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
	vVector.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
	vVector.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

	return vVector;								// Return the resultant vector
}


void CModelMD2::CleanUp(CModel3d *obj)
{

//_BLOCK_TYPE_IS_VALID(p_TexCoord);	


	for(int i=0;i<obj->numFrames;i++)
	{
		RELEASE(obj->pObject[i].pFace);
		RELEASE(obj->pObject[i].pNormals);
		RELEASE(obj->pObject[i].pTexCoords);
		RELEASE(obj->pObject[i].pVerts);
		RELEASE(obj->pObject[i].pSkins);
	}

	RELEASE(pSkinID);
//	RELEASE(obj->pObjectFaces);
	RELEASE(p_TexCoord);
	RELEASE(p_Triangles);
	RELEASE(p_Frames);
	
			

}
CModelMD2::~CModelMD2()
{
	
	

}

int CModelMD2::ComputeNormals(CObject3d *obj)
{
	
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
	
	CVector3 *pNormals = new CVector3[obj->numFaces];
	CVector3 *pTempNormals = new CVector3[obj->numFaces];
	obj->pNormals = new CVector3[obj->numVerts];
	obj->pPlanes = new PLANE[obj->numFaces];
//	Verts = new CVector3[obj->numFamces];
	

	int i,j;
	

for (i=0;i<obj->numFaces;i++)
{
vPoly[0] = obj->pVerts[obj->pFace[i].vertexIndex[0]];
vPoly[1] = obj->pVerts[obj->pFace[i].vertexIndex[1]];
vPoly[2] = obj->pVerts[obj->pFace[i].vertexIndex[2]];

vVector1 = Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
vVector2 = Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon

vNormal  = Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
vNormal  = Normalize(vNormal);				// Normalize the cross product to give us the polygons normal
			
			
obj->pPlanes[i].normal = vNormal;
obj->pPlanes[i].d = (vNormal * vPoly[0]);

pNormals[i] = vNormal;							// Assign the normal to the list of normals
}	

	
	//vertex normals
	CVector3 vSum(0.0, 0.0, 0.0); // = {0.0, 0.0, 0.0};
	CVector3 vZero = vSum;
	int shared=0;

	for (i=0;i<obj->numVerts;i++)
	{
		for (j=0;j<obj->numFaces;j++)
		{
			if((obj->pFace[j].vertexIndex[0] == i) ||
				(obj->pFace[j].vertexIndex[1] == i) ||
				(obj->pFace[j].vertexIndex[2] == i))
			{
			//	vSum = AddVector(vSum,pTempNormals[j]);
				vSum = vSum + pTempNormals[j];
				shared++;
			}
			
		}
		//obj->pNormals[i] = DivideVectorByScaler(vSum,float(-shared));
		obj->pNormals[i] = vSum/float(-shared);
		obj->pNormals[i] = Normalize(obj->pNormals[i]);

		vSum = vZero;
		shared = 0;

	}

	delete [] pTempNormals;
	delete [] pNormals;


	return 0;

}

int CModelMD2::Convert(CModel3d *pModel)
{
	SetRenderType(pModel,GL_POINTS);
	
	memset(pModel,0,sizeof(CModel3d));
	pModel->numFrames = m_Header.numFrames;
	
	pModel->interpol = 0;
	pModel->elapsedTime  = 0;
	pModel->lastTime = 0;	
	
	

	ParseAnimations(pModel);
	
	for (int j=0;j<m_Header.numFrames; j++)
	{
		CObject3d obj = {0};
		
		obj.numFaces = m_Header.numTris;
		obj.numVerts = m_Header.numXYZ;
		obj.numTexCoords = m_Header.numST;

		obj.pVerts = new CVector3[obj.numVerts];
		

		int i;

		for (i=0; i<obj.numVerts; i++)
		{
			obj.pVerts[i].x = p_Frames[j].pVertices[i].vertex[0];
			obj.pVerts[i].y = p_Frames[j].pVertices[i].vertex[1];
			obj.pVerts[i].z = p_Frames[j].pVertices[i].vertex[2];
		}

		delete p_Frames[j].pVertices;

		if(j > 0)
		{
			pModel->pObject.push_back(obj);
			continue;
		}
		
		obj.pTexCoords = new CVector2[obj.numTexCoords];
		obj.pFace = new OBJ_FACE[obj.numFaces];

			for (i=0;i<obj.numTexCoords;i++)
			{
				//obj.pTexCoords[i].x = p_TexCoord[i].u; /// (float) m_Header.skinwidth;
				//obj.pTexCoords[i].y = 1 -  p_TexCoord[i].v; /// (float) m_Header.skinheight;
				
				obj.pTexCoords[i].x = p_TexCoord[i].u; /// (float) m_Header.skinwidth;
				obj.pTexCoords[i].y = p_TexCoord[i].v; /// (float) m_Header.skinheight;
				
			}

			

			for (i=0;i<m_Header.numTris;i++)
			{
				obj.pFace[i].vertexIndex[0] = p_Triangles[i].vertexIndex[0];
				obj.pFace[i].vertexIndex[1] = p_Triangles[i].vertexIndex[1];
				obj.pFace[i].vertexIndex[2] = p_Triangles[i].vertexIndex[2];

				obj.pFace[i].texIndex[0] = p_Triangles[i].texIndex[0];
				obj.pFace[i].texIndex[1] = p_Triangles[i].texIndex[1];
				obj.pFace[i].texIndex[2] = p_Triangles[i].texIndex[2];
			}
			
			pModel->pObject.push_back(obj);
		}
	pModel->pObject[0].textureID = this->textureID;
	pModel->pObject[0].numberSkins = numberSkins;
	pModel->pObject[0].pSkins = new int[numberSkins];


	return 1;
}


int CModelMD2::Load(CModel3d *pModel, char* filename, char *skin)
{
	p_file = fopen(filename,"rb");

	if(!p_file)
	{
msg("Couldnt Open File")
		return 0;
	}

	fread(&m_Header,1,sizeof(MD2_HEADER),p_file);

	if(m_Header.version!=8)
	{
		msg("Wrong Version")
		return 0;
	}

	unsigned char buffer[MD2_MAX_FRAMESIZE]; 
	MD2_TEXINDEX *texIndex = new MD2_TEXINDEX[m_Header.numST];
	
	p_Skins = new MD2_SKIN[m_Header.numSkins];
    p_TexCoord = new MD2_TEXCOORD[m_Header.numST];

	p_Triangles = new MD2_FACE[m_Header.numTris];
	p_Frames = new MD2_FRAME[m_Header.numFrames];

	numberSkins = m_Header.numSkins;

	fseek(p_file,m_Header.offsetSkins,SEEK_SET);
	fread(p_Skins,sizeof(MD2_SKIN),m_Header.numSkins,p_file);

	fseek(p_file,m_Header.offsetST,SEEK_SET);
	fread(texIndex,sizeof(MD2_TEXINDEX),m_Header.numST,p_file);

	fseek(p_file,m_Header.offsetTris,SEEK_SET);
	fread(p_Triangles,sizeof(MD2_FACE),m_Header.numTris,p_file);

	fseek(p_file,m_Header.offsetFrames,SEEK_SET);
	
	
	
	for(int i = 0; i < m_Header.numFrames; i++)
	{
	
		MD2_FRAME_INDEX *p_frame = (MD2_FRAME_INDEX*)buffer;

		
		p_Frames[i].pVertices = new MD2_VERTEX[m_Header.numXYZ];

		fread(p_frame,1, m_Header.framesize,p_file);

		strcpy(p_Frames[i].strName,p_frame->name);

		MD2_VERTEX *pVerts = p_Frames[i].pVertices;

		for(int j=0;j<m_Header.numXYZ;j++)
		{
			pVerts[j].vertex[0] = p_frame->framePoint[j].vertex[0] * p_frame->scale[0] + p_frame->translate[0];
			pVerts[j].vertex[2] = (p_frame->framePoint[j].vertex[1] * p_frame->scale[1] + p_frame->translate[1]);
			pVerts[j].vertex[1] = p_frame->framePoint[j].vertex[2] * p_frame->scale[2] + p_frame->translate[2];
		}

	}
	
	
	tImage *tex;
	if(skin)
	{
	tex = 	LoadSkin(skin,textureID);
	}
	
	else
	{
		for (int i =0; i<numberSkins; i++)
		{
			LoadSkin(p_Skins[i],pSkinID[i]);
		}
	}
	

	MD2_TEXINDEX *stPtr;
	stPtr = texIndex;
    
	
	
	for (i = 0; i < m_Header.numST; i++)
     {
		 p_TexCoord[i].u = (float)stPtr[i].u / tex->sizeX;
		 p_TexCoord[i].v =  (float)stPtr[i].v / tex->sizeY;
     
     }

	delete [] texIndex;
	 
	Convert(pModel);

	return 1;
}

tImage *CModelMD2::LoadSkin(char *skin, unsigned int &texID)
{

		// Define a pointer to a tImage
	tImage *pImage = NULL;

	// If the file is a jpeg, load the jpeg and store the data in pImage
	if(strstr(skin, ".jpg"))
	{
		pImage = LoadJPG(skin);
	}
	// If the file is a tga, load the tga and store the data in pImage
	else if(strstr(skin, ".tga"))
	{
		pImage = LoadTGA(skin);
	}
	// If the file is a bitmap, load the bitmap and store the data in pImage
	else if(strstr(skin, ".bmp"))
	{
		pImage = LoadBMP(skin);
	}
	else if(strstr(skin, ".pcx"))
	{
		pImage = LoadPCXTexture(skin);
	}
	// Else we don't support the file format that is trying to be loaded
	else
		return false;

	// Make sure valid image data was given to pImage, otherwise return false
	if(pImage == NULL)								
		return false;
	
	glGenTextures(1,&texID);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glBindTexture(GL_TEXTURE_2D,texID);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	
	if(pImage->type!=PCX)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, pImage->sizeX, pImage->sizeY,
            GL_RGBA, GL_UNSIGNED_BYTE, pImage->data);


	return pImage;
}

int CModelMD2::ParseAnimations(CModel3d *pModel)
{
	string strEnd = "";
	string strStart;
	AnimInfo animInfo;


	for (int i = 0; i<pModel->numFrames; i++)
	{
		strStart = p_Frames[i].strName;
		
		int frameNum=0;
		for(int j=0; j<strStart.length();j++)
		{
			if ( (isdigit(strStart[j]))  && (j>= strStart.length()-2) )
			{
				frameNum = atoi(&strStart[j]);
				strStart.erase(j, strStart.length() - j);
				break;
			}
		}

		if( strStart!=strEnd || i == pModel->numFrames -1 )
		{
			if(strEnd != "")
			{
				strcpy(animInfo.frameName,strEnd.c_str());
				animInfo.endFrame = i;
				
				pModel->pAnimInfo.push_back(animInfo);
				
				
				memset(&animInfo,0,sizeof(AnimInfo));
				pModel->numAnimations ++;
				
			}

			animInfo.startFrame = frameNum - 1 + i;
			//assert(!animInfo.startFrame>200);
		}

		strEnd = strStart;
	}



	return 1;
}




float ReturnCurrentTime(CModel3d *pModel, int nextFrame)
{
	//static float elapsedTime   = 0.0f;
	//static float lastTime	  = 0.0f;

float	kAnimationSpeed = 5.0f;

	
	// This function is very similar to finding the frames per second.
	// Instead of checking when we reach a second, we check if we reach
	// 1 second / our animation speed. (1000 ms / kAnimationSpeed).
	// That's how we know when we need to switch to the next key frame.
	// In the process, we get the t value for how we are at to going to the
	// next animation key frame.  We use time to do the interpolation, that way
	// it runs the same speed on any persons computer, regardless of their specs.
	// It might look chopier on a junky computer, but the key frames still be
	// changing the same time as the other persons, it will just be not as smooth
	// of a transition between each frame.  The more frames per second we get, the
	// smoother the animation will be.

	// Get the current time in milliseconds
	float time = GetTickCount();

	// Find the time that has elapsed since the last time that was stored
	pModel->elapsedTime = time - pModel->lastTime;

	// To find the current t we divide the elapsed time by the ratio of 1 second / our anim speed.
	// Since we aren't using 1 second as our t = 1, we need to divide the speed by 1000
	// milliseconds to get our new ratio, which is a 5th of a second.
	float t = pModel->elapsedTime / (1000.0f / kAnimationSpeed);
	
	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
	if (pModel->elapsedTime >= (1000.0f / kAnimationSpeed) )
	{
		// Set our current frame to the next key frame (which could be the start of the anim)
		pModel->currentFrame = nextFrame;

		// Set our last time to the current time just like we would when getting our FPS.
		pModel->lastTime = time;
	}

	// Return the time t so we can plug this into our interpolation.
	return t;
}

void SetRenderType(CModel3d *pModel,int type)
{
	pModel->render_type = type;
}

void AnimateModel(CModel3d *pModel,float speed)
{
	

	
	int currFrame = pModel->currentFrame;

	AnimInfo *frame;
	
	
	frame = &(pModel->pAnimInfo[pModel->currentAnimation]);
	
	
	pModel->nextFrame = (pModel->currentFrame + 1) % frame->endFrame;
	
	
	
	if (pModel->nextFrame == 0)
		pModel->nextFrame=frame->startFrame;

	int nextFrame = pModel->nextFrame;
	

	time = ReturnCurrentTime(pModel,pModel->nextFrame);
	
	
	
glBindTexture(GL_TEXTURE_2D,pModel->pObject[0].textureID);

glBegin(pModel->render_type);
	
	for(int i=0;i<pModel->pObject[0].numFaces;i++)
	{
		
		CVector3 point = pModel->pObject[0].pVerts[pModel->pObject[0].pFace[i].vertexIndex[0]];
		int index,index2;
		
	
		for (int j=0; j<3; j++)
		{
			index = pModel->pObject[0].pFace[i].vertexIndex[j];
			index2 = pModel->pObject[0].pFace[i].texIndex[j];
			
			float u,v;
			u = pModel->pObject[0].pTexCoords[index2].x;
			v = pModel->pObject[0].pTexCoords[index2].y;
		//	glTexCoord2f(pModel->pObject[0].pTexCoords[index2].x,pModel->pObject[0].pTexCoords[index2].y);
			glTexCoord2f(u,v);
			CVector3 vPoint1;
			CVector3 vPoint2;
			

			vPoint1 = pModel->pObject[currFrame].pVerts[index];
			vPoint2 = pModel->pObject[nextFrame].pVerts[index];
			

			CVector3 point;
			
			point.x = vPoint1.x + time * (vPoint2.x - vPoint1.x);
			point.y = vPoint1.y + time * (vPoint2.y - vPoint1.y);
			point.z = vPoint1.z + time * (vPoint2.z - vPoint1.z);
			
			



			glVertex3f(point.x, // Find the interpolated X
								   point.y, // Find the interpolated Y
								   point.z);// Find the interpolated
		  
		}
		
	}
	glEnd();


	pModel->interpol += speed;


}


void CModelMD2::SetConn(CObject3d *obj)
{
	
	int numFound = 0;
	
	CVector3 v1,v2,v3;

	for(int l=0; l<obj->numFaces;l++)
	{
		for(int g=0;g<3;g++)
		{
			obj->pFace[l].faceNeighbour[g] = -1;
		}
	}
	
	
	for(int i=0; i <obj->numFaces; i ++)
	{
		numFound = 0;
		
		for(int j=0; j <obj->numFaces; j ++)
		{
			
			if(i==j)
				continue;
				

			for(int v=0; v<3;v++)
			{
				bool exists = false;
				
				int vertA1 = obj->pFace[i].vertexIndex[v];
				int vertA2 = obj->pFace[i].vertexIndex[( v+1 )%3];
				
				
				
				for(int x=0;x<3;x++)
				{
				//int	p2 = obj->pFace[j].vertexIndex[x];

				int vertB1 = obj->pFace[j].vertexIndex[x];
				int vertB2 = obj->pFace[j].vertexIndex[( x+1 )%3];

			// Check If They Are Neighbours - IE, The Edges Are The Same
				if (( vertA1 == vertB1 && vertA2 == vertB2 ) || ( vertA1 == vertB2 && vertA2 == vertB1 ))
				{
				obj->pFace[i].faceNeighbour[v] = j;
				obj->pFace[j].faceNeighbour[x] = i;
				exists = true;
				break;
				}

				
				
				
					if((obj->pFace[j].vertexIndex[x]==0))
					{
						if( ((j!=1) && (j!=2)) )
						{
							numFound++;	
						}
				
					}
				
				}

						

			}
		
		}
		
		
	}

	int count = 0;
char buffer[80];
for(l=0; l<obj->numFaces;l++)
	{
		for(int g=0;g<3;g++)
		{
			if(obj->pFace[l].faceNeighbour[g] == -1)
			{
				count++;
			}
		}
	}


}

int CModelMD2::SideOfPlane(const PLANE &plane, const CVector3 &pos)
{
	float distance;

	distance = (plane.normal.x * pos.x +
               plane.normal.y * pos.y +
				plane.normal.z * pos.z) - plane.d;

	if(distance > 0)
		return 1;
	else
		return -1;

}


float CModelMD2::DistanceFromPlane(const PLANE &plane, const CVector3 &pos)
{
	float distance;

	distance = plane.normal.x * pos.x +
               plane.normal.y * pos.y +
				plane.normal.z * pos.z - plane.d;

	return distance;
	
		
}


void ComputeModelViewInvers(CVector3 &dest, CVector3 &lpos)
{
	
	double mv[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	
		
	CVector3 xtrans(mv[0],mv[1],mv[2]);
	CVector3 ytrans(mv[4],mv[5],mv[6]);
	CVector3 ztrans(mv[8],mv[9],mv[10]);
	
	dest.x =  lpos.x + (-mv[12]); 
	dest.y =  lpos.y + (-mv[13]);
	dest.z =  lpos.z + (-mv[14]); 

    CVector3 mid = dest;	

	dest.x = (xtrans * mid ); 
	dest.y = (ytrans * mid ) ;
	dest.z = (ztrans * mid ); 
	
}

void CModelMD2::ProjectSiloute(CVector3 &lpos,CModel3d *pModel,float time,bool outline)
{
	
for(int k =0; k <pModel->pObject[0].numFaces; k++)
{
	for(int n=0;n<3;n++)
	{
		pModel->pObject[0].pFace[k].drawn[n] = 0;

	}
}


CVector3 olight;


int currFrame = pModel->currentFrame;
int nextFrame = pModel->nextFrame;
	

	//get the object space light
	ComputeModelViewInvers(olight,lpos);

	for (int i=0; i< pModel->pObject[0].numFaces; i++)
	{
		
		if(SideOfPlane(pModel->pObject[0].pPlanes[i],olight) !=-1)
		{
			for(int j=0;j<3;j++)
			{
				PLANE plane;
				
				int faceNeighbour = pModel->pObject[0].pFace[i].faceNeighbour[j];
	
				plane = pModel->pObject[0].pPlanes[faceNeighbour];
				if(SideOfPlane(plane,lpos)==-1)
				{
					
					
					//the first vertex of shadow volume
					int index = pModel->pObject[0].pFace[i].vertexIndex[j];
					
					
					
					CVector3 vPoint1 = pModel->pObject[currFrame].pVerts[index];
					CVector3 vPoint2 = pModel->pObject[nextFrame].pVerts[index];
					
					
					CVector3 v1,v2;
					
					//interpolate for animatoin
					v1.x = vPoint1.x + time * (vPoint2.x - vPoint1.x);
					v1.y = vPoint1.y + time * (vPoint2.y - vPoint1.y);
					v1.z = vPoint1.z + time * (vPoint2.z - vPoint1.z);
		
					//the second vert
					index = pModel->pObject[0].pFace[i].vertexIndex[(j+1)%3];
					
					vPoint1 = pModel->pObject[currFrame].pVerts[index];
					vPoint2 = pModel->pObject[nextFrame].pVerts[index];
					
					v2.x = vPoint1.x + time * (vPoint2.x - vPoint1.x);
					v2.y = vPoint1.y + time * (vPoint2.y - vPoint1.y);
					v2.z = vPoint1.z + time * (vPoint2.z - vPoint1.z);
		
					CVector3 p3 = (v1 - olight);
					CVector3 p4 = (v2 - olight);
			
					//extrude
					p3 = p3 *50000;
					p4 = p4 *50000;

		
				//draw the volume
				DrawQuad2(v1,p3,v2,p4,outline);
		
				}
			
			}
		}
		
	}

}


