#include "stdafx.h"
#include "..\Header\Pui.h"
#include "Export_Function.h"

CPui::CPui(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_fFrame(0.f), m_bCheck(false), m_eCategory(UI_NONE)
{
}

CPui::CPui(const CPui& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom)
	, m_fFrame(0.f), m_bCheck(false), m_eCategory(UI_NONE)
{
}

CPui::~CPui()
{
}

HRESULT CPui::Ready_GameObject(void)
{
	return S_OK;
}

_int CPui::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CPui::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CPui::Render_GameObject()
{
}

void CPui::Free(void)
{
	__super::Free();
}
