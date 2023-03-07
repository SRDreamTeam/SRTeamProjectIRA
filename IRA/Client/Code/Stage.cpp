#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Doewole.h"
#include "Doewole_Body.h"
#include "Doewole_Shadow.h"
#include "Doewole_LeftClaw.h"
#include "Doewole_RightClaw.h"
#include "Effect_Doewole_Vanish.h"
#include "Ghost.h"
#include "SylphBow.h"
#include "SylphBowPair.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	return S_OK;

}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{
	// _DEBUG용
}

HRESULT CStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트

	CGameObject*	pGameObject = nullptr;

	// 오브젝트 

	pGameObject = CStaticCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject , OBJ_NONE), E_FAIL);

	/*pGameObject = CDynamicCamera::Create(m_pGraphicDev,
											&_vec3(0.f, 10.f, -10.f),
											&_vec3(0.f, 0.f, 1.f),
											&_vec3(0.f, 1.f, 0.f),
											D3DXToRadian(60.f), 
											(_float)WINCX / WINCY,
											0.1f, 
											1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject , OBJ_NONE), E_FAIL);*/

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject , OBJ_ETC), E_FAIL);

	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject , OBJ_ETC), E_FAIL);
		
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*	pGameObject = nullptr;
	CGameObject*	pBulletObject = nullptr;

	
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject, OBJ_PLAYER), E_FAIL);

	pGameObject = CSylphBow::Create(m_pGraphicDev,{0.f,0.f,0.f},0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SylphBow", pGameObject, OBJ_BOW), E_FAIL);

	pGameObject = CSylphBowPair::Create(m_pGraphicDev, { 0.f,0.f,0.f }, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SylphBowPair", pGameObject, OBJ_BOW), E_FAIL);

	pGameObject = CGhost::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ghost", pGameObject, OBJ_GHOST), E_FAIL);

	CGameObject* pDoewole = CDoewole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pDoewole, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole", pDoewole, OBJ_NONE), E_FAIL);

	pGameObject = CDoewole_Body::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_Body*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_Body", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CDoewole_LeftClaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_LeftClaw*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_LeftClaw", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CDoewole_RightClaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_RightClaw*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_RightClaw", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CDoewole_Shadow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_Shadow*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_Shadow", pGameObject, OBJ_NONE), E_FAIL);
	// 
	pGameObject = CGreenEvilSlime::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CEvilSoul::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_2", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CEvilSoul::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_3", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CMutationEvilSoul::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_4", pGameObject, OBJ_MONSTER), E_FAIL);

	pGameObject = CMutationEvilSoul::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_5", pGameObject, OBJ_MONSTER), E_FAIL);

	

	pGameObject = CStatus::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Status_1", pGameObject, OBJ_MONSTER), E_FAIL);

	//pGameObject = CStatus_Hp::Create(m_pGraphicDev,1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Status_1", pGameObject, OBJ_MONSTER), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트
	CGameObject*	pGameObject = nullptr;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	
	tLightInfo.Type    = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free(void)
{
	__super::Free();
}
