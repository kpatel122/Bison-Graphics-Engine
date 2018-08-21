

/************************************************************************
#include <stdlib.h>         
#include <stdio.h>
#include <malloc.h>

#include "shader.h"

#define SHADERBUF_SIZE  (1024*1024-8)

char *currPos,*endPos;
char *shaderBuffer;


/*
void ShaderLoadFile(char *filename)
{
	FILE *shaderFile;
	shaderFile = fopen(filename,"r");
	
	shaderBuffer = (char*)malloc(SHADERBUF_SIZE);
	fread(shderBuffer,
}
*//////////////////////////////////////////////////////////////////////////////////





#include "shader.h"
#include "CVector.h"
#include "bsp.h"
#include "pak.h"


#define FULL_EXTENSION	true

extern bspTexture * pTextures;
extern	bspTexture * pShaderRefs;

char buff[80];
extern int numTextures;
extern  int numShaderRefs;
int shaderStart;

#define SHADERBUF_SIZE  (1024*1024-8)

char *currPos,*endPos;
char *shaderBuffer;
short *shaderfound;

extern Shader *shaders;

#include <iostream>
using namespace std;

char pbuffer[80];
FILE *fp;


/*Utility Functions*/
void Log();
void Log(char *msg);
void Init();
void LowerCase(char *buff);
int GetLen(FILE* fp);

/*Parsing Functions*/
void ShaderLoadFile(char *filename);
char *NextToken();
void ClassifyShader(Shader *shader,int pass,int numArgs,char **args);
int ParseTokenString(char *buff,Shader *shader,int pass);
void BuildTokenString(char *str);
void ShaderParseFile();
int GetTextureRef(char *buff);
void ShaderParseFile();
void ShaderMakeDefaults();

