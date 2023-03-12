#include "stdafx.h"
#include "..\Header\Jar.h"
#include "Export_Function.h"

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CJar::CJar(const CJar& rhs)
	: CDynamicObject(rhs)
{
}

CJar::~CJar()
{
}

HRESULT CJar::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CJar::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CJar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CJar::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//if (-1 != m_byDrawID)
	//	m_pTextureCom->Set_Texture(m_byDrawID);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CJar* CJar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJar* pInstance = new CJar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJar::Free(void)
{
	__super::Free();
}

HRESULT CJar::Add_Component(void)
{
	return E_NOTIMPL;
}

void CJar::SetUp_OnTerrain(void)
{
}
