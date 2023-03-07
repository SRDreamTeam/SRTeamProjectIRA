#include "stdafx.h"
#include "..\Header\MonsterBullet_2.h"
#include "Export_Function.h"

CMonsterBullet_2::CMonsterBullet_2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev), m_iDirCount(0)
{
}

CMonsterBullet_2::CMonsterBullet_2(const CMonsterBullet_2& rhs)
	: CBullet(rhs), m_iDirCount(0)
{
}

CMonsterBullet_2::~CMonsterBullet_2()
{
}


HRESULT CMonsterBullet_2::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 10.f;
	m_pTransformCom->Set_Pos(m_vOriginPos.x, m_vOriginPos.y, m_vOriginPos.z);
	m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CMonsterBullet_2::Update_GameObject(const _float& fTimeDelta)
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

	m_pTransformCom->Bullet_Move(vDir, m_fSpeed, fTimeDelta, m_iDirCount);
	SetUp_OnTerrain();
	Distance_Dead_Check();

	return OBJ_NOEVENT;
}

void CMonsterBullet_2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMonsterBullet_2::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonsterBullet_2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Monster_Bullet_2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Monster_Bullet_2", pComponent });

	return S_OK;
}

void CMonsterBullet_2::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);
	m_pTransformCom->Set_Pos(vPos.x, 3.f, vPos.z);
}

void CMonsterBullet_2::Change_State(void)
{
}

void CMonsterBullet_2::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == BULLET_IDLE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}
}

void CMonsterBullet_2::Distance_Dead_Check(void)
{
	_vec3		vPosCheck;

	vPosCheck = m_vOriginPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((40 <= vPosCheck.x) || (-40 >= vPosCheck.x)) || ((40 <= vPosCheck.z) || (-40 >= vPosCheck.z)) || (-40 >= ((-vPosCheck.x) + (-vPosCheck.z))))
	{
		m_bDead = true;
	}
}

CMonsterBullet_2* CMonsterBullet_2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _int _Dir_Count)
{
	CMonsterBullet_2* pInstance = new CMonsterBullet_2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Set_FireInfo(_Monster_Pos, _Dir_Count);

	return pInstance;
}

void CMonsterBullet_2::Set_FireInfo(_vec3 _Monster_Pos, _int _Dir_Count)
{
	m_pTransformCom->m_vInfo[INFO_POS] = _Monster_Pos;
	m_vOriginPos = _Monster_Pos;
	m_iDirCount = _Dir_Count;
	m_pTransformCom->UpdatePos_OnWorld();
}

void CMonsterBullet_2::Free(void)
{
	__super::Free();
}
