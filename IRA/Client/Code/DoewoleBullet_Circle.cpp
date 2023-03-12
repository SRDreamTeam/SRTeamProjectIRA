#include "stdafx.h"
#include "..\Header\DoewoleBullet_Circle.h"
#include "Export_Function.h"
#include "CollisionMgr.h"

CDoewoleBullet_Circle::CDoewoleBullet_Circle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev), m_iDirCount(0)
{
}

CDoewoleBullet_Circle::CDoewoleBullet_Circle(const CDoewoleBullet_Circle& rhs)
	: CBullet(rhs), m_iDirCount(0)
{
}

CDoewoleBullet_Circle::~CDoewoleBullet_Circle()
{
}


HRESULT CDoewoleBullet_Circle::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 30.f;

	return S_OK;
}

_int CDoewoleBullet_Circle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Check(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	if ((((vDir.x > -1.0f) && (vDir.x < 1.0f)) && ((vDir.z > -1.0f) && (vDir.z < 1.0f))))
	{
		m_bDead = true;
	}

	m_pTransformCom->Bullet_Move_Boss(vDir, m_fSpeed, fTimeDelta, m_iDirCount);

	SetUp_OnTerrain();
	Distance_Dead_Check();

	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_Circle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_Circle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_Circle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Circle"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_Circle", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->Set_Radius(5.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDoewoleBullet_Circle::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);
	m_pTransformCom->Set_Pos(vPos.x, 3.f, vPos.z);
}

void CDoewoleBullet_Circle::Change_State(void)
{
}

void CDoewoleBullet_Circle::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == BULLET_IDLE)
	{
		m_fFrame += 5.f * fTimeDelta;

		if (5.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}
}

void CDoewoleBullet_Circle::Distance_Dead_Check(void)
{
	_vec3		vPosCheck;

	vPosCheck = m_vOriginPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((40 <= vPosCheck.x) || (-40 >= vPosCheck.x)) || ((40 <= vPosCheck.z) || (-40 >= vPosCheck.z)) || (-40 >= ((-vPosCheck.x) + (-vPosCheck.z))))
	{
		m_bDead = true;
	}
}

CDoewoleBullet_Circle* CDoewoleBullet_Circle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _int _Dir_Count)
{
	CDoewoleBullet_Circle* pInstance = new CDoewoleBullet_Circle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Set_FireInfo(_Monster_Pos, _Dir_Count);

	return pInstance;
}

void CDoewoleBullet_Circle::Set_FireInfo(_vec3 _Monster_Pos, _int _Dir_Count)
{
	m_pTransformCom->m_vInfo[INFO_POS] = _Monster_Pos;
	m_vOriginPos = _Monster_Pos;
	m_iDirCount = _Dir_Count;
	m_pTransformCom->UpdatePos_OnWorld();
}

void CDoewoleBullet_Circle::Free(void)
{
	__super::Free();
}
