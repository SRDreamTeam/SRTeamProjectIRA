#include "stdafx.h"
#include "..\Header\Ghost.h"
#include "Export_Function.h"

CGhost::CGhost(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
{
}

CGhost::CGhost(const CGhost& rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pCalculatorCom(rhs.m_pCalculatorCom)
{
}

CGhost::~CGhost()
{
}

HRESULT CGhost::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale({ 5.f, 5.f, 1.f });

	return S_OK;
}

_int CGhost::Update_GameObject(const _float& fTimeDelta)
{
	
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGhost::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CGhost::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	
	// 자식들 포문 돌리면서 플레이어 현재 스프라이트 출력

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CGhost::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });




	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}


CGhost * CGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGhost * pInstance = new CGhost(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhost::Free(void)
{
	__super::Free();
}


