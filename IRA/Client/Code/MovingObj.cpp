#include "stdafx.h"
#include "..\Header\MovingObj.h"
#include "Export_Function.h"

CMovingObj::CMovingObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CMovingObj::CMovingObj(const CMovingObj& rhs)
	: CDynamicObject(rhs)
{
}

CMovingObj::~CMovingObj()
{
}

HRESULT CMovingObj::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CMovingObj::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMovingObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMovingObj::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//if (-1 != m_byDrawID)
	//	m_pTextureCom->Set_Texture(m_byDrawID);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CMovingObj* CMovingObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMovingObj* pInstance = new CMovingObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMovingObj::Free(void)
{
	__super::Free();
}

HRESULT CMovingObj::Add_Component(void)
{
	return E_NOTIMPL;
}

void CMovingObj::SetUp_OnTerrain(void)
{
}
