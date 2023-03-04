#include "stdafx.h"
#include "..\Header\Monster.h"
#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_eState(MONSTER_IDLE)
	, m_bCheck(false), m_pTextureCom_2(nullptr), m_fSpeed(2.f), m_fFrame(0.f), m_eName(NAME_END)
{
}

CMonster::CMonster(const CMonster & rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom), m_eState(MONSTER_IDLE)
	, m_bCheck(false), m_pTextureCom_2(nullptr), m_fSpeed(2.f), m_fFrame(0.f), m_eName(NAME_END)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject(void)
{
	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMonster::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMonster::Render_GameObject()
{
}

void CMonster::Free(void)
{
	__super::Free();
}
