#include "stdafx.h"
#include "..\Header\DoewoleBullet_Standard.h"
#include "Export_Function.h"
#include <Effect_StandardBullet_Death.h>

CDoewoleBullet_Standard::CDoewoleBullet_Standard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDoewoleBullet_Standard::CDoewoleBullet_Standard(const CDoewoleBullet_Standard& rhs)
	: CBullet(rhs)
{
}

CDoewoleBullet_Standard::~CDoewoleBullet_Standard()
{
}


HRESULT CDoewoleBullet_Standard::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = rand() % 10 * 3.f + 10.f;

	m_fMaxframe = 7.f;

	m_pTransformCom->m_vScale = { 3.f , 3.f , 3.f };

	CTransform* pTransformCom = dynamic_cast<CTransform*>( Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTransformCom, -1);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	if (pTransformCom->m_vInfo[INFO_POS].x >= pPlayerTransformCom->m_vInfo[INFO_POS].x)
		m_pTransformCom->m_vInfo[INFO_POS] = { pTransformCom->m_vInfo[INFO_POS].x - (rand() % 30) * 0.02f ,3.f,  pTransformCom->m_vInfo[INFO_POS].z - (rand() % 30) * 0.02f };
	else
		m_pTransformCom->m_vInfo[INFO_POS] = { pTransformCom->m_vInfo[INFO_POS].x + (rand() % 30) * 0.02f ,3.f,  pTransformCom->m_vInfo[INFO_POS].z - (rand() % 30) * 0.02f };

	pTransformCom->m_vInfo[INFO_POS].y = 3.f;

	m_vDir = m_pTransformCom->m_vInfo[INFO_POS] - pTransformCom->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CDoewoleBullet_Standard::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead || m_bHit)
	{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);

	m_fAccTime += fTimeDelta;

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_Standard::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_Standard::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_Standard::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Standard"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_Standard", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(10.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDoewoleBullet_Standard::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fMaxframe < m_fFrame)
		m_fFrame = 0.f;

	if (m_fAccTime > 5.f)
		m_bDead = true;
}

CDoewoleBullet_Standard* CDoewoleBullet_Standard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewoleBullet_Standard* pInstance = new CDoewoleBullet_Standard(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewoleBullet_Standard::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_StandardBullet_Death::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}


void CDoewoleBullet_Standard::Free(void)
{
	__super::Free();
}
