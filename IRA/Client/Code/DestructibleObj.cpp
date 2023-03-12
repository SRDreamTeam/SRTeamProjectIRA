#include "stdafx.h"
#include "..\Header\DestructibleObj.h"
#include "Export_Function.h"

CDestructibleObj::CDestructibleObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CDestructibleObj::CDestructibleObj(const CDestructibleObj& rhs)
	: CDynamicObject(rhs)
{
}

CDestructibleObj::~CDestructibleObj()
{
}

HRESULT CDestructibleObj::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return S_OK;
}

_int CDestructibleObj::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();
	if (4 < m_iHitCnt)
		m_fFrame = 0.f;

	if (m_bHit)
	{
		++m_iHitCnt;
		++m_fFrame;
		m_bHit = false;
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CDestructibleObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CDestructibleObj::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ***
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// 나중에 지워라 

	m_pTextureCom[m_eID]->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	// ***
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// 나중에 지워라


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CDestructibleObj* CDestructibleObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDestructibleObj* pInstance = new CDestructibleObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDestructibleObj::Free(void)
{
	__super::Free();
}

HRESULT CDestructibleObj::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pTextureCom[THORN1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn01_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn01_", pComponent });

	pComponent = m_pTextureCom[THORN2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn02_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn02_", pComponent });

	pComponent = m_pTextureCom[THORN3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn03_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn03_", pComponent });

	pComponent = m_pTextureCom[THORN4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn04_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn04_", pComponent });

	pComponent = m_pTextureCom[THORN5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn05_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn05_", pComponent });

	pComponent = m_pTextureCom[THORN6] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn06_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn06_", pComponent });

	pComponent = m_pTextureCom[THORN_DEST_EFFECT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_InfectionThorns_Effect_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_InfectionThorns_Effect_", pComponent });

	pComponent = m_pTextureCom[THORN_HIT_EFFECT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_InfectionThorns_HitEffect_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_InfectionThorns_HitEffect_", pComponent });


	return S_OK;
}

void CDestructibleObj::SetUp_OnTerrain(void)
{
}
