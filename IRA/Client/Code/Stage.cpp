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
#include "Player_Shadow.h"
#include "Doewole_LeftClaw.h"
#include "Doewole_RightClaw.h"
#include "Effect_Doewole_Vanish.h"
#include "Ghost.h"
#include "SylphBow.h"
#include "SylphBowPair.h"
#include "StaticObject.h"
#include <KeyMgr.h>
#include "CollisionMgr.h"

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
	if (!m_bTerrainInit)
	{
		//Load_Terrain_Info(L"../../Data/Terrain/TestMap.dat");
		Load_Terrain_Info(L"../../Data/Terrain/BossDowoleMap.dat");
		m_bTerrainInit = true;

		//Load_Object_Info(L"../../Data/Object/SizeTest.dat");
		Load_Object_Info(L"../../Data/Object/BossDowoleMapObj.dat");
	}

	if (!g_bSphereMake)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_C))
			g_bSphereMake = true;
	}

	if (g_bSphereMake)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_V))
		{
			if (!g_bSphereRender)
				g_bSphereRender = true;
			else
				g_bSphereRender = false;
		}
	}


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
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	/*pGameObject = CDynamicCamera::Create(m_pGraphicDev,
											&_vec3(0.f, 10.f, -10.f),
											&_vec3(0.f, 0.f, 1.f),
											&_vec3(0.f, 1.f, 0.f),
											D3DXToRadian(60.f), 
											(_float)WINCX / WINCY,
											0.1f, 
											1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);*/

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject , OBJ_ETC), E_FAIL);
		
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*	pGameObject = nullptr;
	CGameObject*	pBulletObject = nullptr;

	
	CGameObject* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pPlayer),E_FAIL);
	

	pGameObject = CPlayer_Shadow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CPlayer_Shadow*>(pGameObject)->Set_Owner(pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Shadow", pGameObject), E_FAIL);

	pGameObject = CSylphBow::Create(m_pGraphicDev,{0.f,0.f,0.f},0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SylphBow", pGameObject), E_FAIL);

	pGameObject = CSylphBowPair::Create(m_pGraphicDev, { 0.f,0.f,0.f }, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SylphBowPair", pGameObject), E_FAIL);

	pGameObject = CGhost::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ghost", pGameObject), E_FAIL);

	CGameObject* pDoewole = CDoewole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pDoewole, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole", pDoewole), E_FAIL);

	pGameObject = CDoewole_Body::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_Body*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_Body", pGameObject), E_FAIL);

	pGameObject = CDoewole_LeftClaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_LeftClaw*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_LeftClaw", pGameObject), E_FAIL);

	pGameObject = CDoewole_RightClaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_RightClaw*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_RightClaw", pGameObject), E_FAIL);

	pGameObject = CDoewole_Shadow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDoewole_Shadow*>(pGameObject)->Set_Owner(pDoewole);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Doewole_Shadow", pGameObject ), E_FAIL);
	 
	/*pGameObject = CGreenEvilSlime::Create(m_pGraphicDev);
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
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_5", pGameObject, OBJ_MONSTER), E_FAIL);*/

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트
	CGameObject*	pGameObject = nullptr;

	pGameObject = CStatus::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FrontUI_Status", pGameObject), E_FAIL);

	pGameObject = CApostle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FrontUI_Apostle", pGameObject), E_FAIL);

	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FrontUI_QuickSlot", pGameObject), E_FAIL);

	pGameObject = CMiniMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FrontUI_MiniMap", pGameObject), E_FAIL);

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

HRESULT CStage::Load_Terrain_Info(const _tchar* pPath)
{
	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));

	CGameObject* pGameObject = nullptr;
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//wstring strFilePath;
	//wstring strHeightmapFilePath;
	//strFilePath = pHeightmapFilePath;
	//strHeightmapFilePath = strFilePath.substr(0, 3);
	//strHeightmapFilePath += strFilePath.substr(10);

	//_tchar* pHeightmapPath = nullptr;
	//pHeightmapPath = new TCHAR[dwStrByte];
	//lstrcpy(pHeightmapPath, strHeightmapFilePath.c_str());

	//CTerrainTex* pTerrainBufCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	//NULL_CHECK_RETURN(pTerrainBufCom, E_FAIL);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	NULL_CHECK_RETURN(pTerrain, E_FAIL);

	//pTerrainBufCom->Set_VtxCnt(tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv);
	//pTerrainBufCom->Set_DetailLevel(tTerrainData.fDetailLevel);
	//pTerrainBufCom->Set_HeightMapLevel(tTerrainData.iHeightMapLevel);
	//pTerrainBufCom->Set_HeightMapFilePath(pHeightmapPath);
	//pTerrainBufCom->Set_VtxPos(tTerrainDataPointer.pPos, tTerrainDataPointer.dwVtxCnt);
	pTerrain->Set_DrawID(tTerrainData.byDrawID);
	pTerrain->Set_DrawOption(tTerrainData.byDrawOption);

	CloseHandle(hFile);

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	//pTerrainBufCom->Update_Buffer_Data();

	return S_OK;
}

HRESULT CStage::Load_Object_Info(const _tchar* pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	STATICOBJDATA tStaticObjData;

	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ZeroMemory(&tStaticObjData, sizeof(STATICOBJDATA));

		ReadFile(hFile, &tStaticObjData, sizeof(STATICOBJDATA), &dwByte, nullptr);

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		_tchar* pObjKey = nullptr;
		pObjKey = new TCHAR[dwStrByte];

		ReadFile(hFile, pObjKey, dwStrByte, &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete_Array(pObjKey);
			break;
		}

		pGameObject = CStaticObject::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);

		wstring strComponentKey = pObjKey;

		if (string::npos != strComponentKey.find('|'))	// 클론인경우
		{	
			size_t startPos = strComponentKey.find('|');
			strComponentKey = strComponentKey.substr(0, startPos);
		}

		int iNum = strComponentKey.length() - 1;
		for (; iNum >= 0; --iNum)
		{
			if (0 == isdigit(pObjKey[iNum]))	// 숫자가 아닌 경우
				break;
		}

		strComponentKey = strComponentKey.substr(0, iNum+1);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_StaticObj_ID(dynamic_cast<CStaticObject*>(pGameObject)->CompareID(strComponentKey));
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(dynamic_cast<CStaticObject*>(pGameObject)->Get_ObjKey(), pGameObject), E_FAIL);
		// dynamic은 Dynamic으로 따로 빼기
		Safe_Delete_Array<_tchar*>(pObjKey);

		CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CStaticObject*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransformCom, E_FAIL);

		pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
		pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
		pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);

	}

	CloseHandle(hFile);

	return 0;
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
