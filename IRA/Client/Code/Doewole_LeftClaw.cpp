#include "stdafx.h"
#include "..\Header\Doewole_LeftClaw.h"
#include "Export_Function.h"

CDoewole_LeftClaw::CDoewole_LeftClaw(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDoewole(pGraphicDev)

{
}

CDoewole_LeftClaw::CDoewole_LeftClaw(const CDoewole_LeftClaw & rhs)
	: CDoewole(rhs.m_pGraphicDev)
{
}

CDoewole_LeftClaw::~CDoewole_LeftClaw()
{
}

HRESULT CDoewole_LeftClaw::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { -0.248f * 15.f , 0.467f * 15.f , 1.f };

	m_fFrame = 10.f;

	return S_OK;
}

_int CDoewole_LeftClaw::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 10.f * fTimeDelta;

	if (10.f < m_fFrame)
		m_fFrame = 0.f;

	// ================Doewole�� ��ġ�� �°� ����============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pDoewoleTransformCom, -1);

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 12.f;
	m_pTransformCom->m_vInfo[INFO_POS].x -= 10.f;
	// =====================================================

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	_vec3 v = m_pColliderCom->Get_SpherePos();

	return 0;
}

void CDoewole_LeftClaw::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewole_LeftClaw::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	//m_pSphereBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewole_LeftClaw::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->Set_Radius(10.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Engine::Clone_ProtoComponent(L"Proto_SphereTex"));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_SphereTex", pComponent });

	return S_OK;
}

CDoewole_LeftClaw * CDoewole_LeftClaw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole_LeftClaw * pInstance = new CDoewole_LeftClaw(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole_LeftClaw::Free(void)
{
	__super::Free();
}

