#include "stdafx.h"
#include "..\Header\Effect_AlertCircle.h"
#include "Export_Function.h"

CEffect_AlertCircle::CEffect_AlertCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_AlertCircle::CEffect_AlertCircle(const CEffect_AlertCircle & rhs)
	: CEffect(rhs)
{
}

CEffect_AlertCircle::~CEffect_AlertCircle()
{
	Free();
}

HRESULT CEffect_AlertCircle::Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	m_vMaxScale = vScale;
	m_fAliveTime = fAliveTime;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CEffect_AlertCircle::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	m_pTransformCom->m_vScale.x += 0.3f;
	m_pTransformCom->m_vScale.y += 0.3f;
	m_pTransformCom->m_vScale.z += 0.3f;

	if (m_pTransformCom->m_vScale.x >= m_vMaxScale.x) m_pTransformCom->m_vScale.x = m_vMaxScale.x;
	if (m_pTransformCom->m_vScale.y >= m_vMaxScale.y) m_pTransformCom->m_vScale.y = m_vMaxScale.y;
	if (m_pTransformCom->m_vScale.z >= m_vMaxScale.z) m_pTransformCom->m_vScale.z = m_vMaxScale.z;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NONE;
}

void CEffect_AlertCircle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}

void CEffect_AlertCircle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_AlertCircle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_AlertCircle"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_AlertCircle", pComponent });

	return S_OK;
}

CEffect_AlertCircle* CEffect_AlertCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime)
{
	CEffect_AlertCircle* pInstance = new CEffect_AlertCircle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_AlertCircle::Free(void)
{
	__super::Free();
}


