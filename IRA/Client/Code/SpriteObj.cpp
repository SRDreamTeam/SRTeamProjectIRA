#include "stdafx.h"
#include "..\Header\SpriteObj.h"
#include "Export_Function.h"

CSpriteObj::CSpriteObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CSpriteObj::CSpriteObj(const CSpriteObj& rhs)
	: CDynamicObject(rhs)
{
}

CSpriteObj::~CSpriteObj()
{
}

HRESULT CSpriteObj::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CSpriteObj::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	//_float f = m_pTextureCom[m_eID]->

	if(m_bSprite)
		m_fFrame += m_pTextureCom[m_eID]->Get_MaxFrameCnt() * fTimeDelta;

	if (m_pTextureCom[m_eID]->Get_MaxFrameCnt() < m_fFrame)
	{
		m_fFrame = 0.f;
		//m_bSprite = false;
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CSpriteObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CSpriteObj::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ***
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// 나중에 지워라 

	m_pTextureCom[m_eID]->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	// ***
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// 나중에 지워라
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSpriteObj* CSpriteObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpriteObj* pInstance = new CSpriteObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpriteObj::Free(void)
{
	__super::Free();
}
