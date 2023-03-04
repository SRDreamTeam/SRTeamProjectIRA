#include "stdafx.h"
#include "..\Header\GhostChild.h"
#include "Export_Function.h"
#include "Player.h"


CGhostChild::CGhostChild(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGhost(pGraphicDev)
{
}

CGhostChild::CGhostChild(const CGhostChild& rhs)
	:CGhost(rhs)
{
}

CGhostChild::~CGhostChild()
{
}

HRESULT CGhostChild::Ready_GameObject(void)
{

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	m_Sprite = pPlayer->Get_Run_Frame();

	


	__super::Ready_GameObject();

	return S_OK;
}

_int CGhostChild::Update_GameObject(const _float& fTimeDelta)
{

	m_Alive_Time += 4.f * fTimeDelta;

	if (m_Alive_Time > 4.f) {

	}


	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CGhostChild::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CGhostChild::Render_GameObject()
{

	__super::Render_GameObject();
}

CGhostChild* CGhostChild::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGhostChild* pInstance = new CGhostChild(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhostChild::Free(void)
{
	__super::Free();
}
