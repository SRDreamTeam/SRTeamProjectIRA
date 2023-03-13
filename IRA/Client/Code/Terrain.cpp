#include "stdafx.h"
#include "..\Header\Terrain.h"
#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
	for (int i = 0; i < 4; ++i)
		m_pBufferCom[i] = nullptr;
}

CTerrain::CTerrain(const CTerrain & rhs)
	: Engine::CGameObject(rhs)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
{
	for (int i = 0; i < 4; ++i)
		m_pBufferCom[i] = rhs.m_pBufferCom[i];
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);


	//Key_Input(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CTerrain::Key_Input(const _float& fTimeDelta)
{
	_vec3	vDir;

	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);


}

void CTerrain::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (-1 != m_byDrawID)
		m_pTextureCom->Set_Texture(m_byDrawID);

	FAILED_CHECK_RETURN(SetUp_Material(), );

	//for(int i = 0; i < 4; ++i)
	//	m_pBufferCom[i]->Render_Buffer();

	m_pBufferCom[0]->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom[0] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(m_pBufferCom[0], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });

	//pComponent = m_pBufferCom[1] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Add_Vert"));
	//NULL_CHECK_RETURN(m_pBufferCom[1], E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Add_Vert", pComponent });

	//pComponent = m_pBufferCom[2] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Add_Horz"));
	//NULL_CHECK_RETURN(m_pBufferCom[2], E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Add_Horz", pComponent });

	//pComponent = m_pBufferCom[3] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Add_Horz"));
	//NULL_CHECK_RETURN(m_pBufferCom[3], E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Add_Horz", pComponent });

 	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
 	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
 	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
 
 	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Terrain"));
 	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
 	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Terrain", pComponent });

	return S_OK;
}

HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);


	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain * pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTerrain::Free(void)
{
	__super::Free();
}
