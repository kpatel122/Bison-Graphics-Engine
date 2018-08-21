// T3D Game Console, creates a game console application

// INCLUDES ///////////////////////////////////////////////
//#define INITGUID
//#define WIN32_LEAN_AND_MEAN  // just say no to MFC


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "winmm.lib")				// Used for timeGetTime()
#pragma comment(lib, "zlibstat.lib")
#pragma comment(lib, "strmiids.lib")



#include <windows.h>					// Include the much need windows.h
#include <gl\gl.h>						// Header File For The OpenGL32 Library
#include <gl\glu.h>						// Header File For The GLu32 Library
#include <gl\glaux.h>					// Header File For The GLaux Library
#include <iostream>						// Include our c++ standard header
#include <fstream>						// Include this to use ifstream
#include <string>						// Include this to use the string class
#include <vector>	
							// Include this to use STL vectors
#include <mmsystem.h>					// Include this to use timeGetTime()
#include "CVector.h"
#include "image.h"
#include "ogl_lib1.h"


#include "CAMERA.H"
#include "Frustum.h"
#include "OglExt.h"

#include "object3D.h"
#include "MD2.h"
#include "CMedia.h"

#include "particle.h"
#include "HiResTimer.h"


#include "BVM/vm.h"
CVM vm;
CLog log2;
CScriptLoader scriptLoader;
	


CHiResTimer g_timer;
using namespace std;


#include "bsp.h"

float acc = 0.0;

char buffer2[80];
char buffer3[80];
CFrustum frustum;


CCamera g_Camera;

int render_type = GL_POINTS;

	int nextFrame;

#define msg(x) MessageBox(NULL,x,"ERROR",MB_OK);

LARGE_INTEGER timerFrequency; // High resolution timer frequency
static LARGE_INTEGER startTime;	// Time at start of simulation
float fps; //to calculate fps

int nFrames;
int nLastFrames;

float time;

CMediaPlayer Media;

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"


CModelMD2 Rhino;
CModelMD2 GunMD2;

CModel3d obj;
CModel3d gun;



bool sordoloaded = false;
CModelMD2 Sordo;
CModelMD2 SordoGun;

CModel3d sordo;
CModel3d sordogun;




CParticleBlockExplosion ExpPart;

CParticleBlockExplosion ExpPart1;
CParticleBlockExplosion ExpPart2;
PARTICLE_DESC desc;
PARTICLE_DESC smk;

PARTICLE_DESC prt1;
PARTICLE_DESC prt2;

// MACROS /////////////////////////////////////////////////

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// GLOBALS ////////////////////////////////////////////////
HWND      main_window_handle = NULL; // globally track main window
HINSTANCE main_instance = NULL;// globally track hinstance

HDC g_hdc;
char buffer[1024];                     // general printing buffer

// FUNCTIONS //////////////////////////////////////////////




LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT		ps;		// used in WM_PAINT
HDC				hdc;	// handle to a device context
static          HGLRC hrc;
char buffer[80];        // used to print strings

 int oldMouseX, oldMouseY;


// what is the message 
switch(msg)
	{
	



	case WM_CREATE: 
        {
		// do initialization stuff here
		hdc = GetDC(hwnd);
		g_hdc = hdc;
		
		Init_Pixel_Format();

		hrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc,hrc);
		
		QueryPerformanceFrequency(&timerFrequency);
		QueryPerformanceCounter(&startTime);
		SetTimer(hwnd, 1, 1000, NULL);

        // return success
		return(0);
		} break;
   
	case WM_SIZE: 
		{
		// simply validate the window 
   	    
		int height = HIWORD(lparam);
		int width  = LOWORD(lparam);

		Resize_Scene(width,height);
        // return success
		return(0);
   		} break;
	
	case WM_DESTROY: 
		{
		
		wglMakeCurrent(hdc,NULL);
		wglDeleteContext(hrc);
		// kill the application, this sends a WM_QUIT message 
		PostQuitMessage(0);

        // return success
		return(0);
		} break;
	
		case WM_TIMER:
			{
			
			
			LARGE_INTEGER currentTime;
			float fps;

			
			// Get the current Time
			QueryPerformanceCounter(&currentTime);

			fps = (float)(nFrames - nLastFrames)/((float)(currentTime.QuadPart - startTime.QuadPart)/
				(float)timerFrequency.QuadPart);

			// Display the fps
			sprintf(buffer2,"%0.1f fps             2004 Kunal Patel",fps);
			//sprintf(buffer2,"pos x %f, pos y %f, pos z %f",g_Camera.View().x,g_Camera.View().y,g_Camera.View().z);


			///SetWindowText(hwnd, buffer);
			SetWindowText(hwnd, buffer2);
			nLastFrames = nFrames;

			// Resets the timer
			QueryPerformanceCounter(&startTime);
		
			}
			break;	

					case WM_RBUTTONDOWN:
		{
	
// Increase the current animation and mod it by the max animations
		obj.currentAnimation = (obj.currentAnimation + 1) % (obj.numAnimations);
		gun.currentAnimation = (gun.currentAnimation + 1) % (gun.numAnimations);

	//	obj.currentAnimation = 16;
		// Set the current frame to be the starting frame of the new animation
		obj.currentFrame = obj.pAnimInfo[obj.currentAnimation].startFrame;
		gun.currentFrame = gun.pAnimInfo[gun.currentAnimation].startFrame;

	if(sordoloaded == true)
	{
		sordo.currentAnimation = (sordo.currentAnimation + 1) % (sordo.numAnimations);
		sordogun.currentAnimation = (sordogun.currentAnimation + 1) % (sordogun.numAnimations);

	//	obj.currentAnimation = 16;
		// Set the current frame to be the starting frame of the new animation
		sordo.currentFrame = sordo.pAnimInfo[sordo.currentAnimation].startFrame;
		sordogun.currentFrame = sordogun.pAnimInfo[sordogun.currentAnimation].startFrame;
	}


	}break;




	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

