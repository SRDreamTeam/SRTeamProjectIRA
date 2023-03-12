#include "stdafx.h"
#include "..\Header\Door.h"
#include "Export_Function.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CDoor::CDoor(const CDoor& rhs)
	: CDynamicObject(rhs)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CDoor::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CDoor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoor::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//if (-1 != m_byDrawID)
	//	m_pTextureCom->Set_Texture(m_byDrawID);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoor* pInstance = new CDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoor::Free(void)
{
	__super::Free();
}

HRESULT CDoor::Add_Component(void)
{
	return E_NOTIMPL;
}

void CDoor::SetUp_OnTerrain(void)
{
}
