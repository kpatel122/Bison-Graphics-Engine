#ifndef __SHADER_H__
#define __SHADER_H__


#include <windows.h>		
#include <stdlib.h>         
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <iostream.h>
#include <string.h>

#include <math.h> 

#include <gl\gl.h>						// Header File For The OpenGL32 Library
#include <gl\glu.h>						// Header File For The GLu32 Library
#include <gl\glaux.h>					// Header File For The GLaux Library



typedef unsigned int uint;

/* Shader flags */
enum
{
    SHADER_NOCULL        = 1 << 0,
    SHADER_TRANSPARENT   = 1 << 1,
    SHADER_DEPTHWRITE    = 1 << 2,  /* Also used for pass flag */
    SHADER_SKY           = 1 << 3,
    SHADER_NOMIPMAPS     = 1 << 4,
    SHADER_NEEDCOLOURS   = 1 << 5,
    SHADER_DEFORMVERTS   = 1 << 6
};

enum
{
    TEXFILE_NOMIPMAPS  = 1 << 0,
    TEXFILE_CLAMP      = 1 << 1
};

/* Shaderpass flags */
enum
{
    SHADER_LIGHTMAP   = 1 << 0,
    SHADER_BLEND      = 1 << 1,
    SHADER_ALPHAFUNC  = 1 << 3,
    SHADER_TCMOD      = 1 << 4,
    SHADER_ANIMMAP    = 1 << 5,
    SHADER_TCGEN_ENV  = 1 << 6
};	

/* Transform functions */
enum
{
    SHADER_FUNC_SIN             = 1,
    SHADER_FUNC_TRIANGLE        = 2,
    SHADER_FUNC_SQUARE          = 3,
    SHADER_FUNC_SAWTOOTH        = 4,
    SHADER_FUNC_INVERSESAWTOOTH = 5,
	SHADER_FUNC_NOISE           = 6
};

/* *Gen functions */
enum
{
    SHADER_GEN_IDENTITY = 0,
    SHADER_GEN_WAVE     = 1,
    SHADER_GEN_VERTEX   = 2
};

/* tcmod functions */
enum
{
    SHADER_TCMOD_SCALE               = 1 << 1,
    SHADER_TCMOD_ROTATE              = 1 << 2,
    SHADER_TCMOD_SCROLL              = 1 << 3,
    SHADER_TCMOD_TRANSFORM           = 1 << 4,
    SHADER_TCMOD_TURB                = 1 << 5,
    SHADER_TCMOD_STRETCH             = 1 << 6,
};

#define MAX_ANIM_FRAMES 10
#define MAX_SHADER_PASS 5
#define MAX_SHADER_ARGS (MAX_ANIM_FRAMES+1)
 
#define BLEND_SRC 0
#define BLEND_DEST 1

typedef struct
{
	uint func;
	float args[4];

}ShaderFunc;

typedef struct
{
	uint flags;
	uint blend[2]; //0 = source, 1 = destination
	uint alpha;
	uint depth;
	int texture;
	
	
	uint tcMod;//tc mod type
	float tcModScale[2];//tc mod params
	float tcModRotate;
    float tcModScroll[2];
    float tcModTransform[6];
    float tcModTurb[4];
	ShaderFunc tcModStrechFunc; //for stretch param
	
	uint rgbGen;
	ShaderFunc rgbFunc; //for rgbgen keyword

	float animFps;
	int animNumframes;
	int animFrames[MAX_ANIM_FRAMES];
	

}ShaderPass;

typedef struct
{

	uint flags;
	ShaderPass pass[MAX_SHADER_PASS];
    int numPasses;
	float skyheight;
}Shader;

typedef struct
{
	char *name;//keyword for the shader
	int minParams,maxParams;//param info,
	void (*Set)(Shader *shader,int pass,int numArgs,char **args);//the function
}ShaderDesc;

extern ShaderDesc shaderKeys[];

void ShaderReadAll();

void ShaderLoadFile(char *filename); 

int GetLen(FILE* fp);

#endif