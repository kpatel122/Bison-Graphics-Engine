#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>	



#define msg(x) MessageBox(NULL,x,"ERROR",MB_OK);
#include "ogl_lib1.h"

bool full_screen = true;

#define MSG(x) MessageBox(NULL,x,"OpenGL Message",MB_OK);

void Init_Pixel_Format()
{
	int pixel_format;

		static PIXELFORMATDESCRIPTOR pfd; 

	 pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = 16;						// Here we use our #define for the color bits
    pfd.cDepthBits = 32;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits
 

		pixel_format = ChoosePixelFormat(g_hdc,&pfd);
		SetPixelFormat(g_hdc,pixel_format,&pfd);
}

void Init_OGL3D()
{
    
	glViewport(0,0,640,480);						// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what our our drawing boundries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(70.0f,(GLfloat)640/(GLfloat)480, 10.0f , 4000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelvie
	
	Change_Screen_Mode(1);
}

void Init_OGL2D()
{
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
glShadeModel(GL_SMOOTH);

glFrontFace(GL_CCW);
glViewport(0,0,640,480);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();

glOrtho(0,640,480,0,-1,1);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

}


void Resize_Scene(GLsizei Width, GLsizei Height)
{
	if (Height==0)		// Prevent A Divide By Zero If The window Is Too Small
		Height=1;

	glViewport(0, 0, 640, 480);	// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);		// Select The Projection Matrix
	glLoadIdentity();				// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
		glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matri
}

void Change_Screen_Mode(bool mode)
{
	DEVMODE dmScreenSettings;			// Developer Mode

	memset(&dmScreenSettings, 0, sizeof(DEVMODE));		// Clear Room To Store Setting
	dmScreenSettings.dmSize	= sizeof(DEVMODE);		// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth	= 640;			// Screen Width
	dmScreenSettings.dmPelsHeight	= 480;			// Screen Height
	dmScreenSettings.dmFields	= DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;	// Pixel Mode
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);	// Switch To Full Screen


}

int Load_Bitmap(char *filename,OGL_BITMAP *bitmap)
{
	FILE *file_ptr;
	BITMAPFILEHEADER bit_head;
	UCHAR temp_rgb;

	file_ptr = fopen(filename,"rb");
	if(!file_ptr)
		return 0;

	fread(&bit_head,sizeof(BITMAPFILEHEADER),1,file_ptr);

	if(bit_head.bfType != BITMAP_ID)
	{
		fclose(file_ptr);
		return 0;
	}

	fread(&bitmap->bit_info,sizeof(BITMAPINFOHEADER),1,file_ptr);
	fseek(file_ptr,bit_head.bfOffBits,SEEK_SET);
	
	bitmap->data = (UCHAR*)malloc(bitmap->bit_info.biSizeImage);

	
	if(!bitmap->data)
	{
		fclose(file_ptr);
		return 0;
	}

	fread(bitmap->data,1,bitmap->bit_info.biSizeImage,file_ptr);

	if(!bitmap->data)
	{
		fclose(file_ptr);
		return 0;
	}

	for(unsigned int i=0;i<(unsigned int)bitmap->bit_info.biSizeImage; i+=3)
	{
		temp_rgb = bitmap->data[i];
		bitmap->data[i] = bitmap->data[i+2];
		bitmap->data[i+2] = temp_rgb;
	}

	fclose(file_ptr);

	return 1;
}

void Destroy_Bitmap(OGL_BITMAP *bitmap)
{
	if(bitmap->data)
		free(bitmap->data);
}

