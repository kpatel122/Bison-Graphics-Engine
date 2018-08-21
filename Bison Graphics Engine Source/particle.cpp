#include <windows.h>   // include important windows stuff
#include <windowsx.h>
#include <winuser.h>  
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
//#include <gl\glaux.h>	

#include "CVector.h"
#include "PARTICLE.H"

#define RAND_MAX1 0x7fff

float RandomNumber(float fMin, float fMax){
  if (fMin == fMax) return(fMin);
  float fRandom = (float)rand() / (float)RAND_MAX1;
  return((fRandom * (float)fabs(fMax-fMin))+fMin);
}


#define PI		(3.14159265359f)
#define DEG2RAD(a)	(PI/180*(a))
#define RAD2DEG(a)	(180/PI*(a))






void CParticle::Update(float time)
{
	
	age += time;

	if(age>life_length)
	{
	
		
		if((colour_start.col[A]=colour_start.col[A] - (2*time))<0)
			Kill();	
		
	}

	CVector3 vec;
	
	

	vec.x = velocity.x * direction.x;
	vec.y = velocity.y * direction.y;
	vec.z = velocity.z * direction.z ;


	
	postion = postion + (vec*time);
	
}

int CParticleSystem::Init_Particle_System(int number_particles)
{
		pParticles = new CParticle[number_particles];
	
	if(!pParticles)
		return 0;


	
	this->number_particles = number_particles;

	for(int i=0;i<number_particles;i++)
	{
		pParticles[i].alive = false;
	}
	

	
	
	Load_Texture_2D("prt.bmp",&tex);
	
	
	glBindTexture(GL_TEXTURE_2D,tex.tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex.width, tex.height, GL_RGB, GL_UNSIGNED_BYTE,tex.image.data);
	
	return 1;
}
void CParticleBlockExplosion::Create_Particle(CVector3 pos,CVector3 vel,CVector3 dir,float life,int size,
				float rot_speed,int *rot ,COLOUR *col)
{
int pindex = -1;
	for(int i=0;i<number_particles;i++)
		if(pParticles[i].IsAlive()==false)
		{
			pindex = i;
			break;
		}
		if(pindex == -1)
			return;
			
			pParticles[pindex].direction = dir;
			pParticles[pindex].age = 0;
			pParticles[pindex].life_length = life;
			pParticles[pindex].alive = true;
			pParticles[pindex].colour_start.col[R] = col->col[R]; pParticles[i].colour_start.col[G] = col->col[G];pParticles[i].colour_start.col[B] =col->col[B];
			pParticles[pindex].colour_start.col[A] = col->col[A] = 1.0f;
		
			pParticles[pindex].velocity = vel;
			pParticles[pindex].rot[0] = rot[0];
			pParticles[pindex].rot[1] = rot[1];
			pParticles[pindex].rot[2] = rot[2];
			pParticles[pindex].angle_counter = 0;
			pParticles[pindex].rot_speed = rot_speed;

			pParticles[pindex].postion = pos;
 			pParticles[pindex].size = size;
}


void CParticleSystem::Create_Particle(CVector3 pos,CVector3 vel,CVector3 dir,float life,int size,
				float rot_speed,int *rot ,COLOUR *col)
{
int pindex = -1;
	for(int i=0;i<number_particles;i++)
		if(pParticles[i].IsAlive()==false)
		{
			pindex = i;
			break;
		}
		if(pindex == -1)
			return;
			
			pParticles[pindex].direction = dir;
			pParticles[pindex].age = 0;
			pParticles[pindex].life_length = life;
			pParticles[pindex].alive = true;
			pParticles[pindex].colour_start.col[R] = col->col[R]; pParticles[i].colour_start.col[G] = col->col[G];pParticles[i].colour_start.col[B] =col->col[B];
			pParticles[pindex].colour_start.col[A] = col->col[A] = 1.0f;
		
			pParticles[pindex].velocity = vel;
		

			pParticles[pindex].postion = pos;
 			pParticles[pindex].size = size;
}


