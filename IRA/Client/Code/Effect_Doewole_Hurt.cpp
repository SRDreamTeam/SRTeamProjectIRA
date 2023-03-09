#include "stdafx.h"
#include "..\Header\Effect_Doewole_Hurt.h"
#include "Export_Function.h"

CEffect_Doewole_Hurt::CEffect_Doewole_Hurt(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Doewole_Hurt::CEffect_Doewole_Hurt(const CEffect_Doewole_Hurt & rhs)
	: CEffect(rhs)
{
}

CEffect_Doewole_Hurt::~CEffect_Doewole_Hurt()
{
	Free();
}

HRESULT CEffect_Doewole_Hurt::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = vScale;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CEffect_Doewole_Hurt::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAccTime += fTimeDelta;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);


	return OBJ_NONE;
}

void CEffect_Doewole_Hurt::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if(m_fAccTime > 0.3f)
		m_bDead = true;
}

void CEffect_Doewole_Hurt::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Doewole_Hurt::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Doewole_Hurt"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Doewole_Hurt", pComponent });

	return S_OK;
}

CEffect_Doewole_Hurt* CEffect_Doewole_Hurt::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CEffect_Doewole_Hurt* pInstance = new CEffect_Doewole_Hurt(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Doewole_Hurt::Free(void)
{
	__super::Free();
}