int Write_Bitmap(char *filename,OGL_BITMAP *bitmap, int width, int height)
{
FILE *file_ptr;
UCHAR temp_rgb;
BITMAPFILEHEADER bit_head;
UCHAR *temp_data;



file_ptr = fopen(filename,"rb");
	if(!file_ptr)
		return 0;

bit_head.bfSize = sizeof(BITMAPFILEHEADER);
bit_head.bfType = BITMAP_ID;
bit_head.bfReserved1 = bit_head.bfReserved2 = 0;
bit_head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

bitmap->bit_info.biSize = sizeof(BITMAPINFOHEADER);
bitmap->bit_info.biPlanes = 1;
bitmap->bit_info.biBitCount = 24;
bitmap->bit_info.biCompression = BI_RGB;
bitmap->bit_info.biSizeImage = width * abs(height) *3;
bitmap->bit_info.biXPelsPerMeter = 0;
bitmap->bit_info.biYPelsPerMeter = 0;
bitmap->bit_info.biClrImportant = 0;
bitmap->bit_info.biWidth = width;
bitmap->bit_info.biHeight = height;

temp_data = (UCHAR*) malloc(bitmap->bit_info.biSizeImage);
temp_data = bitmap ->data;

for (unsigned int x =0; x< (unsigned int)bitmap->bit_info.biSizeImage; x+=3)
{
	temp_rgb = temp_data[x];
	temp_data[x] = temp_data[x+2];
	temp_data[x+2] = temp_rgb;

}

fwrite(&bit_head,1,sizeof(BITMAPFILEHEADER),file_ptr);
fwrite(&bitmap->bit_info,1,sizeof(BITMAPINFOHEADER),file_ptr);
fwrite(temp_data,1,bitmap->bit_info.biSizeImage,file_ptr);

free(temp_data);
fclose(file_ptr);

return 1;

}

int Draw_Bitmap(OGL_BITMAP *bitmap, int x, int y, int z)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glRasterPos3i(x,y,z);
	glDrawPixels(bitmap->bit_info.biWidth,bitmap->bit_info.biHeight,
		GL_RGB,GL_UNSIGNED_BYTE,bitmap->data);

	return 1;
}

int Load_Targa(char *filename,OGL_TARGA *targa)
{

FILE *file_ptr;
UCHAR uchar_bad;
short int sint_bad;
long size;
int color_mode;
long x;
UCHAR color_swap;

file_ptr = fopen(filename,"rb");

	
	if(!file_ptr)
	{
		MessageBox(NULL,"! open file","err",MB_OK);
		return 0;
	}

fread(&uchar_bad,sizeof(UCHAR),1,file_ptr);
fread(&uchar_bad,sizeof(UCHAR),1,file_ptr);

fread(&targa->img_type_code,sizeof(UCHAR),1,file_ptr);

if((targa->img_type_code!=2) && (targa->img_type_code!=3))
{
	MessageBox(NULL,"! type code","err",MB_OK);
	fclose(file_ptr);
	return 0;
}

fread(&sint_bad,sizeof(short int),1,file_ptr);
fread(&sint_bad,sizeof(short int),1,file_ptr);

fread(&uchar_bad,sizeof(UCHAR),1,file_ptr);

fread(&sint_bad,sizeof(short int),1,file_ptr);
fread(&sint_bad,sizeof(short int),1,file_ptr);

fread(&targa->width,sizeof(short int),1,file_ptr);
fread(&targa->height,sizeof(short int),1,file_ptr);

fread(&targa->bit_count,sizeof(short int),1,file_ptr);

fread(&uchar_bad,sizeof(UCHAR),1,file_ptr);

color_mode = targa->bit_count / 8;
size = targa->width * targa->height * color_mode;

targa->data = (UCHAR*)malloc(sizeof(UCHAR)*size);

fread(targa->data,sizeof(UCHAR),size,file_ptr);



for(x=0;x<size;x+=color_mode)
{
	
	color_swap = targa->data[x];
	targa->data[x] = targa->data[x+1];
	targa->data[x+1] = color_swap;
	
	
	
}


fclose(file_ptr);

return 1;
}

void Destroy_Targa(OGL_TARGA *targa)
{
	if(targa->data)
		free(targa->data);
}


int Draw_Targa(OGL_TARGA *targa, int x, int y)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glRasterPos2i(x,y);
	glDrawPixels(targa->width,targa->height,
		GL_RGB,GL_UNSIGNED_BYTE,targa->data);
return 1;
}




