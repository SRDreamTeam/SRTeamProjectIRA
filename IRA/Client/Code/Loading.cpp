#include "stdafx.h"
#include "..\Header\Loading.h"
#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szString, sizeof(_tchar) * 128);
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{
}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*	pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag = 0;

	EnterCriticalSection(&(pLoading->Get_Crt()));

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(&(pLoading->Get_Crt()));

	// _endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	Set_String(L"Texture Loading.....");
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player0.png")), E_FAIL);
	// 준석 추가함 (23.03.02) 슬라임
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilSlime_Move_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Attack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilSlime_Attack_0%d.png", 9)), E_FAIL);
	
	// 준석 추가함 (23.03.03) 이블소울
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Stand_045_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Move_045_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Stand_135_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Move_135_0%d.png", 7)), E_FAIL);

	// 준석 추가함 (23.03.04) 뮤턴트 이블소울
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_All", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_MutationEvilSoul_Stand_045_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_All", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_MutationEvilSoul_Stand_135_0%d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);

	Set_String(L"Buffer Loading.....");
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	Set_String(L"ETC Loading.....");
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	m_bFinish = true;

	Set_String(L"Loading Complete!!!!!!!");

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading *	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