void CParticleSystem::Update_Particles(float time)
{
	
	elapsed_sec =  time;
	
	for(int i=0;i<number_particles;i++)
	{
		if(pParticles[i].IsAlive())
		{
			
			
			pParticles[i].Update(time);
		
			
			
		}

	}

}


void CParticleBlockExplosion::Render_Particles()
{

	
	static float angle = 0;
	
	
	float rotx,roty,rotz;


		 // enable alpha blending and texturing
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  // set the blend mode
 glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  //glBlendFunc(GL_ONE, GL_ONE);
 // glBlendFunc(GL_ZERO, GL_ONE);
  


  glBindTexture(GL_TEXTURE_2D, tex.tex_id);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



  // to avoid constant dereferencing...
  CVector3 partPos;
  OutputDebugString("hello111\n");
 
float  size = 0;

	


  for (int i = 0; i < number_particles; ++i)
  {
	if(pParticles[i].IsAlive()==true)
	{
		

  
	
	
	glColor4f(pParticles[i].colour_start.col[R],pParticles[i].colour_start.col[G],pParticles[i].colour_start.col[B],
		pParticles[i].colour_start.col[A]);

		
	//	rotx = pParticles[i].rot[0];
	//	roty = 
		partPos = pParticles[i].postion;
		size = pParticles[i].size;
		angle = pParticles[i].angle_counter;
		
		
		//glRotatef(90,0,0,1);
		glPushMatrix();
	
	//glTranslatef(partPos.x,partPos.y,-200);
	glTranslatef(partPos.x,partPos.y,partPos.z);

	glRotatef(angle,pParticles[i].rot[0],pParticles[i].rot[1],pParticles[i].rot[2]);



	
	
	// draw the quads
  glBegin(GL_TRIANGLES);

glTexCoord2f(0.0, 1.0);
		glVertex3f(-size, -size,  0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( size, -size, 0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( size,  size,  0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-size, size,  0);
	  glEnd();
	  	glPopMatrix();

	  }
  }


  	
 	glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 
 
}

void CParticleSystem::Render_Particles()
{
	/*
  float  size = 0;
//  glEnable(GL_BLEND);
 // glEnable(GL_TEXTURE_2D);

  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // select the snow texture
//  glBindTexture(GL_TEXTURE_2D, tex.tex_id);
//  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  CVector3 partPos;
  
 // draw the quads
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < number_particles; ++i)
  {
	if(pParticles[i].IsAlive()==true)
	{
		glColor4f(pParticles[i].colour_start.col[R],pParticles[i].colour_start.col[G],pParticles[i].colour_start.col[B],
		pParticles[i].colour_start.col[A]);

		partPos = pParticles[i].postion;
		size = pParticles[i].size;
		glTexCoord2f(0.0, 1.0);
		glVertex3f(partPos.x, partPos.y, partPos.z);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(partPos.x + size, partPos.y, partPos.z);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(partPos.x + size, partPos.y - size, partPos.z);
	//	glTexCoord2f(0.0, 0.0);
	//	glVertex3f(partPos.x, partPos.y - size, partPos.z);
	
	  }
  }
  glEnd();

  */

 // glDisable(GL_BLEND);
  //glDisable(GL_TEXTURE_2D);
	
}

void CParticleExplosion::Build_CosSin_Tables()
{
	// create sin/cos lookup table

	// generate the tables
	for (int ang = 0; ang < 360; ang++)
    {
    // convert ang to radians
    float theta = (float)ang*PI/(float)180;

    // insert next entry into table
    cos_look[ang] = (float)cos(theta);
    sin_look[ang] = (float)sin(theta);

    } // end for ang

}


void CParticleSystem::Start_Particles(PARTICLE_DESC *pd)
{
	CVector3 dir;
	CVector3 vel;
	float size;
	float life;
	float rot_speed;
	int *rot;

	CVector3 pos;


	pdd = *pd;


	int angle;
	COLOUR col;
		
	
	pos.x =RAND_RANGE((int)pdd.min_position.x,(int)pdd.max_position.x);
	pos.y =RAND_RANGE((int)pdd.min_position.y,(int)pdd.max_position.y);
	pos.z = RAND_RANGE((int)pdd.min_position.z,(int)pdd.max_position.z);
		

	while(--pdd.number_particles>0)
	{

		
		

		angle = rand()%360;
		
		Set_Particle_Direction(dir,pd);
		
		dir = Normalize(dir);
		
		vel.x = RandomNumber(pdd.min_vel.x,pdd.max_vel.x);
		vel.y = RandomNumber(pdd.min_vel.y,pdd.max_vel.y);
		vel.z = RandomNumber(pdd.min_vel.z,pdd.max_vel.z);

		
		size =  RandomNumber(pdd.min_size,pdd.max_size);
		life =  RandomNumber(pdd.min_age,pdd.max_age);
		rot_speed = RandomNumber(pdd.min_rotspeed,pdd.max_rotspeed);
		rot[0] = (int)&pdd.rot[0];
		rot[1] = (int)&pdd.rot[1];
		rot[2] = (int)&pdd.rot[2];


		col.col[R] = RandomNumber(pdd.min_colour.col[R],pdd.max_colour.col[R]);
		col.col[G] = RandomNumber(pdd.min_colour.col[G],pdd.max_colour.col[G]);
		col.col[B] = RandomNumber(pdd.min_colour.col[B],pdd.max_colour.col[B]);
		
		this->Create_Particle(pos,vel,dir,life,size,rot_speed,
							rot,&col);
		

	}
	


}


void CParticleExplosion::Start_Particles(PARTICLE_DESC *pd)
{
	CVector3 dir;
	CVector3 vel;
	float size;
	float life;
	CVector3 pos;
	int *rot;
	float rot_speed;


	pdd = *pd;



	COLOUR col;
		
	
	pos.x =RAND_RANGE((int)pdd.min_position.x,(int)pdd.max_position.x);
	pos.y =RAND_RANGE((int)pdd.min_position.y,(int)pdd.max_position.y);
	pos.z = RAND_RANGE((int)pdd.min_position.z,(int)pdd.max_position.z);
		

	while(--pdd.number_particles>0)
	{

		
		Set_Particle_Direction(dir,pd);
		
		dir = Normalize(dir);
		
		vel.x = RandomNumber(pdd.min_vel.x,pdd.max_vel.x);
		vel.y = RandomNumber(pdd.min_vel.y,pdd.max_vel.y);
		vel.z = RandomNumber(pdd.min_vel.z,pdd.max_vel.z);




		
		size =  RandomNumber(pdd.min_size,pdd.max_size);
		life =  RandomNumber(pdd.min_age,pdd.max_age);

		col.col[R] = RandomNumber(pdd.min_colour.col[R],pdd.max_colour.col[R]);
		col.col[G] = RandomNumber(pdd.min_colour.col[G],pdd.max_colour.col[G]);
		col.col[B] = RandomNumber(pdd.min_colour.col[B],pdd.max_colour.col[B]);
		
		rot_speed = RandomNumber(pdd.min_rotspeed,pdd.max_rotspeed);
		rot[0] = (int)&pdd.rot[0];
		rot[1] = (int)&pdd.rot[1];
		rot[2] = (int)&pdd.rot[2];


		this->Create_Particle(pos,vel,dir,life,size,rot_speed,rot,&col);
		

	}
	



}

void CParticleBlockExplosion::Start_Particles(PARTICLE_DESC *pd)
{
	CVector3 dir;
	CVector3 vel;
	float size;
	float life;
	CVector3 pos;
	float rot_speed;
	int rot[3];


	pdd = *pd;



	COLOUR col;
		
	
	pos.x =RAND_RANGE((int)pdd.min_position.x,(int)pdd.max_position.x);
	pos.y =RAND_RANGE((int)pdd.min_position.y,(int)pdd.max_position.y);
	pos.z = RAND_RANGE((int)pdd.min_position.z,(int)pdd.max_position.z);
		
	while(--pdd.number_particles>0)
	{

		
		Set_Particle_Direction(dir,pd);
		
		dir = Normalize(dir);
		
		vel.x = RandomNumber(pdd.min_vel.x,pdd.max_vel.x);
		vel.y = RandomNumber(pdd.min_vel.y,pdd.max_vel.y);
		vel.z = RandomNumber(pdd.min_vel.z,pdd.max_vel.z);

		
		size =  RandomNumber(pdd.min_size,pdd.max_size);
		life =  RandomNumber(pdd.min_age,pdd.max_age);
		//rot_speed = RandomNumber(pdd.min_rotspeed,pdd.max_rotspeed);
		pdd.angle_counter = 0;

		rot_speed = RandomNumber(pdd.min_rotspeed,pdd.max_rotspeed);
		
		rot[0] = RAND_RANGE(0,1);//(int)&pdd.rot[0];
		rot[1] = RAND_RANGE(0,1);//(int)&pdd.rot[1];
		rot[2] = RAND_RANGE(0,1);//(int)&pdd.rot[2];

		
		col.col[R] = RandomNumber(pdd.min_colour.col[R],pdd.max_colour.col[R]);
		col.col[G] = RandomNumber(pdd.min_colour.col[G],pdd.max_colour.col[G]);
		col.col[B] = RandomNumber(pdd.min_colour.col[B],pdd.max_colour.col[B]);
		
		

		this->Create_Particle(pos,vel,dir,life,size,rot_speed,rot,&col);
		

	}
	



}

void CParticleSystem::Emit(PARTICLE_DESC *pd)
{
		pd->accumulated_time += elapsed_sec;

		if(pd->accumulated_time>pd->emition_rate)
		{
			pd->accumulated_time = 0;
			Start_Particles(pd);
		}
}		

void CParticleBlockExplosion::Update_Particles(float time)
{
		elapsed_sec =  time;
	
		for(int i=0;i<number_particles;i++)
		{
			if(pParticles[i].IsAlive())
			{
			
				
				pParticles[i].age += time;

				if(pParticles[i].age>pParticles[i].life_length)
				{
	
		
					if((pParticles[i].colour_start.col[A]=pParticles[i].colour_start.col[A] - (2*time))<0)
					pParticles[i].Kill();	
		
				}

			CVector3 vec;
	
	

			vec.x = pParticles[i].velocity.x * pParticles[i].direction.x;
			vec.y = pParticles[i].velocity.y * pParticles[i].direction.y;
			vec.z = pParticles[i].velocity.z * pParticles[i].direction.z ;


	
			pParticles[i].postion = pParticles[i].postion + (vec*time);
			
			
			pParticles[i].angle_counter +=pParticles[i].rot_speed;	
			
			if(pParticles[i].angle_counter >360)
			{
				pParticles[i].angle_counter = 0;
			}
			}
	}
}


void CParticleSystem::Set_Particle_Direction(CVector3 &vec, PARTICLE_DESC *pd)
{
		vec.x = RandomNumber(pd->min_noise.x,pd->max_noise.x); 
		vec.y = RandomNumber(pd->min_noise.y,pd->max_noise.y);
		vec.z = RandomNumber(pd->min_noise.z,pd->max_noise.z);

		
		
}

void CParticleExplosion::Set_Particle_Direction(CVector3 &vec, PARTICLE_DESC *pd)
{
		int angle = RandomNumber(0,360);
		vec.x =Cos_Look(angle); 
		vec.y =Sin_Look(angle);
		vec.z =RAND_RANGE((int)pdd.min_noise.z,(int)pdd.max_noise.z);
}

void CParticleBlockExplosion::Set_Particle_Direction(CVector3 &vec, PARTICLE_DESC *pd)
{
		vec.x = RandomNumber(pd->min_noise.x,pd->max_noise.x); 
		vec.y = RandomNumber(pd->min_noise.y,pd->max_noise.y);
		vec.z = RandomNumber(pd->min_noise.z,pd->max_noise.z);

}