int Load_Texture_2D(char* filename,OGL_TEXTURE_2D *texture)
{
	 FILE *filePtr;							// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
			// bitmap image data
	int					imageIdx = 0;		// image index counter
	unsigned char		tempRGB;				// swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(&texture->image.bit_info, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	texture->image.data = (unsigned char*)malloc(texture->image.bit_info.biSizeImage);
	
	// verify memory allocation
	if (!texture->image.data)
	{
		free(texture->image.data);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(texture->image.data, 1, texture->image.bit_info.biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (texture->image.data == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx <texture->image.bit_info.biSizeImage; imageIdx+=3)
	{
		tempRGB = texture->image.data[imageIdx];
		texture->image.data[imageIdx] = texture->image.data[imageIdx + 2];
		texture->image.data[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	



	// set width and height info for this texture
	texture->width = texture->image.bit_info.biWidth;
	texture->height = texture->image.bit_info.biHeight;


	// generate the texture object for this texture
	glGenTextures(1, &texture->tex_id);
	
	return 1;
}

void SetTextureParams(OGL_TEXTURE_2D *tex)
{
	glBindTexture(GL_TEXTURE_2D,tex->tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);				
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex->width, tex->height, GL_RGB, GL_UNSIGNED_BYTE,tex->image.data);	
}

/*
int Load_Texture_2D(char* filename,OGL_TEXTURE_2D *texture,int cx, int cy)
{
	HBITMAP temp;
	
	HDC hMemDC;
	hMemDC = CreateCompatibleDC(NULL);
	
	texture->bit = CreateCompatibleBitmap(hMemDC,640,480);

	//ReleaseDC(NULL,hMemDC);

	temp=(HBITMAP) LoadImage(NULL,filename,IMAGE_BITMAP,0,0,
								LR_DEFAULTSIZE|LR_LOADFROMFILE);
	
	SelectObject(hMemDC,temp);

	
	return 1;
}
*/
  /*
int Load_Texture_2D(char* filename,OGL_TEXTURE_2D *texture,int cx, int cy)
{
	unsigned char *source;
	
	FILE *filePtr;							// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
			// bitmap image data
	int					imageIdx = 0;		// image index counter
	unsigned char		tempRGB;				// swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(&texture->image.bit_info, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	
	source = (unsigned char*)bitmapFileHeader.bfOffBits;
	// allocate enough memory for the bitmap image data
	texture->image.data = (unsigned char*)malloc(texture->image.bit_info.biSizeImage);
	
	// verify memory allocation
	if (!texture->image.data)
	{
		free(texture->image.data);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(texture->image.data, 1, texture->image.bit_info.biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (texture->image.data == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx <texture->image.bit_info.biSizeImage; imageIdx+=3)
	{
		tempRGB = texture->image.data[imageIdx];
		texture->image.data[imageIdx] = texture->image.data[imageIdx + 2];
		texture->image.data[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	



	// set width and height info for this texture
	texture->width = texture->image.bit_info.biWidth;
	texture->height = texture->image.bit_info.biHeight;


	// generate the texture object for this texture
	glGenTextures(1, &texture->tex_id);
	
	return 1;
}
*/

void Destroy_Texture_2D(OGL_TEXTURE_2D *texture)
{
	if(texture->image.data)
		free(texture->image.data);
}



void Draw_Text_Rect(float width, float height, float depth,
			           float x, float y, float z)
{
	glPushMatrix();
	
	glTranslatef(x,y,z);
	
		glBegin(GL_QUADS);

		
			
		glVertex3f(0, 0,  1.0f);glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width,0,  1.0f);glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width,height,  1.0f);glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,height,  1.0f);glTexCoord2f(0.0f, 1.0f);
		
		
		glEnd();
		
		glPopMatrix();

}

void Draw_Rect(float width, float height, float depth,
			           float x, float y, float z)
{
	glPushMatrix();
	
	glTranslatef(x,y,z);
	
		glBegin(GL_QUADS);

		
			
		glVertex3f(0, 0,  1.0f);
		glVertex3f(width,0,  1.0f);
		glVertex3f(width,height,  1.0f);
		glVertex3f(0,height,  1.0f);
		
		
		glEnd();
		
		glPopMatrix();

}


void CreateTextures(char *filename,int TexID,unsigned int TexArray[])
{
AUX_RGBImageRec *pBitmap = NULL;

if(!filename)
{	
	msg("Invalid filename")
	return;
}

pBitmap = auxDIBImageLoad(filename);

if(!pBitmap)
{
	msg("Couldnt load bit")
	return;
	
}

glGenTextures(1,&TexArray[TexID]);

glPixelStorei(GL_UNPACK_ALIGNMENT,1);

glBindTexture(GL_TEXTURE_2D,TexArray[TexID]);

// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, TexArray[TexID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}



}
