#include "stdafx.h"
#include "..\Header\Boss.h"
#include "Export_Function.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pSphereBufferCom(nullptr)
{

}

CBoss::CBoss(const CBoss & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_pSphereBufferCom(rhs.m_pSphereBufferCom)
{

}

CBoss::~CBoss()
{
	Free();
}

_int CBoss::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CBoss::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBoss::Free(void)
{
	__super::Free();
}


