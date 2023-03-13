#include "stdafx.h"
#include "..\Header\Jar.h"
#include "Export_Function.h"

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CJar::CJar(const CJar& rhs)
	: CDynamicObject(rhs)
{
}

CJar::~CJar()
{
	Free();
}

HRESULT CJar::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CJar::Update_GameObject(const _float& fTimeDelta)
{	
	m_pTransformCom->Set_Pos(300.f, 0.2f, 300.f);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CJar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CJar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CJar* CJar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJar* pInstance = new CJar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJar::Free(void)
{
	__super::Free();
}

HRESULT CJar::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[JAR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_HistoricSites_DecoObject_"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_HistoricSites_DecoObject_", pComponent });

	return S_OK;
}
