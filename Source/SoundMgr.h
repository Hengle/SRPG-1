#pragma once
#include "Include.h"

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr);
private:
	//directx sdk (마이크로소프트에서 컴퓨터에있는 하드웨어를 쉽게 상요할수 있게
	// 여러가지 함수와, 데이터형을 제공해놓은 헤더파일과 라이브러리파일의 집합.)
	//다이렉트 사운드 인터페이스
	LPDIRECTSOUND				m_pSound; //사운드카드를 대표하는 객체.
	vector<LPDIRECTSOUNDBUFFER> m_vecSoundBuff;	//사운드파일을 로드해서 저장할 벡터

public:
	HRESULT Initialize();	// 장치를 초기화 하는역활.
	HRESULT LoadWave(TCHAR* pFileName);	//파일경로를 통해 파일을 읽어 드리겠다.
	void SoundPlay(int iIndex /*몇번째 파일을 재생할래?*/, DWORD dwFlag/*무한루프, 한번*/); //파일을 재생 하겠다.
	void SoundStop(int iIndex);
	void SoundAllStop();
	bool SoundPlaying(int iIndex);	// 현재 iIndex번째 사운드가 재생되고있는지 아닌지.
	void SoundReset();
	void Release();
private:
	CSoundMgr();
public:
	~CSoundMgr();
};

