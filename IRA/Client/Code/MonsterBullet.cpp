#include "stdafx.h"
#include "..\Header\MonsterBullet.h"
#include "Export_Function.h"

CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CMonsterBullet::CMonsterBullet(const CMonsterBullet& rhs)
	: CBullet(rhs)
{
}

CMonsterBullet::~CMonsterBullet()
{
}

HRESULT CMonsterBullet::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Set_Pos(rand() % 50, 1.f, rand() % 50);
	//m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CMonsterBullet::Update_GameObject(const _float& fTimeDelta)
{	
	//Frame_Check(fTimeDelta);
	//SetUp_OnTerrain();
	//__super::Update_GameObject(fTimeDelta);
	//Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return m_bDead;
}

void CMonsterBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMonsterBullet::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonsterBullet::Add_Component(void)
{	
	//Engine::CComponent* pComponent = nullptr;

	//pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	//NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	//NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Monster_Bullet_1"));
	//NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Monster_Bullet_1", pComponent });

	return S_OK;
}

void CMonsterBullet::SetUp_OnTerrain(void)
{
}

void CMonsterBullet::Change_State(void)
{
}

void CMonsterBullet::Frame_Check(const _float& fTimeDelta)
{
}

CMonsterBullet* CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _bool _Target_Check)
{	
	CMonsterBullet* pInstance = new CMonsterBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonsterBullet::Set_FireInfo(_vec3 _Monster_Pos, _bool _Target_Check)
{
	m_pTransformCom->m_vInfo[INFO_POS] = _Monster_Pos;
	m_bCheck = _Target_Check;
}

void CMonsterBullet::Free(void)
{
	__super::Free();
}