#include "stdafx.h"
#include "..\Header\Status.h"
#include "Export_Function.h"

#include "Status_Hp.h"
#include "Status_Energy.h"
#include "Status_Key.h"
#include "Status_Money.h"
#include "Status_Gem.h"
#include "Status_Back.h"

CStatus::CStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CStatus::CStatus(const CStatus& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CStatus::~CStatus()
{
}

HRESULT CStatus::Ready_GameObject(void)
{	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CStatus::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;
		Create_StatusUi();
	}

	return 0;
}

void CStatus::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStatus::Render_GameObject()
{
}

HRESULT CStatus::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CStatus::Change_State(void)
{
}

void CStatus::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CStatus::Create_StatusUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	for (size_t i = 0; i < 5; i++)
	{
		pBulletObject = CStatus_Back::Create(m_pGraphicDev, (i + 1));
		NULL_CHECK_RETURN(pBulletObject, -1);
		pLayer->Add_BulletObject(  pBulletObject);
	}

	for (size_t i = 0; i < 5; i++)
	{
		pBulletObject = CStatus_Hp::Create(m_pGraphicDev,(i + 1));
		NULL_CHECK_RETURN(pBulletObject, -1);
		pLayer->Add_BulletObject(  pBulletObject);
	}

	for (size_t i = 0; i < 5; i++)
	{
		pBulletObject = CStatus_Energy::Create(m_pGraphicDev, (i + 1));
		NULL_CHECK_RETURN(pBulletObject, -1);
		pLayer->Add_BulletObject(  pBulletObject);
	}

	pBulletObject = CStatus_Key::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pBulletObject, -1);
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CStatus_Money::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pBulletObject, -1);
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CStatus_Gem::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pBulletObject, -1);
	pLayer->Add_BulletObject(  pBulletObject);

	return S_OK;
}

CStatus* CStatus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStatus* pInstance = new CStatus(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStatus::Free(void)
{
	__super::Free();
}
