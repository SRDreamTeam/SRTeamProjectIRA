#include "stdafx.h"
#include "..\Header\DoewoleBullet_SwordShot.h"
#include "Export_Function.h"

CDoewoleBullet_SwordShot::CDoewoleBullet_SwordShot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev), m_iDirCount(0)
{
}

CDoewoleBullet_SwordShot::CDoewoleBullet_SwordShot(const CDoewoleBullet_SwordShot& rhs)
	: CBullet(rhs), m_iDirCount(0)
{
}

CDoewoleBullet_SwordShot::~CDoewoleBullet_SwordShot()
{
}


HRESULT CDoewoleBullet_SwordShot::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 30.f;

	return S_OK;
}

_int CDoewoleBullet_SwordShot::Update_GameObject(const _float& fTimeDelta)
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

void CDoewoleBullet_SwordShot::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_SwordShot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_SwordShot::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_SwordShot"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_SwordShot", pComponent });

	return S_OK;
}

void CDoewoleBullet_SwordShot::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);
	m_pTransformCom->Set_Pos(vPos.x, 3.f, vPos.z);
}

void CDoewoleBullet_SwordShot::Change_State(void)
{
}

void CDoewoleBullet_SwordShot::Frame_Check(const _float& fTimeDelta)
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

void CDoewoleBullet_SwordShot::Distance_Dead_Check(void)
{
	_vec3		vPosCheck;

	vPosCheck = m_vOriginPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((40 <= vPosCheck.x) || (-40 >= vPosCheck.x)) || ((40 <= vPosCheck.z) || (-40 >= vPosCheck.z)) || (-40 >= ((-vPosCheck.x) + (-vPosCheck.z))))
	{
		m_bDead = true;
	}
}

CDoewoleBullet_SwordShot* CDoewoleBullet_SwordShot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _int _Dir_Count)
{
	CDoewoleBullet_SwordShot* pInstance = new CDoewoleBullet_SwordShot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Set_FireInfo(_Monster_Pos, _Dir_Count);

	return pInstance;
}

void CDoewoleBullet_SwordShot::Set_FireInfo(_vec3 _Monster_Pos, _int _Dir_Count)
{
	m_pTransformCom->m_vInfo[INFO_POS] = _Monster_Pos;
	m_vOriginPos = _Monster_Pos;
	m_iDirCount = _Dir_Count;
	m_pTransformCom->UpdatePos_OnWorld();
}

void CDoewoleBullet_SwordShot::Free(void)
{
	__super::Free();
}
