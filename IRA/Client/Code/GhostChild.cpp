#include "stdafx.h"
#include "..\Header\GhostChild.h"
#include "Export_Function.h"


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

	__super::Ready_GameObject();

	return S_OK;
}

_int CGhostChild::Update_GameObject(const _float& fTimeDelta)
{





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
