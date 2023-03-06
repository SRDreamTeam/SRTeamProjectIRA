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

	m_pTransformCom->m_vScale = { -0.248f * 30.f , 0.467f * 30.f , 1.f };

	return S_OK;
}

_int CDoewole_LeftClaw::Update_GameObject(const _float& fTimeDelta)
{
	State_Update(fTimeDelta);

	CBoss::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CDoewole_LeftClaw::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewole_LeftClaw::Render_GameObject()
{
	if (!m_bRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	if (m_eCurState == CDoewole::IDLE)
		m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);

	else if (m_eCurState == CDoewole::STANDARD_ATTACK)
		m_pTextureCom[STANDARD_ATTACK]->Set_Texture((_uint)m_fFrame);

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

	pComponent = m_pTextureCom[STAND] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Stand"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Stand", pComponent });

	pComponent = m_pTextureCom[STANDARD_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_StandardAttack"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_StandardAttack", pComponent });

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

void CDoewole_LeftClaw::State_Update(const _float& fTimeDelta)
{
	m_eCurState = dynamic_cast<CDoewole*>(m_pOwner)->Get_State();

	if (m_eCurState != m_ePreState)
		m_fFrame = 0.f;

	switch (m_eCurState)
	{
	case CDoewole::IDLE:
		Idle(fTimeDelta);
		break;
	case CDoewole::MOVE:
		Move(fTimeDelta);
		break;
	case CDoewole::STANDARD_ATTACK:
		Standard_Attack(fTimeDelta);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}

	m_ePreState = m_eCurState;
}

void CDoewole_LeftClaw::Idle(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
		m_fFrame = 0.f;

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 12.f;
	m_pTransformCom->m_vInfo[INFO_POS].x -= 25.f;
	// =====================================================

	m_fMaxFrame = 10.f;
}

void CDoewole_LeftClaw::Move(const _float& fTimeDelta)
{
	m_bRender = false;
}

void CDoewole_LeftClaw::Standard_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
		m_fFrame = m_fMaxFrame;

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 12.f + m_fFrame;
	m_pTransformCom->m_vInfo[INFO_POS].x -= 25.f + m_fFrame * (-4.f);
	// =====================================================

	m_fMaxFrame = 7.f;
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