///////////////////////////////////////////////////////////
void LoadModel(char *model)
{

	sordoloaded = true;


	Sordo.Load(&sordo,"models\\Ogro\\tris.md2","models\\Ogro\\Ogrobase.pcx");
	SordoGun.Load(&sordogun,"models\\Ogro\\weapon.md2", "models\\Ogro\\weapon.pcx");


	SetRenderType(&sordo,GL_TRIANGLES);
	SetRenderType(&sordogun,GL_TRIANGLES);

	Sordo.ComputeNormals(&sordo.pObject[0]);

}



void SetMusic(char *filename)
{
	Media.SetMedia(filename);
}
void InitScript()
{
	

	log2.Init("info.txt");

	scriptLoader.InitLogFile(&log2);
	vm.InitLogFile(&log2);
	scriptLoader.ReadASMFile("bspLoad.bxe");
//	scriptLoader.PrintStats();
	
	vm.AssignScript(scriptLoader.GetScript());
	
	
	vm.RegisterHost("BspLoadMap",(void*)BspLoadMap,1);
	vm.RegisterHost("LoadModel",(void *)LoadModel,1);
	vm.RegisterHost("SetMusic",(void *)SetMusic,1);
	
	vm.RunScript();
}

void InitPartDesc()
{
	smk.min_position = smk.max_position = CVector3(0,0,0);
    smk.min_noise = CVector3(-1000,-1000,0); smk.max_noise = CVector3(1000,1000,0);
	smk.emition_rate = 0.2;
    
	smk.min_vel = CVector3(-500,-500,0); smk.max_vel = CVector3(500,500,0);
	
	smk.max_age = 4;
	smk.min_age = 1;
	smk.min_size = 1;
	smk.max_size = 3;
	smk.min_colour = desc.min_colour;
	smk.max_colour = desc.max_colour;
	smk.number_particles = 50;
	smk.rot[0] = smk.rot[0] = smk.rot[0] =0; 

	



		desc.min_noise.x=-1.0f; desc.min_noise.y=-1.0f; desc.min_noise.z=1.0;
	desc.max_noise.x= 1.0f; desc.max_noise.y=1.0f; desc.max_noise.z= 1.0f;
	
	
	desc.min_vel.x = 100.0f; desc.min_vel.y = 100.0f; desc.min_vel.z = 0.0f;
	desc.max_vel.x = 100.0f; desc.max_vel.y = 100.0f; desc.max_vel.z = 0.0f;

	
	desc.min_size = 1.0f;
	desc.max_size =3.0f;
	
	desc.min_age = 0.2f;
	desc.max_age = 4.0f;

//	desc.min_position = //CVector3(-100,-100,-200);
//	desc.max_position = //CVector3(100,100,-200);

desc.min_position.x = 0.0f;desc.min_position.y = 0.0f;desc.min_position.z = -70.0f;
	desc.max_position.x = 0.0f;desc.max_position.y = 0.0f;desc.max_position.z = -70.0f;;
	
	
	desc.min_colour.col[R] = 1.0f;desc.min_colour.col[R] = 1.0f;
	desc.min_colour.col[G] = 0.1f;desc.max_colour.col[G] = 0.5f;
	desc.min_colour.col[B] = 0.2f;desc.max_colour.col[B] = 1.0f;
	desc.min_rotspeed = 2;
	desc.max_rotspeed = 5;
	desc.rot[0] = 1;
	desc.rot[1] = 0;
	desc.rot[2] = 1;

	

	desc.emition_rate = 0.5f;
	desc.number_particles = 50;

  
	 
	desc.min_position.x = 741.0f;desc.min_position.y =74.0f;desc.min_position.z = 952.0f;
	desc.max_position.x = 741.0f;desc.max_position.y = 74.0f;desc.max_position.z = 952.0f;;
	

	
	
	
	
	desc.min_vel.x = -70.0f; desc.min_vel.y = 70.0f; desc.min_vel.z = 0.0f;
	desc.max_vel.x = 70.0f; desc.max_vel.y = 170.0f; desc.max_vel.z = 0.0f;
	

	desc.min_colour.col[R] = 0.0f;desc.min_colour.col[R] = 0.1f;
	desc.min_colour.col[G] = 0.1f;desc.max_colour.col[G] = 0.5f;
	desc.min_colour.col[B] = 0.8f;desc.max_colour.col[B] = 1.0f;

	desc.min_age = 0.3;
	desc.max_age = 1.5;
	desc.emition_rate = 0.2;

	desc.min_rotspeed = 0.3;
	desc.max_rotspeed = 4;
	desc.rot[0] = 1;
	desc.rot[1] = 0;
	desc.rot[2] = 1;


	prt2.min_rotspeed = 2;
	prt2.max_rotspeed = 7;
	prt2 = desc;
	prt2.min_position.x = 900.0f;prt2.min_position.y =74.0f;prt2.min_position.z = 952.0f;
	prt2.max_position.x = 900.0f;prt2.max_position.y = 74.0f;prt2.max_position.z = 952.0f;;
	
	prt2.min_colour.col[R] = 5.0f;prt2.min_colour.col[R] = 8.1f;
	prt2.min_colour.col[G] = 0.1f;prt2.max_colour.col[G] = 1.0f;
	prt2.min_colour.col[B] = 0.1f;prt2.max_colour.col[B] = 0.3f;
	
	prt2.min_vel.x = -50.0f; prt2.min_vel.y = 100.0f; prt2.min_vel.z = -500.0f;
	prt2.max_vel.x = 50.0f; prt2.max_vel.y = 300.0f; prt2.max_vel.z = 500.0f;
	
	prt2.min_age = 0.2;
	prt2.max_age = 3;
	prt2.emition_rate = 0.5;

	prt2.min_size = 0.5f;
	prt2.max_size =5.0f;

	prt1 = desc;

	prt1.min_position.x = 625.0f;prt1.min_position.y =74.0f;prt1.min_position.z = 952.0f;
	prt1.max_position.x = 625.0f;prt1.max_position.y = 74.0f;prt1.max_position.z = 952.0f;;
	
	prt1.min_colour.col[R] = 0.3f;prt1.min_colour.col[R] = 1.0f;
	prt1.min_colour.col[G] = 0.2f;prt1.max_colour.col[G] = 0.6f;
	prt1.min_colour.col[B] = 0.0f;prt1.max_colour.col[B] = 1.2f;

		
	prt1.min_vel.x = -100.0f; prt2.min_vel.y = 2.0f; prt2.min_vel.z = -200.0f;
	prt1.max_vel.x = 100.0f; prt2.max_vel.y = 200.0f; prt2.max_vel.z = 200.0f;
	
	
	prt1.min_age = 0.2;
	prt1.max_age = 3;
	prt1.emition_rate = 0.7;

	prt1.min_size = 2.0f;
	prt1.max_size =5.0f;
	
}

