#include <windows.h>
#include <gl\gl.h>						// Header File For The OpenGL32 Library
#include <gl\glu.h>						// Header File For The GLu32 Library
#include <gl\glaux.h>					// Header File For The GLaux Library
#include "image.h"
#include "Shader.h"

bool CreateTexture(UINT &texture, LPSTR strFileName, UINT flags)
{
	if(!strFileName) 
		return false;


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// Define a pointer to a tImage
	tImage *pImage = NULL;

	// If the file is a jpeg, load the jpeg and store the data in pImage
	if(strstr(strFileName, ".jpg"))
	{
		pImage = LoadJPG(strFileName);
	}
	// If the file is a tga, load the tga and store the data in pImage
	else if(strstr(strFileName, ".tga"))
	{
		pImage = LoadTGA(strFileName);
	}
	// If the file is a bitmap, load the bitmap and store the data in pImage
	else if(strstr(strFileName, ".bmp"))
	{
		pImage = LoadBMP(strFileName);
	}
	// Else we don't support the file format that is trying to be loaded
	else
		return false;

	// Make sure valid image data was given to pImage, otherwise return false
	if(pImage == NULL)								
		return false;

	// Generate a texture with the associative texture ID stored in the array
	
	//glGenTextures(1, &texture);

	// This sets the alignment requirements for the start of each pixel row in memory.
//	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture);



    

	
	
	// Assume that the texture is a 24 bit RGB texture (We convert 16-bit ones to 24-bit)
	int textureType = GL_RGB;

	// If the image is 32-bit (4 channels), then we need to specify GL_RGBA for an alpha
	if(pImage->channels == 4)
		textureType = GL_RGBA;
		
	
	unsigned char *tex;

	

	tex = pImage->data;
	int width  = pImage->sizeX;
	int height = pImage->sizeY;

	int size = width*height* (textureType == GL_RGB ? 3 : 4);
	
	width  /= 1 << 1;
	height /= 1 << 1;

    tex = (unsigned char *)malloc(size);

	gluScaleImage(textureType, pImage->sizeX, pImage->sizeY, GL_UNSIGNED_BYTE,  pImage->data,
		      width, height, GL_UNSIGNED_BYTE, tex);

	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (flags & TEXFILE_CLAMP)
    {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    else
    {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    if (flags & TEXFILE_NOMIPMAPS)
    {
	glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, textureType,
		     GL_UNSIGNED_BYTE, tex);
    }
    else
    {
	gluBuild2DMipmaps(GL_TEXTURE_2D, textureType, width, height, textureType,
			  GL_UNSIGNED_BYTE, tex);
    }


		
	/*
	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX, 
					  pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);

	//Assign the mip map levels and texture info
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	*/
	// Now we need to free the image data that we loaded since openGL stored it as a texture

	if (pImage)										// If we loaded the image
	{
		if (pImage->data)							// If there is texture data
		{
			free(pImage->data);						// Free the texture data, we don't need it anymore
		}

		free(pImage);
		free(tex);						// Free the image structure
	}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	return true;
}

bool FindTextureExtension(char *filename,char *ret)
{



	
	
	FILE *fp;
	char *strJpegPath = {0};
	char *strTargaPath = {0};

	
//	GetCurrentDirectory(1024,strJpegPath);
	
	
	strJpegPath = new char[1024];
	strTargaPath = new char[1024];

	//strcpy(strJpegPath,"\\");
	strcpy(strJpegPath,"");
	strcpy(strTargaPath,strJpegPath);

	
	strcat(strJpegPath ,filename);
	strcat(strTargaPath, filename);

	
	/*check to see if the extension is already present*/
	fp = fopen(strJpegPath,"rb");
	if(fp!=NULL)
	{
		strcpy(ret,strJpegPath);
		return 1;
	}
	
	int len = strlen(strJpegPath);
	strcpy(&strJpegPath[len-3], "jpg");
	fp = fopen(strJpegPath,"rb");
	if(fp!=NULL)
	{
		strcpy(ret,strJpegPath);
		return 1;
	}


	strcpy(strJpegPath,strTargaPath);
	strcat(strJpegPath,".jpg");
	
	fp = fopen(strJpegPath,"rb");

	if(fp!=NULL)
	{
	//	strcat(filename,".jpg");
		strcpy(ret,strJpegPath);
		return 1;
	}


	strcat(strTargaPath,".tga");

	fp = fopen(strTargaPath,"rb");

	if(fp!=NULL)
	{
		//strcat(filename,".tga");
		strcpy(ret,strTargaPath);
		return 1;
	}
	

	return 0; 
}

bool ChangeTextureGamma(unsigned char *pImage,int size,float factor)
{
	if(!pImage)
		return 0;
	
	for (int i =0;i<size/3;i++, pImage+=3)
	{
		float r=0,g=0,b = 0;
		float scale = 1.0f, temp = 0;

		r =(float)pImage[0];
		g =(float)pImage[1];
		b =(float)pImage[2];

		r= r * factor/255.0f;
		g= g * factor/255.0f;
		b= b * factor/255.0f;

		if(r>1.0 && (temp = (1.0f/r)) < scale  ) scale = temp;
		if(g>1.0 && (temp = (1.0f/g)) < scale  ) scale = temp;
		if(b>1.0 && (temp = (1.0f/b)) < scale  ) scale = temp;

		scale *= 255.0f;

		r*=scale;g*=scale;b*=scale;

		pImage[0] = (char)r;
		pImage[1] = (char)g;
		pImage[2] = (char)b;
	}
	
	return 1;

}

bool CreateTexture(UINT &texID,unsigned char *pImage, int width, int height, short gamma = 0)
{
	
	if(!pImage)
		return 0;
	
	glGenTextures(1,&texID);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glBindTexture(GL_TEXTURE_2D, texID);

	if(gamma)
	ChangeTextureGamma(pImage,width*height*3,gamma);

	gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,pImage);

	//Assign the mip map levels		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	return 1;
}
