#include "stdafx.h"
#include "..\Header\MiniMap.h"
#include "Export_Function.h"

#include "MiniMap_Back.h"
#include "MiniMap_Gage.h"
//#include "MiniMap_Frame.h"
//#include "MiniMap_Key_Q.h"
//#include "MiniMap_Pattern.h"
//#include "MiniMap_Count.h"
//#include "MiniMap_Pattern_Glow.h"

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CMiniMap::CMiniMap(const CMiniMap& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CMiniMap::~CMiniMap()
{
}

HRESULT CMiniMap::Ready_GameObject(void)
{	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMiniMap::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;
		Create_StatusUi();
	}

	return 0;
}

void CMiniMap::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap::Render_GameObject()
{
}

HRESULT CMiniMap::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CMiniMap::Change_State(void)
{
}

void CMiniMap::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CMiniMap::Create_StatusUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	pBulletObject = CMiniMap_Back::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pBulletObject, -1);
	pLayer->Add_BulletObject(OBJ_BULLET, pBulletObject);

	pBulletObject = CMiniMap_Gage::Create(m_pGraphicDev, 2);
	NULL_CHECK_RETURN(pBulletObject, -1);
	pLayer->Add_BulletObject(OBJ_BULLET, pBulletObject);

	//for (size_t i = 0; i < 3; i++)
	//{
	//	pBulletObject = CMiniMap_Count::Create(m_pGraphicDev, (i + 1));
	//	NULL_CHECK_RETURN(pBulletObject, -1);
	//	pLayer->Add_BulletObject(OBJ_BULLET, pBulletObject);
	//}

	//pBulletObject = CMiniMap_Key_Q::Create(m_pGraphicDev, 0);
	//NULL_CHECK_RETURN(pBulletObject, -1);
	//pLayer->Add_BulletObject(OBJ_BULLET, pBulletObject);

	return S_OK;
}

CMiniMap* CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniMap* pInstance = new CMiniMap(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniMap::Free(void)
{
	__super::Free();
}
