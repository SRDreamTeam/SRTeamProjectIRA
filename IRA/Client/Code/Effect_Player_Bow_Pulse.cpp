#include "stdafx.h"
#include "..\Header\Effect_Player_Bow_Pulse.h"
#include "Export_Function.h"

CEffect_Player_Bow_Pulse::CEffect_Player_Bow_Pulse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Bow_Pulse::CEffect_Player_Bow_Pulse(const CEffect_Player_Bow_Pulse & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Bow_Pulse::~CEffect_Player_Bow_Pulse()
{
	Free();
}

HRESULT CEffect_Player_Bow_Pulse::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 0.95f , 1.98f, 1.f };

	m_fMaxFrame = 9.f;

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SylphBow", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x  , pTransform->m_vInfo[INFO_POS].y-2.f , pTransform->m_vInfo[INFO_POS].z };

	return S_OK;
}

_int CEffect_Player_Bow_Pulse::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"SylphBow", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x + 0.5f , pTransform->m_vInfo[INFO_POS].y - 2.3f , pTransform->m_vInfo[INFO_POS].z };


	m_fFrame += m_fMaxFrame * fTimeDelta * 1.5f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Player_Bow_Pulse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Player_Bow_Pulse::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CEffect_Player_Bow_Pulse::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_BowPulse_Sylph"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_BowPulse_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Bow_Pulse* CEffect_Player_Bow_Pulse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Player_Bow_Pulse* pInstance = new CEffect_Player_Bow_Pulse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Bow_Pulse::Free(void)
{
	__super::Free();
}


