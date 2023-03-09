#include "stdafx.h"
#include "..\Header\DynamicObject.h"
#include "Export_Function.h"

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& rhs)
	: CLandscape(rhs)
{
}

CDynamicObject::~CDynamicObject()
{
}

HRESULT CDynamicObject::Ready_GameObject(void)
{
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CDynamicObject::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CDynamicObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDynamicObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDynamicObject::Free(void)
{
	__super::Free();
}

HRESULT CDynamicObject::Add_Component(void)
{
	return E_NOTIMPL;
}

void CDynamicObject::SetUp_OnTerrain(void)
{
}