/*Shader Pass Functions*/
void ShaderPassMap(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassBlend(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassRGB(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassTcMod(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassSurfaceParm(Shader *shader,int pass,int numArgs,char **args);
void ShaderSkyParms(Shader *shader,int pass,int numArgs,char **args);

void ShaderPassDepth(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassDepthWrite(Shader *shader,int pass,int numArgs,char **args);
void ShaderCull(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassClampMap(Shader *shader,int pass,int numArgs,char **args);
void ShaderPassAnimMap(Shader *shader,int pass,int numArgs,char **args);


ShaderDesc shaderKeys[] = {
	{"map",1,1,ShaderPassMap},
	{"blendfunc",1,2,ShaderPassBlend},
	{"rgbgen", 1, 6,ShaderPassRGB},
	{"tcmod", 2, 7, ShaderPassTcMod},
	{"surfaceparm", 1, 1, ShaderPassSurfaceParm},
    {"skyparms", 3, 3, ShaderSkyParms},
	{"depthfunc", 1, 1, ShaderPassDepth},
    {"depthwrite", 0, 0, ShaderPassDepthWrite},
	{"cull", 1, 1, ShaderCull},
	 {"animmap", 3, MAX_SHADER_ARGS, ShaderPassAnimMap},
    {"clampmap", 1, 1, ShaderPassClampMap},
	{NULL,0,0,NULL}
};





int count = 0;

inline void Syntax()
{
	std::cout<<"Syntax Error\n"<<endl;
}

void Log(char *msg)
{
	fprintf(fp,msg);
};

void Log()
{
	fprintf(fp,pbuffer);
};

void Init()
{
	fp = fopen("info.txt","w");
}


void LowerCase(char *buff)
{
	/*************************************
	takes the string in buff
	and sets every character to lowercase
	*************************************/

	//check for a valid string
	if(!buff)
		return;
int len = strlen(buff);
	for(int i=0;i<len;i++)
	{
		*buff = tolower(*buff);
		buff++;
	}
}

int GetLen(FILE* fp)
{
	
	/***********************
	Gets the length of a file
	note: OPTIMIZE ME!
	*************************/
	
	int len = 0;
	
	char tok;
	
	//get a char till we get to the end
	//of the file
	while(tok = fgetc(fp)!=EOF)
	{
		len++;
	
	}
	
	//reset the file pointer
	rewind(fp);
	
	//return the length
	return len;
}

void ShaderReadAll()
{


//char dir[105] = {"C:\\Program Files\\Quake III Arena\\baseq3\\pak0.pk3"};
char dir[105] = {"pak\\pak0.pk3"};
//#define PRINT_INFO
Init();





shaders = (Shader*)malloc(numShaderRefs * sizeof(Shader));//new Shader[numTextures];

shaderfound = (short*)malloc(numShaderRefs * sizeof(short));

memset(shaderfound,0,numShaderRefs * sizeof(short));

shaderStart = numShaderRefs;

numTextures = 0;
pTextures = (bspTexture*)malloc(MAX_TEXTURES * sizeof(bspTexture));



char shaderlist[4096], *fname;

pak_openpak(dir);

int len;
int num; 



/*
char ss[] = "scripts/base.shader\0"
            "scripts/base_button.shader\0" 
			"scripts/base_floor.shader\0" 
			"scripts/base_light.shader\0" 
			"scripts/base_object.shader\0" 
			"scripts/base_support.shader\0" 
			"scripts/base_trim.shader\0" 
			"scripts/base_wall.shader\0" 
			"scripts/ctf.shader\0" 
			"scripts/eerie.shader\0" 
			"scripts/gfx.shader\0" 
			"scripts/gothic_block.shader\0" 
			"scripts/gothic_floor.shader\0" 
			"scripts/gothic_light.shader\0" 
			"scripts/gothic_trim.shader\0" 
			"scripts/gothic_wall.shader\0" 
			"scripts/hell.shader\0" 
			"scripts/liquid.shader\0" 
			"scripts/menu.shader\0" 
			"scripts/models.shader\0" 
			"scripts/organics.shader\0"  
			"scripts/sfx.shader\0" 
			"scripts/shrine.shader\0" 
			"scripts/skin.shader\0" 
			"scripts/sky.shader\0"
			"scripts/test.shader\0"
			"scripts/terrademoQ3.shader\0";
*/










num = pak_listshaders(4096,shaderlist);

//num = 27;

//fname = ss;
fname = shaderlist;
num -= 1;
shaderBuffer = (char*)malloc(1024*1024-8);



while (num--)
{


	std::cout<<fname<<"\n";
len = pak_readfile(fname,1024*1024-8,(unsigned char*)shaderBuffer);

//Log(shaderBuffer);
currPos = shaderBuffer;
endPos = &shaderBuffer[len];



ShaderParseFile();


fname+= strlen(fname)+1;

}

ShaderMakeDefaults();

RELEASEC(shaderBuffer);
RELEASEC(shaderfound);



}

void ShaderLoadFile(char *filename)
{
	/******************************
	Loads and parses a shader file
	******************************/


	//setups up a text file used for debugging output
//	Init();
	
	
	//open the shader file
	FILE *shaderFile;
	shaderFile = fopen(filename,"r");
	
	//check its valid
	if(!shaderFile)
	return;
	
	

	//get the legth tof the file
	int len;
	len = GetLen(shaderFile);
	
	
	//sprintf(buff,"shader is %d bytes",len);
	//Log(buff);
	

	//alloacte storage for the file
//	shaderBuffer = (char*)malloc(len+1);

	//check its valid
//	if(!shaderBuffer)
//		return;
	
	//clear the buffer
//	memset(shaderBuffer,'0',sizeof(shaderBuffer));
	
	//read in the shader file
	fread(shaderBuffer,len+1,sizeof(char),shaderFile);
	shaderBuffer[len] = '\0';
	
	//log it(for debugging)
	Log(shaderBuffer);
	
	
	//set up pointers
	currPos = shaderBuffer;
	endPos = &shaderBuffer[len];

	
//	shaders = (Shader*)malloc(numTextures * sizeof(Shader));//new Shader[numTextures];

		
	//parse the file
	ShaderParseFile();
	
	//free the buffer
	if(shaderBuffer)
	free(shaderBuffer);

	free(shaders);

}

char *NextToken()
{
	/******************************
	sets the currPos pointer to the 
	first char of a new token string
	*******************************/

//skip the following characters
while (*currPos == ' ' || *currPos == '\n' || *currPos == '\t'|| *currPos == '\r')
	{
		//make sure we havent gone past the end 
		//of the file
		if (currPos == endPos-1)
			return NULL;
		
		//move the buffer pointer
		currPos++;

	}
//skip comments denoted by '//'
if((*currPos == '/') && (*(currPos+1)=='/') )
{
	//move till we get to a new line
	while(*currPos!='\n')
		currPos++;
	
	//get the next valid line
	//note: RECURSION IS NOT COOL!
	return NextToken();
}
	//return where we're at
	return currPos;
	
}

void ClassifyShader(Shader *shader,int pass,int numArgs,char **args)
{
	
	ShaderDesc *key;
	for(key = shaderKeys; key->name!=NULL;key++)
	{
		if(strcmp(key->name,args[0])==0)
		{
			if(numArgs > key->maxParams || numArgs < key->minParams )
			{
				Syntax();
				return;
			}
			
			

			key->Set(shader,pass,numArgs,args);
			return;
		}
	
	}
	
}

int ParseTokenString(char *buff,Shader *shader,int pass)
{
	
	/***************************************
	given a token string of tokens
	take each individual token
	and build an array of arguments with each
	element coresponding to one parameter
	*****************************************/
	
	//array of args
	char *str[10];
	
	
	//count the number of arguments
	int numArg=0;
	int len=0;
	int currArg = 0;
	
	//set the string to lower case characters
	LowerCase(buff);

#ifdef PRINT_INFO	
	cout<<"STRING: "<<buff<<"\n";
#endif		
	//check the buffer is valid
	if(!buff)
		return -1;
	
	if(strcmp("noshader",buff) == 0)
		return -1;

	//check for invalid args
	if(*buff=='{' || *buff=='}' )
		return -1;
		
	
	//temp is used to extract
	//individual parameters
	//from the string of parameters(i.e buff)
	char *temp;
	temp = buff;

	//looping counters
	int i = 0;
	int s=0;
	
	//get the length of the buffer
	int lenb = strlen(buff);
	
	//loop throug the string
	for(int j=0; j<lenb;)
	{
		//loop till we find the end of the parameter
		//or till we hit the end of the string
		while((*buff !=' ') && (*buff!= '\0')) 
			{

				
				
				i++;//the length of the string
				buff++;//the buffer
				j++;//the overall string


			}
			
			//terminate the string
			temp[i] = '\0';
			
			//allocate memor for the string
		str[s] = temp;//(char*)malloc((strlen(temp))+1); 
			//copy it into the array
		//	strcpy(str[s],temp); 
			numArg++; //
			//increase the array indicy
			s++;
			
			//move to the next char
			do
			{
				buff++;
				j++;
			}while(*buff == ' ');
		
			//move temp beyond the string we just made
			temp = buff;
			//time for a new string
			i=0;

			
		}//end for
	
	
	
		
	 
	//classify the shader
	ClassifyShader(shader,pass,(numArg-1),str);	
	
//	free the buffer
//	for(i=0;i<s;i++)
//	free(str[i]);
	
	
	
	return numArg;
}


void BuildTokenString(char *str)
{
	
//char str[1024];

	/*****************************
	builds a string from the buffer
	puts the string in *str
	*****************************/

//the length of of the string
int len=0;


//loop till we get to the end of the string 
//while( (*currPos !='\n') &&  (*currPos !='\t') && (*currPos !='\r'))
while( (*currPos !='\n') )
{
	//skip carrige returns
	if ((*currPos == '\t') || (*currPos =='\r'))
	*currPos = ' ';	
	
	if((*currPos == '/') && ((*(currPos+1)) == *currPos))
		break;
	
		
		str[len] = *currPos;//put the value into str
		currPos++;//move to next char
		len++; //increase len
}


//cut the string make sure theres not an empty space at the end
if (str[len] == ' ')
str[len-1] = '\0';
else
str[len] = '\0';



}

void ReadPass(char *buff,Shader *shader, int &pass)
{
	/************************
	reads in and sets up a new
	render pass
	**************************/
	
    
	
	shader->pass[pass].depth = GL_LEQUAL;
	shader->pass[pass].flags = 0;
	shader->pass[pass].texture = -1;
	shader->pass[pass].tcMod = 0;
	shader->pass[pass].rgbGen = SHADER_GEN_IDENTITY;

	//if we are here we know the current token is '{'
	//so move beyond it and get the next valid token
	currPos++;
#ifdef PRINT_INFO	
	cout<<"STRING {\n";
#endif
	//Get the first token
	char *tok = NextToken();
	
	// '}' denotes the end of the pass
	while(*tok!='}')
	{
		
		
		BuildTokenString(buff);//build a token string
		ParseTokenString(buff,shader,pass);//parse the string
		tok = NextToken();//get the next token
	}
	currPos++;

	//cout<<"End Render Pass\n";
}

int GetTextureRef(char *buff)
{
	/*******************************
	given a texture name returns the
	index from the textures array
	*******************************/
	
	//check for a valid buffer
	if(!buff)
		return -1;

	int len = strlen(buff);
	if(buff[len-1] == ' ')
		buff[len-1] = '\0';
	
	/*
	if(!fullpath)
	{
		//loop through textures
		for (int i=0;i<numTextures;i++)
		{
			//check the name
			if((strcmp(buff,pTextures[i].name) == 0))
			{
				
				
				sprintf(pbuffer,"Found SHADER.... %s \n",pTextures[i].name);
				Log();
		//		sprintf(pbuff,"found %s %d  index %d",buff,count,i);
		//		msg(pbuff);

				if(i==29)
				{
					bool k;
					k = true;
				}

				return i;//return the index
			}
		}
	}
	else
		{
		char *str = (char*)malloc(1024* sizeof(char));
			for (int i=0;i<numTextures;i++)
			{
				
				    strcpy(str,pTextures[i].name);

					strcat(str,".jpg");
					if((strcmp(buff,str) == 0))
					{
						free(str);
						return i;
					}
					else
					{
						strcpy(str,pTextures[i].name);
						strcat(str,".tga");
						if((strcmp(buff,str) == 0))
						{
							free(str);
							return i;
						}
					}
			}

		free(str);	
		}
	*/
	
	for (int i=0;i<numTextures;i++)
	{
		if(!strcmp(pTextures[i].name,buff))
		{
			return i;
		}
	}

	if((numTextures) == MAX_TEXTURES)
		return -1;
	pTextures[numTextures].contents = 0;
	pTextures[numTextures].flags = 0;
	strcpy(pTextures[numTextures].name,(buff));

	//we didnt find one, return error
	return numTextures++;

}

void ShaderSkip()
{
	char *tok;
    int braceCount;

    /* Opening brace */
    tok = NextToken();
    if (tok[0] != '{') Syntax();
	
	currPos++;
    for (braceCount = 1; braceCount > 0 && currPos < endPos; currPos++)
    {
	if (*currPos == '{')
	    braceCount++;
	else if (*currPos == '}')
	    braceCount--;
    }

}


int GetShaderRef(char *shader)
{

	int len = strlen(shader);
	if(shader[len-1] == ' ')
	shader[len-1] = '\0';
	

	
	for(int i=0; i<numShaderRefs;i++)
	{
		
		if(i == 29)
		{
			bool sowhat  = 1;
		}
		count++;
		if(strcmp(shader,pShaderRefs[i].name)==0)
			return i;


	}

	return -1;
}

void ShaderMakeDefaults()
{
	int i, f, firsttrisurf, lasttrisurf, trisurf, md3;
    char fname[128];

    /* Find first and last trisurf */
    firsttrisurf = -1;
    for (f = 0; f < numFaces; f++)
    {
	if (pFaces[f].type == FACETYPE_TRISURF)
	{
	    firsttrisurf = f;
	    break;
	}
    }
    if (firsttrisurf >= 0)
    {
	for (f = firsttrisurf; f < numFaces; f++)
	{
	    if (pFaces[f].type != FACETYPE_TRISURF)
	    {
		lasttrisurf = f;
		break;
	    }
	}
    }
	
    for (i=0; i < numShaderRefs; ++i)
    {
	if (shaderfound[i]) continue;

	/* Special exception: noshader */
	if (!strcmp(pShaderRefs[i].name, "noshader"))
	{
	    shaders[i].numPasses = 0;
	    continue;
	}
	
	/* Append tga to get file name */
	strcpy(fname, pShaderRefs[i].name);
	strcat(fname, ".tga");

	/* Check if shader is for an md3 */
	md3 = 0;
	trisurf = 0;
	if (i >= shaderStart)
	{
	    md3 = 1;
	}
	else
	{
	    /* Check if shader is for a trisurf */
	    if (firsttrisurf >= 0)
	    {
		for (f=firsttrisurf; f <= lasttrisurf; f++)
		{
		    if (pFaces[f].textureID == i)
		    {
			trisurf = 1;
			break;
		    }
		}
	    }
	}

	if (md3)
	{
	    shaders[i].flags = SHADER_NOCULL;
	    shaders[i].numPasses = 1;
	    shaders[i].pass[0].flags = SHADER_DEPTHWRITE;
	    shaders[i].pass[0].texture = GetTextureRef(fname);
	    shaders[i].pass[0].depth = GL_LEQUAL;
	    shaders[i].pass[0].rgbGen = SHADER_GEN_IDENTITY;
	}
	else if (trisurf)
	{
	    shaders[i].flags = SHADER_NOCULL | SHADER_NEEDCOLOURS;
	    shaders[i].numPasses = 1;
	    shaders[i].pass[0].flags = SHADER_DEPTHWRITE;
		shaders[i].pass[0].texture = GetTextureRef(fname);
	    shaders[i].pass[0].depth = GL_LEQUAL;
	    shaders[i].pass[0].rgbGen = SHADER_GEN_VERTEX;	    
	}
	else
	{
	    shaders[i].flags = 0;
	    shaders[i].numPasses = 2;
	    shaders[i].pass[0].flags = SHADER_LIGHTMAP | SHADER_DEPTHWRITE;
	    shaders[i].pass[0].texture = -1;
	    shaders[i].pass[0].depth = GL_LEQUAL;
	    shaders[i].pass[0].rgbGen = SHADER_GEN_IDENTITY;
	    
	    shaders[i].pass[1].flags = SHADER_BLEND;
	    shaders[i].pass[1].texture = GetTextureRef(fname);
	    shaders[i].pass[1].blend[BLEND_SRC]  = GL_DST_COLOR;
	    shaders[i].pass[1].blend[BLEND_DEST] = GL_ZERO;
	    shaders[i].pass[1].depth = GL_LEQUAL;
	    shaders[i].pass[1].rgbGen = SHADER_GEN_IDENTITY;
	}
    }
}


void ShaderParseFile()
{
	
	/*******************
	Parses a shader file
	********************/
	
	char *tok;//the current token
	tok = currPos;
	char buff[1024];//the buffer that holds a token string	
	int id =0;
	int currPass = -1;
	
	//loop till we get to the end of the shader fike
	while(currPos<(endPos-1))
	{
		//get the next token
		tok = NextToken();
		
		//make sure its valid
		if(!tok)
			return;

		//get the opening string i.e the
		//texture
		BuildTokenString(buff);
		
		if(!strcmp(buff,"textures//skies//toxicskytim_blue"))
		{
			std::cout<<""<<endl;
		}

		//the first token is the texture to
		//which the shader refers to
		id = GetShaderRef(buff);
		
		if(id == -1)
		{
			ShaderSkip();
			continue;
		}
		if(id == 29)
		{
			bool sayhello = true;
		}

		shaderfound[id] = 1;
		

		
		shaders[id].numPasses = 0;
		shaders[id].flags = 0;

#ifdef PRINT_INFO
		cout<<"Shader: "<<buff<<"id is "<<id<<"\n";
#endif		
		tok = NextToken();
		
		//chaeck for opening brce
		if (tok[0]!= '{')
		Syntax();
		
		//move beyond the brace
		/*note : THIS IS UGLY*/
		tok++;
		currPos++;
		
		//get the next token
		tok = NextToken();
	
		//loop till we hit the end of the shader
 		while(tok[0] != '}')
		{
			
			
			//check for a new render pass
			if(tok[0] == '{')
			{
			shaders[id].numPasses++;
			currPass++;
			ReadPass(buff,&shaders[id],currPass);//read in the render pass
			}
			else
			{
			BuildTokenString(buff);//read in the token string
			ParseTokenString(buff,&shaders[id],currPass);//parse it
			}
			
			//get the next token
			tok = NextToken();
		}

		if (! (shaders[id].flags & SHADER_DEPTHWRITE) &&
	    ! (shaders[id].flags & SHADER_TRANSPARENT) &&
	    ! (shaders[id].flags & SHADER_SKY) &&
	    shaders[id].numPasses > 0)
		{
	    shaders[id].pass[0].flags |= SHADER_DEPTHWRITE;
		}


		currPass = -1;
		currPos++;//move on...
	}
		
}


/******************Shader Function**********************************/
void ShaderCull(Shader *shader,int pass,int numArgs,char **args)
{
	if( (!strcmp(args[1],"none")) || (!strcmp(args[1],"disable")) )
	shader->flags |=SHADER_NOCULL;
}

/******************Shader Pass Functions****************************/

void ShaderParseFunction(char **args, ShaderFunc *func)
{
	if (!strcmp(args[0], "sin"))
	    func->func = SHADER_FUNC_SIN;
	else if (!strcmp(args[0], "triangle"))
	    func->func = SHADER_FUNC_TRIANGLE;
	else if (!strcmp(args[0], "square"))
	    func->func = SHADER_FUNC_SQUARE;
	else if (!strcmp(args[0], "sawtooth"))
	    func->func = SHADER_FUNC_SAWTOOTH;
	else if (!strcmp(args[0], "inversesawtooth"))
	    func->func = SHADER_FUNC_INVERSESAWTOOTH;
	else if (!strcmp(args[0], "noise"))
	    func->func = SHADER_FUNC_NOISE;
	else
	    Syntax();

	func->args[0] = atof(args[1]);
	func->args[1] = atof(args[2]);
	func->args[2] = atof(args[3]);
	func->args[3] = atof(args[4]);
}

void ShaderPassTcMod(Shader *shader,int pass,int numArgs,char **args)
{
	
	
	shader->pass[pass].flags |= SHADER_TCMOD;
    
    if (!strcmp(args[1], "scale"))
    {
	if (numArgs != 3) Syntax();
	shader->pass[pass].tcMod |= SHADER_TCMOD_SCALE;
	shader->pass[pass].tcModScale[0] = atof(args[2]);
	shader->pass[pass].tcModScale[1] = atof(args[3]);
	}
    else if (!strcmp(args[1], "rotate"))
    {
	shader->pass[pass].tcMod |= SHADER_TCMOD_ROTATE;
	shader->pass[pass].tcModRotate = atof(args[2]);
    }
    else if (!strcmp(args[1], "scroll"))
    {
	if (numArgs != 3) Syntax();
	shader->pass[pass].tcMod |= SHADER_TCMOD_SCROLL;
	shader->pass[pass].tcModScroll[1] = atof(args[2]);
	shader->pass[pass].tcModScroll[2] = atof(args[3]);
    }
    else if (!strcmp(args[1], "stretch"))
    {
	if (numArgs != 6) Syntax();
	shader->pass[pass].tcMod |= SHADER_TCMOD_STRETCH;
	ShaderParseFunction(&args[2], &shader->pass[pass].tcModStrechFunc);
    }
    else if (!strcmp(args[1], "transform"))
    {
	int i;
	if (numArgs != 7) Syntax();
	shader->pass[pass].tcMod |= SHADER_TCMOD_TRANSFORM;
	for (i=1; i < 7; ++i)
	    shader->pass[pass].tcModTransform[(i-1)] = atof(args[i+1]);
    }
    else if (!strcmp(args[1], "turb"))
    {
	int i, a1;
	if (numArgs == 5)
	    a1 = 1;
	else if (numArgs == 6)
	    a1 = 2;
	else
	    Syntax();
	shader->pass[pass].tcMod |= SHADER_TCMOD_TURB;
	for (i=0; i < 4; ++i)
	    shader->pass[pass].tcModTurb[i] = atof(args[(i+1)+a1]);
    }
    else
	Syntax();

}

void ShaderPassRGB(Shader *shader,int pass,int numArgs,char **args)
{
	/*****************CHANGE ME!************************/
	if (!strcmp(args[1], "identity"))
	return; /* Default */
    else if (!strcmp(args[1], "wave"))
    {
	if (numArgs != 6)
	    Syntax();

	shader->pass[pass].rgbGen = SHADER_GEN_WAVE;

	ShaderParseFunction(&args[2], &shader->pass[pass].rgbFunc);
    }
    else if (!strcmp(args[1], "vertex") || !strcmp(args[1], "lightingdiffuse"))
    {
	shader->flags |= SHADER_NEEDCOLOURS;
	shader->pass[pass].rgbGen = SHADER_GEN_VERTEX;
    }
}


void ShaderPassMap(Shader *shader,int pass,int numArgs,char **args)
{
	if (numArgs > shaderKeys[0].maxParams || numArgs < shaderKeys[0].minParams )
	return;

	if((strcmp(args[1],"$lightmap")==0)){
		shader->pass[pass].flags |= SHADER_LIGHTMAP;
		shader->pass[pass].flags |=SHADER_DEPTHWRITE;}
	else
	{
		int id;
		id = GetTextureRef(args[1]);
		
		shader->pass[pass].texture = id;
		
		/**********ADD ME*******************
		if(shader->flags & SHADER_NOMIPMAPS)
			(textures[id].flags |=NO_MIPMAPS)
		************************************/
	}


}

void ShaderPassBlend(Shader *shader,int pass,int numArgs,char **args)
{
	shader->pass[pass].flags |= SHADER_BLEND;
	if(numArgs == 1)
	{
		if((strcmp(args[1],"add")==0))
		{
		shader->pass[pass].blend[BLEND_SRC] = shader->pass[pass].blend[BLEND_DEST] = GL_ONE;
		return;
		}
		else
		if((strcmp(args[1],"filter")==0))
		{
		shader->pass[pass].blend[BLEND_SRC] = GL_DST_COLOR; 
		shader->pass[pass].blend[BLEND_DEST] = GL_ZERO;
		return;
		}
		else
		if((strcmp(args[1],"blend")==0))
		{
		shader->pass[pass].blend[BLEND_SRC] = GL_SRC_ALPHA; 
		shader->pass[pass].blend[BLEND_DEST] = GL_ONE_MINUS_SRC_ALPHA;
		return;
		}
		else
		{
		shader->pass[pass].blend[BLEND_SRC] = shader->pass[pass].blend[BLEND_DEST] = GL_ONE;
		return;
			Syntax();
		}
	}
	else
	for (int i=0;i<2;i++)
	{
		if(!(strcmp(args[(i+1)],"gl_zero")))
			shader->pass[pass].blend[i] = GL_ZERO;
		else
		if(!(strcmp(args[(i+1)],"gl_one")))
			shader->pass[pass].blend[i] = GL_ONE;
		else
		if(!(strcmp(args[(i+1)],"gl_dst_color")))
			shader->pass[pass].blend[i] = GL_DST_COLOR;
		else
		if(!(strcmp(args[(i+1)],"gl_one_minus_src_alpha")))
			shader->pass[pass].blend[i] = GL_ONE_MINUS_SRC_ALPHA;
		else
		if(!(strcmp(args[(i+1)],"gl_src_alpha")))
			shader->pass[pass].blend[i] = GL_SRC_ALPHA;
		else
		if(!(strcmp(args[(i+1)],"gl_src_color")))
			shader->pass[pass].blend[i] = GL_SRC_COLOR;
		else
		if(!(strcmp(args[(i+1)],"gl_one_minus_dst_color")))
			shader->pass[pass].blend[i] = GL_ONE_MINUS_DST_COLOR;
		else
		if(!(strcmp(args[(i+1)],"gl_one_minus_src_color")))
			shader->pass[pass].blend[i] = GL_ONE_MINUS_SRC_COLOR;
		else
		if(!(strcmp(args[(i+1)],"gl_dst_alpha")))
			shader->pass[pass].blend[i] = GL_DST_ALPHA;
		else
		if(!(strcmp(args[(i+1)],"gl_one_minus_dst_alpha")))
			shader->pass[pass].blend[i] = GL_ONE_MINUS_DST_ALPHA;
		else
		Syntax();

	}
}


void ShaderSkyParms(Shader *shader,int pass,int numArgs,char **args)
{
    shader->skyheight = atof(args[2]);
}


void ShaderPassSurfaceParm(Shader *shader,int pass,int numArgs,char **args)
{
    if (!strcmp(args[1], "trans"))
	shader->flags |= SHADER_TRANSPARENT;
    else if (!strcmp(args[1], "sky"))
	shader->flags |= SHADER_SKY;
}

void ShaderPassDepth(Shader *shader,int pass,int numArgs,char **args)
{
    if (!strcmp(args[1], "equal"))
	shader->pass[pass].depth = GL_EQUAL;
    else
	Syntax();
}

void ShaderPassDepthWrite(Shader *shader,int pass,int numArgs,char **args)
{
    /* FIXME: Why oh why is depthwrite enabled in the sky shaders ???? */
    if (shader->flags & SHADER_SKY) return;
    
    shader->flags |= SHADER_DEPTHWRITE;
    shader->pass[pass].flags |= SHADER_DEPTHWRITE;
}

void ShaderPassClampMap(Shader *shader,int pass,int numArgs,char **args)
{
   shader->pass[pass].texture = GetTextureRef(args[1]);
    pTextures[shader->pass[pass].texture].flags |= TEXFILE_CLAMP;
    if (shader->flags & SHADER_NOMIPMAPS)
	pTextures[shader->pass[pass].texture].flags |= TEXFILE_NOMIPMAPS;       
}


void ShaderPassAnimMap(Shader *shader,int pass,int numArgs,char **args)
{
    int i;
    shader->pass[pass].flags |= SHADER_ANIMMAP;
    shader->pass[pass].animFps = atof(args[1]);
    shader->pass[pass].animNumframes = numArgs - 1;
    for (i=1; i < (numArgs); ++i)
	shader->pass[pass].animFrames[i-1] = GetTextureRef(args[(i+1)]);
}

