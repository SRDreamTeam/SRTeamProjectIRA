#include "stdafx.h"
#include "..\Header\Doewole.h"
#include "Export_Function.h"
#include "Effect_Doewole_Vanish.h"

CDoewole::CDoewole(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBoss(pGraphicDev)
{
}

CDoewole::CDoewole(const CDoewole & rhs)
	: CBoss(rhs)
{
}

CDoewole::~CDoewole()
{

}

HRESULT CDoewole::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vInfo[INFO_POS] = { 40.f , 0.f , 40.f };

	return S_OK;
}

_int CDoewole::Update_GameObject(const _float& fTimeDelta)
{
	State_Update(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CDoewole::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}


void CDoewole::Idle(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 4.f)
	{
		m_fAccTime = 0.f;
		m_eCurState = MOVE;
	
		CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);
		NULL_CHECK(pEffect);
		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
		pLayer->Add_GameObject(L"Effect_Doewole_Vanish", pEffect, OBJ_NONE);
	}
}

void CDoewole::Move(const _float& fTimeDelta)
{
	m_pTransformCom->m_vInfo[INFO_POS].x -= m_fMoveSpeed * fTimeDelta;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 3.f)
	{
		m_fAccTime = 0.f;
		m_eCurState = IDLE;
		m_fMoveSpeed *= -1.f;
	}
}

HRESULT CDoewole::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CDoewole::State_Update(const _float& fTimeDelta)
{
	switch ((int)m_eCurState)
	{
	case (int)CDoewole::IDLE:
		Idle(fTimeDelta);
		break;
	case (int)CDoewole::MOVE:
		Move(fTimeDelta);
		break;
	case (int)CDoewole::STATE_END:
		break;
	default:
		break;
	}

}
CDoewole * CDoewole::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole * pInstance = new CDoewole(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole::Free(void)
{
	__super::Free();
}


