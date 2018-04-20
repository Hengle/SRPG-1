#include "stdafx.h"
#include "Stage.h"
#include "Tile.h"

CTile::CTile()
{
}


CTile::~CTile()
{
	this->Release();
}

void CTile::InitTile()
{
	for (int i = 0; i < STAGEY; ++i)		// Y쪽
	{
		for (int j = 0; j < STAGEX; ++j)	// X쪽
		{
			float fx = float((j * TILECX) + TILECX / 2);
			float fy = float((i * TILECY) + TILECY / 2);
			int iIndex = i * STAGEX + j;
			m_vecTile.push_back(CreateTile(fx, fy, iIndex));
		}
	}
}

PTILE CTile::CreateTile(float fx_, float fy_, int index_)
{
	TILE* pTile = new TILE;

	pTile->iIndex = index_;
	pTile->iPath = -2;

	pTile->fX = fx_;
	pTile->fY = fy_;
	pTile->fCX = 64;
	pTile->fCY = 64;

	pTile->iDraw = 0;
	pTile->iSelect = 0;
	pTile->iType = E_TILE_NONE;
	pTile->eTeam = E_TEAM_NULL;
	pTile->iMarker = -1;
	pTile->bAlive = pTile->bFull = false;
	
	pTile->nears.left = (index_ % STAGEX > 0) ? index_ - 1 : -1;
	pTile->nears.right = (index_ % STAGEX < STAGEX - 1) ? index_ + 1 : -1;
	pTile->nears.top = (index_ - STAGEX >= 0) ? index_ - STAGEX : -1;
	pTile->nears.bottom = (index_ + STAGEX < STAGEX  * STAGEY ) ? index_ + STAGEX : -1;

	m_Info.fCX = TILECX;
	m_Info.fCY = TILECY;

	m_fSpeed = 5.f;
	return pTile;
}

void CTile::CheckKey()
{
	/*
	if (GetAsyncKeyState('W'))
		CStage::m_tCam.y -= m_fSpeed;
	if (GetAsyncKeyState('S'))
		CStage::m_tCam.y += m_fSpeed;
	if (GetAsyncKeyState('A'))
		CStage::m_tCam.x -= m_fSpeed;
	if (GetAsyncKeyState('D'))
		CStage::m_tCam.x += m_fSpeed;
		*/

	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		if (m_bBoxShowUp == true)
			m_bBoxShowUp = false;
		else
			m_bBoxShowUp = true;
	}

	if (GetAsyncKeyState('T') & 0x0001)
	{
		if (m_bTextShowUp == true)
			m_bTextShowUp = false;
		else
			m_bTextShowUp = true;
	}
}

void CTile::TargetTile()
{
	pfMouse.x = CMousePoint::GetPos().x + CStage::m_tCam.x - WINCX / 2;
	pfMouse.y = CMousePoint::GetPos().y + CStage::m_tCam.y - WINCY / 2;

	m_iPrevTarget = m_iTarget;
	int iIndex = int(pfMouse.y / TILECY) * STAGEX + int(pfMouse.x / TILECX);

	if (iIndex < 0 || iIndex >= STAGEX * STAGEY)
	{
		m_iTarget = -1;
		return;
	}

	else m_iTarget = iIndex;

	m_bOverTile = (m_iPrevTarget != m_iTarget) ? true : false;
}

void CTile::HighlightMove()
{
	if (!m_bOverTile)
		return;

	if (m_vecTile[m_iTarget]->iType == E_TILE_MOVE)
		m_vecTile[m_iTarget]->iSelect = 1;
	if (m_iPrevTarget > -1)
		m_vecTile[m_iPrevTarget]->iSelect = 0;
}

void CTile::HighlightAct(int range, E_TEAM_ID team_, E_PATH_TYPE type_)
{
	if (!m_bOverTile)
		return;

	GET_SINGLE(CRangefinder)->Clear();
	if (m_vecTile[m_iTarget]->iType == E_TILE_ACT ||
		m_vecTile[m_iTarget]->iType == E_TILE_TARGET)
	{
		GET_SINGLE(CRangefinder)->Pathfind(m_iTarget, range, team_, type_);
	}
}

void CTile::ChangeTile(int curr_, int marker_, E_TEAM_ID team_)
{
	m_vecTile[curr_]->iMarker = marker_;
	m_vecTile[curr_]->eTeam = team_;
}

