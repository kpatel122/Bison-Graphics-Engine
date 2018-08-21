#include "CMedia.h"

int CMediaPlayer::Initialize()
{
	CoInitialize(NULL);
	// Create the filter graph manager.
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
    IID_IGraphBuilder, (void **)&pGraph);
    pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
    pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
	pGraph->QueryInterface(IID_IBasicAudio,(void**)&pBasicAudio);
	pGraph->QueryInterface(IID_IBasicVideo,(void**)&pBasicVideo);
	

	return (1);
}
int CMediaPlayer::AttachToWindow(HWND whandle)
{
//Set the video window.
    pVidWin->put_Owner((OAHWND)whandle);
    pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

    RECT grc;
    GetClientRect(whandle, &grc);
    pVidWin->SetWindowPosition(0, 0, grc.right, grc.bottom);

	return(1);
}
void CMediaPlayer::CleanUp()
{
	pVidWin->put_Visible(OAFALSE);
    pVidWin->put_Owner(NULL);   
    pMediaControl->Release();
    pVidWin->Release();
	pBasicAudio->Release();
	pBasicVideo->Release();
    pGraph->Release();
	
	CoUninitialize();
}

void CMediaPlayer::SetMedia(char *filename) 
{
	WCHAR wPath[1024];
	MultiByteToWideChar(CP_ACP,0,filename,-1,wPath,sizeof(wPath)/sizeof(wPath[0]));
	pGraph->RenderFile(wPath, NULL);
	 

}
void CMediaPlayer::Play()
{
	pMediaControl->Run();
}

void CMediaPlayer::SetScreenMode(int mode)
{
	if(GetScreenMode() == mode)
		return;
	
	mScreenMode = mode;
	pVidWin->put_FullScreenMode(mode);
}

