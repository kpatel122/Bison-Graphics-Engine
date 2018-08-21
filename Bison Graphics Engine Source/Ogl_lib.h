#ifndef ogl_lib1
#define ogl_lib1


extern HDC g_hdc;
extern bool full_screen;


#define BITMAP_ID 0x4D42


typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE; 


typedef struct OGL_BITMAP
{
	BITMAPINFOHEADER bit_info;
	UCHAR *data;


}OGL_BITMAP;

typedef struct OGL_TEXTURE_2D
{
	int width;
	int height;
	unsigned int tex_id;
	struct OGL_BITMAP image;
	HBITMAP bit;

}OGL_TEXTURE_2D;

typedef struct OGL_TARGA
{
	UCHAR img_type_code;
	short int width;
	short int height;
	UCHAR bit_count;
	UCHAR *data;


}OGL_TARGA;

typedef struct OGL_TEXTURE
{
	BITMAPINFOHEADER bit_info;
	UCHAR *data;
	unsigned int tex_id;

}OGL_TEXTURE;


// OpenGL Interface
void Init_Pixel_Format();
void Init_OGL2D();
void Init_OGL3D();
void Resize_Scene(GLsizei Width, GLsizei Height);
void Change_Screen_Mode(bool Mode);


//Bitmap Interface
int Draw_Bitmap(OGL_BITMAP *bitmap, int x, int y, int z);
int Load_Bitmap(char *filename,OGL_BITMAP *bitmap);
int Write_Bitmap(char *filename,OGL_BITMAP *bitmap, int width, int height);
void Destroy_Bitmap(OGL_BITMAP *bitmap);
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

void CreateTextures(char *filename,int TexID,unsigned int TexArray[]);

//Targa Interface
int Draw_Targa(OGL_TARGA *targa, int x, int y);
int Load_Targa(char *filename,OGL_TARGA *targa);
int Write_Targa(char *filename,OGL_TARGA *targa, int width, int height);
void Destroy_Targa(OGL_TARGA *targa);

//texture map interface
int Load_Texture_2D(char* filename,OGL_TEXTURE_2D *texture);
void Destroy_Texture_2D(OGL_TEXTURE_2D *texture);
void SetTextureParams(OGL_TEXTURE_2D *tex);

//primatives functions
void Draw_Text_Rect(float width, float height, float depth,
			           float x, float y, float z);

void Draw_Rect(float width, float height, float depth,
			           float x, float y, float z);

#endif