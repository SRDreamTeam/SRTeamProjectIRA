#include "stdafx.h"
#include "..\Header\Effect_Thorn_Destruction.h"
#include "Export_Function.h"

CEffect_Thorn_Destruction::CEffect_Thorn_Destruction(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Thorn_Destruction::CEffect_Thorn_Destruction(const CEffect_Thorn_Destruction & rhs)
	: CEffect(rhs)
{
}

CEffect_Thorn_Destruction::~CEffect_Thorn_Destruction()
{
	Free();
}

HRESULT CEffect_Thorn_Destruction::Ready_GameObject(const _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 3.f , 3.f, 3.f };
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 7.f;

	return S_OK;
}

_int CEffect_Thorn_Destruction::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta;

	return 0;
}

void CEffect_Thorn_Destruction::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Thorn_Destruction::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Thorn_Destruction::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Thorn_Destruction"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Thorn_Destruction", pComponent });

	return S_OK;
}

CEffect_Thorn_Destruction* CEffect_Thorn_Destruction::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos)
{
	CEffect_Thorn_Destruction* pInstance = new CEffect_Thorn_Destruction(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Thorn_Destruction::Free(void)
{
	__super::Free();
}