void DrawModel2()
{

	if(sordoloaded)
	{

		glFrontFace(GL_CW);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		AnimateModel(&sordo,0.2);
		AnimateModel(&sordogun,0);
		glFrontFace(GL_CCW);
	}

}
void DrawModel()
{
	static float angle=0;

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	AnimateModel(&obj,0.2);
	AnimateModel(&gun,0);

}

void LoadModelRhino()
{
Rhino.Load(&obj,"md2 models\\sodf8\\tris.md2","md2 models\\sodf8\\Abarlith.pcx");
GunMD2.Load(&gun,"md2 models\\sodf8\\weapon.md2", "md2 models\\sodf8\\weapon.pcx");


SetRenderType(&gun,GL_TRIANGLES);
SetRenderType(&obj,GL_TRIANGLES);


Rhino.ComputeNormals(&obj.pObject[0]);
Rhino.SetConn(&obj.pObject[0]);

}




int Game_Main(void *parms = NULL, int num_parms = 0)
{
// this is the main loop of the game, do all your processing
// here
	
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
glLoadIdentity();									// Reset The matrix
	
static float angle = 0;


  
g_Camera.Look();
frustum.CalculateFrustum();


BspRenderMap(g_Camera.Position());


glPushMatrix();

glTranslatef(731, 60,852);	
glRotatef(angle, 0, 1.0f, 0);
DrawModel();
glPopMatrix();


glPushMatrix();

glTranslatef(781, 60,852);	
glRotatef(-angle, 0, 1.0f, 0);
DrawModel2();
glPopMatrix();

g_Camera.Update();

  
acc = g_timer.GetElapsedSeconds();

ExpPart.Emit(&desc);
ExpPart.Update_Particles(acc);
ExpPart.Render_Particles();

ExpPart1.Emit(&prt1);
ExpPart1.Update_Particles(acc);
ExpPart1.Render_Particles();

ExpPart2.Emit(&prt2);
ExpPart2.Update_Particles(acc);
ExpPart2.Render_Particles();



if(KEYDOWN(VK_SPACE))


angle++;

if(angle>360)
{
	angle =0;
}


glFlush();
SwapBuffers(g_hdc);
nFrames++;


// for now test if user is hitting ESC and send WM_CLOSE
if (KEYDOWN(VK_ESCAPE))
   SendMessage(main_window_handle,WM_CLOSE,0,0);


// return success or failure or your own return code here
return(1);

} // end Game_Main


