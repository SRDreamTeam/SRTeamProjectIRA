#include "stdafx.h"
#include "..\Header\Player_Shadow.h"
#include "Export_Function.h"

CPlayer_Shadow::CPlayer_Shadow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDoewole(pGraphicDev)

{
}

CPlayer_Shadow::CPlayer_Shadow(const CPlayer_Shadow & rhs)
	: CDoewole(rhs.m_pGraphicDev)
{
}

CPlayer_Shadow::~CPlayer_Shadow()
{
}

HRESULT CPlayer_Shadow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 4.f , 4.f , 1.f };

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CPlayer_Shadow::Update_GameObject(const _float& fTimeDelta)
{
	
	CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTransformCom, -1);

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransformCom->m_vInfo[INFO_POS].x + 0.2f, 0.01f, pTransformCom->m_vInfo[INFO_POS].z + 1.f };

	CBoss::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CPlayer_Shadow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CPlayer_Shadow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer_Shadow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Shadow"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Shadow", pComponent });

	return S_OK;
}

CPlayer_Shadow * CPlayer_Shadow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Shadow * pInstance = new CPlayer_Shadow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Shadow::Free(void)
{
	__super::Free();
}