void CTile::CheckSave()
{
	if (GetAsyncKeyState('P') & 0x0001)
	{
		SaveData();
	}

	else if (GetAsyncKeyState('O') & 0x0001)
	{
		LoadData();
	}
}


void CTile::SaveData()
{
	if (MessageBox(g_hWnd, L"저장 하시겠습니까 ? ", L"SystemMessage", MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwByte = 0;

	hFile = CreateFile(L"../Tiles.dat", GENERIC_WRITE, NULL,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)	// 오류출력(예외처리)
		MessageBox(g_hWnd, L"저장 오류 !", NULL, NULL);

	for (vector<PTILE>::iterator iter = m_vecTile.begin();
	iter != m_vecTile.end(); ++iter)
	{
		WriteFile(hFile, *iter, sizeof(TILE), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CTile::LoadData()
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwByte;

	//if (MessageBox(g_hWnd, L"로드 하시겠습니까?", L"SystemMessage", MB_OKCANCEL) == IDCANCEL)
	//	return;

	//release
	for (vector<PTILE>::iterator iter = m_vecTile.begin();
	iter != m_vecTile.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_vecTile.clear();

	hFile = CreateFile(L"../Tiles.dat", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, L"로드 오류 : 파일을 찾을수 없습니다.", NULL, NULL);
		return;
	}

	while (true)
	{
		PTILE tile_ = new TILE;
		ReadFile(hFile, tile_, sizeof(TILE), &dwByte, NULL);
		m_vecTile.push_back(tile_);

		if (dwByte == 0)
			break;
	}

	CloseHandle(hFile);
}


void CTile::Initialize()
{
	InitTile();
	m_iTarget = -1;
	m_iPrevTarget = -1;
	CStage::m_tCam.x = 545;
	CStage::m_tCam.y = 545;
	m_bBoxShowUp = false;
	m_bTextShowUp = false;
}

E_SCENE_ID CTile::Progress()
{
	TargetTile();
	CheckKey();
	CheckSave();

	if (GetAsyncKeyState('Q'))
	{
		pfMouse.x = CMousePoint::GetPos().x + CStage::m_tCam.x - WINCX / 2;
		pfMouse.y = CMousePoint::GetPos().y + CStage::m_tCam.y - WINCY / 2;

		int iIndex = int(pfMouse.y / TILECY) * STAGEX + int(pfMouse.x / TILECX);

		if (iIndex < 0 || iIndex >= STAGEX * STAGEY)
			return E_SCENE_NONPASS;

		m_vecTile[iIndex]->iType = E_TILE_BLOCKED;
	}

	return E_SCENE_NONPASS;
}

void CTile::Render(HDC hdc)
{
	RECT rect = { 0, };
	wchar_t wcTmp[128];

	for (int i = 0; i < WINCY / TILECY + 2; ++i)	// Y쪽 연산
	{
		for (int j = 0; j < WINCX / TILECX + 2; ++j)	// X쪽
		{
			int iCullX = int((CStage::m_tCam.x - WINCX / 2) / TILECX);
			int iCullY = int((CStage::m_tCam.y - WINCY / 2) / TILECY);

			int iIndex = (i + iCullY) * STAGEX + (j + iCullX);
			if (iIndex < 0 || iIndex >= STAGEX * STAGEY)	// 벡터의 크기 만큼 예외처리
				break;

			m_Info.fX = int((m_vecTile[iIndex]->fX) + WINCX / 2 - CStage::m_tCam.x);
			m_Info.fY = int((m_vecTile[iIndex]->fY) + WINCY / 2 - CStage::m_tCam.y);
			
			int select = int(m_vecTile[iIndex]->iSelect);
			int type = int(m_vecTile[iIndex]->iType);


			//text
			/*
			rect = {
				int(m_Info.fX - 20),
				int(m_Info.fY - 20),
				int(m_Info.fX),
				int(m_Info.fY),
			};

			swprintf(wcTmp, 128, L"%d", iIndex);
			DrawText(hdc, wcTmp, -1, &rect, NULL);
			*/

			//check select
			if (type == 0 && select == 0)
				continue;

			if (!m_bTextShowUp && type == 4)
				continue;

			(*m_pMapBmp)["Highlight"]->Draw(&hdc, m_Info, 
				select * TILECX, 
				type * TILECY,
				130, RGB(255, 255, 255));
		}
	}
}



void CTile::Release()
{

}

