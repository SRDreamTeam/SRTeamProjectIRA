#include "stdafx.h"
#include "..\Header\Effect_Player_Bow_Charge.h"
#include "Export_Function.h"
#include "Player.h"

CEffect_Player_Bow_Charge::CEffect_Player_Bow_Charge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Bow_Charge::CEffect_Player_Bow_Charge(const CEffect_Player_Bow_Charge & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Bow_Charge::~CEffect_Player_Bow_Charge()
{
	Free();
}

HRESULT CEffect_Player_Bow_Charge::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 3.f , 3.f, 1.f };

	m_fFrame = 0.f;
	m_fMaxFrame = 5.f;

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SylphBow", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x  , pTransform->m_vInfo[INFO_POS].y-2.f , pTransform->m_vInfo[INFO_POS].z };

	return S_OK;
}

_int CEffect_Player_Bow_Charge::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	
	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SylphBow", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x - 0.5f , pTransform->m_vInfo[INFO_POS].y - 2.2f , pTransform->m_vInfo[INFO_POS].z };

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.5f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(fTimeDelta);

	return OBJ_NONE;
}

void CEffect_Player_Bow_Charge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


}

void CEffect_Player_Bow_Charge::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CEffect_Player_Bow_Charge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_BowCharge_Sylph"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_BowCharge_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Bow_Charge* CEffect_Player_Bow_Charge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Player_Bow_Charge* pInstance = new CEffect_Player_Bow_Charge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Bow_Charge::Free(void)
{
	__super::Free();
}


