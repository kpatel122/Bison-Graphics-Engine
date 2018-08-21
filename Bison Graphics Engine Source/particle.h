#ifndef PARTICLE
#define PARTICLE


#include "ogl_lib1.h"

#define X 0
#define Y 1
#define Z 2

#define R 0
#define G 1
#define B 2
#define A 3

#define ALIVE 1
#define DEAD 0

enum PARTICLE_STATE {alive,dying,dead};
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))

//const double PI = 3.1415926535;

typedef struct VECTOR
{
	float pos[3];
}VECTOR;

typedef struct COLOUR
{
	float col[4];
}COLOUR;

typedef struct PARTICLE_DESC
{
	CVector3 min_noise,max_noise;
	CVector3 min_vel,max_vel;
	
	float min_size,max_size;
	float min_age, max_age;

	COLOUR min_colour;
	COLOUR max_colour;
	
	CVector3 min_position;
	CVector3 max_position;
	
	int min_angle,max_angle;
	int number_particles;
	
	float emition_rate;
	float accumulated_time;

	bool rot[3];
    float angle_counter;
	float min_rotspeed;
	float max_rotspeed;

}PARTICLE_DESC;

class CParticle
{
public:
	CParticle(){alive = false ,size=2,life_length=100,colour_start.col[A] = 1.0f, age = 0;}
	~CParticle(){};

	virtual void Update(float time);
	void Init();
	bool IsAlive() {return alive;}
	virtual void Kill() {alive = false;}

	

	 CVector3 postion;
	 CVector3 velocity;
	 CVector3 direction;

	 COLOUR colour_start;
	 COLOUR colour_step;
	
	
	float size;
	float life_length;
	float age;
	bool alive;
	float speed;
	
	bool rot[3];
    float angle_counter;
	float rot_speed;
};

class CParticleSystem
{
public:

	
	CParticleSystem(){};
	virtual ~CParticleSystem(){elapsed_sec = 0;};

	int Init_Particle_System(int number_particles);

	
	
	virtual void Start_Particles(PARTICLE_DESC *pd);
	virtual void Emit(PARTICLE_DESC *pd);
	virtual	void Render_Particles();
	virtual void Update_Particles(float time);
	virtual void Create_Particle(CVector3 pos,CVector3 vel,CVector3 dir,float life,int size,
				float rot_speed,int *rot ,COLOUR *col);
	virtual void Set_Particle_Direction(CVector3 &vec, PARTICLE_DESC *pd);

	void Destroy(){if(pParticles){delete[] pParticles;pParticles = NULL; }   }
	
	CParticle *pParticles;
	PARTICLE_DESC pdd;
	CVector3 postion;
	int number_particles;
	CVector3 gravity;
	float emit_rate;
	float elapsed_sec;
	OGL_TEXTURE_2D tex;
	
	
	
};

class CParticleExplosion : public CParticleSystem
{
public:
	
	CParticleExplosion() {Build_CosSin_Tables();}
	virtual ~CParticleExplosion(){};
	
	float Cos_Look(int index) {return cos_look[index];}
	float Sin_Look(int index) {return sin_look[index];}
	
	

	virtual void Set_Particle_Direction(CVector3 &vec, PARTICLE_DESC *pd);
	///virtual void Update_Particles(float time);
	virtual void Start_Particles(PARTICLE_DESC *pd);


	
	void Build_CosSin_Tables();
	
	float cos_look[360];
	float sin_look[360];

};

class CParticleBlockExplosion: public CParticleExplosion
{
public:
//	CParticleBlockExplosion(){Build_CosSin_Tables();}
//	virtual ~CParticleBlockExplosion(){};

	void Set_Particle_Direction(CVector3 &vec, PARTICLE_DESC *pd);
	///virtual void Update_Particles(float time);
	void Start_Particles(PARTICLE_DESC *pd);
	void Render_Particles();
	void Update_Particles(float time);
	void Draw_Particle();
		virtual void Create_Particle(CVector3 pos,CVector3 vel,CVector3 dir,float life,int size,
				float rot_speed,int *rot ,COLOUR *col);
//protected:
	

//	bool rot[3];
 //   float angle_counter;
//	float rot_speed;

	
	
};
		
#endif