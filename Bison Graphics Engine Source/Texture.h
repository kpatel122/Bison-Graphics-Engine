#ifndef TEXTURE
#define TEXTURE

bool CreateTexture(UINT &texID,unsigned char *pImage, int width, int height,short gamma = 0);
bool CreateTexture(UINT &texture, LPSTR strFileName, UINT flags);
bool ChangeTextureGamma(unsigned char *pImage,int size,float factor);
bool FindTextureExtension(char *filename,char *);



#endif