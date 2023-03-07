#include "stdafx.h"
#include "..\Header\MutationEvilSoul.h"
#include "Export_Function.h"
#include "MonsterBullet_2.h"

CMutationEvilSoul::CMutationEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_eHead(HEAD_FRONT), m_Count(0)
{
}

CMutationEvilSoul::CMutationEvilSoul(const CMutationEvilSoul& rhs)
	: CMonster(rhs), m_eHead(HEAD_FRONT), m_Count(0)
{
}

CMutationEvilSoul::~CMutationEvilSoul()
{
}


HRESULT CMutationEvilSoul::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eName = NAME_MUTATION;

	m_pTransformCom->Set_Pos(_float(rand() % 100), 1.f, _float(rand() % 100));
	m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CMutationEvilSoul::Update_GameObject(const _float& fTimeDelta)
{
	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	Change_State();
	Head_Check((m_pTransformCom->Patrol_Map(m_fSpeed, fTimeDelta)));

	return 0;
}

void CMutationEvilSoul::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMutationEvilSoul::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	if (HEAD_FRONT == m_eHead)
	{
		m_pTextureCom->Set_Texture((_uint)m_fFrame);

	}
	else if (HEAD_BACK == m_eHead)
	{
		m_pTextureCom_2->Set_Texture((_uint)m_fFrame);

	}

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CMutationEvilSoul::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	//45 IDLE, ATTACK -> ALL
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_45_All"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_45_All", pComponent });

	//135 IDLE, ATTACK -> ALL
	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_135_All"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_135_All", pComponent });

	return S_OK;
}

// 준석 수정 (23.03.02) : Player에서 복사해와서 그대로 사용함
void CMutationEvilSoul::SetUp_OnTerrain(void)
{
	_vec3		vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos;

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	// 준석 수정 (23.03.02) : 테스트용 추가 
	fHeight += 1.f;

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

void CMutationEvilSoul::Change_State(void)
{
	if ((0.f == m_fFrame) && (5 == m_Count))
	{
		m_eState = MONSTER_ATTACK;
		m_Count = 0;
		Create_Bullet();
	}
}

void CMutationEvilSoul::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == MONSTER_IDLE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
			m_Count++;
		}
	}

	if (m_eState == MONSTER_MOVE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}

	if (m_eState == MONSTER_ATTACK)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_eState = MONSTER_IDLE;
			m_bCheck = true;
		}
	}
}

void CMutationEvilSoul::Head_Check(const _vec3 vDir)
{
	if (0.f < vDir.z)	// 플레이어가 몬스터의 위(뒤)쪽에 위치
		m_eHead = HEAD_BACK;
	else if (0.f > vDir.z)	// 플레이어가 몬스터의 아래(앞)쪽에 위치
		m_eHead = HEAD_FRONT;

	if (0.f < vDir.x)
	{
		m_pTransformCom->Reverse_Scale_x();
	}
}


HRESULT CMutationEvilSoul::Create_Bullet(void)
{
	_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pBulletObject = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		pBulletObject = CMonsterBullet_2::Create(m_pGraphicDev, vMonster_Pos, (i + 1));
		NULL_CHECK(pBulletObject);
		pLayer->Add_BulletObject(OBJ_BULLET, pBulletObject);
	}
	
	return S_OK;
}

CMutationEvilSoul* CMutationEvilSoul::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMutationEvilSoul* pInstance = new CMutationEvilSoul(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMutationEvilSoul::Free(void)
{
	__super::Free();
}