int Game_Init(void *parms = NULL, int num_parms = 0)
{
// this is called once after the initial window is created and
// before the main event loop is entered, do all your initialization
// here

//char dir[105] = {"C:\\Program Files\\Quake III Arena\\baseq3\\pak0.pk3"};

//BspLoadMap(dir);

Media.Initialize();
Media.AttachToWindow(main_window_handle);


InitScript();


g_Camera.PositionCamera( 259, 216,-81,	455, 166, -93,	0, 1, 0);

	
	glDisable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glViewport(0,0,640,480);						// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what our our drawing boundries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(70.0f,(GLfloat)640/(GLfloat)480, 10.0, 3000.0);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();		

 
	LoadModelRhino();

	ExpPart.Init_Particle_System(400); //explosion particles
	ExpPart1.Init_Particle_System(400);
	ExpPart2.Init_Particle_System(400);



	Media.Play();

	InitPartDesc();
	g_timer.Init();


  // return success or failure or your own return code here
return(1);

} // end Game_Init

/////////////////////////////////////////////////////////////

int Game_Shutdown(void *parms = NULL, int num_parms = 0)
{
// this is called after the game is exited and the main event
// loop while is exited, do all you cleanup and shutdown here

Rhino.CleanUp(&obj);
GunMD2.CleanUp(&gun);
Media.CleanUp();
bspCleanUp();

ExpPart.Destroy();
ExpPart1.Destroy();
ExpPart2.Destroy();

// return success or failure or your own return code here
return(1);

} // end Game_Shutdown

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASSEX winclass; // this will hold the class we create
HWND	   hwnd;	 // generic window handle
MSG		   msg;		 // generic message
HDC        hdc;      // graphics device context

// first fill in the window class stucture
winclass.cbSize         = sizeof(WNDCLASSEX);
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL;
winclass.lpszClassName	= WINDOW_CLASS_NAME;
winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

// save hinstance in global
main_instance = hinstance;

// register the window class
if (!RegisterClassEx(&winclass))
	return(0);

// create the window
if (!(hwnd = CreateWindowEx(NULL,                  // extended style
                            WINDOW_CLASS_NAME,     // class
						    "Bison 1.0", // title
						    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					 	    0,0,	  // initial x,y
						    640,480,  // initial width, height
						    NULL,	  // handle to parent 
						    NULL,	  // handle to menu
						    hinstance,// instance of this application
						    NULL)))	// extra creation parms
return(0);

// save main window handle
main_window_handle = hwnd;

// initialize game here
Game_Init();

// enter main event loop
while(TRUE)
	{
    // test if there is a message in queue, if so get it
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	   { 
	   // test if this is a quit
       if (msg.message == WM_QUIT)
           break;
	
	   // translate any accelerator keys
	   TranslateMessage(&msg);

	   // send the message to the window proc
	   DispatchMessage(&msg);
	   } // end if
    
       // main game processing goes here
       Game_Main();
       
	} // end while

// closedown game here
Game_Shutdown();

// return to Windows like this
return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////

