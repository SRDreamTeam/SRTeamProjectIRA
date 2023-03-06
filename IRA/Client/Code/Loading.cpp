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
#pragma region Monster_Texture
	// 슬라임_일반 충돌형_1
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilSlime_Move_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Attack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilSlime_Attack_0%d.png", 9)), E_FAIL);
	// 이블소울_일반 원거리형_1
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Stand_045_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Move_045_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Stand_135_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Move_135_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Monster_Bullet_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Spr_Bullet_Deva_Circle_White_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Monster_Bullet_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Spr_Bullet_SwordShot_White_0%d.png", 7)), E_FAIL);
	// 뮤턴트 이블소울_변형 원거리형_1
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_All", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_MutationEvilSoul_Stand_045_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_All", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_MutationEvilSoul_Stand_135_0%d.png", 7)), E_FAIL);
#pragma endregion

#pragma region Ui_Texture
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_FieldHp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_FieldHp.png")), E_FAIL);
#pragma endregion

	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_Vanish", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_Vanish/Spr_Bullet_Deva_Circle_White_0%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Dowewole/Boss_DowewoleBody/Up_FaceOn/Spr_Boss_DoewoleBody_Up_FaceOn_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Dowewole/Boss_DowewoleClaw/Stand/Spr_Boss_DoewoleClaw_Stand_%d.png", 10)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_Vanish", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_Vanish/Spr_Effect_No051_Vanish_%d.png", 6)), E_FAIL);

#pragma region Player_Texture
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_000", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_000_0%d.png",8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_045_0%d.png",8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_090", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_090_0%d.png",8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_135_0%d.png",8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_180", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_180_0%d.png",8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_000", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_000_0%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_045_0%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_090", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_090_0%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_135_0%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_180", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_180_0%d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Move_Attack_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Move_Attack/Spr_Yeon_ChargeAttack_045_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Move_Attack_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Move_Attack/Spr_Yeon_ChargeAttack_135_%d.png", 10)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand_Attack/Spr_Yeon_ChargeStandingAttack_045_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand_Attack/Spr_Yeon_ChargeStandingAttack_135_%d.png", 10)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Dash_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Dash/Spr_Yeon_Dash_045_0%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Dash_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Dash/Spr_Yeon_Dash_135_0%d.png", 6)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Death/Spr_Yeon_Death_%d.png", 12)), E_FAIL);

#pragma endregion

	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Shadow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Shadow/spr_ShadowV2.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);

	Set_String(L"Buffer Loading.....");
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_SphereTex", CSphereTex::Create(m_pGraphicDev)), E_FAIL);

	Set_String(L"ETC Loading.....");
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);
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
