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

	if (1 == m_iPattern)
	{
		if (m_fAccTime > 4.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = MOVE;
			++m_iPattern;

			CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);
			NULL_CHECK(pEffect);
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			//pLayer->Add_GameObject(L"Effect_Doewole_Vanish", pEffect, OBJ_NONE);
			pLayer->Add_BulletObject(OBJ_NONE, pEffect);
		}

	}

	else if (2 == m_iPattern)
	{
		if (m_fAccTime > 2.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = STANDARD_ATTACK;
			++m_iPattern;
		}
	}

	else if (3 == m_iPattern)
	{
		if (m_fAccTime > 2.f)
		{
			m_fAccTime = 0.f;
			//m_eCurState = STANDARD_ATTACK;
			++m_iPattern;
		}
	}

}

void CDoewole::Move(const _float& fTimeDelta)
{
	if(m_fAccTime > 1.f)
		m_pTransformCom->m_vInfo[INFO_POS].x -= m_fMoveSpeed * fTimeDelta;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 3.f)
	{
		m_fAccTime = 0.f;
		m_eCurState = IDLE;
		m_fMoveSpeed *= -1.f;
	}
}

void CDoewole::Standard_Attack(const _float& fTimeDelta)
{

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

	if (m_iPattern > m_iMaxPattern)
		m_iPattern = 0;
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


