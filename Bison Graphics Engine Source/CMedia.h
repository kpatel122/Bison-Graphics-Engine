#include <windows.h>
#include <dshow.h>


#define WINDOW OAFALSE
#define FULLSCREEN OATRUE

//enum MEDIA_STATE = {PLAYING,STOP,PAUSED;}

class CMediaPlayer
{
public:
	
	CMediaPlayer(){pGraph=NULL,pMediaControl=NULL,pVidWin=NULL;mScreenMode = WINDOW;}
	~CMediaPlayer(){};
	
	void SetMedia(char* filename ) ;
	int AttachToWindow(HWND whandle);
	
	void Play();
	void Stop();
	void Plause();
	void CleanUp();
	int Initialize();
	void SetScreenMode(int mode);	

	void SetVolume(int volume){mVolume = volume; pBasicAudio->put_Volume(mVolume);}
	void SetPan(int balance){mBalance = balance; pBasicAudio->put_Balance(mBalance);}

	int GetVolume(){return mVolume;}
	int GetPan(){return mBalance;}
	int GetScreenMode(){return mScreenMode;}


private:

int mVolume;
int mBalance; 
int mScreenMode;

//MEDIA_STATE state;
IVideoWindow    *pVidWin;
IGraphBuilder   *pGraph;
IMediaControl   *pMediaControl;
IBasicAudio		*pBasicAudio;
IBasicVideo		*pBasicVideo;


};